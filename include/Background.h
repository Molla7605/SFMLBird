#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "MainHeader.h"

class Background : public sf::Transformable, public sf::Drawable {
public:
    Background();
    Background(sf::Texture& texture, const sf::IntRect& screen_size, float scroll_speed);

    void setTexture(sf::Texture& texture);
    const sf::Texture* getTexture() const;

    void setScreenSize(const sf::IntRect& screen_size);
    const sf::FloatRect& getScreenSize() const;

    void setScrollSpeed(float speed);
    float getScrollSpeed() const;

    void update(const sf::Time& delta_time);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateBackground();

    bool m_background_need_update;

    sf::Texture* m_texture;
    std::deque<sf::Sprite> m_sprites;
    sf::FloatRect m_screen;
    float m_scroll_speed;

};

#endif // !BACKGROUND_H
