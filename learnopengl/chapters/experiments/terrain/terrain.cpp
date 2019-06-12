// glad, include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_program.h"
#include "core_types.h"

#include "vec4.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include "transform.h"
#include "fps_camera.h"
#include "free_camera.h"
#include "perlin.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glp::FreeCamera<float32>& camera, float32 deltaTime);




double OctavePerlin(double x, double y, double z, int octaves, double persistence) {
    double total = 0;
    double frequency = 2;
    double amplitude = 1;
    double maxValue = 0;
    for(int i=0;i<octaves;i++) {
        total += (glp::PerlinNoise<float32>::noise(x * frequency, y * frequency, z * frequency) * amplitude + 1)/2;
        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total / maxValue;
}


void createGrid(uint16 gridSize, std::vector<float32>& grid, std::vector<float32>& colors, float32 time) {
    grid = std::vector<float32>((gridSize * gridSize)/16 * 3);
    colors = std::vector<float32>((gridSize * gridSize)/16 * 3);

    uint64 curr = 0;
    uint64 colorCurr = 0;

    for (uint16 i = 0; i < gridSize; i+=4) {
        for (uint16 j = 0; j < gridSize; j+=4) {

            float32 height = 200*OctavePerlin(static_cast<float32>(i) / gridSize, static_cast<float32>(j) / gridSize, 0, 5, 0.5);

            grid[curr++] = i;
            grid[curr++] = height;
            grid[curr++] = j;

            // float32 r = distribution(generator) * noise;
            // float32 g = distribution(generator) * noise;
            // float32 b = distribution(generator) * noise;

            if (height <= 4*2) {
                colors[colorCurr++] = 0.0f;
                colors[colorCurr++] = 1.0f;
                colors[colorCurr++] = 1.0f;
            } else {
                colors[colorCurr++] = 0.0f;
                colors[colorCurr++] = 1.0f;
                colors[colorCurr++] = 0.0f;
            }
        }
    }
}

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

    // Set callback for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    glp::ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/experiments/terrain/shaders/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/experiments/terrain/shaders/fragment.glsl"
    );

    // Create a grid
    uint16 gridSize = 512;
    std::vector<float32> grid, colors;

    gridSize = gridSize / 4;
    // Find the indices for the EBO to draw the needed triangles
    std::vector<uint32> indices;
    uint32 j = 0;
    for (uint64 i = 0; i < gridSize-1; i++) {
        while (true) {
            indices.push_back(j); indices.push_back(j+1); indices.push_back(j + gridSize);
            indices.push_back(j+1); indices.push_back(j+gridSize+1); indices.push_back(j+gridSize);

            if (j == gridSize*(i+1) - 2) {
                j+=2;
                break;
            } else {
                j++;
            }
        }
    }




    std::cout << "Num of vertices drawn: " << indices.size() << std::endl;
    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)

    shaderProgram.use(); // don't forget to activate/use the shader before setting uniforms!

    glp::Mat4 projection {glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH)/HEIGHT)};
    shaderProgram.setMat4("projection", projection);

    unsigned int VBO, VAO, EBO;
    unsigned int color_VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &color_VBO);

    createGrid(gridSize*4, grid, colors, 0);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, grid.size()*sizeof(float32), grid.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float32), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float32), colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float32), (void*)0);
    glEnableVertexAttribArray(1);

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Input
        processInput(window, camera, deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        glp::Mat4<float32> newView = camera.getView();
        shaderProgram.setMat4("view", newView);

        glp::Mat4 model {glp::translate(-25.0f, -5.0f, -50.0f)};
        shaderProgram.setMat4("model", model);


        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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