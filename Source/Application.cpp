#include "Application.h"

#include <iostream>
#include <chrono>
#include <SFML/System/Clock.hpp>

#include "Display.h"

#include "States/SPlaying.h"

namespace
{
    void checkFps ()
    {
        static sf::Clock timer;
        static sf::Clock printTimer;
        static auto numFrames = 0;

        numFrames++;

        if (printTimer.getElapsedTime().asSeconds() >= 1.0f)
        {
            auto fps = (float)numFrames / timer.getElapsedTime().asSeconds();
            printTimer.restart();
            std::cout << fps << std::endl;

            numFrames = 0;
            timer.restart();
        }
    }

}

Application::Application()
{
    pushState(std::make_unique<State::Playing>(*this));
    m_camera.position.y = World_Constants::CH_SIZE * 9;
    m_camera.position.x = World_Constants::CH_SIZE / 2;
    m_camera.position.z = World_Constants::CH_SIZE / 2;
}

void Application::runMainGameLoop()
{
    sf::Clock clock;
    auto begin = std::chrono::system_clock::now();
    auto frameCount = 0;

    while (Display::isOpen())
    {
        checkFps ();
        m_states.top()->input   (m_camera);
        /*
        while(std::chrono::duration<double>
             (std::chrono::system_clock::now() - begin).count() <
              frameCount * 0.010)*/
        {
            auto dt = clock.restart().asSeconds();
            m_states.top()->update  (m_camera, dt);
        }
        m_renderer.clear();
        m_states.top()->draw    (m_renderer);

        m_renderer.update(m_camera);
        Display::checkForClose();
        ++frameCount;
    }
}

void Application::pushState(std::unique_ptr<State::Game_State> state)
{
    m_states.push(std::move(state));
}

void Application::popState()
{
    m_states.pop();
}

const Camera& Application::getCamera()
{
    return m_camera;
}
