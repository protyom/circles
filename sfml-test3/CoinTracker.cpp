#include "CoinTracker.h"




CoinTracker::CoinTracker(vector<Circle*>* circles, vector<Coin*>* coins):circles_(circles),coins_(coins) {
}

void CoinTracker::update(float time) {
    for(auto circle : *circles_) {
        for (auto coin : *coins_) {
            if (circle->isInside(coin->getPosition()) && coin->isActive()) {
                circle->catchCoin();
                coin->setVisible(false);
                coin->setActive(false);
            }
        }
    }
}

CoinTracker::~CoinTracker() {
}
