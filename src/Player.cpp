#include "../headers/Player.h"

extern std::string resourcePath;

int Player::maxHealth = 100;

Player::Player(const TmxLevel& lvl,  std::string&& F, float X, float Y, std::vector<Trap>* traps, std::vector<Items>* items): traps(traps), items(items), hitSound(bufferHit), walkingSound(bufferWalking){
    x = X, y = Y, file = std::forward<std::string>(F);
    if(!hurtTexture.loadFromFile(resourcePath + "image/player_hurt.png")){
        std::cerr << "Failed to load hurtTexture";
    }
    if(!staticTexture.loadFromFile(resourcePath + "image/player_idle.png")){
        std::cerr << "Failed to load staticTexture";
    }
    if(!runTexture.loadFromFile(resourcePath + "image/player_run.png")){
        std::cerr << "Failed to load runTexture";
    }
    if(!attackTexture.loadFromFile(resourcePath + "image/player_attack.png")){
        std::cerr << "Failed to load attackTexture";
    }
    if(!image.loadFromFile(file)){
        std::cerr << "not loaded file\n";
    }
    if(!texture.loadFromImage(image)){
        std::cerr << "not loaded image\n";
    }
    if (!bufferHit.loadFromFile(resourcePath + "music/hit.wav")) {
        std::cerr << "Failed to load music\n";
    }

    hitSound.setBuffer(bufferHit);

    if (!bufferWalking.loadFromFile(resourcePath + "music/walking.wav")) {
        std::cerr << "Failed to load music\n";
    }

    walkingSound.setBuffer(bufferWalking);
    walkingSound.setVolume(300);
    hitbox.setSize(sf::Vector2f(30, 30));
       
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({0, 0}, {64, 64}));
    sprite->setPosition({x, y});
    view.setCenter({x + 35, y + 35});

    if(lvl.GetAllObjects("map").size() != 0){ 
        obj = lvl.GetAllObjects("map");
    }else{
        std::cerr << "ERROR!\n";
    }
}

void Player::move(float time){
    dx = 0.f;
    dy = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            whatKindOfAnimation = 0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                moveView(0, time);
                }else{
                    pos = 64;
                    dx += 0.f;
                    dy += -spead;
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
                whatKindOfAnimation = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    moveView(1, time);
                }else{
                    pos = 194;
                    dx += spead;
                    dy += 0.f;
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
                whatKindOfAnimation = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    moveView(2, time);
                }else{
                    pos = 0;
                    dx += 0.f;
                    dy += spead;
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
                whatKindOfAnimation = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    moveView(3, time);
                }else{
                    pos = 128;
                    dx += -spead;
                    dy += 0;
                }
            }
        
    
    
    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0) {
        dx /= len;
        dy /= len;
    }
    float timeInSeconds = time / 1'000'00.f;
    x += dx * timeInSeconds * spead;
    y += dy * timeInSeconds * spead;
    };

sf::FloatRect Player::getHitbox() const{
    return playerHitbox;
}

