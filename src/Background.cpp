#include "Background.h"

Background::Background() :
        m_background_need_update(false),
        m_scroll_speed(0.0f),
        m_screen(),
        m_texture(nullptr),
        m_sprites() {}

Background::Background(sf::Texture& texture, const sf::IntRect& screen_size, float scroll_speed) :
        m_background_need_update(true),
        m_scroll_speed(scroll_speed),
        m_screen(screen_size),
        m_texture(&texture),
        m_sprites() {}

void Background::setTexture(sf::Texture& texture) {
    if (m_texture == &texture) return;

    m_texture = &texture;

    m_background_need_update = true;
}

const sf::Texture* Background::getTexture() const {
    return m_texture;
}

void Background::setScreenSize(const sf::IntRect& screen_size) {
    m_screen = static_cast<sf::FloatRect>(screen_size);

    m_background_need_update = true;
}

const sf::FloatRect& Background::getScreenSize() const {
    return m_screen;
}

void Background::setScrollSpeed(float speed) {
    m_scroll_speed = speed;
}

float Background::getScrollSpeed() const {
    return m_scroll_speed;
}

void Background::update(const sf::Time& delta_time) {
    if (m_background_need_update) updateBackground();

    std::size_t invisible_sprite = 0;
    for (auto& sprite : m_sprites) {
        sprite.move(-m_scroll_speed * delta_time.asSeconds(), 0.0f);

        auto sprite_bound = sprite.getGlobalBounds();
        if (m_screen.left > sprite_bound.left + sprite_bound.width) {
            invisible_sprite++;
        }
    }

    for (std::size_t count = 0; count < invisible_sprite; count++) {
        sf::Sprite target = std::move(m_sprites.front());
        m_sprites.pop_front();

        auto sprite_bound = m_sprites.back().getGlobalBounds();
        target.setPosition(sprite_bound.left + sprite_bound.width, 0.0f);

        m_sprites.push_back(target);
    }
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& sprite : m_sprites) {
        target.draw(sprite, states);
    }
}

void Background::updateBackground() {
    if (m_texture == nullptr) return;

    m_sprites.clear();
    auto size = m_screen.getSize();
    sf::Vector2f texture_size = static_cast<sf::Vector2f>(m_texture->getSize());
    float count_scale = size.x / static_cast<float>(texture_size.x);

    std::size_t sprite_count = static_cast<std::size_t>(std::ceil(count_scale * 3));
    for (std::size_t count = 0; count < sprite_count; count++) {
        m_sprites.emplace_back(*m_texture);
        m_sprites.back().move(texture_size.x * static_cast<float>(count), 0.0f);
    }

    m_background_need_update = false;
};
