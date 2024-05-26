#include "Bird.h"
#include "ResourcePath.h"

const sf::FloatRect Bird::PLAYER_BOUND {{-25.0f, -25.0f}, {50.0f, 50.0f}};

Bird::Bird() :
    m_angle_min_max(-50.0f, 90.0f),
    m_gravity(2000.0f),
    m_angle(0.0f),
    m_angle_velocity(0.0f),
    m_velocity(),
    m_jump_scale(500.0f),
    m_bird_texture(),
    m_jump_sound(),
    m_sound_buffer(),
    m_sprite()
{
    m_bird_texture.loadFromFile(BIRD_IMAGE_PATH);
    m_sprite.setTexture(m_bird_texture, 5);
    m_sprite.setAnimationSpeed(sf::milliseconds(200));
    m_sprite.scale(0.3f, 0.3f);

    m_sound_buffer.loadFromFile(JUMP_SOUND_PATH);
    m_jump_sound.setBuffer(m_sound_buffer);
    m_jump_sound.setVolume(2.0f);

    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.0f);
}

void Bird::jump() noexcept {
    m_velocity.y = -m_jump_scale;
    m_jump_sound.play();
}

void Bird::update(const sf::Time &delta_time) {
    m_velocity.y += m_gravity * delta_time.asSeconds();

    m_angle_velocity = ((m_jump_scale / m_velocity.y == 0.0f ? 0.01f : m_velocity.y) * 0.4f) - 50.0f;
    m_angle += m_angle_velocity * delta_time.asSeconds();
    m_angle = std::clamp(m_angle, m_angle_min_max.x, m_angle_min_max.y);


    move(m_velocity * delta_time.asSeconds());
    m_sprite.setRotation(m_angle);

    m_sprite.update(delta_time);
}

sf::FloatRect Bird::getGlobalBounds() const noexcept {
    return getTransform().transformRect(getLocalBounds());
}

sf::FloatRect Bird::getLocalBounds() const noexcept {
    return PLAYER_BOUND;
}

void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(m_sprite, states);
}
