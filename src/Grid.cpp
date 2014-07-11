#include "Grid.hpp"

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

Grid::Grid(int numNodes, const sf::Vector2i& gridSize)
    : m_NumNodes(numNodes)
    , GRID_SIZE(gridSize)
    , HORIZONTAL_COST(10)
    , VERTICAL_COST(10)
    , DIAGONAL_COST(14)
    , m_Nodes(m_NumNodes, std::vector<Node>(m_NumNodes, Node({ -1, -1 }, { 0, 0 })))
    , m_StartPosition(-1, -1)
    , m_EndPosition(-1, -1)
    , m_HasFoundPath(false)
{
    createNodes();
    createLines();
}

Grid::Grid(const std::string& file, const sf::Vector2i& gridSize)
    : GRID_SIZE(gridSize)
    , HORIZONTAL_COST(10)
    , VERTICAL_COST(10)
    , DIAGONAL_COST(14)
    , m_StartPosition(-1, -1)
    , m_EndPosition(-1, -1)
    , m_HasFoundPath(false)

{
    // Multiply by 2 and add 1 because we need
    // extra columns and rows for the walls
    m_NumNodes = parseNumNodes(file) * 2 + 1;
    m_Nodes = std::vector<std::vector<Node>>(m_NumNodes, std::vector<Node>(m_NumNodes, Node({ -1, -1 }, { 0, 0 })));

    createNodes();
    createLines();

    parseNodes(file);
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
    if (std::find(m_Walls.begin(), m_Walls.end(), position) == m_Walls.end())
    {
        m_Walls.push_back(position);
        m_Nodes[position.x][position.y].setColor(sf::Color::Black);
    }
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
    return { GRID_SIZE.x / m_NumNodes, GRID_SIZE.y / m_NumNodes };
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
    for (int x = 0; x < m_NumNodes; ++x)
    {
        for (int y = 0; y < m_NumNodes; ++y)
        {
            sf::Vector2i position(x, y);
            sf::Vector2i size(GRID_SIZE.x / m_NumNodes, GRID_SIZE.y / m_NumNodes);

            Node node(position, size);
            m_Nodes[x][y] = node;
        }
    }
}

void Grid::createLines()
{
    for (int x = 0; x < m_NumNodes; ++x)
    {
        for (int y = 0; y < m_NumNodes; ++y)
        {
            sf::RectangleShape shape;
            shape.setFillColor(sf::Color::Black);
            shape.setPosition(x * (GRID_SIZE.x / m_NumNodes), y * (GRID_SIZE.y / m_NumNodes));

            // Horizontal line
            shape.setSize({ GRID_SIZE.x, 1.f });
            m_Lines.push_back(shape);

            // Vertical line
            shape.setSize({ 1.f, GRID_SIZE.y });
            m_Lines.push_back(shape);
        }
    }
}

