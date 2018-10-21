#pragma once
#include "GameObject.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <exception>

using namespace std;

class Coin :
    public GameObject {
private:
    sf::Sprite coinSprite_;
    sf::Texture coinTexture_;
    sf::Vector2f pos_;
    sf::IntRect pictRect_;
    bool isActive_;
    bool isVisible_;
    float allTime_;
    int callNumbers_;
    float sleepTime_;

    float maxSpawnX_;
    float maxSpawnY_;
public:
    Coin(sf::RenderWindow*,const string&);
    Coin(const Coin&);

    void draw();

    sf::Vector2f getPosition()const;

    bool isVisible()const;
    bool isActive()const;


    void setActive(bool state);
    void setMaxSpawnVect(const sf::Vector2f&);
    void setPosition(const sf::Vector2f&);
    void setVisible(bool state);

    void sleep();
    void spawn();

    void update(float time);
    ~Coin();
};