void Player::update(float time, std::list<std::shared_ptr<Enemy>>& enemies, Key& key, std::vector<fire>& fires, bool isActivityLights, std::vector<Light<fire>>& lights, Light<Player>& lightPlayer, std::ostringstream& ostream){
    recharging += time;  
    int saveEnemiesKilled;

        if (whatKindOfAnimation == 1){
            animationOfTakeDamage(time);
            hurtTime -= time;
            if (hurtTime <= 0){
                whatKindOfAnimation = 0;
            }
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){   
            if(hitSound.getStatus() == sf::SoundSource::Status::Stopped){
                hitSound.play();
            }
            attackAnimation(time);
            sf::Vector2f attackOffset;
            switch(pos) {
                case 0:
                    attackOffset = {0.f, 40.f};
                    hitbox.setSize(sf::Vector2f(60, 30));
                    break;     
                case 64:
                   attackOffset = {0.f, -10.f};
                   hitbox.setSize(sf::Vector2f(60, 30));
                   break;
                case 128:
                    attackOffset = {-10.f, 0.f};
                    hitbox.setSize(sf::Vector2f(30, 60));
                    break;
                case 194:
                    attackOffset = {40.f, 0.f};
                    hitbox.setSize(sf::Vector2f(30, 60));
                    break;
                default:   attackOffset = {0.f, 0.f}; break;
            }
            hitbox.setPosition(sprite->getPosition() + attackOffset);
    
            for(auto it = enemies.begin(); it != enemies.end(); ) {
                if (hitbox.getGlobalBounds().findIntersection((*it)->draw().getGlobalBounds()).has_value()) {
                    if (recharging > 1.f) {
                        (*it)->takeDamage(damage, time); 
                        recharging = 0.f;
                    }
                }
                ++it;
            }
        }else{
            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) &&
                !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) &&
                !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) &&
                !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))){
                staticAnimation(time);
                walkingSound.stop();
            }else {
                if(walkingSound.getStatus() == sf::SoundSource::Status::Stopped){
                    walkingSound.play();
                }
                move(time);
                runAnimation(time);
            }
        }
    
    saveEnemiesKilled = enemiesKilled;
    for(auto it = enemies.begin(); it != enemies.end(); ++it){

        if (!(*it)->getIsLiveli() && (*it)->getDeathTime() < 0){
            std::random_device rd;                         
            std::mt19937 gen(rd());   
            std::uniform_int_distribution<> chance(1, 100);
            int roll = chance(gen);

            if (roll <= 1) { 
                items->push_back(Items(x, y, 1)); 
            } else if (roll <= 2) { 
                items->push_back(Items(x, y, 2)); 
            } else if (roll <= 10) { 
                items->push_back(Items(x, y, 0));
            } else if (roll <= 20) { 
                items->push_back(Items(x, y, 3)); 
            }
            std::cout << items->size() << "\n";
            it = enemies.erase(it);
            ++enemiesKilled;
        }
    }
        
    
    if(saveEnemiesKilled != enemiesKilled){
        Enemy::firstHealth += 2.5;
        for(auto it = enemies.begin(); it != enemies.end(); ++it){
            (*it)->addHealth();
        }
    }
    timeShowMessage -= time;
    isTakeItem(ostream);
    isColitionWithTraps(time);
    isActivityLight(isActivityLights, time, lights, lightPlayer);
    isColitionWithFire(fires, time);
    isColitionWithMap();
    takeKey(key);
}

int Player::getHealth(){
    return health;
}

void Player::runAnimation(float time){
    texture = runTexture;
    if(currentAnimation != 0){
        currentAnimation = 0;
        currentFrame = 0;
    }
    makeAnimation(time, 8);
}

void Player::staticAnimation(float time){
    texture = staticTexture;
    if(currentAnimation != 1){
        currentAnimation = 1;
        currentFrame = 0;
    }
    makeAnimation(time, 4);
}

void Player::attackAnimation(float time){
    texture = attackTexture;
    if(currentAnimation != 2){
        currentAnimation = 2;
        currentFrame = 0;
    }
    makeAnimation(time, 8);
}

void Player::isNotGoingBeyount(float X, float Y){
}

sf::Sprite Player::draw(){
    return *sprite;
}

sf::Vector2f Player::getCoordinate() const{
    return {x + 32, y + 32};
}

int Player::getCollectedKey(){
    return collectedKeys;
}

void Player::isColitionWithFire(std::vector<fire>& fires, float time){
    timeOfTakeDamageWithFire -= time;
    sf::Vector2f spritePos = sprite->getPosition();

    sf::Vector2f hitboxPos = spritePos + sf::Vector2f((64 - 30) / 2.f, (64 - 30) / 2.f);
    sf::Vector2f hitboxSize(30.f, 30.f);
    playerHitbox = sf::FloatRect(hitboxPos, hitboxSize);
    for(auto it = fires.begin(); it != fires.end(); ++it){
        if(playerHitbox.findIntersection(it->getHitbox().getGlobalBounds()).has_value()){
            if(timeOfTakeDamageWithFire <= 0.f){
                takeDamage(5, time);
                timeOfTakeDamageWithFire = 3.0f;
            }
            auto maybeIntersection = playerHitbox.findIntersection(it->getHitbox().getGlobalBounds());
            if (!maybeIntersection.has_value()) continue;

            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = it->getCoordinate() + it->getHitbox().getSize() / 2.f;
            sf::Vector2f playerCenter = playerHitbox.position + playerHitbox.size / 2.f;

            sf::Vector2f offset = playerCenter - objCenter;

            if (intersection.size.x < intersection.size.y) {
                if (offset.x > 0) {
                    x += intersection.size.x;
                } else{
                    x -= intersection.size.x;
                }
                dx = 0;
            } else {
                if (offset.y > 0) {
                    y += intersection.size.y;
                } else{
                    y -= intersection.size.y;
                }
                dy = 0;
            }
        }
    }
}

