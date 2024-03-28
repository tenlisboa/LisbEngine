
#include <GLFW/glfw3.h>
#include <core/window.hpp>
#include <string>
#include <functional>

#if !defined(LISBENGINE_GAME_HPP)
#define LISBENGINE_GAME_HPP

namespace core
{
    class Game
    {
    public:
        Window *Win{nullptr};
        Game *CurrentGame{nullptr};

        Game(std::string gameName, int screenWidth, int screenHeigth)
        {
            mLastXMousePos = screenWidth / 2.0f;
            mLastYMousePos = screenHeigth / 2.0f;
            core::Window w(gameName, screenWidth, screenHeigth);
            w.Open();
            Win = &w;
        }

        ~Game()
        {
            Close();
        }

        void Start()
        {
            CurrentGame = &(*this);

            glEnable(GL_DEPTH_TEST);
            glfwSetInputMode(Win->It, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // TODO: Maybe something related with Window Config

            // TODO: It can be some abstraction of inputs or something
            // auto prevP = glfwGetWindowUserPointer(Win->It);
            // glfwSetWindowUserPointer(Win->It, &CurrentGame);
            // glfwSetCursorPosCallback(Win->It, [](GLFWwindow *w, double x, double y)
            //                          { static_cast<Game *>(glfwGetWindowUserPointer(w))->mouseCallback(w, x, y); });
            // glfwSetScrollCallback(Win->It, [](GLFWwindow *w, double x, double y)
            //                       { static_cast<Game *>(glfwGetWindowUserPointer(w))->OnScroll((float)x, (float)y); });
            // glfwSetWindowUserPointer(Win->It, prevP);

            while (Win->IsOpen())
            {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                Tick(mDelta);
                Render();

                glfwSwapBuffers(Win->It);
                glfwPollEvents();

                calculateDelta(static_cast<float>(glfwGetTime()));
            }
        }

        void Close()
        {
            CurrentGame = nullptr;
            Win->Close();
        }

        bool IsPressing(int key)
        {
            return glfwGetKey(Win->It, key) == GLFW_PRESS;
        }

        virtual void OnMouseMovement(float xOffset, float yOffset) = 0;
        virtual void OnScroll(float xOffset, float yOffset) = 0;
        virtual void Tick(float delta) = 0;
        virtual void Render() = 0;

    private:
        float mDelta{0.0f};
        float mLastFrameTime{0.0f};

        bool mFirstMouseEvent{true};
        float mLastXMousePos;
        float mLastYMousePos;

        void calculateDelta(float currentFrame)
        {
            mDelta = currentFrame - mLastFrameTime;
            mLastFrameTime = currentFrame;
        }

        void mouseCallback(GLFWwindow *window, double xpos, double ypos)
        {
            if (mFirstMouseEvent)
            {
                mLastXMousePos = xpos;
                mLastYMousePos = ypos;
                mFirstMouseEvent = false;
            }

            float xoffset = xpos - mLastXMousePos;
            float yoffset = mLastYMousePos - ypos;

            mLastXMousePos = xpos;
            mLastYMousePos = ypos;

            OnMouseMovement(xoffset, yoffset);
        }
    };
} // namespace core

#endif // LISBENGINE_GAME_HPP
