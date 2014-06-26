#ifndef GRID_HPP
#define GRID_HPP

#include "Node.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
public:
    Grid(int numNodes, const sf::Vector2i& gridSize);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setStartPosition(const sf::Vector2i& position);
    void setEndPosition(const sf::Vector2i& position);
    void addWall(const sf::Vector2i& position);

    sf::Vector2i getGridSize() const;
    sf::Vector2i getNodeSize() const;

    void reset();

private:
    void createNodes();
    void createLines();

    void drawNodes(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawLines(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    const int NUM_NODES;
    const sf::Vector2i GRID_SIZE;

    std::vector<std::vector<Node>> m_Nodes;
    std::vector<sf::RectangleShape> m_Lines;

    sf::Vector2i m_StartPosition;
    sf::Vector2i m_EndPosition;

    std::vector<sf::Vector2i> m_Walls;
};

#endif
