#ifndef STARTGAME_H
#define STARTGAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>
#include <variant>
#include <list>
#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <CoreFoundation/CoreFoundation.h>


#include "Player.h"
#include "view.h"
#include "key.h"
#include "Enemy.h"
#include "../tinyxml2/tinyxml2.h"
#include "TmxLevel.h"
#include "menu.h"
#include "fire.h"
#include "light.h"
#include "trap.h"
#include "items.h"
#include "healthBar.h"


namespace Start
{
    void drawAndUpdate(sf::RenderWindow&, Player&, Menu&, sf::Clock&,
        TmxLevel&, TmxLevel&, std::vector<Trap>&, std::vector<Items>&,
         std::vector<fire>&, std::list<std::shared_ptr<Enemy>>&, Key&, std::vector<Light<fire>>&,
          sf::RenderTexture&, Light<Player>&, sf::Text&, sf::Text&,
           sf::Text&, sf::Text&, sf::Text&, std::ostringstream&, std::list<healthBar>&);
    void start(sf::RenderWindow&);  
    std::string getResourcePath();
} 

#endif