#ifndef GRID_HPP
#define GRID_HPP

#include "Node.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
public:
    Grid(int numNodes);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void createNodes();

private:
    const int NUM_NODES;

    std::vector<std::vector<Node>> m_Nodes;
};

#endif
