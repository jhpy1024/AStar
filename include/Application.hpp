#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Grid.hpp"

#include <SFML/Graphics.hpp>

class Application
{
public:
    Application(int width, int height, int numNodes);

    void run();
    void handleInput();
    void update();
    void draw();

private:
    void handleMousePress(const sf::Event& event);
    void handleKeyPress(const sf::Event& event);

    void beginSearch();
    void reset();

private:
    const int WIDTH;
    const int HEIGHT;
    const int NUM_NODES;

    sf::RenderWindow m_Window;

    Grid m_Grid;

    bool m_IsStartSet;
    bool m_IsEndSet;

    sf::Vector2i m_StartPosition;
    sf::Vector2i m_EndPosition;
    std::vector<sf::Vector2i> m_Walls;
};

#endif
