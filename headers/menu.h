#ifndef MENU_H
#define MENU_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <list>


#include "SFML/Graphics.hpp"
#include "view.h"
#include "Player.h"
#include "Enemy.h"
#include "TmxLevel.h"
#include "trap.h"
#include "healthBar.h"


class Trap;

class Menu{
private:
    int kills, keys, maxKills, maxKeys, isSave, PlayerHealth;
    bool isMainMenu = true, InRound, takeItemFire, isOpen = false;
    float x, y, timeShowMesage = 0.f, Opacity, PlayerLightMaxDist, PlayerLightIsActivity, timeOfTakeFire;
    std::vector<std::pair<float, bool>> vectorLight;
    sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
    Player* player;
    Light<Player>* light;
    std::list<std::shared_ptr<Enemy>>* enemys;
    std::list<healthBar>* healthBars;
    TmxLevel* level;
    int menuNum = 0;
    float scaleX, scaleY, posX, posY;
    mutable sf::Text menu1, menu2, menu3, menu4, menu5, menu6, savedMesage, volumeText;
    sf::Font font;
    std::unique_ptr<sf::Sprite> menuBg;
    sf::RenderWindow* window;
    std::vector<Light<fire>>* lights;
    sf::RectangleShape sliderBar;
    sf::CircleShape sliderKnob;
    float volume = 50.f;
    bool isDragging = false;
    sf::Music music; 

public:
    Menu(sf::RenderWindow*, Player*, std::list<std::shared_ptr<Enemy>>*, std::vector<Light<fire>>*, Light<Player>*, TmxLevel*, float, std::vector<Trap>*, std::list<healthBar>*);
    void drawMainMenu(float);
    void drawStatistics();
    void drawPause(float);
    void drawEnd(float);
    void saveData();
    void drawSetings(float);
    bool getIsOpen();
    void updateSliderKnob();
    float getVolume();
};


#endif