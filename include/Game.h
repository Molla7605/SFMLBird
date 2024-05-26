#ifndef GAME_H
#define GAME_H

#include "MainHeader.h"
#include "Background.h"
#include "Wall.h"
#include "Bird.h"

class Game {
public:
    Game();

    bool isEnded() const;

    void processKeyInput(const sf::Keyboard::Scancode& key, bool is_pressed);
    void update(const sf::Time& delta_time);

    void restart();

private:
    Bird m_player;

    Background m_background;
    sf::Texture m_background_texture;

};

#endif // !GAME_H
