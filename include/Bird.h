#ifndef BIRD_H
#define BIRD_H

#include "MainHeader.h"
#include "AnimatedSprite.h"

class Bird : public sf::Transformable, public sf::Drawable {
public:
    Bird();

    void jump() noexcept;

    void update(const sf::Time& delta_time);

    sf::FloatRect getGlobalBounds() const noexcept;
    sf::FloatRect getLocalBounds() const noexcept;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static const sf::FloatRect PLAYER_BOUND;

    AnimatedSprite m_sprite;
    sf::Texture m_bird_texture;

    sf::Vector2f m_velocity;
    const sf::Vector2f m_angle_min_max;

    const float m_gravity;
    const float m_jump_scale;
    float m_angle_velocity;
    float m_angle;

    sf::SoundBuffer m_sound_buffer;
    sf::Sound m_jump_sound;

};

#endif // !BIRD_H
