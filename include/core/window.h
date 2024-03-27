//
// Created by Gabriel Lisboa on 21/03/24.
//

#ifndef LISBENGINE_WINDOW_H
#define LISBENGINE_WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "exceptions/base.h"
#include <iostream>
#include <utility>

const unsigned int DEFAULT_SCREEN_WIDTH = 800;
const unsigned int DEFAULT_SCREEN_HEIGHT = 600;

class Window
{
public:
    GLFWwindow *it{nullptr};

    Window(std::string name) : screenWidth(DEFAULT_SCREEN_WIDTH), screenHeight(DEFAULT_SCREEN_HEIGHT)
    {
        windowName = std::move(name);

        initializeWindow();
    }

    Window(std::string name, const int width, const int height)
    {
        windowName = std::move(name);
        screenWidth = width;
        screenHeight = height;

        initializeWindow();
    }

    void open()
    {
        it = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), nullptr, nullptr);

        if (it == nullptr)
        {
            glfwTerminate();
            throw ExceptionBase("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(it);
        glfwSetFramebufferSizeCallback(it, Window::resizeWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw ExceptionBase("Failed to load GLAD");
        }
    }

    void close() const
    {
        glfwSetWindowShouldClose(it, true);
    }

    [[nodiscard]] bool isOpen() const
    {
        return !glfwWindowShouldClose(it);
    }

private:
    int screenWidth, screenHeight;
    std::string windowName;

    void initializeWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    static void resizeWindow(GLFWwindow *_window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};

#endif // LISBENGINE_WINDOW_H
