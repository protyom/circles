#include "Coin.h"






Coin::Coin(sf::RenderWindow* window,const string& filename):GameObject(window) {

    
    if (!coinTexture_.loadFromFile(filename)) {
        cerr << "Texture wasn't opened";
        throw exception("Texture wasn't opened");
    }
    pos_ = sf::Vector2f(0.f, 0.f);
    coinSprite_.setPosition(pos_);
    coinSprite_.setTexture(coinTexture_);
    pictRect_ = sf::IntRect(0,0,200, 200);
    coinSprite_.setScale(sf::Vector2f(0.5f,0.5f));
    coinSprite_.setTextureRect(pictRect_);
    
    
    allTime_ = 0.0f;
    callNumbers_ = 0;

    isActive_ = false;
    isVisible_ = false;
}

Coin::Coin(const Coin &coin):GameObject(coin.window_) {



    /*if (!coinTexture_.loadFromFile(coin.)) {
        cerr << "Texture wasn't opened";
        throw exception("Texture wasn't opened");
    }*/
    coinTexture_ = coin.coinTexture_;
    coinSprite_.setTexture(coinTexture_);
    pictRect_ = coin.pictRect_;
    coinSprite_.setScale(sf::Vector2f(0.5f, 0.5f));
    coinSprite_.setTextureRect(pictRect_);


    allTime_ = 0.0f;
    callNumbers_ = 0;

    isActive_ = coin.isActive();
    isVisible_ = coin.isVisible();
}

void Coin::setVisible(bool state) {
    isVisible_ = state;
}

void Coin::setActive(bool state) {
    isActive_ = state;
    sleepTime_ = 0;
}

void Coin::setMaxSpawnVect(const sf::Vector2f& vect) {
    maxSpawnX_ = vect.x;
    maxSpawnY_ = vect.y;
}

bool Coin::isVisible() const {
    return isVisible_;
}

bool Coin::isActive() const {
    return isActive_;
}

void Coin::setPosition(const sf::Vector2f &pos) {
    pos_ = pos;
    auto temp = pos;
    temp.x -= 100.f;
    temp.y -= 100.f;
    coinSprite_.setPosition(temp);
}

sf::Vector2f Coin::getPosition() const {
    return pos_;
}

void Coin::draw() {
    if (isVisible_) {
        window_->draw(coinSprite_);
    }
}

void Coin::spawn() {
    if (!isActive_ && !isVisible_) {
        sf::Vector2f newPos = sf::Vector2f(static_cast<float>(60+rand() % (static_cast<int>(maxSpawnX_-120))),
            static_cast<float>(60+rand() % (static_cast<int>(maxSpawnY_ - 120))));
        //setPosition(newPos);
        setPosition(newPos);
        this->setActive(true);
        this->setVisible(true);
    }
}

void Coin::sleep() {
    this->setActive(false);
    this->setVisible(false);
}

void Coin::update(float time) {
    allTime_ += time;
    if (isActive_) {
        if ((callNumbers_ % 25) == 0) {
            if (pictRect_.left == 1000) {
                pictRect_.left = 0;
            } else {
                pictRect_.left += 200;
            }
        }

        coinSprite_.setTextureRect(pictRect_);
        callNumbers_++;
    }
    else {
        sleepTime_ += time;
        if (sleepTime_ > 3000) {
            this->spawn();
        }
    }
}


Coin::~Coin() {
}
