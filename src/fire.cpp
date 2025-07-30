#include "../headers/fire.h"

extern std::string resourcePath;

std::vector<std::pair<float, float>> fire::coordinate = {{416, 208}, {412, 320}, {912, 64}, {380, 490}, {1376, 208}, {1200, 208}, {904, 544}, {1064, 544}, {1296, 976}, {1396, 1104}, {1504, 528}, {752, 1104}, {352, 1072}, {408, 864}, {296, 710}, {672, 512}, {192, 512}, {48, 896}, {224, 1152}};

fire::fire(float x, float y): x(x), y(y){
    if(!texture.loadFromFile(resourcePath + "image/fire.png")){
        std::cerr << "Failed to load image in fire\n";
    };
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({88, 0}, {44, 48}));
    sprite->setPosition({x, y});
    hitbox.setSize({23, 23});
    hitbox.setPosition({x + 10, y + 12});
}

void fire::update(float time){
    currentFrame += frameSpead * time;
    if(currentFrame > 6) currentFrame -= 6;
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({88, 48 * int(currentFrame)}, {44, 46}));
    sprite->setPosition({x, y});
}

sf::Sprite fire::draw(){
    return *sprite;
}

sf::RectangleShape fire::getHitbox(){
    return hitbox;
}

sf::Vector2f fire::getCoordinate(){
    return {x + 22, y + 24};
}