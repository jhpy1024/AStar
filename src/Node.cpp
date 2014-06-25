#include "Node.hpp"

Node::Node(const sf::Vector2i& position, const sf::Vector2i& size)
    : m_Position(position)
    , m_ParentPosition(-1, -1)
    , m_Score(0)
    , m_MovementCost(0)
    , m_HeuristicCost(0)
    , m_Size(size)
    , m_Shape(sf::Vector2f(size))
{
    m_Shape.setPosition(sf::Vector2f(position.x * size.x, position.y * size.y));
    m_Shape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Shape);
}

sf::Vector2i Node::getPosition() const
{
    return m_Position;
}

sf::Vector2i Node::getParentPosition() const
{
    return m_ParentPosition;
}

int Node::getScore() const
{
    return m_Score;
}

int Node::getMovementCost() const
{
    return m_MovementCost;
}

int Node::getHeuristicCost() const
{
    return m_HeuristicCost;
}

void Node::setParentPosition(const sf::Vector2i& parentPosition)
{
    m_ParentPosition = parentPosition;
}

void Node::setScore(int score)
{
    m_Score = score;
    m_ScoreText.setString("F=" + std::to_string(score));
}

void Node::setMovementCost(int movementCost)
{
    m_MovementCost = movementCost;
    m_MovementCostText.setString("G=" + std::to_string(movementCost));
}

void Node::setHeuristicCost(int heuristicCost)
{
    m_HeuristicCost = heuristicCost;
    m_HeuristicCostText.setString("H=" + std::to_string(heuristicCost));
}
