#include "../headers/view.h"

sf::View view(sf::FloatRect({0.f, 0.f}, {640.f, 480.f}));
float currentZoom = 1.0f;

sf::View updateView(sf::Vector2f coordinate) {
    view.move(coordinate);
    return view;
}

void moveView(int dir, float time) {
    float x = 0, y = 0, spead = 1000.f;
    switch (dir) {
        case 0: y = -spead; break;
        case 1: x = spead;  break;
        case 2: y = spead;  break;
        case 3: x = -spead; break;
    }
    view.move({x * time, y * time});
}

void cangeView() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9) && currentZoom < 1.f) {
            view.zoom(1.01);
            currentZoom *= 1.01f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8) && currentZoom > 0.32f) {
            view.zoom(0.98f);
            currentZoom *= 0.98f;
        }

    }

}

float getCurrentZoom(){
    return currentZoom;
}