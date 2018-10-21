#include "Background.h"



Background::Background(sf::RenderWindow * window, const string & filename, float width, float height) :
    GameObject(window), height_(height), width_(width) {
    if (!backTexture_.loadFromFile(filename)) {
        cerr << "Texture wasn't opened";
        throw exception("Texture wasn't opened");
    }
    backTexture_.setRepeated(true);
    backSprite_.setTexture(backTexture_);
    backSprite_.setTextureRect(sf::IntRect(0, 0, width_, height_));
    backSprite_.setPosition(sf::Vector2f(0.f, 0.f));
}

void Background::update(float time) {
}

void Background::draw() {
    window_->draw(backSprite_);
}

Background::~Background() {
}
