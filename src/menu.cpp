#include "../headers/menu.h"

extern std::string resourcePath;

Menu::Menu(sf::RenderWindow* window, Player* player,std::list<std::shared_ptr<Enemy>>* enemys,
            std::vector<Light<fire>>* lights, Light<Player>* light, TmxLevel* level, float time, std::vector<Trap>* traps, std::list<healthBar>* healthBars):
                window(window), player(player), menu1(font),
                menu2(font), menu3(font), menu4(font), menu5(font), menu6(font), savedMesage(font),
                enemys(enemys), lights(lights), light(light), level(level), healthBars(healthBars), volumeText(font){
    if(!font.openFromFile(resourcePath + "font/CyrilicOld.ttf")){
        std::cerr << "Failed to load font in menu\n";
    };
    
    menu1.setFont(font);
    menu2.setFont(font);
    menu3.setFont(font);
    menu4.setFont(font);
    menu5.setFont(font);
    menu6.setFont(font);
    savedMesage.setFont(font);

    menu1.setCharacterSize(36);
    menu2.setCharacterSize(36);
    menu3.setCharacterSize(36);
    menu4.setCharacterSize(36);
    menu5.setCharacterSize(36);
    menu6.setCharacterSize(36);
    savedMesage.setCharacterSize(36);

    menu1.setPosition({100, 90});
    menu2.setPosition({100, 150});
    menu3.setPosition({100, 210});
    menu4.setPosition({100, 270});
    menu5.setPosition({100, 330});
    menu6.setPosition({100, 390});
    savedMesage.setPosition({500, 400});

    savedMesage.setString("Saved");
    savedMesage.setFillColor(sf::Color::Green);

    sliderBar.setSize({200, 6});
    sliderBar.setFillColor(sf::Color::White);

    sliderKnob.setRadius(10);
    sliderKnob.setFillColor(sf::Color::Red);

    volumeText.setString("50 %");
    volumeText.setCharacterSize(24);
    volumeText.setFillColor(sf::Color::White);

    if(!music.openFromFile(resourcePath + "music/music.ogg")){
        std::cerr << "Failed to load music in menu\n";
    }

    std::fstream file(resourcePath + "statistics/statistics.txt",std::ios::out | std::ios::in);
    std::string line;
    while (std::getline(file, line)) {
        if(line.find("MaxKills: ") != std::string::npos){
            maxKills = std::stoi(line.substr(10));
        }else if(line.find("MaxKeys: ") != std::string::npos){
            maxKeys = std::stoi(line.substr(9));
        }else if(line.find("PlayerCoordinate:") != std::string::npos){
            std::istringstream iss(line.substr(18));
            iss >> x >> y;
        }else if (line.find("Kills: ") != std::string::npos) {
            kills = std::stoi(line.substr(7));
        }else if (line.find("Keys: ") != std::string::npos) {
            keys = std::stoi(line.substr(6));
        }else if(line.find("IsSave: ") != std::string::npos){
            isSave = std::stoi(line.substr(8));
        }else if(line.find("PlayerHealth: ") != std::string::npos){
            PlayerHealth = std::stoi(line.substr(14));
        }else if(line.find("PlayerLight:") != std::string::npos){
            std::istringstream iss(line.substr(13));
            if(iss >> PlayerLightMaxDist >> PlayerLightIsActivity){
                light->setDataForContinue({PlayerLightMaxDist, PlayerLightIsActivity}, time);
            } 
        }else if(line.find("PlayerInRound: ") != std::string::npos){
            InRound = std::stoi(line.substr(15));
        }else if(line.find("InfoOfTakeFire:") != std::string::npos){
            std::istringstream iss(line.substr(16));
            if(iss >> takeItemFire >> timeOfTakeFire){
                player->setInfoOfTakeFire({takeItemFire, timeOfTakeFire});
            }
        }else if (line.find("LightValues:") != std::string::npos) {
            int i = 0;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                float val;
                bool flag;
                if (iss >> val >> flag) {
                    if (lights != nullptr && i < lights->size()) {
                        (*lights)[i].setDataForContinue(std::pair<float, bool>(val, flag), time);
                    } else {
                        std::cerr << "Помилка: lights == nullptr або індекс i виходить за межі! i = " 
                                  << i << ", size = " << (lights ? lights->size() : 0) << "\n";
                    }
                    ++i;
                }else{
                    break;
                }

            }
        }else if(line.find("EnemysNumbers:") != std::string::npos){
            int numbers = std::stoi(line.substr(15));
            while(enemys->size() < numbers){
                enemys->push_back(std::unique_ptr<Enemy>(Enemy::spawn(*level, resourcePath + "image/orc1_run_full.png", *player, traps)));
                healthBars->push_back(std::shared_ptr<Enemy>(enemys->back()));
            }
        }else if(line.find("EnemysValues:") != std::string::npos){
            int i = 0;
            auto it = (*enemys).begin();
            while(std::getline(file, line)){
                std::istringstream iss(line);
                float health, x, y;
                if(iss >> health >> x >> y){
                    (*it)->setDataForContinue(std::pair<float, sf::Vector2f>(health, {x, y}));
                    ++it;
                }else{
                    break;
                }
            }
        }
    }

    file.close();

    if (!menuBackground.loadFromFile(resourcePath + "image/menu_image.png")) {
        std::cout << "Could not load menu background!" << std::endl;
    }
    menuBg = std::make_unique<sf::Sprite>(menuBackground);
    scaleX = static_cast<float>(window->getSize().x) / menuBackground.getSize().x;
    scaleY = static_cast<float>(window->getSize().y) / menuBackground.getSize().y;
    menuBg->setScale({scaleX, scaleY});
    menuBg->setPosition({0, 0});
}

