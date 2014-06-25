#include "Grid.hpp"

Grid::Grid(int numNodes)
    : NUM_NODES(numNodes)
{
    m_Nodes.reserve(NUM_NODES * NUM_NODES);
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}
