#include "Application.hpp"

Application::Application(int width, int height, int numNodes)
    : WIDTH(width)
    , HEIGHT(height)
    , NUM_NODES(numNodes)
    , m_Window(sf::VideoMode(WIDTH, HEIGHT), "Set Start Position", sf::Style::Close)
    , m_Grid(NUM_NODES, { WIDTH, HEIGHT })
    , m_IsStartSet(false)
    , m_IsEndSet(false)
    , m_StartPosition(-1, -1)
    , m_EndPosition(-1, -1)
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
            case sf::Event::MouseButtonPressed:
                handleMousePress(event);
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

void Application::handleMousePress(const sf::Event& event)
{
    if (event.mouseButton.button == sf::Mouse::Left)
    {
        auto gridX = event.mouseButton.x / m_Grid.getNodeSize().x;
        auto gridY = event.mouseButton.y / m_Grid.getNodeSize().y;

        if (!m_IsStartSet)
        {
            m_StartPosition = { gridX, gridY };
            m_IsStartSet = true;
            m_Window.setTitle("Set End Position");
            m_Grid.setNodeColor(m_StartPosition, sf::Color::Green);
        }
        else if (!m_IsEndSet)
        {
            m_EndPosition = { gridX, gridY };
            m_IsEndSet = true;
            m_Window.setTitle("Add Walls");
            m_Grid.setNodeColor(m_EndPosition, sf::Color::Red);
        }
        else
        {
            m_Walls.push_back({ gridX, gridY });
            m_Grid.setNodeColor({ gridX, gridY }, sf::Color::Blue);
        }
    }
}
