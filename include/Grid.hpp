#ifndef GRID_HPP
#define GRID_HPP

#include "Node.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

enum class Direction
{
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

class Grid : public sf::Drawable
{
public:
    Grid(int numNodes, const sf::Vector2i& gridSize);
    Grid(const std::string& file, const sf::Vector2i& gridSize);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setStartPosition(const sf::Vector2i& position);
    void setEndPosition(const sf::Vector2i& position);
    void addWall(const sf::Vector2i& position);
    void removeWall(const sf::Vector2i& position);

    sf::Vector2i getGridSize() const;
    sf::Vector2i getNodeSize() const;

    void reset();

    void beginSearch();

private:
    void createNodes();
    void createLines();

    void parseNodes(const std::string& file);
    std::vector<std::vector<std::string>> extractNodes(const std::string& file);
    int parseNumNodes(const std::string& file) const;

    void drawNodes(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawLines(sf::RenderTarget& target, sf::RenderStates states) const;

    int calculateHeuristicCost(const sf::Vector2i& from, const sf::Vector2i& to) const;
    int calculateMovementCost(const sf::Vector2i& from, const sf::Vector2i& to) const;

    sf::Vector2i getLowestScoredNode(const std::vector<sf::Vector2i>& nodes) const;
    std::vector<sf::Vector2i> getNeighborNodes(const sf::Vector2i& node) const;
    sf::Vector2i getAdjacentNode(const sf::Vector2i& from, Direction direction) const;
    std::vector<sf::Vector2i> getAdjacentNodes(const sf::Vector2i& from, const std::vector<Direction>& directions) const;

    bool isNodeOnEdge(const sf::Vector2i& node) const;
    bool isNodeOnLeft(const sf::Vector2i& node) const;
    bool isNodeOnRight(const sf::Vector2i& node) const;
    bool isNodeOnTop(const sf::Vector2i& node) const;
    bool isNodeOnBottom(const sf::Vector2i& node) const;

    void buildPath(const sf::Vector2i& position);
    void printPath();
    void colorPath();

private:
    int m_NumNodes;
    const sf::Vector2i GRID_SIZE;
    const int HORIZONTAL_COST;
    const int VERTICAL_COST;
    const int DIAGONAL_COST;

    std::vector<std::vector<Node>> m_Nodes;
    std::vector<sf::RectangleShape> m_Lines;

    sf::Vector2i m_StartPosition;
    sf::Vector2i m_EndPosition;

    std::vector<sf::Vector2i> m_Walls;

    bool m_HasFoundPath;
    std::vector<sf::Vector2i> m_Path;
};

#endif
