#include "Application.hpp"

Application::Application(int width, int height, int numNodes)
    : WIDTH(width)
    , HEIGHT(height)
    , NUM_NODES(numNodes)
    , m_Window(sf::VideoMode(WIDTH, HEIGHT), "A* Algorithm", sf::Style::Close)
    , m_Grid(NUM_NODES, { WIDTH, HEIGHT })
{

}

void Application::run()
{
    while (m_Window.isOpen())
    {
        handleInput();
        update();
        draw();
    }
}

void Application::handleInput()
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_Window.close();
                break;
            default:
                break;
        }
    }
}

void Application::update()
{

}

void Application::draw()
{
    m_Window.clear();
    m_Window.draw(m_Grid);
    m_Window.display();
}
