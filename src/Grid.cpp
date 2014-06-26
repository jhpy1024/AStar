#include "Grid.hpp"

Grid::Grid(int numNodes, const sf::Vector2i& gridSize)
    : NUM_NODES(numNodes)
    , GRID_SIZE(gridSize)
    , m_Nodes(NUM_NODES, std::vector<Node>(NUM_NODES, Node({ -1, -1 }, { 0, 0 })))
{
    createNodes();
    createLines();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    drawNodes(target, states);
    drawLines(target, states);
}

void Grid::setNodeColor(const sf::Vector2i& position, const sf::Color& color)
{
    m_Nodes[position.x][position.y].setColor(color);
}

sf::Vector2i Grid::getGridSize() const
{
    return GRID_SIZE;
}

sf::Vector2i Grid::getNodeSize() const
{
    return { GRID_SIZE.x / NUM_NODES, GRID_SIZE.y / NUM_NODES };
}

void Grid::reset()
{
    createNodes();
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

void Grid::createLines()
{
    for (int x = 0; x < NUM_NODES; ++x)
    {
        for (int y = 0; y < NUM_NODES; ++y)
        {
            sf::RectangleShape shape;
            shape.setFillColor(sf::Color::Black);
            shape.setPosition(x * (GRID_SIZE.x / NUM_NODES), y * (GRID_SIZE.y / NUM_NODES));

            // Horizontal line
            shape.setSize({ GRID_SIZE.x, 1.f });
            m_Lines.push_back(shape);

            // Vertical line
            shape.setSize({ 1.f, GRID_SIZE.y });
            m_Lines.push_back(shape);
        }
    }
}

void Grid::drawNodes(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int x = 0; x < NUM_NODES; ++x)
    {
        for (int y = 0; y < NUM_NODES; ++y)
        {
            target.draw(m_Nodes[x][y]);
        }
    }
}

void Grid::drawLines(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& line : m_Lines)
        target.draw(line);
}
