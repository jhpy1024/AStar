#include "Grid.hpp"

#include <cassert>

Grid::Grid(int numNodes, const sf::Vector2i& gridSize)
    : NUM_NODES(numNodes)
    , GRID_SIZE(gridSize)
    , HORIZONTAL_COST(10)
    , VERTICAL_COST(10)
    , DIAGONAL_COST(14)
    , m_Nodes(NUM_NODES, std::vector<Node>(NUM_NODES, Node({ -1, -1 }, { 0, 0 })))
    , m_StartPosition(-1, -1)
    , m_EndPosition(-1, -1)
    , m_HasFoundPath(false)
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

void Grid::removeWall(const sf::Vector2i& position)
{
    if (std::find(m_Walls.begin(), m_Walls.end(), position) != m_Walls.end())
    {
        m_Walls.erase(std::find(m_Walls.begin(), m_Walls.end(), position));
        m_Nodes[position.x][position.y].setColor(sf::Color::White);
    }
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
    m_Path.clear();

    m_HasFoundPath = false;
}

void Grid::beginSearch()
{
    std::vector<sf::Vector2i> closedSet = m_Walls;
    std::vector<sf::Vector2i> openSet = { m_StartPosition };

    auto& startNode = m_Nodes[m_StartPosition.x][m_StartPosition.y];
    startNode.setMovementCost(0);
    startNode.setHeuristicCost(calculateHeuristicCost(m_StartPosition, m_EndPosition));
    startNode.setScore(startNode.getMovementCost() + startNode.getHeuristicCost());

    while (!openSet.empty())
    {
        auto currentNodePosition = getLowestScoredNode(openSet);
        if (currentNodePosition == m_EndPosition)
        {
            std::printf("Found path: ");
            m_HasFoundPath = true;
            buildPath(m_EndPosition);
            printPath();
            colorPath();
            break;
        }

        openSet.erase(std::find(openSet.begin(), openSet.end(), currentNodePosition));
        closedSet.push_back(currentNodePosition);

        for (auto& neighborPosition : getNeighborNodes(currentNodePosition))
        {
            if (std::find(closedSet.begin(), closedSet.end(), neighborPosition) != closedSet.end())
                continue;

            auto& currentNode = m_Nodes[currentNodePosition.x][currentNodePosition.y];
            auto tentativeMovementCost = currentNode.getMovementCost()
                                       + calculateMovementCost(currentNodePosition, neighborPosition);

            bool neighborInOpenSet = (std::find(openSet.begin(), openSet.end(), neighborPosition) != openSet.end());
            auto& neighborNode = m_Nodes[neighborPosition.x][neighborPosition.y];
            if (!neighborInOpenSet || (tentativeMovementCost < neighborNode.getMovementCost()))
            {
                neighborNode.setParentPosition(currentNodePosition);
                neighborNode.setMovementCost(tentativeMovementCost);
                neighborNode.setScore(neighborNode.getMovementCost()
                                    + calculateHeuristicCost(neighborPosition, m_EndPosition));

                if (!neighborInOpenSet)
                    openSet.push_back(neighborPosition);
            }
        }
    }

    if (!m_HasFoundPath)
        std::printf("Found no path :(\n");
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
    auto heuristicCost = std::ceil((deltaX + deltaY));

    return heuristicCost;
}

