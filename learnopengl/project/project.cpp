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
#include "model.h"
#include "lights/directional.h"
#include "lights/spot.h"
#include "cubemap.h"
#include "drone.h"
#include "collision.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cameraModeCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime);
void processInput(GLFWwindow *window, glp::Drone<float32> &camera, float32 deltaTime);

template <typename T = float32>
bool isInLand(const glp::Vec3<T>& dronePosition) {
    return dronePosition.x >= 250 && dronePosition.x <= 850 &&
           dronePosition.y <= 400 &&
           dronePosition.z >= 250 && dronePosition.z <= 850;
}

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ComputerGraphicsProject", nullptr, nullptr);

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
    // Skybox
    glp::ShaderProgram skyboxProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/fragment.glsl"
    );

    // Terrain
    glp::ShaderProgram terrainProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/fragment.glsl");

    // Drone
    glp::ShaderProgram droneProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/fragment.glsl"
    );

    // Bounding box(for debugging)
    glp::ShaderProgram cubeShader(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/bbox/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/bbox/fragment.glsl"
    );

    // All the shaders where lights are applied
    std::vector<glp::ShaderProgram*> shadersWithLights;
    shadersWithLights.push_back(&terrainProgram);
    shadersWithLights.push_back(&droneProgram);

    // Models
    // Terrain
    glp::Model terrainModel("/Users/lpraat/develop/computer_graphics/learnopengl/project/models/terrain/terrain.obj");
    terrainModel.getNodes()[1].meshes[0].addTexture("texture_diffuse", "/Users/lpraat/develop/computer_graphics/learnopengl/project/models/terrain/diff2.jpg");
    terrainModel.getNodes()[1].meshes[0].addTexture("texture_normal", "/Users/lpraat/develop/computer_graphics/learnopengl/project/models/terrain/nrm.png");
    //terrainModel.print();

    // Drone
    glp::Model droneModel("/Users/lpraat/develop/computer_graphics/learnopengl/project/models/drone/drone_obj.obj");
    //droneModel.print();

    // Cubemap
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
    // Directional light
    glp::DirectionalLight<float32> directionalLight {
        {0.05f, 0.05f, 0.05f}, // ambient
        {0.4f, 0.4f, 0.4f},    // diffuse
        {0.5f, 0.5f, 0.5f},    // specular
        {0.5f, -0.5f, 0.8f}    // light direction specified as FROM the source
    };

    // Spotlight
    glp::SpotLight<float32> spotLight {
        {0.0f, 0.0f, 0.0f}, // position
        {0.0f, 0.0f, 0.0f}, // direction
        {0.0f, 0.0f, 0.0f}, // ambient
        {1.0f, 1.0f, 1.0f}, // diffuse
        {1.0f, 1.0f, 1.0f}, // specular
        1.0f, // constant decay term
        0.00002f, // linear decay term
        0.0005f, // quadratic decay term
        std::cos(12.5f * glp::toRadians()), // cone in
        std::cos(17.5f * glp::toRadians())  // cone out
    };

    // Set lights to shaders
    for (uint32 i = 0; i < shadersWithLights.size(); i++) {
        glp::ShaderProgram* s = shadersWithLights[i];
        s->use();
        s->setFloat("shininess", 32.0f);

        // Directional light
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

    // Collision detector for the terrain
    glp::CollisionDetector<float32> collisionDetector(droneModel, terrainModel);
    collisionDetector.createTerrainGrid();

    // Enable z-buffer
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // Projection matrix
    glp::Mat4 projection{glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH) / HEIGHT)};

    // View matrices
    glp::Mat4<float32> lookAtView = glp::Mat4<float32>::identity();
    glp::Mat4<float32> cameraView = glp::Mat4<float32>::identity();

    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    glp::Vec3<float32> cameraPosition;
    glp::Vec3<float32> cameraFrontDirection;

    // Starting position
    drone.setPosition({500, 90, 500});
    camera.setPosition({500, 90, 500});

    const float32 bBoxScale = 40.0f;

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

        if (collisionDetector.hasCollided(drone.getPosition(), bBoxScale) || !isInLand(drone.getPosition())) {
            drone.setPosition(oldPosition);
            drone.setOrientation(oldOrientation);
        }

        // Update spotlight position and direction
        spotLight.position = drone.getPosition();

        // As light is specified from the hit point  we use -y, +z
        spotLight.direction = drone.getLightDirection();

        // Set new camera position and direction
        cameraPosition = camera.getPosition();

        // Drone
        droneProgram.use();
        droneProgram.setVec3("spotLight.position", spotLight.position);
        droneProgram.setVec3("spotLight.direction", spotLight.direction);

        glp::Mat4<float32> model = drone.getModel().dot(glp::rotateY(180.0f)).dot(glp::scale(10.0f));
        auto lookAt = drone.getLookAt();
        auto lookAtPosition = lookAt.first;
        lookAtView = lookAt.second;

        droneModel.getNodes()[0].model = model;
        droneModel.getNodes()[3].model = glp::translate(0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(-0.25f, -0.0f, -0.0f));
        droneModel.getNodes()[5].model = glp::translate(-0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(+0.25f, -0.0f, -0.0f));

        droneModel.updateModelMatrices();

        if (lookAtMode) {
            droneProgram.setVec3("viewPos", lookAtPosition);
            droneModel.draw(droneProgram, projection, lookAtView);
        } else {
            droneProgram.setVec3("viewPos", cameraPosition);
            droneModel.draw(droneProgram, projection, cameraView);
        }

        droneModel.resetModelMatrices();

        // // Debug
        // cubeShader.use();
        // cubeShader.setMat4("model", glp::translate(drone.getPosition()).dot(glp::scale(bBoxScale)));
        // if (lookAtMode) {
        //     cubeShader.setVec3("viewPos", lookAt.first);
        //     cubeShader.setMat4("projection", projection);
        //     cubeShader.setMat4("view", lookAtView);
        // } else {
        //     cubeShader.setVec3("viewPos", cameraPosition);
        //     cubeShader.setMat4("projection", projection);
        //     cubeShader.setMat4("view", cameraView);
        // }
        // collisionDetector.debug();

        // Terrain
        terrainProgram.use();
        terrainProgram.setVec3("spotLight.position", spotLight.position);
        terrainProgram.setVec3("spotLight.direction", spotLight.direction);

        terrainModel.getNodes()[0].model = glp::translate<float32>(0, 0, 1000.0f);
        terrainModel.updateModelMatrices();

        if (lookAtMode) {
            terrainProgram.setVec3("viewPos", lookAtPosition);
            terrainModel.draw(terrainProgram, projection, lookAtView);
        } else {
            terrainProgram.setVec3("viewPos", cameraPosition);
            terrainModel.draw(terrainProgram, projection, cameraView);
        }

        terrainModel.resetModelMatrices();

        // Skybox
        skyboxProgram.use();

        if (lookAtMode) {
            // Remove translation from the view matrix
            lookAtView.w0 = 0; lookAtView.w1 = 0; lookAtView.w2 = 0; lookAtView.w3 = 1;
            lookAtView.x3 = 0; lookAtView.y3 = 0; lookAtView.z3 = 0;

            skyboxProgram.setMat4("viewProjection", projection.dot(lookAtView));
        } else {
            // Remove translation from the view matrix
            cameraView.w0 = 0; cameraView.w1 = 0; cameraView.w2 = 0; cameraView.w3 = 1;
            cameraView.x3 = 0; cameraView.y3 = 0; cameraView.z3 = 0;

            skyboxProgram.setMat4("viewProjection", projection.dot(cameraView));
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cameraModeCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        lookAtMode = !lookAtMode;
    }
}

void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime) {
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