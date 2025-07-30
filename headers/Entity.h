#ifndef ENTITY_H
#define ENTITY_H

#include<iostream>
#include <vector>


#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "TmxLevel.h"


class Entity{
protected:
    std::vector<TmxObject> obj;
    float dx = 0, dy = 0, currentFrame = 0, frameSpead = 8.f, spead = 1000.f, x, y, recharging = 0;
    int health = 100;
    int pos = -1;
    bool isLiveli = true, inRound = true;

    std::string file;
    sf::Image image;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    

public:
    virtual void isColitionWithMap() = 0;
    virtual void takeDamage(int, float) = 0;
    virtual void death(float) = 0; 
    virtual void animationOfTakeDamage(float) = 0;
    virtual bool getIsLiveli() = 0; 
    virtual sf::Sprite draw() = 0;
    virtual sf::Vector2f getCoordinate() const = 0;
    virtual void staticAnimation(float) = 0;
    virtual bool getInRound() const = 0;

    virtual ~Entity() = 0;
};

#endif