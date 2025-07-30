#include "../headers/healthBar.h"

extern std::string resourcePath;

healthBar::healthBar(std::shared_ptr<Enemy> enemy): enemy(enemy){
    maxHealt = health = enemy->getHealth();
    if(!texture.loadFromFile(resourcePath + "image/healthBar.png")){
        std::cout << "Failed to load healthBar\n";
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    bar.setFillColor(sf::Color(0, 0, 0));
    sprite->setTextureRect(sf::IntRect({0, 0}, {60, 10}));
}

void healthBar::update(){
    maxHealt = Enemy::firstHealth;
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition({enemy->getCoordinate().x - 35, enemy->getCoordinate().y - 45});
    bar.setPosition({enemy->getCoordinate().x - 35 + 60 - (maxHealt - health) * 60 / maxHealt, enemy->getCoordinate().y - 45});
    if(enemy->getHealth() > 0 && enemy->getHealth() < maxHealt){
        sprite->setTextureRect(sf::IntRect({0, 0}, {60, 10}));
        bar.setSize(sf::Vector2f((maxHealt - health) * 60 / maxHealt, 10));
    }
    health = enemy->getHealth();
}

bool healthBar::isDraw(){
    return health < maxHealt;
}

sf::Sprite healthBar::draw(){
    return *sprite;
}

sf::RectangleShape& healthBar::returnBar(){
    return bar;
}

bool healthBar::isLiveli(){
    return health > 0;
}

