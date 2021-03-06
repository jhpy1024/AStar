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
    createShape();
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
}

void Node::setMovementCost(int movementCost)
{
    m_MovementCost = movementCost;
}

void Node::setHeuristicCost(int heuristicCost)
{
    m_HeuristicCost = heuristicCost;
}

void Node::setColor(const sf::Color& color)
{
    m_Shape.setFillColor(color);
}

void Node::createShape()
{
    m_Shape.setPosition(m_Position.x * m_Size.x, m_Position.y * m_Size.y);
    m_Shape.setFillColor(sf::Color::White);
}
