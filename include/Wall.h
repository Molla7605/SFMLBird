#ifndef WALL_H
#define WALL_H

#include "MainHeader.h"

class Wall {
public:
    Wall(float hole_length);

    bool test(const sf::FloatRect& rect) const;

private:
    sf::Texture m_wall_texture;
    sf::Sprite m_up;
    sf::Sprite m_bottom;

};

#endif // !WALL_H
