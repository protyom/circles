#pragma once
#include <vector>
#include <string>
#include "Circle.h"
#include "Coin.h"
#include "Bounds.h"
#include "Background.h"
#include <SFML\Network.hpp>
#include <TGUI\TGUI.hpp>

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

    tgui::Gui gui_;
    NetworkType netType_;
    sf::TcpSocket mainSocket_;
    vector<  sf::TcpSocket* >  sockets_;
    sf::TcpListener listener;
    int numberOfPlayers;
    int circlesNum;
    int coinsNum;

    const float MAX_RAD = 500.f;

    bool isMenu_;
    bool endGame;
public:
    GameScenario(sf::RenderWindow*, const string&, const string&,const string&);
    void addNewCircle(const int&,const float&,const float&);
    void addNewPlayable(const float&, const float&);
    void addNewCoin(const int&);
    //void addBounds(const string&,float,float);
    
    void draw();
    
    bool getEndGame()const;

    void handleEvent(sf::Event& event);
    void handleMenu(tgui::Button::Ptr but);
    void handleClientMenu(tgui::Button::Ptr but, tgui::Label::Ptr label, tgui::EditBox::Ptr editBox);
    void handleServerMenu(tgui::Button::Ptr but, tgui::Label::Ptr label);

    void loadClientMenu();
    void loadMainMenu();
    void loadMultiplayerMenu();
    void loadServerMenu();
    
    void removeMenu();

    void setUpNetwork(NetworkType newNetType);
    void startNew();

    void update(float);
    void updateForOffline(float);
    void updateForClient(float);
    void updateForServer(float);

    ~GameScenario();
};

