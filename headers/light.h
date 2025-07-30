#ifndef LIGHT_H
#define LIGHT_H

#include <SFML/Graphics.hpp>


#include "Player.h"
#include "TmxLevel.h"
#include "fire.h"


struct RayHit {
    sf::Vector2f point;
    float angle;
};
template <typename T>
class Light{
    //float x, y;
    float maxDist = 80.f, timeUpdate = 0.f;
    bool isActivitys = true, isUpdate = false;;
    std::vector<RayHit> rayHits;
    T* LightObject;
    std::vector<TmxObject> obj;
    sf::VertexArray lightShape;

    RayHit castRay(sf::Vector2f, sf::Vector2f);
    bool getLineIntersection(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f&);
public:
    Light(T*, std::vector<TmxObject>);
    sf::VertexArray update(float);
    std::vector<RayHit>& getLight();
    bool isActivity();
    void reduceMaxDist(float); 
    void enlargeMaxDist(float);
    float getMaxDist();
    sf::Vector2f getCoordinate();
    std::pair<float, bool> takeDataForContinue();
    void setDataForContinue(std::pair<float, bool>, float time);
    void restart(float time);
};

template<typename T>
Light<T>::Light(T* LightObject, std::vector<TmxObject> obj): LightObject(LightObject), obj(obj){
    if constexpr (std::is_same<T, fire>::value) {
        rayHits.clear();
        for (int i = 0; i < 360; ++i) {
            float angle = i * M_PI / 180.f;             
            sf::Vector2f dir(cos(angle), sin(angle));
            RayHit hit;
            hit = castRay(LightObject->getCoordinate(), dir);
            rayHits.push_back(hit);
        }
        
        lightShape = sf::VertexArray(sf::PrimitiveType::TriangleFan, rayHits.size() + 2);
        lightShape[0].position = LightObject->getCoordinate();
        lightShape[0].color = sf::Color(255, 255, 200, 0);
        for (int i = 0; i < rayHits.size(); ++i) {
            lightShape[i + 1].position = rayHits[i].point;
            lightShape[i + 1].color = sf::Color(255, 255, 200, 255);
        }
        lightShape[rayHits.size() + 1] = lightShape[1];
        
    }
};

template<typename T>
sf::VertexArray Light<T>::update(float time){
    timeUpdate += time;
    if (isUpdate && timeUpdate > 0.0034f) {
        //std::cout << "update\n";
        timeUpdate = 0;
        rayHits.clear();
        for (int i = 0; i < 75; ++i) {
            float angle = i * 2 * M_PI / 75.f;             
            sf::Vector2f dir(cos(angle), sin(angle));
            RayHit hit;
            hit = castRay(LightObject->getCoordinate(), dir);
            rayHits.push_back(hit);
        }
        lightShape = sf::VertexArray(sf::PrimitiveType::TriangleFan, rayHits.size() + 2);
        lightShape[0].position = LightObject->getCoordinate();
        lightShape[0].color = sf::Color(255, 255, 200, 0);
        for (int i = 0; i < rayHits.size(); ++i) {
            lightShape[i + 1].position = rayHits[i].point;
            lightShape[i + 1].color = sf::Color(255, 255, 200, 255);
        }
        lightShape[rayHits.size() + 1] = lightShape[1];
        isUpdate = false;
    }

    return lightShape;
}