void Grid::parseNodes(const std::string& file)
{
    auto nodeStrings = extractNodes(file);

    // Add initial walls
    for (int i = 0; i < m_NumNodes; ++i)
    {
        addWall({ i, 0 });
        addWall({ 0, i });
        addWall({ i, m_NumNodes - 1 });
        addWall({ m_NumNodes - 1, i });
    }

    for (unsigned x = 0; x < nodeStrings.size(); ++x)
    {
        for (unsigned y = 0; y < nodeStrings[x].size(); ++y)
        {
            // TODO:
            //      If two adjacent cells have a common wall, add that wall between those two cells
            bool hasNorthWall = (nodeStrings[y][x][0] == '0');
            bool hasEastWall = (nodeStrings[y][x][1] == '0');
            bool hasSouthWall = (nodeStrings[y][x][2] == '0');
            bool hasWestWall = (nodeStrings[y][x][3] == '0');

            auto cellX = x * 2 + 1;
            auto cellY = y * 2 + 1;

            sf::Vector2i north = { cellX, cellY - 1 },
                         northeast = { cellX + 1, cellY - 1 }, 
                         east = { cellX + 1, cellY },
                         southeast = { cellX + 1, cellY + 1 },
                         south = { cellX, cellY + 1 },
                         southwest = { cellX - 1, cellY + 1 },
                         west = { cellX - 1, cellY },
                         northwest = { cellX - 1, cellY - 1 };
            
            if ((hasNorthWall && hasEastWall) || (hasEastWall && hasNorthWall))
            {
                addWall(north); addWall(northeast); addWall(east);
            }
            if ((hasNorthWall && hasWestWall) || (hasWestWall && hasNorthWall))
            {
                addWall(north); addWall(northwest); addWall(west);
            }
            if ((hasSouthWall && hasEastWall) || (hasEastWall && hasSouthWall))
            {
                addWall(south); addWall(southeast); addWall(east);
            }
            if ((hasSouthWall && hasWestWall) || (hasWestWall && hasSouthWall))
            {  
                addWall(south); addWall(southwest); addWall(west);
            }
            if (hasNorthWall)
            {
                addWall(north);

                if ((x != 0) && (x != nodeStrings.size() - 1))
                {
                    // If the cell to the left/right also has a north
                    // wall, add a wall inbetween the two cells
                    if (nodeStrings[y][x-1][0] == '0')
                        addWall({ cellX - 1, cellY - 1 });
                    if (nodeStrings[y][x+1][0] == '0')
                        addWall({ cellX + 1, cellY - 1 });
                }
            }
            if (hasEastWall)
            {
                addWall(east);

                if ((y != 0) && (y != nodeStrings.size() - 1))
                {
                    // If the cell above/below also has a east
                    // wall, add a wall inbetween the two cells
                    if (nodeStrings[y-1][x][1] == '0')
                        addWall({ cellX + 1, cellY - 1 });
                    if (nodeStrings[y+1][x][2] == '0')
                        addWall({ cellX + 1, cellY + 1 });
                }
            }
            if (hasSouthWall)
            {
                addWall(south);

                if ((x != 0) && (x != nodeStrings.size() - 1))
                {
                    // If the cell to the left/right also has a south
                    // wall, add a wall inbetween the two cells
                    if (nodeStrings[y][x-1][2] == '0')
                        addWall({ cellX - 1, cellY + 1 });
                    if (nodeStrings[y][x+1][2] == '0')
                        addWall({ cellX + 1, cellY + 1 });
                }

            }
            if (hasWestWall)
            {
                addWall(west);

                if ((y != 0) && (y != nodeStrings.size() - 1))
                {
                    // If the cell above/below also has a west
                    // wall, add a wall inbetween the two cells
                    if (nodeStrings[y-1][x][3] == '0')
                        addWall({ cellX - 1, cellY - 1 });
                    if (nodeStrings[y+1][x][3] == '0')
                        addWall({ cellX - 1, cellY + 1 });
                }
            }
        }
    }
}

std::vector<std::vector<std::string>> Grid::extractNodes(const std::string& file)
{
    std::vector<std::vector<std::string>> nodeStrings;

    std::ifstream inputFile(file);
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::vector<std::string> row;

        std::stringstream strStream(line);
        std::string node;
        while (std::getline(strStream, node, ' '))
        {
            row.push_back(node);
        }

        nodeStrings.push_back(row);
    }

    return nodeStrings;
}

int Grid::parseNumNodes(const std::string& file) const
{
    int numNodes = 0;

    std::ifstream inputFile(file);
    
    std::string line;
    if (std::getline(inputFile, line))
    {
        std::stringstream strStream(line);
        std::string node;
        while (std::getline(strStream, node, ' '))
            ++numNodes;
    }
    else
    {
        std::cerr << "Error parsing file, it has no lines!" << std::endl;
    }
    
    std::printf("Num nodes = %i\n", numNodes);

    return numNodes;
}

void Grid::drawNodes(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int x = 0; x < m_NumNodes; ++x)
    {
        for (int y = 0; y < m_NumNodes; ++y)
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
        || (node.x == m_NumNodes - 1)
        || (node.y == 0)
        || (node.y == m_NumNodes - 1);
}

bool Grid::isNodeOnLeft(const sf::Vector2i& node) const
{
    return node.x == 0;
}

bool Grid::isNodeOnRight(const sf::Vector2i& node) const
{
    return node.x == m_NumNodes - 1;
}

bool Grid::isNodeOnTop(const sf::Vector2i& node) const
{
    return node.y == 0;
}

bool Grid::isNodeOnBottom(const sf::Vector2i& node) const
{
    return node.y == m_NumNodes - 1;
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
