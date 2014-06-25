#include "Node.hpp"

Node::Node(const sf::Vector2i& position)
    : m_Position(position)
    , m_ParentPosition(-1, -1)
    , m_Score(0)
    , m_MovementCost(0)
    , m_HeuristicCost(0)
{

}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

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
