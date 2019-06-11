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
#include "free_camera.h"
#include <cmath>
#include "constants.h"
#include "model.h"
#include "lights/directional.h"
#include "lights/spot.h"
#include "lights/point.h"
#include "cubemap.h"
#include "drone.h"
#include <string>
#include "collision.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cameraModeCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime);
void processInput(GLFWwindow *window, glp::Drone<float32> &camera, float32 deltaTime);

const uint32 WIDTH = 1280;
const uint32 HEIGHT = 720;

glp::Drone<float32> drone;
glp::FreeCamera<float32> camera;

bool lookAtMode = false;

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

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, WIDTH, HEIGHT);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, cameraModeCallBack);

    // Shaders
    glp::ShaderProgram skyboxProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/fragment.glsl"
    );

    glp::ShaderProgram terrainProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/fragment.glsl");

    glp::ShaderProgram droneProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/fragment.glsl"
    );

    glp::ShaderProgram cubeShader(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/bbox/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/bbox/fragment.glsl"
    );

    std::vector<glp::ShaderProgram*> shadersWithLights;
    shadersWithLights.push_back(&terrainProgram);
    shadersWithLights.push_back(&droneProgram);

    // Models
    glp::Model terrainModel("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/terrain.obj");
    terrainModel.getNodes()[1].meshes[0].addTexture("texture_diffuse", "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/diff2.jpg");
    terrainModel.getNodes()[1].meshes[0].addTexture("texture_normal", "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/nrm.png");
    //terrainModel.print();

    glp::Model droneModel("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/drone/drone_obj.obj");
    //droneModel.print();

    // Skybox
    glp::CubeMap skybox(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/right.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/left.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/top.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/bottom.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/front.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/back.jpg"

    );

    // Lights
    glp::DirectionalLight<float32> directionalLight {
        {0.05f, 0.05f, 0.05f},
        {0.4f, 0.4f, 0.4f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, -0.5f, 0.8f}
    };

    glp::SpotLight<float32> spotLight {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        1.0f,
        0.00002f,
        0.00002f,
        std::cos(12.5f * glp::toRadians()),
        std::cos(17.5f * glp::toRadians())
    };

    // Set lights to shaders
    for (uint32 i = 0; i < shadersWithLights.size(); i++) {
        // Directional light
        glp::ShaderProgram* s = shadersWithLights[i];
        s->use();
        s->setFloat("shininess", 32.0f);

        s->setVec3("dirLight.direction", directionalLight.dir);
        s->setVec3("dirLight.ambient", directionalLight.ambient);
        s->setVec3("dirLight.diffuse", directionalLight.diffuse);
        s->setVec3("dirLight.specular", directionalLight.specular);

        // Spotlight
        s->setVec3("spotLight.ambient", spotLight.ambient);
        s->setVec3("spotLight.diffuse", spotLight.diffuse);
        s->setVec3("spotLight.specular", spotLight.specular);
        s->setFloat("spotLight.constant", spotLight.constant);
        s->setFloat("spotLight.linear", spotLight.linear);
        s->setFloat("spotLight.quadratic", spotLight.quadratic);
        s->setFloat("spotLight.cutOff", spotLight.cutOff);
        s->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
    }

    // Set projection matrix
    glp::Mat4 projection{glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH) / HEIGHT)};

    terrainProgram.use();
    terrainProgram.setMat4("projection", projection);

    droneProgram.use();
    droneProgram.setMat4("projection", projection);

    skyboxProgram.use();
    skyboxProgram.setMat4("projection", projection);

    cubeShader.use();
    cubeShader.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // View matrices
    glp::Mat4<float32> lookAtView = glp::Mat4<float32>::identity();
    glp::Mat4<float32> cameraView = glp::Mat4<float32>::identity();

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    glp::Vec3<float32> cameraPosition;
    glp::Vec3<float32> cameraFrontDirection;

    glp::CollisionDetector<float32> collisionDetector(droneModel, terrainModel);
    collisionDetector.createTerrainGrid();

    // Starting position
    drone.setPosition({301.373, 50.2463, 256.307});
    camera.setPosition({301.373, 50.2463, 256.307});

    const float32 bBoxScale = 20.0f;

    // TODO constrain drone max x and z to something like (700, 700)
    // TODO enable/disable spotlight
    // TODO add other objects in the scene(and show light with spotlight)
    // TODO maybe add some drone physics

    while (!glfwWindowShouldClose(window))
    {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        glp::Vec3<float32> oldPosition = drone.getPosition();
        glp::Quat<float32> oldOrientation = drone.getOrientation();

        // User Input
        if (lookAtMode) {
            processInput(window, drone, deltaTime);
            drone.updatePosition();
            drone.updateOrientation();
        } else {
            processInput(window, camera, deltaTime);
            cameraView = camera.getView();
        }

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (collisionDetector.hasCollided(drone.getPosition(), bBoxScale)) {
            drone.setPosition(oldPosition);
            drone.setOrientation(oldOrientation);
        }

        // Update spotlight position and direction
        spotLight.position = drone.getPosition();

        // As light is specified from the hit point  we use -y, +z
        spotLight.direction = drone.getLightDirection();

        // Set new camera position and direction
        cameraPosition = camera.getPosition();
        cameraFrontDirection = camera.getFrontDirection();

        // Drone
        droneProgram.use();
        droneProgram.setVec3("spotLight.position", spotLight.position);
        droneProgram.setVec3("spotLight.direction", spotLight.direction);

        glp::Mat4<float32> model = drone.getModel().dot(glp::rotateY(180.0f)).dot(glp::scale(10.0f));
        auto lookAt = drone.getLookAt();
        auto lookAtPosition = lookAt.first;

        lookAtView = lookAt.second;

        if (lookAtMode) {
            droneProgram.setVec3("viewPos", lookAt.first);
            droneProgram.setMat4("view", lookAtView);
        } else {
            droneProgram.setVec3("viewPos", cameraPosition);
            droneProgram.setMat4("view", cameraView);
        }

        droneModel.getNodes()[0].model = model;
        droneModel.getNodes()[3].model = glp::translate(0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(-0.25f, -0.0f, -0.0f));
        droneModel.getNodes()[5].model = glp::translate(-0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(+0.25f, -0.0f, -0.0f));

        droneModel.updateModelMatrices();
        droneModel.draw(droneProgram);

        droneModel.resetModelMatrices();

        // Debug
    //     cubeShader.use();
    //     cubeShader.setMat4("model", glp::translate(drone.getPosition()).dot(glp::scale(bBoxScale)));
    //     if (lookAtMode) {
    //         cubeShader.setVec3("viewPos", lookAt.first);
    //         cubeShader.setMat4("view", lookAtView);
    //     } else {
    //         cubeShader.setVec3("viewPos", cameraPosition);
    //         cubeShader.setMat4("view", cameraView);
    //     }
    //    // collisionDetector.debug();

        // Terrain
        terrainProgram.use();
        terrainProgram.setVec3("spotLight.position", spotLight.position);
        terrainProgram.setVec3("spotLight.direction", spotLight.direction);

        if (lookAtMode) {
            terrainProgram.setVec3("viewPos", lookAt.first);
            terrainProgram.setMat4("view", lookAtView);
        } else {
            terrainProgram.setVec3("viewPos", cameraPosition);
            terrainProgram.setMat4("view", cameraView);
        }

        // TODO CARE THEY ARE SET IN MESH NOW
        // glp::Mat4<float32> terrainModel = glp::translate(0.0f, -0.75f, 0.0f);
        // terrainProgram.setMat4("model", terrainModel);
        // terrainProgram.setMat3("normalMatrix", terrainModel.mat3().transposedInverse());

        terrainModel.getNodes()[0].model = glp::translate<float32>(0, 0, 1000);
        terrainModel.updateModelMatrices();
        terrainModel.draw(terrainProgram);
        terrainModel.resetModelMatrices();

        // Skybox
        skyboxProgram.use();

        if (lookAtMode) {
            // Remove translation from the view matrix
            lookAtView.w0 = 0; lookAtView.w1 = 0; lookAtView.w2 = 0; lookAtView.w3 = 1;
            lookAtView.x3 = 0; lookAtView.y3 = 0; lookAtView.z3 = 0;

            skyboxProgram.setMat4("view", lookAtView);
        } else {
            // Remove translation from the view matrix
            cameraView.w0 = 0; cameraView.w1 = 0; cameraView.w2 = 0; cameraView.w3 = 1;
            cameraView.x3 = 0; cameraView.y3 = 0; cameraView.z3 = 0;

            skyboxProgram.setMat4("view", cameraView);
        }

        skybox.draw(skyboxProgram);

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

void cameraModeCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        lookAtMode = !lookAtMode;
    }
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

void processInput(GLFWwindow *window, glp::Drone<float32> &drone, float32 deltaTime)
{
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        drone.updateDeltaPosition(glp::DroneMovement::DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::PITCH_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::PITCH_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::YAW_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::YAW_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::ROLL_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        drone.updateDeltaOrientation(glp::DroneMovement::ROLL_M, deltaTime);
    }
}