void Player::takeDamage(int damage, float time){
    if(currentAnimation != 3){
        currentAnimation = 3;
        currentFrame = 0;
    }
    whatKindOfAnimation = 1;
    hurtTime = 0.2f;
    health -= damage;
    if(health <= 0){
        isLiveli = false;
        death(time);
    }
}

void Player::death(float time){
}

bool Player::getIsLiveli(){
    return isLiveli;
}

void Player::animationOfTakeDamage(float time){
    if(currentAnimation != 4){
        currentAnimation = 4;
        currentFrame = 0;
    }
    texture = hurtTexture;
    makeAnimation(time, 6);
}


void Player::makeAnimation(float time, int i){
    currentFrame += frameSpead * time * 1.5;
    if(currentFrame > i) currentFrame -= i;
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({64 * int(currentFrame), pos}, {64, 64}));
    sprite->setPosition({x, y});
    view.setCenter({x + 35, y + 35});
}


void Player::isColitionWithMap(){
    sf::Vector2f spritePos = sprite->getPosition();

    sf::Vector2f hitboxPos = spritePos + sf::Vector2f((64 - 30) / 2.f, (64 - 30) / 2.f);
    sf::Vector2f hitboxSize(30.f, 30.f);
    playerHitbox = sf::FloatRect(hitboxPos, hitboxSize);

    for (const auto& object : obj) {
        if (object.name == "solid"){

            auto maybeIntersection = playerHitbox.findIntersection(object.rect);
            if (!maybeIntersection.has_value()) continue;
            
            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
            sf::Vector2f playerCenter = playerHitbox.getCenter();

            sf::Vector2f offset = playerCenter - objCenter;

            if (intersection.size.x < intersection.size.y) {
                if (offset.x > 0) {
                    x += intersection.size.x;
                } else{
                    x -= intersection.size.x;
                }
                dx = 0;
            } else {
                if (offset.y > 0) {
                    y += intersection.size.y;
                } else{
                    y -= intersection.size.y;
                }
                dy = 0;
            }
        }else if(object.name == "round"){
            auto maybeIntersection = playerHitbox.findIntersection(object.rect);
            if(maybeIntersection.has_value()){
                inRound = true;
            }

        }else if (object.name == "round2"){
            auto maybeIntersection = playerHitbox.findIntersection(object.rect);
            if(maybeIntersection.has_value()){
                inRound = false;
            }
        }else if(object.name == "solidRound2" &&  inRound == false){
            auto maybeIntersection = playerHitbox.findIntersection(object.rect);
            if (!maybeIntersection.has_value()) continue;
            
            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
            sf::Vector2f playerCenter = playerHitbox.getCenter();

            sf::Vector2f offset = playerCenter - objCenter;

            if (intersection.size.x < intersection.size.y) {
                if (offset.x > 0) {
                    x += intersection.size.x;
                } else{
                    x -= intersection.size.x;
                }
                dx = 0;
            } else {
                if (offset.y > 0) {
                    y += intersection.size.y;
                } else{
                    y -= intersection.size.y;
                }
                dy = 0;
            }
        }else if(object.name == "solidRound" &&  inRound == true){
            auto maybeIntersection = playerHitbox.findIntersection(object.rect);
            if (!maybeIntersection.has_value()) continue;
            
            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
            sf::Vector2f playerCenter = playerHitbox.getCenter();

            sf::Vector2f offset = playerCenter - objCenter;

            if (intersection.size.x < intersection.size.y) {
                if (offset.x > 0) {
                    x += intersection.size.x;
                } else{
                    x -= intersection.size.x;
                }
                dx = 0;
            } else {
                if (offset.y > 0) {
                    y += intersection.size.y;
                } else{
                    y -= intersection.size.y;
                }
                dy = 0;
            }
        }
    }
}

