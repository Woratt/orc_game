#ifndef KEY_H
#define KEY_H

#include<iostream>
#include <random>


#include "SFML/Graphics.hpp"
#include "Player.h"
#include "../tinyxml2/tinyxml2.h"
#include "TmxLevel.h"

using namespace tinyxml2;

class Player;

class Key
{
public:
    float x, y;
    static size_t quantity;
    sf::Image image;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    sf::FloatRect keyHitbox;
    Key(const std::string&, const TmxLevel&);
    void update(Player*, const std::vector<TmxObject>&);
    sf::Vector2f getCoordinate();
    ~Key();

    
};


#endif