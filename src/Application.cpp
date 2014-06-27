#include "Application.hpp"

Application::Application(int width, int height, int numNodes)
    : WIDTH(width)
    , HEIGHT(height)
    , NUM_NODES(numNodes)
    , m_Window(sf::VideoMode(WIDTH, HEIGHT), "Set Start Position", sf::Style::Close)
    , m_Grid(NUM_NODES, { WIDTH, HEIGHT })
    , m_IsStartSet(false)
    , m_IsEndSet(false)
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
            case sf::Event::KeyPressed:
                handleKeyPress(event);
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
    auto gridX = event.mouseButton.x / m_Grid.getNodeSize().x;
    auto gridY = event.mouseButton.y / m_Grid.getNodeSize().y;

    if (event.mouseButton.button == sf::Mouse::Left)
    {
        if (!m_IsStartSet)
        {
            m_IsStartSet = true;
            m_Window.setTitle("Set End Position");
            m_Grid.setStartPosition({ gridX, gridY });
        }
        else if (!m_IsEndSet)
        {
            m_IsEndSet = true;
            m_Window.setTitle("Add Walls (Left Click) | Remove Walls (Right Click)");
            m_Grid.setEndPosition({ gridX, gridY });
        }
        else
        {
            m_Grid.addWall({ gridX, gridY });
        }
    }
    else if (event.mouseButton.button == sf::Mouse::Right)
    {
        m_Grid.removeWall({ gridX, gridY });
    }
}

void Application::handleKeyPress(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Space)
    {
        beginSearch();
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
        reset();
    }
}

void Application::beginSearch()
{
    m_Grid.beginSearch();
}

void Application::reset()
{
    m_Window.setTitle("Set Start Position");

    m_IsStartSet = false;
    m_IsEndSet = false;

    m_Grid.reset();
}
