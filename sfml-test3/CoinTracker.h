#pragma once
#include <vector>
#include "Circle.h"
#include "Coin.h"

using namespace std;

class CoinTracker {
private:
    vector<Circle*>* circles_;
    vector<Coin*>* coins_;
public:
    CoinTracker(vector<Circle*>*, vector<Coin*>*);
    void update(float time);
    ~CoinTracker();
};

