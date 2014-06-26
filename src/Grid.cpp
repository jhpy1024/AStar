#include "Grid.hpp"

#include <cassert>

Grid::Grid(int numNodes, const sf::Vector2i& gridSize)
    : NUM_NODES(numNodes)
    , GRID_SIZE(gridSize)
    , m_Nodes(NUM_NODES, std::vector<Node>(NUM_NODES, Node({ -1, -1 }, { 0, 0 })))
    , m_StartPosition(-1, -1)
    , m_EndPosition(-1, -1)
{
    createNodes();
    createLines();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    drawNodes(target, states);
    drawLines(target, states);
}

void Grid::setStartPosition(const sf::Vector2i& position)
{
    m_StartPosition = position;
    m_Nodes[position.x][position.y].setColor(sf::Color::Green);
}

void Grid::setEndPosition(const sf::Vector2i& position)
{
    m_EndPosition = position;
    m_Nodes[position.x][position.y].setColor(sf::Color::Red);
}

void Grid::addWall(const sf::Vector2i& position)
{
    m_Walls.push_back(position);
    m_Nodes[position.x][position.y].setColor(sf::Color::Cyan);
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

    m_Walls.clear();
}

void Grid::beginSearch()
{
    std::vector<sf::Vector2i> closedSet;
    std::vector<sf::Vector2i> openSet = { m_StartPosition };

    auto& startNode = m_Nodes[m_StartPosition.x][m_StartPosition.y];
    startNode.setMovementCost(0);
    startNode.setHeuristicCost(calculateHeuristicCost(m_StartPosition, m_EndPosition));
    startNode.setScore(startNode.getMovementCost() + startNode.getHeuristicCost());

    while (!openSet.empty())
    {
        auto currentNodePosition = getLowestScoredNode(openSet);
        if (currentNodePosition == m_EndPosition)
            ;// TODO: reconstruct path

        openSet.erase(std::find(openSet.begin(), openSet.end(), currentNodePosition));
        closedSet.push_back(currentNodePosition);

        for (auto& neighbor : getNeighborNodes(currentNodePosition))
        {
            if (closedSet.find(neighbor) != closedSet.end())
                continue;

            auto& currentNode = m_Nodes[currentNodePosition.x][currentNodePosition.y];
            auto tentativeMovementCost = currentNode.getMovementCost() + calculateDistance()
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

int Grid::calculateHeuristicCost(const sf::Vector2i& from, const sf::Vector2i& to) const
{
    auto deltaX = std::abs(to.x - from.x);
    auto deltaY = std::abs(to.y - from.y);
    auto heuristicCost = std::ceil((deltaX + deltaY) / 2);

    return heuristicCost;
}

sf::Vector2i Grid::getLowestScoredNode(const std::vector<sf::Vector2i>& nodes) const
{
    assert(nodes.size() > 0);

    sf::Vector2i lowest = nodes[0];

    for (auto& node : nodes)
    {
        if (m_Nodes[node.x][node.y].getScore() < m_Nodes[lowest.x][lowest.y].getScore())
            lowest = node;
    }

    return lowest;
}

std::vector<sf::Vector2i> Grid::getNeighborNodes(const sf::Vector2i& node) const
{
    std::vector<sf::Vector2i> neighbors;

    if (!isNodeOnEdge(node))
    {
        neighbors.push_back({ node.x, node.y - 1 });        // North
        neighbors.push_back({ node.x + 1, node.y - 1 });    // North East
        neighbors.push_back({ node.x + 1, node.y });        // East
        neighbors.push_back({ node.x + 1, node.y + 1 });    // South East
        neighbors.push_back({ node.x, node.y + 1 });        // South
        neighbors.push_back({ node.x - 1, node.y - 1 });    // South West
        neighbors.push_back({ node.x - 1, node.y });        // West
        neighbors.push_back({ node.x - 1, node.y + 1 });    // North West
    }
    else if (isNodeOnLeft(node))
    {
        if (isNodeOnTop(node))
        {
            neighbors.push_back({ node.x + 1, node.y });        // East
            neighbors.push_back({ node.x + 1, node.y + 1 });    // South East
            neighbors.push_back({ node.x, node.y + 1 });        // South
        }
        else if (isNodeOnBottom(node))
        {
            neighbors.push_back({ node.x, node.y - 1 });        // North
            neighbors.push_back({ node.x + 1, node.y - 1 });    // North East
            neighbors.push_back({ node.x + 1, node.y });        // East
        }
        else
        {
            neighbors.push_back({ node.x, node.y - 1 });        // North
            neighbors.push_back({ node.x, node.y - 1 });        // North East
            neighbors.push_back({ node.x + 1, node.y });        // East
            neighbors.push_back({ node.x + 1, node.y + 1 });    // South East
            neighbors.push_back({ node.x, node.y + 1 });        // South
        }
    }
    else if (isNodeOnRight(node))
    {
        if (isNodeOnTop(node))
        {
            neighbors.push_back({ node.x, node.y + 1 });        // South
            neighbors.push_back({ node.x - 1, node.y + 1 });    // South West
            neighbors.push_back({ node.x - 1, node.y });        // West
        }
        else if (isNodeOnBottom(node))
        {
            neighbors.push_back({ node.x - 1, node.y });        // West
            neighbors.push_back({ node.x - 1, node.y - 1 });    // North West
            neighbors.push_back({ node.x, node.y - 1 });        // North
        }
        else
        {
            neighbors.push_back({ node.x, node.y + 1 });        // South
            neighbors.push_back({ node.x - 1, node.y + 1 });    // South West
            neighbors.push_back({ node.x - 1, node.y });        // West
            neighbors.push_back({ node.x - 1, node.y - 1 });    // North West
            neighbors.push_back({ node.x, node.y - 1 });        // North
        }
    }
    else if (isNodeOnTop(node))
    {
        if (isNodeOnLeft(node))
        {
            neighbors.push_back({ node.x + 1, node.y });        // East
            neighbors.push_back({ node.x + 1, node.y + 1 });    // South East
            neighbors.push_back({ node.x, node.y + 1 });        // South
        }
        else if (isNodeOnRight(node))
        {
            neighbors.push_back({ node.x, node.y + 1 });        // South
            neighbors.push_back({ node.x - 1, node.y + 1 });    // South West
            neighbors.push_back({ node.x - 1, node.y });        // West
        }
        else
        {
            neighbors.push_back({ node.x + 1, node.y });        // East
            neighbors.push_back({ node.x + 1, node.y + 1 });    // South East
            neighbors.push_back({ node.x, node.y + 1 });        // South
            neighbors.push_back({ node.x - 1, node.y + 1 });    // South West
            neighbors.push_back({ node.x - 1, node.y });        // West
        }
    }
    else if (isNodeOnBottom(node))
    {
        if (isNodeOnLeft(node))
        {
            neighbors.push_back({ node.x, node.y - 1 });        // North
            neighbors.push_back({ node.x + 1, node.y - 1 });    // North East
            neighbors.push_back({ node.x + 1, node.y });        // East
        }
        else if (isNodeOnRight(node))
        {
            neighbors.push_back({ node.x - 1, node.y });        // West
            neighbors.push_back({ node.x - 1, node.y - 1 });    // North West
            neighbors.push_back({ node.x, node.y - 1 });        // North
        }
        else
        {
            neighbors.push_back({ node.x - 1, node.y });        // West
            neighbors.push_back({ node.x - 1, node.y - 1 });    // North West
            neighbors.push_back({ node.x, node.y - 1 });        // North
            neighbors.push_back({ node.x + 1, node.y - 1 });    // North East
            neighbors.push_back({ node.x + 1, node.y });        // East
        }
    }

    return neighbors;
}

bool Grid::isNodeOnEdge(const sf::Vector2i& node) const
{
    return (node.x != 0)
        && (node.x != NUM_NODES - 1)
        && (node.y != 0)
        && (node.y != NUM_NODES - 1);
}

bool Grid::isNodeOnLeft(const sf::Vector2i& node) const
{
    return node.x == 0;
}

bool Grid::isNodeOnRight(const sf::Vector2i& node) const
{
    return node.x == NUM_NODES - 1;
}

bool Grid::isNodeOnTop(const sf::Vector2i& node) const
{
    return node.y == 0;
}

bool Grid::isNodeOnBottom(const sf::Vector2i& node) const
{
    return node.y == NUM_NODES - 1;
}
