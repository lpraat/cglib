// glad, include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core_types.h"
#include "vec4.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include "transform.h"
#include "shader_program.h"
#include "fps_camera.h"
#include "free_camera.h"
#include "perlin.h"
#include <cmath>
#include "constants.h"
#include "mesh.h"
#include "model.h"
#include "lights/directional.h"
#include "lights/spot.h"
#include "lights/point.h"
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime);
uint32 loadTexture(const char *path);

const uint32 WIDTH = 800;
const uint32 HEIGHT = 600;

glp::FreeCamera<float32> camera;
float32 lastX = WIDTH / 2.0f;
float32 lastY = HEIGHT / 2.0f;
bool firstMouse = true;

int main()
{
    // Initialize glfw
    glfwInit();

    // Set some options
    // See https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object that holds all the windowing data
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We pass GLAD the function to load the address of the OpenGL function pointers
    // which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct
    // function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set callback for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    glp::ShaderProgram shaderProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/model_loading/model_loading2/shaders/nanosuit/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/model_loading/model_loading2/shaders/nanosuit/fragment.glsl");

    glp::ShaderProgram lightProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/model_loading/model_loading2/shaders/lamp/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/model_loading/model_loading2/shaders/lamp/fragment.glsl"
    );

    // glp::Model terrain("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/terrain.obj");
    // terrain.getNodes()[1].meshes[0].addTexture("texture_diffuse", "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/diff2.jpg");
    // terrain.print();

    glp::Model nano("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/nanosuit/nanosuit.obj");
    nano.print();

    // TODO depth testing, back face ecc
    // Use z-buffer
    glEnable(GL_DEPTH_TEST);
    std::vector<float32> vertices = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};



    // light cube
    uint32 VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float32), vertices.data(), GL_STATIC_DRAW);

    // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    std::vector<glp::PointLight<float32>> pointLights = {
        {
            glp::Vec3(0.7f, 0.2f, 2.0f),
            {0.05f, 0.05f, 0.05f},
            {0.8f, 0.8f, 0.8f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            0.09f,
            0.032f
        },
        {
            glp::Vec3(2.3f, -3.3f, -4.0f),
            {0.05f, 0.05f, 0.05f},
            {0.8f, 0.8f, 0.8f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            0.09f,
            0.032f
        },
        {
            glp::Vec3(-4.0f, 2.0f, -12.0f),
            {0.05f, 0.05f, 0.05f},
            {0.8f, 0.8f, 0.8f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            0.09f,
            0.032f
        },
        {
            glp::Vec3(0.0f, 0.0f, -3.0f),
            {0.05f, 0.05f, 0.05f},
            {0.8f, 0.8f, 0.8f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            0.09f,
            0.032f
        }
    };

    // Lights
    glp::DirectionalLight<float32> directionalLight {
        {0.05f, 0.05f, 0.05f},
        {0.4f, 0.4f, 0.4f},
        {0.5f, 0.5f, 0.5f},
        {-0.2f, -1.0f, -0.3f}
    };

    glp::SpotLight<float32> spotLight {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        1.0f,
        0.09f,
        0.032f,
        std::cos(12.5f * glp::toRadians()),
        std::cos(17.5f * glp::toRadians())
    };

    shaderProgram.use();

    shaderProgram.setFloat("shininess", 32.0f);

    // Directional light
    shaderProgram.setVec3("dirLight.direction", directionalLight.dir);
    shaderProgram.setVec3("dirLight.ambient", directionalLight.ambient);
    shaderProgram.setVec3("dirLight.diffuse", directionalLight.diffuse);
    shaderProgram.setVec3("dirLight.specular", directionalLight.specular);

    // Point lights
    for (uint32 i = 0; i < pointLights.size(); i++) {
        shaderProgram.setVec3(std::string("pointLights[") + std::to_string(i) + std::string("].position"), pointLights[i].position);
        shaderProgram.setVec3(std::string("pointLights[") + std::to_string(i) + std::string("].ambient"), pointLights[i].ambient);
        shaderProgram.setVec3(std::string("pointLights[") + std::to_string(i) + std::string("].diffuse"), pointLights[i].diffuse);
        shaderProgram.setVec3(std::string("pointLights[") + std::to_string(i) + std::string("].specular"), pointLights[i].specular);
        shaderProgram.setFloat(std::string("pointLights[") + std::to_string(i) + std::string("].constant"), pointLights[i].constant);
        shaderProgram.setFloat(std::string("pointLights[") + std::to_string(i) + std::string("].linear"), pointLights[i].linear);
        shaderProgram.setFloat(std::string("pointLights[") + std::to_string(i) + std::string("].quadratic"), pointLights[i].quadratic);
    }

    // Spotlight
    shaderProgram.setVec3("spotLight.ambient", spotLight.ambient);
    shaderProgram.setVec3("spotLight.diffuse", spotLight.diffuse);
    shaderProgram.setVec3("spotLight.specular", spotLight.specular);
    shaderProgram.setFloat("spotLight.constant", spotLight.constant);
    shaderProgram.setFloat("spotLight.linear", spotLight.linear);
    shaderProgram.setFloat("spotLight.quadratic", spotLight.quadratic);
    shaderProgram.setFloat("spotLight.cutOff", spotLight.cutOff);
    shaderProgram.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);


    shaderProgram.use();
    glp::Mat4 projection{glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH) / HEIGHT)};
    shaderProgram.setMat4("projection", projection);

    lightProgram.use();
    lightProgram.setMat4("projection", projection);


    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;
    while (!glfwWindowShouldClose(window))
    {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Input
        processInput(window, camera, deltaTime);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        spotLight.position = camera.getPosition();
        spotLight.direction = camera.getFrontDirection();

        shaderProgram.use();
        shaderProgram.setVec3("spotLight.position", spotLight.position);
        shaderProgram.setVec3("spotLight.direction", spotLight.direction);

        shaderProgram.setVec3("viewPos", camera.getPosition());

        glp::Mat4<float32> newView = camera.getView();
        shaderProgram.setMat4("view", newView);

        glp::Mat4<float32> model = glp::translate(0.0f, -1.75f, 0.0f);
        shaderProgram.setMat4("model", model);
        nano.draw(shaderProgram);


        lightProgram.use();
        lightProgram.setMat4("view", camera.getView());

        glBindVertexArray(VAO);
        for (uint32 i = 0; i < pointLights.size(); i++) {
            lightProgram.setMat4("model", glp::translate(pointLights[i].position).dot(glp::scale(0.2f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        //terrain.draw(shaderProgram);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear resources
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime)
{
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_M, deltaTime);
    }
}