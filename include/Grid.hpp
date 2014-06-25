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

private:
    void createNodes();

private:
    const int NUM_NODES;
    const sf::Vector2i GRID_SIZE;

    std::vector<std::vector<Node>> m_Nodes;
};

#endif