int Grid::calculateMovementCost(const sf::Vector2i& from, const sf::Vector2i& to) const
{
    auto deltaX = std::abs(to.x - from.x);
    auto deltaY = std::abs(to.y - from.y);

    return HORIZONTAL_COST * deltaX + VERTICAL_COST * deltaY;
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
        auto adjNodes = getAdjacentNodes(node,
            { Direction::North, Direction::East, Direction::South, Direction::West });
        neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
    }
    else if (isNodeOnLeft(node))
    {
        if (isNodeOnTop(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::East, Direction::South });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else if (isNodeOnBottom(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::North, Direction::East });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else
        {
            auto adjNodes = getAdjacentNodes(node,
                { Direction::North, Direction::East, Direction::South });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
    }
    else if (isNodeOnRight(node))
    {
        if (isNodeOnTop(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::South, Direction::West });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else if (isNodeOnBottom(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::West, Direction::North });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else
        {
            auto adjNodes = getAdjacentNodes(node,
                { Direction::South, Direction::West, Direction::North });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
    }
    else if (isNodeOnTop(node))
    {
        if (isNodeOnLeft(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::East, Direction::South });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else if (isNodeOnRight(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::South, Direction::West });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else
        {
            auto adjNodes = getAdjacentNodes(node,
                { Direction::East, Direction::South, Direction::West });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
    }
    else if (isNodeOnBottom(node))
    {
        if (isNodeOnLeft(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::North, Direction::East });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else if (isNodeOnRight(node))
        {
            auto adjNodes = getAdjacentNodes(node, { Direction::West, Direction::North });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
        else
        {
            auto adjNodes = getAdjacentNodes(node,
                { Direction::West, Direction::North, Direction::East });
            neighbors.insert(neighbors.end(), adjNodes.begin(), adjNodes.end());
        }
    }

    return neighbors;
}

sf::Vector2i Grid::getAdjacentNode(const sf::Vector2i& from, Direction direction) const
{
    sf::Vector2i adjacentNode(-1, -1);

    switch (direction)
    {
        case Direction::North:
            adjacentNode = { from.x, from.y - 1 };
            break;
        case Direction::NorthEast:
            adjacentNode = { from.x + 1, from.y - 1 };
            break;
        case Direction::East:
            adjacentNode = { from.x + 1, from.y };
            break;
        case Direction::SouthEast:
            adjacentNode = { from.x + 1, from.y + 1 };
            break;
        case Direction::South:
            adjacentNode = { from.x, from.y + 1 };
            break;
        case Direction::SouthWest:
            adjacentNode = { from.x - 1, from.y + 1 };
            break;
        case Direction::West:
            adjacentNode = { from.x - 1, from.y };
            break;
        case Direction::NorthWest:
            adjacentNode = { from.x - 1, from.y - 1 };
            break;
        default:
            break;
    }

    return adjacentNode;
}

std::vector<sf::Vector2i> Grid::getAdjacentNodes(const sf::Vector2i& from, const std::vector<Direction>& directions) const
{
    std::vector<sf::Vector2i> adjacentNodes;

    for (const auto& direction : directions)
    {
        adjacentNodes.push_back(getAdjacentNode(from, direction));
    }

    return adjacentNodes;
}

bool Grid::isNodeOnEdge(const sf::Vector2i& node) const
{
    return (node.x == 0)
        || (node.x == NUM_NODES - 1)
        || (node.y == 0)
        || (node.y == NUM_NODES - 1);
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

void Grid::buildPath(const sf::Vector2i& position)
{
    auto& currentNode = m_Nodes[position.x][position.y];
    auto parentPosition = currentNode.getParentPosition();

    if ((parentPosition.x == -1) && (parentPosition.y == -1))
    {
        m_Path.push_back(position);
        return;
    }

    m_Path.push_back(position);
    buildPath(parentPosition);
}

void Grid::printPath()
{
    for (int i = m_Path.size() - 1; i >= 0; --i)
    {
        std::printf("(%i, %i)", m_Path[i].x, m_Path[i].y);
        if (i > 0)
            std::printf("->");

            }

    std::printf("\n");
}

void Grid::colorPath()
{
    for (int i = m_Path.size() - 1; i >= 0; --i)
    {
        bool isStartCell = (sf::Vector2i(m_Path[i].x, m_Path[i].y) == m_StartPosition);
        bool isEndCell = (sf::Vector2i(m_Path[i].x, m_Path[i].y) == m_EndPosition);

        if (!isStartCell && !isEndCell)
        {
            m_Nodes[m_Path[i].x][m_Path[i].y].setColor(sf::Color::Yellow);
        }
    }
}
