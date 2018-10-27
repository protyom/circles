#pragma once
#include <vector>
#include <string>
#include "Circle.h"
#include "Coin.h"
#include "Bounds.h"
#include "Background.h"
#include <SFML\Network.hpp>

enum NetworkType {
    OFFLINE,
    SERVER,
    CLIENT
};

class GameScenario:
    public GameObject {
private:
    vector<Circle*> circles_;
    vector<Coin*> coins_;

    sf::View view;
    string coinFilename_;
    Bounds gameBounds_;
    Background back_;

    NetworkType netType_;
    sf::TcpSocket mainSocket_;
    vector<  sf::TcpSocket* >  sockets_;
    int numberOfPlayers;

    const float MAX_RAD = 500.f;
    bool endGame;
public:
    GameScenario(sf::RenderWindow*, const string&, const string&,const string&);
    void addNewCircle(const int&,const float&,const float&);
    void addNewPlayable(const float&, const float&);
    void addNewCoin(const int&);
    //void addBounds(const string&,float,float);
    
    void draw();
    
    bool getEndGame()const;
    
    void setUpNetwork(NetworkType newNetType);
    void startNew();

    void update(float);
    void updateForOffline(float);
    void updateForClient(float);
    void updateForServer(float);

    ~GameScenario();
};

