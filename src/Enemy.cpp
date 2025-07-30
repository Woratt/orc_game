#include "../headers/Enemy.h"

extern std::string resourcePath;

float Enemy::firstHealth = 20;
float Enemy::volume = 50;

float heuristic(NodeGraph* a, NodeGraph* b) {
    return std::max(std::abs(a->x - b->x), std::abs(a->y - b->y));
}
struct Compare {
    bool operator()(NodeGraph* a, NodeGraph* b) const {
        return a->fCost() > b->fCost();
    }
};

std::vector<NodeGraph*> AStar(NodeGraph* start, NodeGraph* goal,TmxLevel& level) {
    std::priority_queue<NodeGraph*, std::vector<NodeGraph*>, Compare> openSet;
    std::unordered_set<NodeGraph*> closedSet;

    start->gCost = 0;
    start->hCost = heuristic(start, goal);
    start->parent = nullptr;
    openSet.push(start);

    while (!openSet.empty()) {
        NodeGraph* current = openSet.top();
        openSet.pop();

        if (current == goal) {
            std::vector<NodeGraph*> path;
            while (current) {
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current);

        for (NodeGraph* neighbor : current->neighbors) {
            if(!neighbor) continue;
            if (!level.isPassable(neighbor->x, neighbor->y)) continue;
            if (closedSet.count(neighbor)) continue;


            float tentativeG = current->gCost + 1.0f;

            if (tentativeG < neighbor->gCost) {
                neighbor->parent = current;
                neighbor->gCost = tentativeG;
                neighbor->hCost = heuristic(neighbor, goal);
                openSet.push(neighbor);
            }
        }
    }

    return {};
}

Enemy::Enemy(TmxLevel& lvl, const std::string& F, float X, float Y, std::vector<Trap>* traps): lvl(&lvl), traps(traps), hitSound(bufferHit), walkingSound(bufferWalking), damageSound(bufferDamage){
    if(!staticTexture.loadFromFile(resourcePath + "image/orc2_idle_full.png")){
        std::cerr << "Failed to load staticTexture";
    }
    if(!runTexture.loadFromFile(resourcePath + "image/orc2_run_full.png")){
        std::cerr << "Failed to load runTexture";
    }
    if(!hurtTexture.loadFromFile(resourcePath + "image/orc2_hurt_full.png")){
        std::cerr << "Failed to load hurtTexture";
    }
    if(!attackTexture.loadFromFile(resourcePath + "image/orc2_attack_full.png")){
        std::cerr << "Failed to load attackTexture";
    }
    if(!deathTexture.loadFromFile(resourcePath + "image/orc2_death_full.png")){
        std::cerr << "Failed to load deathTexture";
    }
    if (!bufferHit.loadFromFile(resourcePath + "music/hit.wav")) {
        std::cerr << "Failed to load music\n";
    }

    hitSound.setBuffer(bufferHit);

    if (!bufferWalking.loadFromFile(resourcePath + "music/walkingEnemy.wav")) {
        std::cerr << "Failed to load music\n";
    }

    walkingSound.setBuffer(bufferWalking);

    if (!bufferDamage.loadFromFile(resourcePath + "music/damage.wav")) {
        std::cerr << "Failed to load music\n";
    }

    damageSound.setBuffer(bufferDamage);
    walkingSound.setVolume(volume * 6);
    damageSound.setVolume(volume / 2);
    hitSound.setVolume(volume * 2);

    file = std::move(F), x = X, y = Y;
    texture = staticTexture;
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect({0, 0}, {64, 64}));
    sprite->setPosition({x, y});
    hitbox.setSize(sf::Vector2f(30, 30));

    if(lvl.GetAllObjects("map").size() != 0){ 
        obj = lvl.GetAllObjects("map");
    }else{
        std::cerr << "ERROR!\n";
    }

    health = firstHealth;
}

sf::Sprite Enemy::draw(){
    return *sprite;
}

