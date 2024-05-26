#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "MainHeader.h"

class AnimatedSprite : public sf::Transformable, public sf::Drawable {
public:
    AnimatedSprite();
	AnimatedSprite(sf::Texture& texture, std::size_t frame_count, sf::Time animation_speed);

    void update(const sf::Time& delta_time);

    void setTexture(sf::Texture& texture, std::size_t frame_count);
    const sf::Texture* getTexture() const;
    std::size_t getFrameCount() const;

    void setAnimationSpeed(const sf::Time& speed);
    const sf::Time& getAnimationSpeed() const;

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    void pause();
    void resume();

    bool isPaused() const;

	void resetAnimation();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateVertex();
    void updateTexcoords();

    std::size_t m_frame_count;
    std::size_t m_frame_counter;

    sf::Time m_time;
    sf::Time m_animation_speed;

    sf::Texture* m_texture;
    sf::FloatRect m_sprite_rect;

    sf::VertexArray m_vertex_array;
};

#endif // !ANIMATED_SPRITE_H
