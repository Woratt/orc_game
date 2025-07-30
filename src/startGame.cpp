#include "../headers/startGame.h"


extern std::string resourcePath;
std::string Start::getResourcePath() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();

    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

    char path[PATH_MAX];

    if (resourcesURL) {
        if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX)) {
            CFRelease(resourcesURL);
            return std::string(path) + "/";
        }
        CFRelease(resourcesURL);
    }
    return "";
}

void Start::drawAndUpdate(sf::RenderWindow& window, Player& hero, Menu& menu, sf::Clock& clock,
    TmxLevel& level, TmxLevel& objectLevel, std::vector<Trap>& traps, std::vector<Items>& items,
     std::vector<fire>& fires, std::list<std::shared_ptr<Enemy>>& enemys, Key& key, std::vector<Light<fire>>& lights,
      sf::RenderTexture& darkness, Light<Player>& light, sf::Text& fpsText, sf::Text& fireTextTime, sf::Text& textHealth,
      sf::Text& textKey, sf::Text& messageText, std::ostringstream& ostream, std::list<healthBar>& healthBars){
   window.setView(view);
   menu.drawMainMenu(0.33);
   float DeltaTime = 0;
   int frameCount = 0;
   float fps = 0.0f;
   float fpstime = 0.f;
   float Opacity = 255;

    sf::Music music(resourcePath + "music/m.wav"); 
    while (window.isOpen()) {
       float time = clock.restart().asSeconds();
       DeltaTime += time;
       while (auto event = window.pollEvent()) {
           if(event->is<sf::Event::Closed>()) {
               window.close();
           }
       }
       if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
       }
       music.setVolume(menu.getVolume() * 2);
       if(!hero.getIsLiveli()){
           music.stop();
           menu.drawEnd(time);
           continue;
       }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
            clock.stop();
           music.stop();
           menu.drawPause(time);
           continue;
       }
       window.setView(view);
       window.clear();
       hero.update(time, enemys, key, fires, light.isActivity(), lights, light, ostream);
       if(DeltaTime > 20){
           DeltaTime = 0;
           if(enemys.size() <= 20){
               enemys.push_back(std::shared_ptr<Enemy>(Enemy::spawn(level, resourcePath + "image/orc1_run_full.png", hero, &traps)));
               healthBars.push_back(std::shared_ptr<Enemy>(enemys.back()));
            }
        }
        cangeView();
        level.DrawLayers(window, 0, 3);
        objectLevel.Draw(window);
        for(auto it = traps.begin(); it != traps.end(); ++it){
            it->update(time);
           window.draw(it->draw());
        }
        for(int i = 0; i < fire::coordinate.size(); ++i){
            if(hero.getCoordinate().y >= fires[i].getCoordinate().y){
                fires[i].update(time);
                window.draw(fires[i].draw());
            }
        }
        if(hero.getInRound()) window.draw(hero.draw());
        for(auto it = enemys.begin(); it != enemys.end(); ++it){
            if((*it)->getInRound()){
                (*it)->move(time, hero);
                window.draw((*it)->draw());
            }
            
        }
        for(auto it = healthBars.begin(); it != healthBars.end(); ++it){
            it->update();
            if(it->isDraw()){
                window.draw(it->draw());
                window.draw(it->returnBar());
            }
            if(!it->isLiveli()){
                it = healthBars.erase(it);
            }
        }
        for(auto it = items.begin(); it != items.end(); ++it){
            it->update(time);
            window.draw(it->draw());
        }
        level.DrawLayers(window, 3, 4);
       if(!hero.getInRound()) window.draw(hero.draw());
       for(auto it = enemys.begin(); it != enemys.end(); ++it){
           if(!(*it)->getInRound()){
               (*it)->move(time, hero);
               window.draw((*it)->draw());
           }
           
       }
       for(int i = 0; i < fire::coordinate.size(); ++i){
           if(hero.getCoordinate().y < fires[i].getCoordinate().y){
               fires[i].update(time);
               window.draw(fires[i].draw());
           }
       }
       window.draw(*(key.sprite));
           
       darkness.clear(sf::Color(29, 28, 43, 255));
       darkness.draw(light.update(time), sf::BlendNone);
       for(auto it = lights.begin(); it != lights.end(); ++it){
           darkness.draw(it->update(time), sf::BlendNone);
       }
       darkness.display();
       frameCount++;
           fpstime += time;
           if (fpstime >= 1.0f) {
               fps = frameCount / fpstime;
               fpstime = 0.f;
               frameCount = 0; 
           }
           std::stringstream ss;
           ss << "FPS: " << static_cast<int>(fps);
           fpsText.setString(ss.str());
           
           sf::Sprite maskSprite(darkness.getTexture());
           if(!hero.takeInfoOfTakeFire().first && !(hero.takeInfoOfTakeFire().second > 0)){
               window.draw(maskSprite, sf::BlendMultiply);
           }else{
               hero.takeInfoOfTakeFire().second -= time;
               fireTextTime.setCharacterSize(20 * getCurrentZoom());
               std::ostringstream LightingTime;
               LightingTime << int(hero.takeInfoOfTakeFire().second);
               fireTextTime.setString("Lighting time: " + LightingTime.str());
               fireTextTime.setPosition({view.getCenter().x - 300 * getCurrentZoom(), view.getCenter().y - 140 * getCurrentZoom()});
               window.draw(fireTextTime);
               light.enlargeMaxDist(time);
               if(hero.takeInfoOfTakeFire().second < 0){
                   hero.takeInfoOfTakeFire().first = false;
               }
           }

           std::ostringstream playerHealth;
           playerHealth << hero.getHealth();
           textHealth.setCharacterSize(20 * getCurrentZoom());
           textHealth.setFillColor(sf::Color::Red);
           textHealth.setString("Health: " + playerHealth.str());
           std::ostringstream playerScoreString;  
           ostream.clear();
           ostream.seekp(0);
           playerScoreString << hero.getCollectedKey();
           textKey.setCharacterSize(20 * getCurrentZoom());
           textKey.setString("Key collected: " + playerScoreString.str());
           ostream.clear();
           ostream.seekp(0);
           messageText.setCharacterSize(20 * getCurrentZoom());
           messageText.setString(ostream.str());
           textHealth.setPosition({view.getCenter().x - 300 * getCurrentZoom(), view.getCenter().y - 170 * getCurrentZoom()});
           textKey.setPosition({view.getCenter().x - 300 * getCurrentZoom(), view.getCenter().y - 200 * getCurrentZoom()});
           fpsText.setPosition({view.getCenter().x + 100 * getCurrentZoom(), view.getCenter().y - 200 * getCurrentZoom()});
           messageText.setPosition({view.getCenter().x + 140 * getCurrentZoom(), view.getCenter().y + 200 * getCurrentZoom()});
           window.draw(textHealth);
           window.draw(textKey);
           window.draw(fpsText);
           if(hero.takeTimeShowMessage() > 0){
            messageText.setFillColor(sf::Color(255, 255, 255, Opacity));
            window.draw(messageText);
            Opacity -= time * 200;
           }else{
            Opacity = 255;
           }
           window.display();
   }
}

