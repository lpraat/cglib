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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const uint32 WIDTH = 800;
const uint32 HEIGHT = 600;

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

    ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/coordinate_systems/coordinate_systems_ex2/shaders/vertex.vs",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/coordinate_systems/coordinate_systems_ex2/shaders/fragment.fs"
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
    uint8 *data1 = stbi_load("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/coordinate_systems/container.jpg",
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
    uint8 *data2 = stbi_load("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/coordinate_systems/awesomeface.png",
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

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind textures on corresponding texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // View matrix
        glp::Mat4 view {glp::translate(0.0f, 0.0f, -3.0f)};

        // Projection matrix
        // Since this rarely changes it is better to set it outside the main loop
        glp::Mat4 projection {glp::perspectiveProjection(45.0f, 0.1f, 100.0f, static_cast<float32>(WIDTH)/HEIGHT)};

        shaderProgram.use();

        glBindVertexArray(VAO);
        for (uint32 i = 0; i < 10; i++) {
            glp::Mat4 model {glp::translate(cubePositions[i])};

            float32 angle;
            if (i % 3 == 0) {
                angle = -50.0f * glfwGetTime();
            } else {
                angle = -20.0 * i;
            }
            glp::Quat q(angle, glp::Vec3 {1.0f, 0.3f, 0.5f});
            model = model.dot(q.toRotMatrix());

            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

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

void processInput(GLFWwindow* window) {
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}