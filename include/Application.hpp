#ifndef APPLICATION_HPP
#define APPLICATION_HPP

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
    const int WIDTH;
    const int HEIGHT;
    const int NUM_NODES;

    sf::RenderWindow m_Window;
};

#endif