void Enemy::move(float time, Player& player){
    sf::Vector2f direction = player.getCoordinate() - getCoordinate();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::FloatRect intersection;
    recharging += time; 
    timeNewPant += time;
    sf::Vector2f enemyPos = getCoordinate();
    sf::Vector2f playerPos = player.getCoordinate();
    sf::Vector2f diff = playerPos - playerPosition;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if(isAddHealth){
        health += 2.5f; 
        isAddHealth = false;
    }
    if(timeNewPant > 1.f && dist > 10.f && length < 300.f){
        timeNewPant = 0.f;
        playerPosition = playerPos;
            int ex = (enemyPos.x + 8) / 16;
            int ey = (enemyPos.y + 8) / 16;
            int px = playerPos.x / 16;
            int py = playerPos.y / 16;

            NodeGraph* start = lvl->graph[ey][ex];
            NodeGraph* goal = lvl->graph[py][px];
            if (!start || !goal) return;

            for (auto& row : lvl->graph) {
                for (auto* node : row) {
                    if (node) {
                        node->gCost = INFINITY;
                        node->parent = nullptr;
                    }
                }
            }

            auto nodePath = AStar(start, goal, *lvl);
            if (nodePath.size() > 19) {
                //std::cerr << "AStar generated too long path: " << nodePath.size() << "\n";
                return;
            }
            path.clear();
            for (auto* node : nodePath) {
                if (!node) continue;
                path.emplace_back(node->x, node->y);
            }
            currentPathIndex = 0;
    }else{

    }
    if(!isLiveli) {
        deathTime -= time;
        death(time);
    }else if(isAnimationHurt){
        animationOfTakeDamage(time);
        hurtTime -= time;
        if(hurtTime < 0){
            isAnimationHurt = false;
        }
    }else if (length < 30.f) {
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
        if (recharging >= 2.f) {
            if(hitSound.getStatus() == sf::SoundSource::Status::Stopped){
                hitSound.play();
            }
            if (hitbox.getGlobalBounds().findIntersection(player.getHitbox()).has_value()) {
                player.takeDamage(10, time);
                recharging = 0.f;
            }
            isAttacking = true;
            attackAnimationTime = 0.4f;
        }
        
        if (isAttacking) {
            attackAnimation(time);
            attackAnimationTime -= time;
            
            if (attackAnimationTime <= 0.f) {
                isAttacking = false;
                attackAnimationTime = 0.f;
            }
        }else{
            staticAnimation(time);
        }
    }else if (length < 200.f) {
        if(walkingSound.getStatus() == sf::SoundSource::Status::Stopped){
            walkingSound.play();
        }
        //std::cout << "currentPathIndex: " << currentPathIndex << "\n";
        sf::Vector2f target;
        if(!path.empty() && currentPathIndex < path.size()){
            target = path[currentPathIndex] * 16.f;
        }
        sf::Vector2f dir = target - enemyPos;

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len < 2.f) {
            currentPathIndex++;
        } else {
            float speed = 70.f;
            float timeInSeconds = time / 10.f;
            dir /= std::sqrt(dir.x * dir.x + dir.y * dir.y);
            x += dir.x * speed * time;
            y += dir.y * speed * time;  
            if (std::abs(dir.y) > std::abs(dir.x)) {
                if (dir.y > 0) pos = 0;       
                else pos = 64;                
            } else {
                if (dir.x > 0) pos = 194;     
                else pos = 128;               
            }
        }
        sprite->setPosition({x, y});

        runAnimation(time);
    }else{
        walkingSound.stop();
        staticAnimation(time);
    }

    isColitionWithMap();
    std::cout << volume << "\n";
    walkingSound.setVolume(volume * 6);
    damageSound.setVolume(volume / 2);
    hitSound.setVolume(volume * 2);
}

void Enemy::addHealth(){
    isAddHealth = true;
}



sf::Vector2f Enemy::getCoordinate() const{
    return {x + 35, y + 35};
}

Enemy* Enemy::spawn(TmxLevel& lvl, const std::string& file, Player& player, std::vector<Trap>* traps){
    std::vector<TmxObject> lvlObj = lvl.GetAllObjects("map");
    std::random_device rd;                         
    std::mt19937 gen(rd());   
    std::uniform_int_distribution<> distrX(35, 1520); 
    std::uniform_int_distribution<> distrY(35, 1200);  

    float X = distrX(gen);
    float Y = distrY(gen);
    bool intersects = false;
    while (true) {
        X = distrX(gen);
        Y = distrY(gen);
        sf::Vector2f direction;
        direction.x = player.getCoordinate().x - X;
        direction.y = player.getCoordinate().y - Y;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        bool intersects = false;
        for (const auto& object : lvlObj) {
            if (object.name == "solid" || length < 400.f) {
                if (sf::FloatRect({X, Y}, {64, 64}).findIntersection(object.rect)) {
                    intersects = true;
                    std::cerr << "not spawn\n";
                    break;
                }
            }
        }

        if (!intersects) break;
    }
    return new Enemy(lvl, file, X, Y, traps);
}

void Enemy::update(float time, Player& player){
}

void Enemy::runAnimation(float time){
    if(currentAnimation != 4){
        currentAnimation = 4;
        currentFrame = 0;
    }
    texture = runTexture;
    makeAnimation(time, 8);
}

