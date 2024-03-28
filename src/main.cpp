
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

static bool firstMouseEvent;
static float lastXMousePos;
static float lastYMousePos;

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoff, double yoff);

core::Camera *Camera;

class Snake : public core::Game
{
    core::Texture *FaceTexture{nullptr};
    core::Texture *BoxTexture{nullptr};
    core::Shader *Shader{nullptr};

    unsigned int VBO, VAO;
    std::vector<glm::vec3> cubePositions;

public:
    Snake() : core::Game("Snake", SCR_WIDTH, SCR_HEIGHT)
    {
        lastXMousePos = SCR_WIDTH / 2.0f;
        lastYMousePos = SCR_HEIGHT / 2.0f;

        Camera = new core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        Shader = new core::Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");

        glfwSetCursorPosCallback(Win->It, mouseCallback);
        glfwSetScrollCallback(Win->It, scrollCallback);

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

        FaceTexture = new core::Texture(1);

        FaceTexture
            ->setWrapX(core::WrapParams::Repeat)
            .setWrapY(core::WrapParams::Repeat)
            .setMinFilter(core::FilterParams::LinearMipmapLinear)
            .setMagFilter(core::FilterParams::Linear)
            .loadImage("assets/images/awesomeface.png", GL_RGBA, GL_RGBA);

        BoxTexture = new core::Texture();

        BoxTexture
            ->setWrapX(core::WrapParams::Repeat)
            .setWrapY(core::WrapParams::Repeat)
            .setMinFilter(core::FilterParams::LinearMipmapLinear)
            .setMagFilter(core::FilterParams::Linear)
            .loadImage("assets/images/container.jpg", GL_RGB, GL_RGB);

        Shader->use();
        Shader->setInt("texture1", 0);
        Shader->setInt("texture2", 1);

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
    }

    ~Snake()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete Camera;
        delete Shader;
        delete FaceTexture;
        delete BoxTexture;
    }

    void Tick(float delta) override
    {
        if (IsPressing(GLFW_KEY_ESCAPE))
            Close();
        if (IsPressing(GLFW_KEY_W))
            Camera->Move(core::CameraMovement::FORWARD, delta);
        if (IsPressing(GLFW_KEY_S))
            Camera->Move(core::CameraMovement::BACKWARD, delta);
        if (IsPressing(GLFW_KEY_A))
            Camera->Move(core::CameraMovement::LEFT, delta);
        if (IsPressing(GLFW_KEY_D))
            Camera->Move(core::CameraMovement::RIGHT, delta);
    }

    void Render() override
    {
        BoxTexture->use();
        FaceTexture->use();
        Shader->use();

        glm::mat4 view = Camera->GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(Camera->Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        Shader->setMatrix("view", view);
        Shader->setMatrix("projection", projection);

        glBindVertexArray(VAO);

        for (unsigned int i{0}; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.0f));
            Shader->setMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
    }

    static void OnScroll(float xOffset, float yOffset)
    {
        Camera->Zoom(yOffset);
    }

    static void OnMouseMovement(float xOffset, float yOffset)
    {
        Camera->Look(xOffset, yOffset, true);
    }
};

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouseEvent)
    {
        lastXMousePos = xpos;
        lastYMousePos = ypos;
        firstMouseEvent = false;
    }

    float xoffset = xpos - lastXMousePos;
    float yoffset = lastYMousePos - ypos;

    lastXMousePos = xpos;
    lastYMousePos = ypos;

    Snake::OnMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoff, double yoff)
{
    Snake::OnScroll(static_cast<float>(xoff), static_cast<float>(yoff));
}

int main(int argc, char **argv)
{
    Snake snakeGame;

    snakeGame.Start();
}
