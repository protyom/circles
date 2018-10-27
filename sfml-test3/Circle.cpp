#include "Circle.h"
#include <cmath>
#include <conio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

float vectorLength(const sf::Vector2f& vect) {
    return sqrt(vect.x*vect.x + vect.y*vect.y);
}

sf::Vector2f normalize(const sf::Vector2f& vect) {
    if (vectorLength(vect) != 0) {
        return sf::Vector2f(vect) / vectorLength(vect);
    } else
        return sf::Vector2f();
}

float distanceFromTo(sf::Vector2f from, sf::Vector2f to) {
    return vectorLength(to - from);
}

Circle::Circle(sf::RenderWindow* window,const float& rad, const float& speed): 
    GameObject(window),baseRad_(rad),baseSpeed_(speed) {
    state_ = playState::playing;
    cs_.setRadius(rad);
    rad_ = rad;
    cs_.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    cs_.setOutlineColor(sf::Color(sf::Color::White));
    cs_.setPosition(0, 0);
    pos_.x = 0.f+rad_;
    pos_.y = 0.f+rad_;
    togo_ = pos_;
    speed_ = speed;
    isPlayable_ = false;
}

Circle::Circle(const Circle& circle) : GameObject(circle.window_),
    baseRad_(circle.baseRad_),baseSpeed_(circle.baseSpeed_) {

    cs_.setRadius(circle.rad_);
    rad_ = circle.rad_;
    cs_.setFillColor(circle.cs_.getFillColor());
    cs_.setOutlineColor(circle.cs_.getOutlineColor());
    cs_.setPosition(circle.getPosition()-sf::Vector2f(rad_,rad_));
    pos_ = circle.getPosition();
    togo_ = pos_;
    speed_ = circle.speed_;
    isPlayable_ = circle.isPlayable_;
}

void Circle::draw() {
    window_->draw(cs_);
}

void Circle::setToGoPoint(const sf::Vector2f & togo) {
    togo_ = togo;
}

void Circle::setColor(const sf::Color& color) {
    cs_.setFillColor(color);
}

void Circle::setMaxSpawnVect(const sf::Vector2f& vect) {
    maxSpawnX_ = vect.x;
    maxSpawnY_ = vect.y;
}

void Circle::setPlayable(bool state) {
    isPlayable_ = state;
}

void Circle::setPlayState(playState state) {
    state_ = state;
}

sf::Color Circle::getColor() const {
    return cs_.getFillColor();
}

sf::Vector2f Circle::getToGoPont() const {
    return togo_;
}

bool Circle::isPlayable() const {
    return isPlayable_;
}

void Circle::moveBy(const sf::Vector2f & move) {
    pos_ += move;
    cs_.move(move);
}

bool Circle::isInside(const sf::Vector2f& vect)const {
    if (vectorLength(vect - pos_) < rad_)
        return true;
    else
        return false;
}

void Circle::attachTo(const sf::Vector2f& pos) {
    setToGoPoint(pos);
    setPosition(pos);
}

void Circle::catchCoin() {
    rad_ = rad_ + rad_ / 20;
    cs_.setRadius(rad_);
    if (rad_ > 100.0f) {
        setSpeed(baseSpeed_*baseRad_ / rad_);
    }
}

void Circle::setPosition(const sf::Vector2f &pos) {
    pos_ = pos;
    cs_.setPosition(sf::Vector2f(pos.x - rad_, pos.y - rad_));
}

void Circle::setRad(const float rad) {
    rad_ = rad;
}

void Circle::setSpeed(float speed) {
    speed_ = speed;
}

sf::Vector2f Circle::getPosition() const {
    return pos_;
}

bool Circle::getPlayable() const {
    return isPlayable_;
}

playState Circle::getPlayState() const {
    return state_;
}

float Circle::getRad() const {
    return rad_;
}

float Circle::getSpeed() const {
    return speed_;
}

void Circle::injure() {
    rad_ -= 0.5f;
    cs_.setRadius(rad_);
    if (rad_ > 100.0f) {
        setSpeed(baseSpeed_*baseRad_ / rad_);
    }
    if (rad_ < 5.f) {
        state_ = playState::lost;
    }
}

void Circle::spawn() {
    sf::Vector2f newPos = sf::Vector2f(static_cast<float>(60 + rand() % (static_cast<int>(maxSpawnX_ - 120))),
        static_cast<float>(60 + rand() % (static_cast<int>(maxSpawnY_ - 120))));
    attachTo(newPos);
}

void Circle::update(float time) {
    if (distanceFromTo(pos_, togo_) > 1.f) {
        this->moveBy(normalize(togo_ - pos_)*speed_*time/1000.f);
    }
    else {
        setPosition(togo_);
    }
    if (isPlayable_) {
        sf::Vector2f keyboardMove(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // this->moveBy(sf::Vector2f(-1.0f, 0)*speed_*time);
            keyboardMove += sf::Vector2f(-1.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            //     this->moveBy(sf::Vector2f(0, 1.0f)*speed_*time);
            keyboardMove += sf::Vector2f(0, -1.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //   this->moveBy(sf::Vector2f(1.0f, 0)*speed_*time);
            keyboardMove += sf::Vector2f(1.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            // this->moveBy(sf::Vector2f(0, -1.0f)*speed_*time);
            keyboardMove += sf::Vector2f(0, 1.0f);
        }
        togo_ += normalize(keyboardMove)*speed_*time / 1000.f;
        //system("cls");
        //cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


            sf::Vector2f screenCenter(static_cast<float>(window_->getSize().x / 2), static_cast<float>(window_->getSize().y / 2));

            sf::View view = window_->getView();
            sf::Vector2f realCenterPos = view.getCenter();

            sf::Vector2f clickPos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window_));

            sf::Vector2f moveby = clickPos - screenCenter;
            togo_ = realCenterPos + moveby;
        }
    }
}


