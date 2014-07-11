#include "Application.hpp"

Application::Application(int width, int height, int numNodes)
    : m_Width(width)
    , m_Height(height)
    , m_NumNodes(numNodes)
    , m_Window(sf::VideoMode(m_Width, m_Height), "Set Start Position", sf::Style::Close)
    , m_Grid(m_NumNodes, { m_Width, m_Height })
    , m_IsStartSet(false)
    , m_IsEndSet(false)
    , m_LoadedFile(false)
{

}

Application::Application(int width, int height, const std::string& file)
    : m_Width(width)
    , m_Height(height)
    , m_Window(sf::VideoMode(m_Width, m_Height), "Set Start Position", sf::Style::Close)
    , m_Grid(file, { m_Width, m_Height })
    , m_IsStartSet(false)
    , m_IsEndSet(false)
    , m_LoadedFile(true)
    , m_File(file)
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
            if (m_Grid.setStartPosition({ gridX, gridY }))
            {
                m_IsStartSet = true;
                m_Window.setTitle("Set End Position");
            }
        }
        else if (!m_IsEndSet)
        {
            if (m_Grid.setEndPosition({ gridX, gridY }))
            {
                m_IsEndSet = true;
                m_Window.setTitle("Add Walls (Left Click) | Remove Walls (Right Click)");
            }
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
