#pragma once
#include "GameObject.h"
#include <string>
#include <iostream>

using namespace std;

class Background :
    public GameObject {
private:
    sf::Texture backTexture_;
    sf::Sprite backSprite_;
    float height_;
    float width_;
public:
    Background(sf::RenderWindow* window, const string& filename, float width, float height);
    void update(float time);
    void draw();
    ~Background();
};