void Start::start(sf::RenderWindow& window){
   TmxLevel level;
   TmxLevel objectLevel;
   if (!objectLevel.LoadFromFile(resourcePath + "image/objectMap.tmx")) {
    std::cerr << "Failed to load TMX map" << std::endl;
    }
    if (!level.LoadFromFile(resourcePath + "image/map.tmx")) {
        std::cerr << "Failed to load TMX map" << std::endl;
    }   
   std::vector<Trap> traps;
   for(int i = 0; i < Trap::coordinate.size(); ++i){
       traps.push_back(Trap(Trap::coordinate[i].first, Trap::coordinate[i].second));
   }
   std::vector<Items> items;
   items.push_back(Items(200, 200, 1));
   Player hero(level , resourcePath + "image/player_idle.png", 320, 280, &traps, &items);
   std::list<std::shared_ptr<Enemy>>  enemys;
   std::list<healthBar> healthBars;
   sf::Clock clock;
   std::vector<fire> fires;
   std::vector<Light<fire>> lights;
   Light<Player> light(&hero, level.GetAllObjects("map"));
   for(int i = 0; i < fire::coordinate.size(); ++i){
       fires.push_back(fire(fire::coordinate.at(i).first, fire::coordinate.at(i).second));
   }

   for(int i = 0; i < fires.size(); ++i){
       lights.push_back(Light(&fires[i], level.GetAllObjects("map")));
   }

   Menu menu(&window, &hero, &enemys, &lights, &light, &level, 0.33, &traps, &healthBars);
   Key key(resourcePath + "image/Objects.png", level);

   sf::RenderTexture darkness({1600, 1280});
   sf::Font font(resourcePath + "font/CyrilicOld.ttf");
   sf::Text textKey(font);
   sf::Text textHealth(font);
   sf::Text fpsText(font);
   sf::Text fireTextTime(font);
   sf::Text messageText(font);
   std::ostringstream ostream;
   fpsText.setCharacterSize(24);
   fpsText.setFillColor(sf::Color::White);
   fireTextTime.setFillColor(sf::Color::White);
   drawAndUpdate(window, hero, menu, clock, level, objectLevel, traps, items,
        fires, enemys, key, lights, darkness, light, fpsText, fireTextTime, textHealth, textKey, messageText, ostream, healthBars);
}