void Menu::drawMainMenu(float time) {
    if(isSave){
        menu1.setString("Continue");
        menu2.setString("Restart");
        menu3.setString("Statistics");
        menu4.setString("Setings");
        menu5.setString("Exit");
    }else{
        menu1.setString("Start");
        menu2.setString("Statistics");
        menu3.setString("Setings");
        menu4.setString("Exit");
    }


    bool isMenu = true;
    sf::sleep(sf::milliseconds(200));
    while (isMenu) {
        while (auto event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window->close();
                isMenu = false;
            }
        }
        if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
        }
        menu1.setFillColor(sf::Color::White);
        menu2.setFillColor(sf::Color::White);
        menu3.setFillColor(sf::Color::White);
        menu4.setFillColor(sf::Color::White);
        menu5.setFillColor(sf::Color::White);
        menuNum = 0;
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.1875)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu1.setFillColor(sf::Color::Red); menuNum = 1;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.3125)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu2.setFillColor(sf::Color::Red); menuNum = 2;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.4375)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu3.setFillColor(sf::Color::Red); menuNum = 3;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.5625)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu4.setFillColor(sf::Color::Red); menuNum = 4;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.6875)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu5.setFillColor(sf::Color::Red); menuNum = 5;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if(isSave){
                if (menuNum == 1){
                    player->Continue(x, y, PlayerHealth, InRound);
                    isMenu = false;
                    music.stop();
                }
                if(menuNum == 2){
                    player->restart();
                    enemys->clear();
                    isMenu = false;
                    light->restart(time);
                    isSave = 0;
                    for(auto it = lights->begin(); it != lights->end(); ++it){
                        it->restart(time);
                    }
                    healthBars->clear();
                    music.stop();
                }
                if(menuNum == 3){
                    drawStatistics();  
                    menu1.setString("Continue");
                    menu2.setString("Restart");
                    menu3.setString("Statistics");
                    menu4.setString("Setings");
                    menu5.setString("Exit");
                    sf::sleep(sf::milliseconds(200));
                }
                if (menuNum == 4) {
                    drawSetings(time);
                    menu1.setString("Continue");
                    menu2.setString("Restart");
                    menu3.setString("Statistics");
                    menu4.setString("Setings");
                    menu5.setString("Exit");
                    sf::sleep(sf::milliseconds(200));
                }
                if(menuNum == 5){
                    window->close();
                    if(!isSave){
                        saveData();
                    }
                    isMenu = false;
                    music.stop();
                }
            }else{
                if (menuNum == 1){
                    isMenu = false;
                    music.stop();
                }
                if(menuNum == 2){
                    drawStatistics();  
                    menu1.setString("Start");
                    menu2.setString("Statistics");
                    menu3.setString("Setings");
                    menu4.setString("Exit");
                    sf::sleep(sf::milliseconds(200));
                }
                if (menuNum == 3) {
                    drawSetings(time);
                    menu1.setString("Start");
                    menu2.setString("Statistics");
                    menu3.setString("Setings");
                    menu4.setString("Exit");
                    sf::sleep(sf::milliseconds(200));
                }
                if(menuNum == 4){
                    window->close();
                    if(isSave == 0){
                        saveData();
                    }
                    isMenu = false;
                    music.stop();
                }
            }
        }

        window->clear(sf::Color(129, 181, 221));
        window->setView(window->getDefaultView());
        window->draw(*menuBg);
        window->draw(menu1);
        window->draw(menu2);
        window->draw(menu3);
        window->draw(menu4);
        window->draw(menu5);
        window->display();
    }
}

