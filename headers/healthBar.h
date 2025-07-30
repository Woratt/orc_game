#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <list>


#include "Enemy.h"


class healthBar{
private:
    std::shared_ptr<Enemy> enemy;
    std::unique_ptr<sf::Sprite> sprite;
    int maxHealt, health;
    sf::RectangleShape bar;
    sf::Texture texture;
public:
    healthBar(std::shared_ptr<Enemy>);
    sf::Sprite draw();
    bool isDraw();
    void update();
    sf::RectangleShape& returnBar();
    bool isLiveli();
};

#endif