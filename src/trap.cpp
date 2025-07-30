#include "../headers/trap.h"

extern std::string resourcePath;

std::vector<std::pair<float, float>> Trap::coordinate = {{448, 240}, {944, 288}, {1415, 400}, {1184, 544}, {1392, 48}, {1040, 912}, {992, 1152}, {1400,944}, {1400, 680}, {647, 890}, {930, 580}, {1340, 910}, {1150, 1070}, {67, 255}, {291, 70}};

Trap::Trap(float X, float Y): x(X), y(Y){
    if(!texture.loadFromFile(resourcePath + "image/trap.png")){
        std::cerr << "Failed to load image in trap\n";
    };
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({48, 80 * int(currentFrame)}, {48, 80}));
    sprite->setPosition({x, y});
    hitbox.setSize({45, 64});
    hitbox.setPosition({x + 2, y + 8});
    std::random_device rd;                         
    std::mt19937 gen(rd());   
    std::uniform_int_distribution<> randTime(5, 15); 
    randomTimeOfAttac = randTime(gen);
}

void Trap::update(float time){
    timeOfAttac -= time;

    if(timeOfAttac < 0.f){
        updateAnimation += frameSpeed * time;

        if(updateAnimation > 1.f){
            ++kindOfAnimation;
            updateAnimation = 0.f;
        }

        int anim = int(kindOfAnimation);

        if (anim != k) {
            k = anim;

            switch (anim) {
                case 1: case 3: case 5: case 9: case 10: case 11:
                    currentFrame -= 1; 
                    break;
                case 0: case 2: case 4: case 6: case 7: case 8:
                    currentFrame += 1;
                    break;
            }
        }
        if(kindOfAnimation >= 6){
            isActive = true;
        }

        if(kindOfAnimation >= 11){
            currentFrame = 0;
            timeOfAttac = randomTimeOfAttac;
            kindOfAnimation = 0.f;
            k = -1;
            isActive = false;
        }
        sprite = std::make_unique<sf::Sprite>(texture);
        sprite->setTextureRect(sf::IntRect({48, 80 * int(currentFrame)}, {48, 80}));
        sprite->setPosition({x, y});
    }
}

sf::Sprite Trap::draw(){
    return *sprite;
}

sf::RectangleShape Trap::getHitbox(){
    return hitbox;
}

bool Trap::getIsActive(){
    return isActive;
}