void Menu::drawStatistics(){
    menu1.setString("Statistics: ");
    menu4.setString("Return");

    kills = player->takeEnemiesKilled();
    keys = player->getCollectedKey();
    if(maxKills < player->takeEnemiesKilled()){
        maxKills = player->takeEnemiesKilled();
    }
    if(maxKeys < player->getCollectedKey()){
        maxKeys = player->getCollectedKey();
    }
    std::ostringstream keyString;
    keyString << "Keys: " << keys << " Max keys: " << maxKeys;
    menu2.setString(keyString.str());      
    std::ostringstream killsString;
    killsString << "Kills: " << kills << " Max kills: " << maxKills;
    menu3.setString(killsString.str());

    bool isMenu = true;
    sf::sleep(sf::milliseconds(200));
    while (isMenu) {
        while (auto event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window->close();
                isMenu = false;
            }
        }
        if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
        }
        menu1.setFillColor(sf::Color::White);
        menu2.setFillColor(sf::Color::White);
        menu3.setFillColor(sf::Color::White);
        menu4.setFillColor(sf::Color::White);

        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.5625)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu4.setFillColor(sf::Color::Red); menuNum = 1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if(menuNum == 1){
                isMenu = false;
            }
        }
        
        window->clear(sf::Color(129, 181, 221));
        window->setView(window->getDefaultView());
        window->draw(*menuBg);
        window->draw(menu1);
        window->draw(menu2);
        window->draw(menu3);
        window->draw(menu4);
        window->display();
    }
}

