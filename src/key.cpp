#include "../headers/key.h"

size_t Key::quantity = 1;

Key::Key(const std::string& s, const TmxLevel& lvl){
    if(!image.loadFromFile(s)){
        std::cerr << "not loaded file\n";
    }
    if(!texture.loadFromImage(image)){
        std::cerr << "not loaded image\n";
    }
    sprite = std::make_unique<sf::Sprite>(texture);

    std::random_device rd;                         
    std::mt19937 gen(rd());                       
    std::uniform_int_distribution<> distrX(35, 1520); 
    std::uniform_int_distribution<> distrY(35, 1200); 
    std::vector<TmxObject> lvlObj = lvl.GetAllObjects("map");

    float X;
    float Y;
    bool intersects = false;
    while (true) {
        X = distrX(gen);
        Y = distrY(gen);

        bool intersects = false;
        for (const auto& object : lvlObj) {
            if (object.name == "solid") {
                if (sf::FloatRect({X, Y}, {12, 7}).findIntersection(object.rect)) {
                    intersects = true;
                    std::cerr << "not spawn\n";
                    break;
                }
            }
        }

        if (!intersects) break;
    }


    sprite->setPosition({X, Y});
    sprite->setTextureRect(sf::IntRect({50 ,0}, {12, 7}));
    keyHitbox.size = {12, 7};
    keyHitbox.position = {X, Y};
}

void Key::update(Player* p, const std::vector<TmxObject>& lvlObj){
    if(quantity == 0){
        std::random_device rd;                         
        std::mt19937 gen(rd());                       
        std::uniform_int_distribution<> distrX(35, 1520); 
        std::uniform_int_distribution<> distrY(35, 1200);  

        float X;
        float Y;
        bool intersects = false;
    while (true) {
        X = distrX(gen);
        Y = distrY(gen);

        bool intersects = false;
        for (const auto& object : lvlObj) {
            if (object.name == "solid") {
                if (sf::FloatRect({X, Y}, {12, 7}).findIntersection(object.rect)) {
                    intersects = true;
                    std::cerr << "not spawn\n";
                    break;
                }
            }
        }

        if (!intersects) break;
    }

        sprite->setPosition({X, Y});
        Key::quantity = 1;
        ++p->collectedKeys;
        keyHitbox.position = {X, Y};
    }
}



sf::Vector2f Key::getCoordinate(){
    return {x, y };
}

Key::~Key(){}