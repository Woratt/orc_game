#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>
#include <iostream>

extern sf::View view;
extern float currentZoom;

sf::View updateView(sf::Vector2f coordinate);
void moveView(int dir, float time);
void cangeView();
float getCurrentZoom();
#endif