void Menu::drawPause(float time){
    menu1.setString("Continue");
    menu2.setString("Save");
    menu3.setString("Restart");
    menu4.setString("Statistics");
    menu5.setString("Setings");
    menu6.setString("Exit");
    bool isMenu = true;
    sf::sleep(sf::milliseconds(200));
    while (isMenu) {
        while (auto event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window->close();
                isMenu = false;
            }
        }

        if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
        }

        timeShowMesage -= time;
        menu1.setFillColor(sf::Color::White);
        menu2.setFillColor(sf::Color::White);
        menu3.setFillColor(sf::Color::White);
        menu4.setFillColor(sf::Color::White);
        menu5.setFillColor(sf::Color::White);
        menu6.setFillColor(sf::Color::White);
        menuNum = 0;

        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.1875)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu1.setFillColor(sf::Color::Red); menuNum = 1;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.3125)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu2.setFillColor(sf::Color::Red); menuNum = 2;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.4375)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu3.setFillColor(sf::Color::Red); menuNum = 3;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.5625)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu4.setFillColor(sf::Color::Red); menuNum = 4;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.7083)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu5.setFillColor(sf::Color::Red); menuNum = 5;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.8333)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu6.setFillColor(sf::Color::Red); menuNum = 6;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (menuNum == 1){
                isMenu = false;
                music.stop();
            }
            if(menuNum == 2){
                isSave = 1;
                timeShowMesage = 70.f;
                Opacity = 255;
                std::fstream file(resourcePath + "statistics/statistics.txt", std::ios::in | std::ios::out);
                if (file.is_open()) {     
                file.clear();      
                file.seekp(0);
                file << "Kills: " << player->takeEnemiesKilled() << "\n" << "Keys: " <<  player->getCollectedKey() << "\n" << "MaxKills: " << maxKills
                 << "\n" << "MaxKeys: " << maxKeys << "\n" << "PlayerCoordinate: " << player->getCoordinate().x - 32
                 << " " << player->getCoordinate().y - 32 << "\n" << "PlayerHealth: " << player->getHealth() << "\n" << "PlayerInRound: " << player->getInRound() << "\n"
                  << "IsSave: " << isSave << "\nPlayerLight: " << light->getMaxDist() << " " << light->isActivity() << "\nInfoOfTakeFire: " << player->takeInfoOfTakeFire().first << " " << player->takeInfoOfTakeFire().second << "\n" << "LightValues:\n";
                for(auto it = lights->begin(); it != lights->end(); ++it){
                    file << it->takeDataForContinue().first << " " << it->takeDataForContinue().second << "\n";
                }
                file << "\nEnemysNumbers: " << enemys->size() << "\n" << "EnemysValues:\n";
                for(auto it = enemys->begin(); it != enemys->end(); ++it){
                    file << (*it)->takeDataForContinue().first << " " << (*it)->takeDataForContinue().second.x << " " << (*it)->takeDataForContinue().second.y << "\n";
                }
                file.close();
                } else {
                    std::cout << "Failed to open a file with statistics\n";
                }
            }
            if(menuNum == 3){
                player->restart();
                enemys->clear();
                light->restart(time);
                healthBars->clear();
                isSave = 0;
                for(auto it = lights->begin(); it != lights->end(); ++it){
                    it->restart(time);
                }
                isMenu = false;
                music.stop();
            }
            if (menuNum == 4){
                drawStatistics();
                menu1.setString("Continue");
                menu2.setString("Save");
                menu3.setString("Restart");
                menu4.setString("Statistics");
                menu5.setString("Setings");
                menu6.setString("Exit");
                sf::sleep(sf::milliseconds(200));
            }
            if(menuNum == 5){
                drawSetings(time);
                menu1.setString("Continue");
                menu2.setString("Save");
                menu3.setString("Restart");
                menu4.setString("Statistics");
                menu5.setString("Setings");
                menu6.setString("Exit");
                sf::sleep(sf::milliseconds(200));
            }
            if (menuNum == 6){
                if(isSave == 0){
                    saveData();
                }
                isMenu = false;
                music.stop();
                window->close();
            }
        }

        window->clear(sf::Color(129, 181, 221));
        window->setView(window->getDefaultView());
        window->draw(*menuBg);
        window->draw(menu1);
        window->draw(menu2);
        window->draw(menu3);
        window->draw(menu4);
        window->draw(menu5);
        window->draw(menu6);
        if(timeShowMesage > 0.f){
            savedMesage.setFillColor(sf::Color(0, 255, 0, Opacity));
            window->draw(savedMesage);
            Opacity -= 0.05f;
        }
        window->display();
    }
}

void Menu::updateSliderKnob() {
    float x = sliderBar.getPosition().x + (volume / 100.f) * sliderBar.getSize().x;
    sliderKnob.setPosition({x, sliderBar.getPosition().y - 7});
}

void Menu::drawSetings(float time){
    menu1.setString("Volume: ");
    menu3.setString("Return");
    bool isMenu = true;
    std::cout << "drawSetings\n";
    sliderBar.setPosition({100, 150});
    //sliderKnob.setPosition({100, 200});
    updateSliderKnob();
    sf::sleep(sf::milliseconds(200));
    while (isMenu) {
        while (auto event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window->close();
                isMenu = false;
            }
        }

        if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
        }

        menu3.setFillColor(sf::Color::White);
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.3125)}, {300, 50}).contains(sf::Mouse::getPosition(*window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            float x = sf::Mouse::getPosition(*window).x;
            float startX = sliderBar.getPosition().x;
            float endX = startX + sliderBar.getSize().x;
            x = std::clamp(x, startX, endX);
            volume = ((x - startX) / (sliderBar.getSize().x)) * 100.f;
            volumeText.setString(std::to_string((int)volume) + " %");
            updateSliderKnob();
            music.setVolume(volume * 2);
        }
        menuNum = 0;
        
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.4375)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu3.setFillColor(sf::Color::Red); menuNum = 3;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if(menuNum == 3){
                player->setVolume(volume);
                Enemy::volume = volume;
                isMenu = false;
            }
        }
        
            volumeText.setPosition({330, 140});
            window->clear(sf::Color(129, 181, 221));
            window->setView(window->getDefaultView());
            window->draw(*menuBg);
            window->draw(menu1);
            window->draw(menu3);
            window->draw(sliderBar);
            window->draw(sliderKnob);
            window->draw(volumeText);
            window->display();
        }
}



