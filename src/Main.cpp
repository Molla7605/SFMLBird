#include "MainHeader.h"
#include "Background.h"
#include "Bird.h"
#include "ResourcePath.h"

static const sf::Vector2u window_size(400, 800);

int main() {
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "SFMLBird", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    Bird player;
    player.setPosition(static_cast<sf::Vector2f>(window_size).x / 2.0f, 0.0f);

    sf::Texture background_texture;
    if (!background_texture.loadFromFile(BACKGROUND_IMAGE_PATH)) {
        return -1;
    }

    Background background(background_texture,
                          {{0, 0}, static_cast<sf::Vector2i>(window_size)},
                          100.0f);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time delta_time = clock.restart();

        for (sf::Event event; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                player.jump();
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                auto view = window.getView();

                view.setSize(view.getSize() * (event.mouseWheelScroll.delta == 1.0f ? 0.9f : 1.1f));

                window.setView(view);
            }
        }

        background.update(delta_time);
        player.update(delta_time);

        sf::RectangleShape window_rect(player.getGlobalBounds().getSize());
        window_rect.setPosition(player.getGlobalBounds().getPosition());
        window_rect.setOutlineThickness(3.0f);
        window_rect.setFillColor(sf::Color::Transparent);
        window_rect.setOutlineColor(sf::Color::Red);

        window.clear();

        window.draw(background);
        window.draw(player);
        //window.draw(window_rect);

        window.display();
    }

    return 0;
}
