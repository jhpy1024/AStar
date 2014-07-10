#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Grid.hpp"

#include <SFML/Graphics.hpp>

class Application
{
public:
    Application(int width, int height, int numNodes);
    Application(int width, int height, const std::string& file);

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
    int m_Width;
    int m_Height;
    int m_NumNodes;

    sf::RenderWindow m_Window;

    Grid m_Grid;

    bool m_IsStartSet;
    bool m_IsEndSet;

    bool m_LoadedFile;
    std::string m_File;
};

#endif
