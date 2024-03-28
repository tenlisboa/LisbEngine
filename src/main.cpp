
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/window.hpp"
#include "core/game.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "core/camera.hpp"
#include <math.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Snake : public core::Game
{
    core::Texture *faceTexture{nullptr};
    core::Texture *boxTexture{nullptr};
    core::Shader *shader{nullptr};
    core::Camera *camera{nullptr};

    unsigned int VBO, VAO;
    std::vector<glm::vec3> cubePositions;

public:
    Snake() : core::Game("Snake", SCR_WIDTH, SCR_HEIGHT)
    {
        camera = new core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        shader = new core::Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");

        faceTexture = new core::Texture(1);

        faceTexture
            ->setWrapX(core::WrapParams::Repeat)
            .setWrapY(core::WrapParams::Repeat)
            .setMinFilter(core::FilterParams::LinearMipmapLinear)
            .setMagFilter(core::FilterParams::Linear)
            .loadImage("assets/images/awesomeface.png", GL_RGBA, GL_RGBA);

        boxTexture = new core::Texture();

        boxTexture
            ->setWrapX(core::WrapParams::Repeat)
            .setWrapY(core::WrapParams::Repeat)
            .setMinFilter(core::FilterParams::LinearMipmapLinear)
            .setMagFilter(core::FilterParams::Linear)
            .loadImage("assets/images/container.jpg", GL_RGB, GL_RGB);

        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)};

        shader->use();
        shader->setInt("texture1", 0);
        shader->setInt("texture2", 1);
    }

    ~Snake()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Tick(float delta) override
    {
        if (IsPressing(GLFW_KEY_ESCAPE))
            Close();
        if (IsPressing(GLFW_KEY_W))
            camera->Move(core::CameraMovement::FORWARD, delta);
        if (IsPressing(GLFW_KEY_S))
            camera->Move(core::CameraMovement::BACKWARD, delta);
        if (IsPressing(GLFW_KEY_A))
            camera->Move(core::CameraMovement::LEFT, delta);
        if (IsPressing(GLFW_KEY_D))
            camera->Move(core::CameraMovement::RIGHT, delta);
    }

    void Render() override
    {
        boxTexture->use();
        faceTexture->use();
        shader->use();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(camera->Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader->setMatrix("view", view);
        shader->setMatrix("projection", projection);

        glBindVertexArray(VAO);

        for (unsigned int i{0}; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.0f));
            shader->setMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
    }

    void OnScroll(float xOffset, float yOffset) override
    {
        camera->Zoom(yOffset);
    }

    void OnMouseMovement(float xOffset, float yOffset) override
    {
        camera->Look(xOffset, yOffset, true);
    }
};

core::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

float deltaTime{0.0f};
float lastFrame{0.0f};

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.Look(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.Zoom((float)yoffset);
}

int main(int argc, char **argv)
{
    // Snake snakeGame;

    // snakeGame.Start();
    core::Window window("LisbEngine", SCR_WIDTH, SCR_HEIGHT);
    window.Open();

    core::Shader shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");

    // Set up vertex data and configure vertex attrib
    // ---------------------------------------------------

    // TODO: Maybe something related with Window Config
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window.It, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // TODO: It can be some abstraction of inputs or something
    glfwSetCursorPosCallback(window.It, mouse_callback);
    glfwSetScrollCallback(window.It, scroll_callback);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load faceTexture
    // -------------------------------------
    core::Texture faceTexture(1);

    faceTexture
        .setWrapX(core::WrapParams::Repeat)
        .setWrapY(core::WrapParams::Repeat)
        .setMinFilter(core::FilterParams::LinearMipmapLinear)
        .setMagFilter(core::FilterParams::Linear)
        .loadImage("assets/images/awesomeface.png", GL_RGBA, GL_RGBA);

    // Load boxTexture
    // ----------------------------------------------------
    core::Texture boxTexture;

    boxTexture
        .setWrapX(core::WrapParams::Repeat)
        .setWrapY(core::WrapParams::Repeat)
        .setMinFilter(core::FilterParams::LinearMipmapLinear)
        .setMagFilter(core::FilterParams::Linear)
        .loadImage("assets/images/container.jpg", GL_RGB, GL_RGB);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (window.IsOpen())
    {
        if (glfwGetKey(window.It, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.Close();
        if (glfwGetKey(window.It, GLFW_KEY_W) == GLFW_PRESS)
            camera.Move(core::CameraMovement::FORWARD, deltaTime);
        if (glfwGetKey(window.It, GLFW_KEY_S) == GLFW_PRESS)
            camera.Move(core::CameraMovement::BACKWARD, deltaTime);
        if (glfwGetKey(window.It, GLFW_KEY_A) == GLFW_PRESS)
            camera.Move(core::CameraMovement::LEFT, deltaTime);
        if (glfwGetKey(window.It, GLFW_KEY_D) == GLFW_PRESS)
            camera.Move(core::CameraMovement::RIGHT, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        boxTexture.use();
        faceTexture.use();
        shader.use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMatrix("view", view);
        shader.setMatrix("projection", projection);

        glBindVertexArray(VAO);

        for (unsigned int i{0}; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.0f));
            shader.setMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window.It);
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // Free resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminate cleaning all previous allocate GLFW resources
    glfwTerminate();
    return 0;
}