void Enemy::staticAnimation(float time){
    if(currentAnimation != 3){
        currentAnimation = 3;
        currentFrame = 0;
    }
    texture = staticTexture;
    makeAnimation(time, 4);
}

void Enemy::attackAnimation(float time){
    if(currentAnimation != 2){
        currentAnimation = 2;
        currentFrame = 0;
    }
    texture = attackTexture;
    makeAnimation(time, 8);
}

void Enemy::makeAnimation(float time, int i){
    currentFrame += frameSpead * time * 1.5;
    if(currentFrame > i) currentFrame -= i;
    sprite->setTextureRect(sf::IntRect({64 * int(currentFrame), pos}, {64, 64}));
    sprite->setPosition({x, y});
    enemyHitbox = sprite->getGlobalBounds();
}

void Enemy::takeDamage(int damage, float time){
    if(damageSound.getStatus() == sf::SoundSource::Status::Stopped){
        damageSound.play();
    }
    hurtTime = 0.5f;
    isAnimationHurt = true;
    health -= damage;

    if(health <= 0){
        isLiveli = false;
        death(time);
        deathTime = 0.4f;
    }
}

void Enemy::death(float time){
    animationOfDeath(time);
}

void Enemy::animationOfDeath(float time){
    if(currentAnimation != 1){
        currentAnimation = 1;
        currentFrame = 0;
    }
    texture = deathTexture;
    makeAnimation(time, 8);
}

void Enemy::animationOfTakeDamage(float time){
    if(currentAnimation != 0){
        currentAnimation = 0;
        currentFrame = 0;
    }
    texture = hurtTexture;
    makeAnimation(time, 6);
}

bool Enemy::getIsLiveli(){
    return isLiveli;
}

float Enemy::getDeathTime(){
    return deathTime;
}

void Enemy::isColitionWithMap(){
    sf::Vector2f spritePos = sprite->getPosition();

    sf::Vector2f hitboxPos = spritePos + sf::Vector2f((64 - 50) / 2.f, (64 - 50) / 2.f);
    sf::Vector2f hitboxSize(30.f, 30.f);
    enemyHitbox = sf::FloatRect(hitboxPos, hitboxSize);

    for (const auto& object : obj) {
        if (object.name == "solid"){

        auto maybeIntersection = enemyHitbox.findIntersection(object.rect);
        if (!maybeIntersection.has_value()) continue;

        sf::FloatRect intersection = maybeIntersection.value();

        sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
        sf::Vector2f playerCenter = enemyHitbox.position + enemyHitbox.size / 2.f;

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
            auto maybeIntersection = enemyHitbox.findIntersection(object.rect);
            if(maybeIntersection.has_value()){
                inRound = true;
            }

        }else if (object.name == "round2"){
            auto maybeIntersection = enemyHitbox.findIntersection(object.rect);
            if(maybeIntersection.has_value()){
                inRound = false;
            }
        }else if(object.name == "solidRound2" &&  inRound == false){
            auto maybeIntersection = enemyHitbox.findIntersection(object.rect);
            if (!maybeIntersection.has_value()) continue;

            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
            sf::Vector2f playerCenter = enemyHitbox.position + enemyHitbox.size / 2.f;

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
            auto maybeIntersection = enemyHitbox.findIntersection(object.rect);
            if (!maybeIntersection.has_value()) continue;

            sf::FloatRect intersection = maybeIntersection.value();

            sf::Vector2f objCenter = object.rect.position + object.rect.size / 2.f;
            sf::Vector2f playerCenter = enemyHitbox.position + enemyHitbox.size / 2.f;

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

bool Enemy::getInRound() const {
    return inRound;
}

std::pair<float, sf::Vector2f> Enemy::takeDataForContinue(){
    return std::pair<float, sf::Vector2f>(health, {x, y});
}

void Enemy::setDataForContinue(std::pair<float, sf::Vector2f> pair){
    health = pair.first;
    x = pair.second.x;
    y = pair.second.y;
}

void Enemy::isColitionWithTraps(float time){
    timeOfTakeDamageWithoutTraps -= time;
    for(auto it = traps->begin(); it != traps->end(); ++it){
        if(it->getIsActive() && enemyHitbox.findIntersection(it->getHitbox().getGlobalBounds()).has_value() && timeOfTakeDamageWithoutTraps < 0){
            takeDamage(5, time);
            timeOfTakeDamageWithoutTraps = 2.f;
            break;
        }   
    }
}

int Enemy::getHealth(){
    return health;
}

void Enemy::setVolume(float Volume){
    volume = Volume;
}

Enemy::~Enemy(){}