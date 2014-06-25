#include "Grid.hpp"

Grid::Grid(int numNodes, const sf::Vector2i& gridSize)
    : NUM_NODES(numNodes)
    , GRID_SIZE(gridSize)
    , m_Nodes(NUM_NODES, std::vector<Node>(NUM_NODES, Node({ -1, -1 }, { 0, 0 })))
{
    createNodes();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int x = 0; x < NUM_NODES; ++x)
    {
        for (int y = 0; y < NUM_NODES; ++y)
        {
            target.draw(m_Nodes[x][y]);
        }
    }
}

void Grid::createNodes()
{
    for (int x = 0; x < NUM_NODES; ++x)
    {
        for (int y = 0; y < NUM_NODES; ++y)
        {
            sf::Vector2i position(x, y);
            sf::Vector2i size(GRID_SIZE.x / NUM_NODES, GRID_SIZE.y / NUM_NODES);

            Node node(position, size);
            m_Nodes[x][y] = node;
        }
    }
}
