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

class Window {
public:
    GLFWwindow* it { nullptr };

    Window() {
        windowName = "";
        screenHeight = 0;
        screenWidth = 0;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    Window& setSize(const int width, const int height) {
        screenWidth = width;
        screenHeight = height;

        return *this;
    }

    Window& setName(std::string name) {
        windowName = std::move(name);

        return *this;
    }

    Window& open() {
        it = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), nullptr, nullptr);

        if (it == nullptr) {
            glfwTerminate();
            throw ExceptionBase("Failed to create GLFW window");

        }
        glfwMakeContextCurrent(it);
        glfwSetFramebufferSizeCallback(it, Window::resizeWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw ExceptionBase("Failed to load GLAD");
        }

        return *this;
    }

    void close() const {
        glfwSetWindowShouldClose(it, true);
    }

    [[nodiscard]] bool isOpen() const {
        return !glfwWindowShouldClose(it);
    }

private:
    int screenWidth, screenHeight;
    std::string windowName;

    static void resizeWindow(GLFWwindow* _window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif //LISBENGINE_WINDOW_H
