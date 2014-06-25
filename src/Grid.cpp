#include "Grid.hpp"

Grid::Grid(int numNodes)
    : NUM_NODES(numNodes)
    , m_Nodes(NUM_NODES, std::vector<Node>(NUM_NODES, Node({ -1, -1 })))
{
    createNodes();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void Grid::createNodes()
{
    for (int x = 0; x < NUM_NODES; ++x)
    {
        for (int y = 0; y < NUM_NODES; ++y)
        {
            Node node({ x, y });
            m_Nodes[x][y] = node;
        }
    }
}
