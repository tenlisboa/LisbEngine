
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

        Game(std::string gameName, int screenWidth, int screenHeigth)
        {
            Win = new core::Window(gameName, screenWidth, screenHeigth);
            Win->Open();
        }

        ~Game()
        {
        }

        void Start()
        {
            glEnable(GL_DEPTH_TEST);
            glfwSetInputMode(Win->It, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            while (Win->IsOpen())
            {
                Tick(mDelta);

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                Render();

                glfwSwapBuffers(Win->It);
                glfwPollEvents();

                calculateDelta(static_cast<float>(glfwGetTime()));
            }
        }

        void Close()
        {
            Win->Close();
        }

        bool IsPressing(int key)
        {
            return glfwGetKey(Win->It, key) == GLFW_PRESS;
        }

        static void OnMouseMovement(float xOffset, float yOffset);
        static void OnScroll(float xOffset, float yOffset);
        virtual void Tick(float delta) = 0;
        virtual void Render() = 0;

    private:
        float mDelta{0.0f};
        float mLastFrameTime{0.0f};

        void calculateDelta(float currentFrame)
        {
            mDelta = currentFrame - mLastFrameTime;
            mLastFrameTime = currentFrame;
        }
    };
} // namespace core

#endif // LISBENGINE_GAME_HPP
