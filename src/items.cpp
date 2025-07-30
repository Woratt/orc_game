#include "../headers/items.h"

extern std::string resourcePath;

Items::Items(float x, float y, int type): x(x), y(y), type(type){
    if(!texture.loadFromFile(resourcePath + "image/items.png")){
        std::cerr << "Failed to load image in items\n";
    };
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({0, 64 * type}, {64, 58}));
    sprite->setPosition({x, y});
    sprite->setScale({0.3, 0.3});
    hitbox.setSize({10, 10});
    hitbox.setPosition({x, y});
}

sf::Sprite& Items::draw(){
    return *sprite;
}

int Items::getType(){
    return type;
}

void Items::update(float time){
    if(k){
        currentFrame += frameSpeed * time;
    }else{
        currentFrame -= frameSpeed * time;
    }
    if(currentFrame > 3.9){
        k = false;
    }else if(currentFrame < 0.1){
        k = true;
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({64 * int(currentFrame), 58 * type}, {64, 58}));
    sprite->setScale({0.3, 0.3});
    sprite->setPosition({x, y});
}

sf::RectangleShape Items::getHitbox(){
    return hitbox;
}


