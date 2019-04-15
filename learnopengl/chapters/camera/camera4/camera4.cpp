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


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

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

    ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/camera4/shaders/vertex.vs",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/camera4/shaders/fragment.fs"
    );

    float32 vertices[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    glp::Vec3<float32> cubePositions[] = {
        glp::Vec3( 0.0f,  0.0f,  0.0f),
        glp::Vec3( 2.0f,  5.0f, -15.0f),
        glp::Vec3(-1.5f, -2.2f, -2.5f),
        glp::Vec3(-3.8f, -2.0f, -12.3f),
        glp::Vec3( 2.4f, -0.4f, -3.5f),
        glp::Vec3(-1.7f,  3.0f, -7.5f),
        glp::Vec3( 1.3f, -2.0f, -2.5f),
        glp::Vec3( 1.5f,  2.0f, -2.5f),
        glp::Vec3( 1.5f,  0.2f, -1.5f),
        glp::Vec3(-1.3f,  1.0f, -1.5f)
    };

    // VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and the configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shaderProgram.use(); // don't forget to activate/use the shader before setting uniforms!

    // Projection matrix
    // Since this rarely changes it is better to set it outside the main loop
    glp::Mat4 projection {glp::perspectiveProjection(45.0f, 0.1f, 100.0f, static_cast<float32>(WIDTH)/HEIGHT)};
    shaderProgram.setMat4("projection", projection);

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while (!glfwWindowShouldClose(window)) {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Input
        processInput(window, camera, deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix
        shaderProgram.use();
        glp::Mat4<float32> newView = camera.getView();
        shaderProgram.setMat4("view", newView);


        glBindVertexArray(VAO);
        for (uint32 i = 0; i < 10; i++) {
            glp::Mat4 model {glp::translate(cubePositions[i])};
            float32 angle = -20.0 * i;
            glp::Vec3 axis {1.0f, 0.3f, 0.5f};
            glp::Quat q(angle, axis.normalize());
            model = model.dot(q.toRotMatrix());

            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glBindVertexArray(0); // no need to unbind it every time

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