void Menu::drawEnd(float time){
    menu1.setString("Restart");
    menu2.setString("Statistics");
    menu3.setString("Setings");
    menu4.setString("Exit");

    bool isMenu = true;
    saveData();
    sf::sleep(sf::milliseconds(200));
    while (isMenu) {
        while (auto event = window->pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window->close();
                isMenu = false;
            }
        }

        if(music.getStatus() == sf::SoundSource::Status::Stopped){
            music.play();
        }

        menu1.setFillColor(sf::Color::White);
        menu2.setFillColor(sf::Color::White);
        menu3.setFillColor(sf::Color::White);
        menu4.setFillColor(sf::Color::White);
        menuNum = 0;

        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.1875)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu1.setFillColor(sf::Color::Red); menuNum = 1;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.3125)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu2.setFillColor(sf::Color::Red); menuNum = 2;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.4375)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu3.setFillColor(sf::Color::Red); menuNum = 3;
        }
        if (sf::IntRect({static_cast<int>(window->getSize().x * 0.15625), static_cast<int>(window->getSize().y * 0.5625)}, {300, 50}).contains(sf::Mouse::getPosition(*window))) {
            menu4.setFillColor(sf::Color::Red); menuNum = 4;
        }
        

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if(menuNum == 1){
                player->restart();
                enemys->clear();
                isMenu = false;
                light->restart(time);
                isSave = 0;
                for(auto it = lights->begin(); it != lights->end(); ++it){
                    it->restart(time);
                }
                music.stop();
            }
            if (menuNum == 2){
                drawStatistics();
                menu1.setString("Restart");
                menu2.setString("Statistics");
                menu3.setString("Setings");
                menu4.setString("Exit");
                sf::sleep(sf::milliseconds(200));
            }
            if (menuNum == 3){
                drawSetings(time);
                menu1.setString("Restart");
                menu2.setString("Statistics");
                menu3.setString("Setings");
                menu4.setString("Exit");
                sf::sleep(sf::milliseconds(200));
            }
            if(menuNum == 4){
                isMenu = false;
                music.stop();
                window->close();
            }
        }
            window->clear(sf::Color(129, 181, 221));
            window->setView(window->getDefaultView());
            window->draw(*menuBg);
            window->draw(menu1);
            window->draw(menu2);
            window->draw(menu3);
            window->draw(menu4);
            window->display();
        }

    }


void Menu::saveData(){
    std::ofstream file(resourcePath + "statistics/statistics.txt", std::ios::trunc);
    kills = player->takeEnemiesKilled();
    keys = player->getCollectedKey();
    if(maxKills < player->takeEnemiesKilled()){
        maxKills = player->takeEnemiesKilled();
    }
    if(maxKeys < player->getCollectedKey()){
        maxKeys = player->getCollectedKey();
    }
    if (file.is_open()) {
        file.clear();      
        file.seekp(0);
        file << "Kills: " << kills << "\n" << "Keys: " <<  keys << "\n" << "MaxKills: " << maxKills <<
         "\n" << "MaxKeys: " << maxKeys << "\n" << "IsSave: " << isSave << "\n"; 
        file.close();
    } else {
       std::cout << "Failed to open a file with statistics\n";
    }
}

float Menu::getVolume(){
    return volume;
}

bool Menu::getIsOpen(){
    return isOpen;
}