#include <iostream>
#include <utility>
#include <exceptions/base.h>

#ifndef LISBENGINE_WINDOW_H
#define LISBENGINE_WINDOW_H

const unsigned int DEFAULT_SCREEN_WIDTH = 800;
const unsigned int DEFAULT_SCREEN_HEIGHT = 600;

namespace core
{

    class Window
    {
    public:
        GLFWwindow *It;

        int ScreenWidth, ScreenHeight;
        std::string WindowName;

        Window(std::string name) : ScreenWidth(DEFAULT_SCREEN_WIDTH), ScreenHeight(DEFAULT_SCREEN_HEIGHT)
        {
            WindowName = std::move(name);
            mInitializeWindow();
        }

        Window(std::string name, const int width, const int height)
        {
            WindowName = std::move(name);
            ScreenWidth = width;
            ScreenHeight = height;

            mInitializeWindow();
        }

        ~Window()
        {
            Close();
            glfwTerminate();
        }

        void Open()
        {
            It = glfwCreateWindow(ScreenWidth, ScreenHeight, WindowName.c_str(), nullptr, nullptr);

            if (It == nullptr)
            {
                glfwTerminate();
                throw ExceptionBase("Failed to create GLFW window");
            }
            glfwMakeContextCurrent(It);
            glfwSetFramebufferSizeCallback(It, Window::mResizeWindowCallback);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw ExceptionBase("Failed to load GLAD");
            }
        }

        void Close() const
        {
            glfwSetWindowShouldClose(It, true);
        }

        [[nodiscard]] bool IsOpen() const
        {
            return !glfwWindowShouldClose(It);
        }

    private:
        void mInitializeWindow()
        {
            if (GL_TRUE != glfwInit())
                throw ExceptionBase("Deu merda grandao");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        }

        static void mResizeWindowCallback(GLFWwindow *_window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    };
}

#endif // LISBENGINE_WINDOW_H
