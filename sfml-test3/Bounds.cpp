#include "Bounds.h"






Bounds::Bounds(sf::RenderWindow* window, const string & filename, float width, float height):
    GameObject(window),height_(height),width_(width) {
    

    horizontalBoundsTexture_.create(width, 30);
    if (!horizontalBoundsTexture_.loadFromFile(filename)) {
        cerr << "Texture wasn't opened";
        throw exception("Texture wasn't opened");
    }


    verticalBoundsTexture_.create(30, height);
    if (!verticalBoundsTexture_.loadFromFile(filename)) {
        cerr << "Texture wasn't opened";
        throw exception("Texture wasn't opened");
    }

    horizontalBoundsTexture_.setRepeated(true);
    verticalBoundsTexture_.setRepeated(true);
    

    leftBound_.setTexture(verticalBoundsTexture_);
    leftBound_.setTextureRect(sf::IntRect(0, 0, 30, height_));
    leftBound_.setPosition(sf::Vector2f(0.f,0.f));

    righttBound_.setTexture(verticalBoundsTexture_);
    righttBound_.setTextureRect(sf::IntRect(0, 0, 30, height_));
    righttBound_.setPosition(sf::Vector2f(width_-30.f, 0.f));

    lowerBound_.setTexture(horizontalBoundsTexture_);
    lowerBound_.setTextureRect(sf::IntRect(0, 0, width_, 30));
    lowerBound_.setPosition(sf::Vector2f(0.f, height_-30.f));

    upperBound_.setTexture(horizontalBoundsTexture_);
    upperBound_.setTextureRect(sf::IntRect(0, 0, width_, 30));
    upperBound_.setPosition(sf::Vector2f(0.f, 0.f));
}

void Bounds::update(float time) {}

void Bounds::draw() {
    window_->draw(leftBound_);
    window_->draw(righttBound_);
    window_->draw(lowerBound_);
    window_->draw(upperBound_);
}

float Bounds::getHeight() {
    return height_;
}

float Bounds::getWidth() {
    return width_;
}

char Bounds::isOut(const sf::Vector2f& pos,float rad) {
    if (pos.x-rad<30) {
        return 'l';
    }
    if (pos.x+rad>width_-30) {
        return 'r';
    }
    if (pos.y-rad<30) {
        return 'u';
    }
    if (pos.y+rad>height_ - 30) {
        return 'l';
    }
    return 'i';
}

Bounds::~Bounds() {
}
