#include "../headers/startGame.h"
#include <iostream>

std::string resourcePath = Start::getResourcePath();

int main() {
    sf::RenderWindow window(sf::VideoMode({640, 480}), "Orc Game");

    window.setActive(true);

    Start::start(window);
    
    return 0;
}