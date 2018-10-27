#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

float vectorLength(const sf::Vector2f& vect);
sf::Vector2f normalize(const sf::Vector2f& vect);
float distanceFromTo(sf::Vector2f from, sf::Vector2f to);

enum playState{playing,lost,won};

class Circle :
    public GameObject{
private:
    sf::CircleShape cs_;
    sf::Vector2f pos_;
    sf::Vector2f togo_;
    float speed_;
    float rad_;
    playState state_;
    const float baseRad_;
    const float baseSpeed_;
    bool isPlayable_;

    float maxSpawnX_;
    float maxSpawnY_;
public:
    Circle(sf::RenderWindow*,const float&, const float&);
    Circle(const Circle&);


    void attachTo(const sf::Vector2f&);

    void catchCoin();
    void draw();


    sf::Color getColor()const;
    sf::Vector2f getToGoPont()const;
    sf::Vector2f getPosition()const;
    bool getPlayable()const;
    playState getPlayState()const;
    float getRad()const;
    float getSpeed()const;

    void injure();

    bool isInside(const sf::Vector2f&)const;
    bool isPlayable()const;
    void moveBy(const sf::Vector2f&);

    void setColor(const sf::Color&);
    void setMaxSpawnVect(const sf::Vector2f&);
    void setPlayable(bool);
    void setPlayState(playState state);
    void setPosition(const sf::Vector2f&);
    void setRad(const float rad);
    void setSpeed(float speed);
    void setToGoPoint(const sf::Vector2f&);
    

    void spawn();

    void update(float);
};