bool Player::getInRound() const {
    return inRound;
}

void Player::takeKey(Key& key) const {
    if(playerHitbox.findIntersection(key.keyHitbox).has_value()){
        --key.quantity;
        key.update(const_cast<Player*>(this), obj);
    }
}

int Player::takeEnemiesKilled(){
    return enemiesKilled;
}

void Player::restart(){
    takeItemFire = false;
    timeOfTakeFire = 0.f;
    isLiveli = true;
    health = 100;
    collectedKeys = 0;
    enemiesKilled = 0;
    x = 320;
    y = 280;
    sprite->setPosition({x, y});
}

std::pair<bool, Light<fire>*> Player::isColitionWithLight(std::vector<Light<fire>>& lights){
    
    for (auto& light : lights) {
        auto& points = light.getLight();
        size_t n = points.size();
        if (n < 3) continue;
        int count = 0;

        for (size_t i = 0; i < n; ++i) {
            sf::Vector2f a = points[i].point;
            sf::Vector2f b = points[(i + 1) % n].point;

            if (((a.y > getCoordinate().y) != (b.y > getCoordinate().y)) &&
                (getCoordinate().x < (b.x - a.x) * (getCoordinate().y - a.y) / (b.y - a.y + 0.00001f) + a.x)) {
                count++;
            }
        }

        if ((count % 2) == 1) return {true, &light};
    }

    return {false, &lights[0]};
}

void Player::isActivityLight(bool isActivityLights, float time, std::vector<Light<fire>>& lights, Light<Player>& lightPlayer){
    timeOfTakeDamageWithoutLight += time;
    std::pair<bool, Light<fire>*> pair = isColitionWithLight(lights);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && pair.first){
        lightPlayer.enlargeMaxDist(time);
        (*pair.second).reduceMaxDist(time);
    }else{
        lightPlayer.reduceMaxDist(time);
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && pair.first && !lightPlayer.isActivity()){
        (*pair.second).reduceMaxDist(time);
    }
    
    if(!isActivityLights && timeOfTakeDamageWithoutLight > 2.f && !pair.first){
        timeOfTakeDamageWithoutLight = 0.f;
        takeDamage(5, time);
    }
}

void Player::Continue(float X, float Y, int Health, bool InRound){
    x = X, y =Y, health = Health, inRound = InRound;
}

void Player::isColitionWithTraps(float time){
    timeOfTakeDamageWithoutTraps -= time;
    for(auto it = traps->begin(); it != traps->end(); ++it){
        if(it->getIsActive() && playerHitbox.findIntersection(it->getHitbox().getGlobalBounds()).has_value() && timeOfTakeDamageWithoutTraps < 0){
            takeDamage(10, time);
            timeOfTakeDamageWithoutTraps = 2.f;
            break;
        }   
    }
}

void Player::isTakeItem(std::ostringstream& ostream){
    for(auto it = items->begin(); it != items->end(); ++it){
        if(playerHitbox.findIntersection(it->getHitbox().getGlobalBounds()).has_value()){
            if(it->getType() == 0){
                health += 20;
                ostream << "+20 Health";
                if(health > maxHealth) health = maxHealth;
            }else if(it->getType() == 1){
                takeItemFire = true;
                timeOfTakeFire = 60.f;
            }else if(it->getType() == 2){
                maxHealth += 50;
                health = maxHealth;
                ostream << "+50 Max Health";
            }else{
                damage += 10;
                ostream << "+10 Max Damage";
            }
            timeShowMessage = 4.f;
            it = items->erase(it);
            break;
        }
    }
}

std::pair<bool&, float&> Player::takeInfoOfTakeFire(){
    return std::pair<bool&, float&>(takeItemFire, timeOfTakeFire);
}

void Player::setInfoOfTakeFire(std::pair<bool, float> pair){
    takeItemFire = pair.first;
    timeOfTakeFire = pair.second;
}

float Player::takeTimeShowMessage(){
    return timeShowMessage;
}

void Player::setVolume(float volume){
    hitSound.setVolume(volume * 2);
    walkingSound.setVolume(volume * 2);
}

Player::~Player(){}
