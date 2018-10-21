#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
protected:
	sf::RenderWindow* window_;

public:
	GameObject(sf::RenderWindow*);
    virtual void update(float) = 0;
};

