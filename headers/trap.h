#ifndef TRAP_H
#define TRAP_H

#include <SFML/Graphics.hpp>


#include "Player.h"
#include "TmxLevel.h"
#include "fire.h"


class Trap
{
private:
    sf::Texture texture;
    sf::RectangleShape hitbox;
    float currentFrame = 0, frameSpeed = 8.f, x, y, timeOfAttac = 0.f, updateAnimation = 0.f, randomTimeOfAttac;
    int kindOfAnimation = 0, k = -1;
    std::unique_ptr<sf::Sprite> sprite;
    bool isActive = false;
public:
    static std::vector<std::pair<float, float>> coordinate;
    Trap(float, float);
    void update(float);
    sf::Sprite draw();
    sf::RectangleShape getHitbox();
    bool getIsActive();
};

#endif