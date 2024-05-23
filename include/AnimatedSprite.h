#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "MainHeader.h"

class AnimatedSprite : public sf::Transformable, public sf::Drawable {
public:
	AnimatedSprite(sf::Texture& texture, sf::Vector2u frame_count, sf::Time animation_speed);

    void update(const sf::Time& delta_time);

    bool setTexture(sf::Texture& texture, sf::Vector2u frame_count);
    const sf::Texture* getTexture() const noexcept;
    const sf::Vector2u& getFrameCount() const noexcept;

    void setAnimationSpeed(const sf::Time& speed);
    const sf::Time& getAnimationSpeed() const noexcept;

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

	void resetAnimation();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateVertex() const;
    void updateTexcoords() const;

    mutable bool m_texcoords_need_update;
    mutable bool m_vertex_need_update;

    sf::Vector2u m_frame_count;
    std::size_t m_frame_counter;

    sf::Time m_animation_time;
    sf::Time m_animation_speed;

    sf::Texture* m_texture;
    mutable sf::FloatRect m_sprite_rect;

	mutable sf::VertexArray m_vertex_array;
};

#endif // !ANIMATED_SPRITE_H