template<typename T>
RayHit Light<T>::castRay(sf::Vector2f position, sf::Vector2f dir){
    sf::Vector2f end = position + dir * maxDist;
    sf::Vector2f closestPoint = end;
    float minDistSq = maxDist * maxDist;

    for(auto& object : obj){
        if(object.name == "RightLight" && (object.rect.position.x < LightObject->getCoordinate().x)){
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges = {
                {{object.rect.position.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y}},
            };

            for (auto& edge : edges) {
                sf::Vector2f intersection;
                if (getLineIntersection(position, end, edge.first, edge.second, intersection)) {
                    float distSq = (intersection - position).x * (intersection - position).x +
                                (intersection - position).y * (intersection - position).y;
                    if (distSq < minDistSq) {
                        closestPoint = intersection;
                        minDistSq = distSq;
                    }
                }
            }
        }
        if(object.name == "LeftLight" && (object.rect.position.x > LightObject->getCoordinate().x)){
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges = {
                {{object.rect.position.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y}},
            };

            for (auto& edge : edges) {
                sf::Vector2f intersection;
                if (getLineIntersection(position, end, edge.first, edge.second, intersection)) {
                    float distSq = (intersection - position).x * (intersection - position).x +
                                (intersection - position).y * (intersection - position).y;
                    if (distSq < minDistSq) {
                        closestPoint = intersection;
                        minDistSq = distSq;
                    }
                }
            }
        }
        if(object.name == "BottomLight"){
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges = {
                {{object.rect.position.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y}, {object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x + object.rect.size.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y + object.rect.size.y}},
                {{object.rect.position.x, object.rect.position.y + object.rect.size.y}, {object.rect.position.x, object.rect.position.y}},
            };

            for (auto& edge : edges) {
                sf::Vector2f intersection;
                if (getLineIntersection(position, end, edge.first, edge.second, intersection)) {
                    float distSq = (intersection - position).x * (intersection - position).x +
                                (intersection - position).y * (intersection - position).y;
                    if (distSq < minDistSq) {
                        closestPoint = intersection;
                        minDistSq = distSq;
                    }
                }
            }
        }
    }
    float angle = atan2(dir.y, dir.x);
    return {closestPoint, angle};
}
template<typename T>
bool Light<T>::getLineIntersection(sf::Vector2f p1, sf::Vector2f p2,
    sf::Vector2f p3, sf::Vector2f p4,
    sf::Vector2f& out) {
    float s1_x = p2.x - p1.x, s1_y = p2.y - p1.y;
    float s2_x = p4.x - p3.x, s2_y = p4.y - p3.y;

    float s = (-s1_y * (p1.x - p3.x) + s1_x * (p1.y - p3.y)) /
    (-s2_x * s1_y + s1_x * s2_y);
    float t = ( s2_x * (p1.y - p3.y) - s2_y * (p1.x - p3.x)) /
    (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        out.x = p1.x + (t * s1_x);
        out.y = p1.y + (t * s1_y);
        return true;
    }
    return false;
}

template<typename T>
std::vector<RayHit>& Light<T>::getLight(){
    return rayHits;
}

template<typename T>
bool Light<T>::isActivity(){
    return isActivitys;
}

template<typename T>
void Light<T>::enlargeMaxDist(float time){
    isUpdate = true;
    maxDist += 0.3f;
    if(maxDist <= 5){
         maxDist = 6.f;
         isActivitys = true;
    }
    if(maxDist > 80) maxDist = 80.f;
}

template<typename T>
void Light<T>::reduceMaxDist(float time){
    isUpdate = true;
    maxDist -= 0.03f;
    if(maxDist < 5){
        maxDist = 0;
        isActivitys = false;
    }
}

template<typename T>
float Light<T>::getMaxDist(){
    return maxDist;
}

template<typename T>
sf::Vector2f Light<T>::getCoordinate(){
    return LightObject->getCoordinate();
}

template<typename T>
void Light<T>::restart(float time){
    maxDist = 80.f;
    isActivitys = true;
    isUpdate = true;
    update(time);
}

template<typename T>
std::pair<float, bool> Light<T>::takeDataForContinue(){
    return {maxDist, isActivitys};
}

template<typename T>
void Light<T>::setDataForContinue(std::pair<float, bool> pair, float time){
    if (!LightObject) return;
    maxDist = pair.first;
    isActivitys = pair.second;
    isUpdate = true;
    update(time);
}

#endif