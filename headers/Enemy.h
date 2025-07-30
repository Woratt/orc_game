#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <limits>
#include <cmath>
#include <random>


#include "Entity.h"
#include "Player.h"
#include "TmxLevel.h"
#include "trap.h"


class Player;
class Trap;

class Enemy : public Entity
{
private:
    int whatKindOfAnimation = 0;
    float deathTime = 0.f, hurtTime = 0.f, timeOfTakeDamageWithoutTraps = 0,f;
    bool isAttacking = false, isAddHealth = false;;
    float attackAnimationTime = 0.f;
    float timeNewPant = 0.f;
    sf::FloatRect enemyHitbox;
    sf::Texture hurtTexture, staticTexture, runTexture, attackTexture, deathTexture;
    int health = 20;
    bool isAnimationHurt = false;
    TmxLevel* lvl;
    std::vector<sf::Vector2f> path;
    int currentPathIndex = 0, currentAnimation;
    sf::Vector2f playerPosition = {0,0};
    sf::RectangleShape hitbox;
    std::vector<Trap>* traps;
    sf::SoundBuffer bufferHit, bufferWalking, bufferDamage;
    mutable sf::Sound hitSound, walkingSound, damageSound;
public:
    static float volume;
    static float firstHealth;

    Enemy(TmxLevel&, const std::string&, float, float, std::vector<Trap>*);
    void isColitionWithMap() override;
    void takeDamage(int, float) override;
    void death(float) override;
    void move(float, Player&);
    void animationOfTakeDamage(float) override;
    void attackAnimation(float);
    void runAnimation(float);
    void staticAnimation(float) override;
    void animationOfDeath(float);
    void makeAnimation(float, int);
    bool getIsLiveli() override;
    void update(float, Player&);
    float getDeathTime();
    bool getInRound() const override;
    sf::Sprite draw() override;
    sf::Vector2f getCoordinate() const override;
    static Enemy* spawn(TmxLevel&, const std::string&, Player&, std::vector<Trap>*);
    void addHealth();
    std::pair<float, sf::Vector2f> takeDataForContinue();
    void setDataForContinue(std::pair<float, sf::Vector2f>);
    void isColitionWithTraps(float);
    int getHealth();
    void setVolume(float);
    ~Enemy();
};

#endif