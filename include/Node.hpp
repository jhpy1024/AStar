#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp>

class Node : public sf::Drawable
{
public:
    explicit Node(const sf::Vector2i& position);

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

private:
    sf::Vector2i m_Position;
    sf::Vector2i m_ParentPosition;

    int m_Score;
    int m_MovementCost;
    int m_HeuristicCost;

    static sf::Font m_Font;
    sf::Text m_ScoreText;
    sf::Text m_MovementCostText;
    sf::Text m_HeuristicCostText;
};

#endif
