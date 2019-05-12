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
#include "noise.h"
#include "perlin.h"
#include <cmath>
#include "constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    ShaderProgram lightingProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/multiple_lights1/shaders/lighting/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/multiple_lights1/shaders/lighting/fragment.glsl");

    ShaderProgram lampProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/multiple_lights1/shaders/lamp/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/multiple_lights1/shaders/lamp/fragment.glsl");

    uint32 diffuseMap = loadTexture("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/container2.png");
    uint32 specularMap = loadTexture("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/multiple_lights/container2_specular.png");

    std::vector<float32> vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // positions all containers
    std::vector<glp::Vec3<float32>> cubePositions = {
        glp::Vec3(0.0f, 0.0f, 0.0f),
        glp::Vec3(2.0f, 5.0f, -15.0f),
        glp::Vec3(-1.5f, -2.2f, -2.5f),
        glp::Vec3(-3.8f, -2.0f, -12.3f),
        glp::Vec3(2.4f, -0.4f, -3.5f),
        glp::Vec3(-1.7f, 3.0f, -7.5f),
        glp::Vec3(1.3f, -2.0f, -2.5f),
        glp::Vec3(1.5f, 2.0f, -2.5f),
        glp::Vec3(1.5f, 0.2f, -1.5f),
        glp::Vec3(-1.3f, 1.0f, -1.5f)};

    std::vector<glp::Vec3<float32>> pointLightPositions = {
        glp::Vec3(0.7f, 0.2f, 2.0f),
        glp::Vec3(2.3f, -3.3f, -4.0f),
        glp::Vec3(-4.0f, 2.0f, -12.0f),
        glp::Vec3(0.0f, 0.0f, -3.0f)};

    // cube
    uint32 VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float32), vertices.data(), GL_STATIC_DRAW);

    // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // tex coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light cube
    uint32 lightingVAO;
    glGenVertexArrays(1, &lightingVAO);

    glBindVertexArray(lightingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float32), (void *)0);
    glEnableVertexAttribArray(0);

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);
    glp::Mat4 projection{glp::perspectiveProjection(45.0f, 0.1f, 100.0f, static_cast<float32>(WIDTH) / HEIGHT)};

    lightingProgram.use(); // don't forget to activate/use the shader before setting uniforms!
    lightingProgram.setMat4("projection", projection);

    lampProgram.use();
    lampProgram.setMat4("projection", projection);

    // Since the position is static in this case, we can set it out of the loop
    lightingProgram.use();
    lightingProgram.setInt("material.diffuse", 0);
    lightingProgram.setInt("material.specular", 1);
    lightingProgram.setFloat("material.shininess", 32.0f);
    /*
    Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
    the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
    by defining light types as classes and set their values in there, or by using a more efficient uniform approach
    by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
    */
    // directional light
    lightingProgram.setVec3("dirLightDir", {-0.2f, -1.0f, -0.3f});
    lightingProgram.setVec3("dirLight.ambient", {0.05f, 0.05f, 0.05f});
    lightingProgram.setVec3("dirLight.diffuse", {0.4f, 0.4f, 0.4f});
    lightingProgram.setVec3("dirLight.specular", {0.5f, 0.5f, 0.5f});

    // point light 1
    lightingProgram.setVec3("pointLightPos[0]", pointLightPositions[0]);
    lightingProgram.setVec3("pointLights[0].ambient", {0.05f, 0.05f, 0.05f});
    lightingProgram.setVec3("pointLights[0].diffuse", {0.8f, 0.8f, 0.8f});
    lightingProgram.setVec3("pointLights[0].specular", {1.0f, 1.0f, 1.0f});
    lightingProgram.setFloat("pointLights[0].constant", 1.0f);
    lightingProgram.setFloat("pointLights[0].linear", 0.09);
    lightingProgram.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    lightingProgram.setVec3("pointLightPos[1]", pointLightPositions[1]);
    lightingProgram.setVec3("pointLights[1].ambient", {0.05f, 0.05f, 0.05f});
    lightingProgram.setVec3("pointLights[1].diffuse", {0.8f, 0.8f, 0.8f});
    lightingProgram.setVec3("pointLights[1].specular", {1.0f, 1.0f, 1.0f});
    lightingProgram.setFloat("pointLights[1].constant", 1.0f);
    lightingProgram.setFloat("pointLights[1].linear", 0.09);
    lightingProgram.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    lightingProgram.setVec3("pointLightPos[2]", pointLightPositions[2]);
    lightingProgram.setVec3("pointLights[2].ambient", {0.05f, 0.05f, 0.05f});
    lightingProgram.setVec3("pointLights[2].diffuse", {0.8f, 0.8f, 0.8f});
    lightingProgram.setVec3("pointLights[2].specular", {1.0f, 1.0f, 1.0f});
    lightingProgram.setFloat("pointLights[2].constant", 1.0f);
    lightingProgram.setFloat("pointLights[2].linear", 0.09);
    lightingProgram.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    lightingProgram.setVec3("pointLightPos[3]", pointLightPositions[3]);
    lightingProgram.setVec3("pointLights[3].ambient", {0.05f, 0.05f, 0.05f});
    lightingProgram.setVec3("pointLights[3].diffuse", {0.8f, 0.8f, 0.8f});
    lightingProgram.setVec3("pointLights[3].specular", {1.0f, 1.0f, 1.0f});
    lightingProgram.setFloat("pointLights[3].constant", 1.0f);
    lightingProgram.setFloat("pointLights[3].linear", 0.09);
    lightingProgram.setFloat("pointLights[3].quadratic", 0.032);

    // spotLight
    lightingProgram.setVec3("spotLight.ambient", {0.0f, 0.0f, 0.0f});
    lightingProgram.setVec3("spotLight.diffuse", {1.0f, 1.0f, 1.0f});
    lightingProgram.setVec3("spotLight.specular", {1.0f, 1.0f, 1.0f});
    lightingProgram.setFloat("spotLight.constant", 1.0f);
    lightingProgram.setFloat("spotLight.linear", 0.09);
    lightingProgram.setFloat("spotLight.quadratic", 0.032);
    lightingProgram.setFloat("spotLight.cutOff", std::cos(12.5f * glp::toRadians()));
    lightingProgram.setFloat("spotLight.outerCutOff", std::cos(15.0f * glp::toRadians()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

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

        glp::Mat4<float32> newView = camera.getView();

        lightingProgram.use();
        lightingProgram.setMat4("view", newView);
        lightingProgram.setMat4("model", glp::Mat4<float32>::identity());
        lightingProgram.setVec3("objectColor", {1.0f, 0.5f, 0.31f});
        lightingProgram.setVec3("spotPos", camera.getPosition());
        lightingProgram.setVec3("spotDir", camera.getFrontDirection());

        glBindVertexArray(cubeVAO);
        for (uint32 i = 0; i < 10; i++)
        {
            glp::Mat4 model{glp::translate(cubePositions[i])};
            float32 angle = -20.0 * i;
            glp::Vec3 rotAxis{1.0f, 0.3f, 0.5f};
            glp::Quat q(angle, rotAxis.normalize());
            model = model.dot(q.toRotMatrix());

            lightingProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lampProgram.use();
        lampProgram.setMat4("projection", projection);
        lampProgram.setMat4("view", camera.getView());

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightingVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            lampProgram.setMat4("model", glp::translate(pointLightPositions[i]).dot(glp::scale(0.2f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightingVAO);
    glDeleteBuffers(1, &VBO);

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

uint32 loadTexture(char const *path)
{
    uint32 textureID;
    glGenTextures(1, &textureID);

    int32 width, height, nrComponents;
    uint8 *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}