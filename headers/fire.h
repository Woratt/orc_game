#ifndef FIRE_H
#define FIRE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class fire{
private:
    sf::Texture texture;
    sf::RectangleShape hitbox;
    float currentFrame = 0, frameSpead = 8.f, x, y;
    std::unique_ptr<sf::Sprite> sprite;
public:
    static std::vector<std::pair<float, float>> coordinate;
    fire(float, float);
    void update(float);
    sf::Sprite draw();
    sf::RectangleShape getHitbox();
    sf::Vector2f getCoordinate();
};

#endif