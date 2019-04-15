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


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, float64 xpos, float64 ypos);
void processInput(GLFWwindow* window, glp::FPSCamera<float32>& camera, float32 deltaTime);
void scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset);

const uint32 WIDTH = 800;
const uint32 HEIGHT = 600;

glp::FPSCamera<float32> camera(45.0f);
float32 lastX = WIDTH / 2.0f;
float32 lastY = HEIGHT / 2.0f;
bool firstMouse = true;

float32 fov = 45.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/camera2/shaders/vertex.vs",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/camera2/shaders/fragment.fs"
    );

    // Generating first texture
    uint32 texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Set texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate texture 1
    int32 width1, height1, nrChannels1;
    uint8 *data1 = stbi_load("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/container.jpg",
                            &width1, &height1, &nrChannels1, 0);
    if (!data1) {
        std::cout << "Failed to load first texture" << std::endl;
    }
    // Now that a texture is bound we can start generating a texture using the preivously loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    // At this point the currently bound texture object has a texture image attached to it.
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data1);

    // Generating first texture
    uint32 texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Set texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate texture 2
    int32 width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(true);
    uint8 *data2 = stbi_load("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/camera/awesomeface.png",
                            &width2, &height2, &nrChannels2, 0);
    if (!data1) {
        std::cout << "Failed to load first texture" << std::endl;
    }
    // Now that a texture is bound we can start generating a texture using the preivously loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    // At this point the currently bound texture object has a texture image attached to it.
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);

    float32 vertices[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shaderProgram.use(); // don't forget to activate/use the shader before setting uniforms!
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    // Projection matrix
    // Since this rarely changes it is better to set it outside the main loop


    // Use z-buffer
    glEnable(GL_DEPTH_TEST);

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    while (!glfwWindowShouldClose(window)) {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Input
        processInput(window, camera, deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind textures on corresponding texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // View matrix
        shaderProgram.use();
        glp::Mat4<float32> newView = camera.getView();
        shaderProgram.setMat4("view", newView);

        glp::Mat4 projection {glp::perspectiveProjection(camera.getFov(), 0.1f, 100.0f, static_cast<float32>(WIDTH)/HEIGHT)};
        shaderProgram.setMat4("projection", projection);

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

void processInput(GLFWwindow* window, glp::FPSCamera<float32>& camera, float32 deltaTime) {
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.updatePosition(glp::FPSCameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.updatePosition(glp::FPSCameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.updatePosition(glp::FPSCameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.updatePosition(glp::FPSCameraMovement::RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, float64 xpos, float64 ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float32 xoffset = xpos - lastX;
    float32 yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.updateOrientation(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset) {
    camera.updateZoom(yOffset);
}
