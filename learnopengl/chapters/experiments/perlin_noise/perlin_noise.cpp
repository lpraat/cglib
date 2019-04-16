// glad, include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_program.h"
#include "core_types.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vec4.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include "transform.h"
#include "fps_camera.h"
#include "free_camera.h"
#include <random>
#include "perlin.h"

template <typename T = float32, typename S = uint32>
std::vector<T> createGrid(uint16 dim, float32 time) {
    std::vector<T> grid(dim*dim*6);

    std::default_random_engine generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<T> distribution(0, 1);

    S curr = 0;

    for (uint16 i = 0; i < dim; i++) {
        for (uint16 j = 0; j < dim; j++) {
            grid[curr++] = i;
            grid[curr++] = j;
            grid[curr++] = 0;

            // double noise = PerlinNoise<float32>::noise(
            //     static_cast<float32>(i)/dim,
            //     static_cast<float32>(j)/dim,
            //     time * 0.5);

            double noise = PerlinNoise<float32>::octave(
                3,
                static_cast<float32>(i)/dim * 20,
                static_cast<float32>(j)/dim * 20,
                time);

            noise = (noise + 1) / 2;

            T r = noise;
            T g = noise;
            T b = noise;

            T gr = 0.21*r + 0.72*g + 0.07*b;

            grid[curr++] = gr;
            grid[curr++] = gr;
            grid[curr++] = gr;
        }
    }

    return grid;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glp::FreeCamera<float32>& camera, float32 deltaTime);

const uint32 WIDTH = 800;
const uint32 HEIGHT = 600;

glp::FreeCamera<float32> camera;
float32 lastX = WIDTH / 2.0f;
float32 lastY = HEIGHT / 2.0f;
bool firstMouse = true;

int main() {
    // Initialize glfw
    glfwInit();

    // Set some options
    // See https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object that holds all the windowing data
    GLFWwindow* window = glfwCreateWindow(2600, 1800, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We pass GLAD the function to load the address of the OpenGL function pointers
    // which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct
    // function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Set callback for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/experiments/perlin_noise/shaders/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/experiments/perlin_noise/shaders/fragment.glsl"
    );


    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shaderProgram.use(); // don't forget to activate/use the shader before setting uniforms!

    // Projection matrix
    // Since this rarely changes it is better to set it outside the main loop
    glp::Mat4 projection {glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH)/HEIGHT)};
    shaderProgram.setMat4("projection", projection);

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    uint16 gridSize = 256;
    float32* vertices;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    while (!glfwWindowShouldClose(window)) {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Input
        processInput(window, camera, deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::vector<float32> v = createGrid<float32>(gridSize, currFrame);
        vertices = v.data();

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gridSize*gridSize*6*4, vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float32), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float32), (void*)(3*sizeof(float32)));
        glEnableVertexAttribArray(1);

        shaderProgram.use();

        glp::Mat4<float32> newView = camera.getView();
        shaderProgram.setMat4("view", newView);

        glp::Mat4 model {glp::translate(0.0f, 0.0f, -10.0f)};
        shaderProgram.setMat4("model", model);


        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, gridSize*gridSize);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // clear resources
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, glp::FreeCamera<float32>& camera, float32 deltaTime) {
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.updateDeltaPosition(glp::FreeCameraMovement::DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_M, deltaTime);
    }
}
