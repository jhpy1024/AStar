#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp>

class Node : public sf::Drawable
{
public:
    Node(const sf::Vector2i& position, const sf::Vector2i& size);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2i getPosition() const;
    sf::Vector2i getParentPosition() const;
    int getScore() const;
    int getMovementCost() const;
    int getHeuristicCost() const;

    void setParentPosition(const sf::Vector2i& parentPosition);
    void setScore(int score);
    void setMovementCost(int movementCost);
    void setHeuristicCost(int heuristicCost);
    void setColor(const sf::Color& color);

private:
    void createShape();

private:
    sf::Vector2i m_Position;
    sf::Vector2i m_ParentPosition;

    int m_Score;
    int m_MovementCost;
    int m_HeuristicCost;

    sf::Vector2i m_Size;
    sf::RectangleShape m_Shape;
};

#endif
