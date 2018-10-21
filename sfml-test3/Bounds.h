#pragma once
#include "GameObject.h"
#include <string>
#include <iostream>


using namespace std;

class Bounds :
    public GameObject {
private:
    sf::Texture verticalBoundsTexture_;
    sf::Texture horizontalBoundsTexture_;
    sf::Sprite leftBound_;
    sf::Sprite righttBound_;
    sf::Sprite lowerBound_;
    sf::Sprite upperBound_;
    float height_;
    float width_;
public:
    Bounds(sf::RenderWindow* window, const string& filename,float width,float height);
    void update(float time);
    void draw();
    
    float getHeight();
    float getWidth();

    char isOut(const sf::Vector2f& pos,float rad);
    ~Bounds();
};

