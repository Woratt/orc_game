#ifndef ITEMS_H
#define ITEMS_H

#include "SFML/Graphics.hpp"
#include <iostream>


class Items{
private:
    sf::Texture texture;
    sf::RectangleShape hitbox;
    float currentFrame = 0, frameSpeed = 8.f, x, y, timeOfAttac = 0.f, updateAnimation = 0.f, randomTimeOfAttac;
    std::unique_ptr<sf::Sprite> sprite;
    bool k = true;
    int type;
public:
    Items(float, float, int);
    void update(float);
    sf::Sprite& draw();
    sf::RectangleShape getHitbox();
    int getType();
};

#endif