#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <sstream>
#include <list>


#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "view.h"
#include "key.h"
#include "Entity.h"
#include "Enemy.h"
#include "fire.h"
#include "light.h"
#include "trap.h"
#include "items.h"


class Enemy;
class Key;
template<typename T>
class Light;
class Trap;
class Items;

class Player : public Entity
{
private:
    static int maxHealth;
    int dir = -1, collectedKeys = 0, whatKindOfAnimation = 0, damage = 10;
    float hurtTime = 0.f, timeOfTakeDamageWithFire = 0.f, timeOfTakeDamageWithoutLight = 0.f, timeOfTakeDamageWithoutTraps = 0.f, timeOfTakeFire = 0.f, timeShowMessage = 0.f;
    sf::FloatRect playerHitbox;
    sf::Texture hurtTexture, staticTexture, runTexture, attackTexture;
    sf::RectangleShape hitbox;
    int enemiesKilled = 0, currentAnimation;
    std::vector<Trap>* traps;
    std::vector<Items>* items;
    bool takeItemFire = false;
    sf::SoundBuffer bufferHit, bufferWalking;
    mutable sf::Sound hitSound, walkingSound;
    sf::RenderWindow* window;
    friend class Key;
    
    public:
    Player(const TmxLevel&, std::string&&, float, float, std::vector<Trap>*, std::vector<Items>*);
    ~Player() override;
    void isColitionWithMap() override;
    void update(float time, std::list<std::shared_ptr<Enemy>>&, Key&, std::vector<fire>&, bool, std::vector<Light<fire>>&, Light<Player>&, std::ostringstream&);
    void move(float);
    void takeDamage(int, float) override;
    void animationOfTakeDamage(float) override;
    void attackAnimation(float);
    void runAnimation(float);
    void staticAnimation(float) override;
    void death(float) override;
    void makeAnimation(float, int);
    int getHealth();
    bool getIsLiveli() override;
    sf::FloatRect getHitbox() const;
    sf::Sprite draw() override;
    sf::Vector2f getCoordinate() const override;
    void isNotGoingBeyount(float, float);
    bool getInRound() const override;
    int getCollectedKey();
    void takeKey(Key&) const;
    int takeEnemiesKilled();
    void restart();
    void isColitionWithFire(std::vector<fire>&, float);
    void isActivityLight(bool, float, std::vector<Light<fire>>&, Light<Player>&);
    std::pair<bool, Light<fire>*> isColitionWithLight(std::vector<Light<fire>>&);
    void Continue(float, float, int, bool);
    void isColitionWithTraps(float);
    void isTakeItem(std::ostringstream&);
    std::pair<bool&, float&> takeInfoOfTakeFire();
    float takeTimeShowMessage();
    void setInfoOfTakeFire(std::pair<bool, float>);
    void setVolume(float);
};


#endif