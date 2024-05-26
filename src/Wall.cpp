#include "Wall.h"
#include "ResourcePath.h"

Wall::Wall(float hole_length) {
    m_wall_texture.loadFromFile(WALL_IMAGE_PATH);
    m_up.setTexture(m_wall_texture);
    m_bottom.setTexture(m_wall_texture);

    m_up.scale(1.0f, -1.0f);

    m_up.move(0.0f, hole_length / 2.0f);
    m_bottom.move(0.0f, -hole_length / 2.0f);
}

bool Wall::test(const sf::FloatRect& rect) const {
    return m_up.getGlobalBounds().intersects(rect) ||
           m_bottom.getGlobalBounds().intersects(rect);
}
