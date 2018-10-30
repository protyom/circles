#include "GameScenario.h"



GameScenario::GameScenario(sf::RenderWindow* window, const string &coinFilename, const string &boundsFilename, const string &backFilename) :
    GameObject(window), coinFilename_(coinFilename),
    gameBounds_(window, boundsFilename,
        static_cast<float>(2 * window->getSize().x), static_cast<float>(2 * window->getSize().y)),
    back_(window, backFilename,
        static_cast<float>(2 * window->getSize().x), static_cast<float>(2 * window->getSize().y)),
        gui_(*window){
    view.reset(sf::FloatRect(0, 0, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
    endGame = false;
    isMenu_ = false;
    netType_ = NetworkType::OFFLINE;
}



void GameScenario::addNewCircle(const int& num = 1, const float& rad = 100.0f, const float& speed = 1.0f) {
    int last = circles_.size();
    for (int i = 0; i < num; ++i) {
        circles_.push_back(new Circle(window_, rad, speed));
        circles_[last]->setMaxSpawnVect(sf::Vector2f(gameBounds_.getWidth(), gameBounds_.getHeight()));
        circles_[last]->spawn();
        ++last;
    }
}

void GameScenario::addNewPlayable(const float& rad = 100.0f, const float& speed = 1.0f) {
    circles_.push_back(new Circle(window_, rad, speed));
    circles_[circles_.size() - 1]->setPlayable(true);
    circles_[circles_.size() - 1]->setMaxSpawnVect(sf::Vector2f(gameBounds_.getWidth(), gameBounds_.getHeight()));
    circles_[circles_.size() - 1]->spawn();
}


void GameScenario::addNewCoin(const int& num = 1) {
    for (int i = 0; i < num; ++i) {
        coins_.push_back(new Coin(window_, coinFilename_));
        coins_[i]->setMaxSpawnVect(sf::Vector2f(gameBounds_.getWidth(), gameBounds_.getHeight()));
    }
    for (int i = 0; i < coins_.size(); i++) {
        coins_[i]->spawn();
    }
}

//void GameScenario::addBounds(const string& filename, float width, float height) {}

void GameScenario::draw() {
    back_.draw();
    if (!isMenu_) {
        for (int i = 0; i < coins_.size(); i++) {
            coins_[i]->draw();
        }

        for (int i = 0; i < circles_.size(); i++) {
            circles_[i]->draw();
        }

        gameBounds_.draw();
    } else {
        gui_.draw();
    }
}

bool GameScenario::getEndGame() const {
    return endGame;
}

void GameScenario::handleEvent(sf::Event& event) {
    gui_.handleEvent(event);
}

void GameScenario::handleMenu(tgui::Button::Ptr but) {
    sf::String name(but->getText());
    if (name == "Offline") {
        this->addNewPlayable(100.0f, 1.0f);
        this->addNewCircle(1, 100.0f, 1.0f);
        this->addNewCoin(5);
        this->removeMenu();
    } else if (name == "Multiplayer") {
        this->removeMenu();
        this->loadMultiplayerMenu();
    } else if (name == "Back") {
        this->removeMenu();
        this->loadMainMenu();
    } else if (name == "Client") {
        this->removeMenu();
        this->loadClientMenu();
    } else if (name == "Server") {
        this->removeMenu();
        this->loadServerMenu();
    }
}

void GameScenario::handleClientMenu(tgui::Button::Ptr but,tgui::Label::Ptr label,tgui::EditBox::Ptr editBox) {
    sf::String str(but->getText());
    if (str == "Back") {
        this->removeMenu();
        this->loadMultiplayerMenu();
    } else {
        sf::IpAddress serverIp(editBox->getText());
        if (mainSocket_.connect(serverIp, 27015) != sf::Socket::Done) {
            label->setText("Wrong IP.Reconnect");
            return;
        }
        label->setText(sf::String("Connected to ")+ editBox->getText());
        gui_.draw();
        setUpNetwork(NetworkType::CLIENT);
    }
}

void GameScenario::handleServerMenu(tgui::Button::Ptr but, tgui::Label::Ptr label) {
    sf::String str(but->getText());
    if (str == "Back") {
        this->removeMenu();
        this->loadMultiplayerMenu();
    } else if(str == "Search"){
        listener.listen(27015);
        sf::TcpSocket* client = new sf::TcpSocket();
        sockets_.push_back(client);
        listener.accept(*client);
        label->setText(client->getRemoteAddress().toString() + sf::String(" users connected"));
    } else if (str == "Play!") {
        setUpNetwork(NetworkType::SERVER);
    }
}

void GameScenario::loadClientMenu() {
    isMenu_ = true;
    auto label = tgui::Label::create("Enter IP-Adress");
    label->setSize({ "66.67%", "12.5%" });
    label->setPosition({ "16.67%", "10%" });
    label->setTextSize(40);
    label->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    label->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    label->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    gui_.add(label);

    auto editBoxIp = tgui::EditBox::create();
    editBoxIp->setSize({ "66.67%", "12.5%" });
    editBoxIp->setPosition({ "16.67%", "25%" });
    editBoxIp->setDefaultText("IP Address");
    editBoxIp->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    editBoxIp->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    editBoxIp->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    editBoxIp->setTextSize(40);
    gui_.add(editBoxIp);
    
    auto button2 = tgui::Button::create("Connect");
    button2->setSize({ "10%", "6%" });
    button2->setPosition({ "16.67%", "40%" });
    button2->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button2->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button2->setTextSize(24);
    gui_.add(button2);

    auto button3 = tgui::Button::create("Back");
    button3->setSize({ "16.67%", "10%" });
    button3->setPosition({ "16.67%", "60%" });
    button3->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button3->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button3->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button3->setTextSize(40);
    gui_.add(button3);

    button2->connect("pressed", &GameScenario::handleClientMenu, this, button2, label, editBoxIp);
    button3->connect("pressed", &GameScenario::handleClientMenu, this, button3, label, editBoxIp);
}

void GameScenario::loadMainMenu() {
    isMenu_ = true;
    auto button1 = tgui::Button::create("Offline");
    button1->setSize({ "16.67%", "10%"  });
    button1->setPosition({ "10%", "20%" });
    button1->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button1->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button1->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button1->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button1->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button1->setTextSize(40);
    gui_.add(button1);

    auto button2 = tgui::Button::create("Multiplayer");
    button2->setSize({ "16.67%", "10%" });
    button2->setPosition({ "10%", "40%" });
    button2->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button2->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button2->setTextSize(40);
    gui_.add(button2);

    
    button1->connect("pressed", &GameScenario::handleMenu, this, button1);
    button2->connect("pressed", &GameScenario::handleMenu, this, button2);
}

void GameScenario::loadMultiplayerMenu() {
    isMenu_ = true;
    auto button1 = tgui::Button::create("Client");
    button1->setSize({ "16.67%", "10%" });
    button1->setPosition({ "10%", "20%" });
    button1->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button1->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button1->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button1->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button1->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button1->setTextSize(40);
    gui_.add(button1);

    auto button2 = tgui::Button::create("Server");
    button2->setSize({ "16.67%", "10%" });
    button2->setPosition({ "10%", "40%" });
    button2->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button2->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button2->setTextSize(40);
    gui_.add(button2);

    auto button3 = tgui::Button::create("Back");
    button3->setSize({ "16.67%", "10%" });
    button3->setPosition({ "10%", "60%" });
    button3->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button3->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button3->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button3->setTextSize(40);
    gui_.add(button3);


    button1->connect("pressed", &GameScenario::handleMenu, this, button1);
    button2->connect("pressed", &GameScenario::handleMenu, this, button2);
    button3->connect("pressed", &GameScenario::handleMenu, this, button3);
}

void GameScenario::loadServerMenu() {
    isMenu_ = true;
    auto label = tgui::Label::create(sf::String("Search for clients.Your IP: ")+sf::IpAddress::getLocalAddress().toString());
    label->setSize({ "66.67%", "12.5%" });
    label->setPosition({ "16.67%", "10%" });
    label->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    label->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    label->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    label->setTextSize(40);
    gui_.add(label);

    auto button2 = tgui::Button::create("Search");
    button2->setSize({ "16.67%", "10%" });
    button2->setPosition({ "16.67%", "40%" });
    button2->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button2->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button2->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button2->setTextSize(40);
    gui_.add(button2);

    auto button4 = tgui::Button::create("Play!");
    button4->setSize({ "16.67%", "10%" });
    button4->setPosition({ "16.67%", "60%" });
    button4->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button4->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button4->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button4->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button4->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button4->setTextSize(40);
    gui_.add(button4);

    auto button3 = tgui::Button::create("Back");
    button3->setSize({ "16.67%", "10%" });
    button3->setPosition({ "16.67%", "80%" });
    button3->getRenderer()->setFont("resources/PlayfairDisplay-Regular.ttf");
    button3->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setBackgroundColor(tgui::Color(21, 21, 21, 255));
    button3->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255, 255));
    button3->getRenderer()->setTextColorHover(tgui::Color(0, 0, 0, 255));
    button3->setTextSize(40);
    gui_.add(button3);

    button2->connect("pressed", &GameScenario::handleServerMenu, this, button2, label);
    button3->connect("pressed", &GameScenario::handleServerMenu, this, button3, label);
    button4->connect("pressed", &GameScenario::handleServerMenu, this, button4, label);
}

void GameScenario::removeMenu() {
    gui_.removeAllWidgets();
    isMenu_ = false;
}


void GameScenario::setUpNetwork(NetworkType newNetType) {
    this->removeMenu();
    startNew();
    netType_ = newNetType;
    if (netType_ == NetworkType::SERVER) {
        //sf::TcpListener listener;
        //listener.listen(27015);
        sf::Packet packet;
        /*cout << "Your IP adress is: " << sf::IpAddress::getLocalAddress() << endl;
        cout << "Waiting for clients" << endl;
        cout << "Press \'f\', if you are ready" << endl;
        char done = '\0';
        cin >> done;
        while (done != 'f') {
            sf::TcpSocket* client = new sf::TcpSocket();
            sockets_.push_back(client);
            listener.accept(*client);

            cout << sockets_.size() << " users connected" << endl;
            cout << "Press \'f\', if you are ready" << endl;
            cin >> done;
        }*/
        addNewPlayable(100.0f, 1.0f);
        addNewCircle(sockets_.size(), 100.0f, 1.0f);
        addNewCoin(5);
        for (int i = 0; i < sockets_.size(); i++) {
            packet.clear();
            packet << circles_.size() << i+1;
            sockets_[i]->send(packet);
            for (int j = 0; j < circles_.size(); j++){
                packet.clear();
                sf::Color cl(circles_[j]->getColor());
                packet << cl.r << cl.g << cl.b << cl.a;
                sf::Vector2f pos=circles_[j]->getPosition();
                packet << pos.x << pos.y<< circles_[j]->getRad();
                sockets_[i]->send(packet);
            }
            packet.clear();
            packet << coins_.size() << i;
            sockets_[i]->send(packet);
            for (int j = 0; j < coins_.size(); j++) {
                packet.clear();
                sf::Vector2f pos = coins_[j]->getPosition();
                packet << pos.x << pos.y;
                sockets_[i]->send(packet);
            }
        }

    } else if (netType_ = NetworkType::CLIENT) {
        sf::Packet packet;
        mainSocket_.receive(packet);
        int circles_num=0;
        packet >> circles_num;
        addNewCircle(circles_num, 100.0f, 1.0f);
        int playable_num=0;
        packet >> playable_num;
        circles_[playable_num]->setPlayable(true);
        for (int i = 0; i < circles_.size(); i++) {
            mainSocket_.receive(packet);

            sf::Uint8 r;
            sf::Uint8 g;
            sf::Uint8 b;
            sf::Uint8 a;

            packet >> r >> g >> b >> a;
            circles_[i]->setColor(sf::Color(r, g, b, a));
            

            float x;
            float y;
            packet >> x >> y;
            circles_[i]->attachTo(sf::Vector2f(x, y));


        }
        int coins_num;
        mainSocket_.receive(packet);
        packet >> coins_num;
        addNewCoin(coins_num);
        for (size_t i = 0; i < coins_.size(); i++) {
            mainSocket_.receive(packet);

            float x;
            float y;
            packet >> x >> y;
            coins_[i]->setPosition(sf::Vector2f(x, y));


        }

    }

}

void GameScenario::startNew() {
    for (int i = 0; i < coins_.size(); i++) {
        delete coins_[i];
    }
    for (int i = 0; i < circles_.size(); i++) {
        delete circles_[i];
    }
    for (int i = 0; i < sockets_.size(); i++) {
        delete sockets_[i];
    }

}

GameScenario::~GameScenario() {
    for (int i = 0; i < coins_.size(); i++) {
        delete coins_[i];
    }
    for (int i = 0; i < circles_.size(); i++) {
        delete circles_[i];
    }
    for (int i = 0; i < sockets_.size(); i++) {
        delete sockets_[i];
    }
}

void GameScenario::update(float time) {
    if (!isMenu_) {
        if (netType_ == NetworkType::OFFLINE) {
            updateForOffline(time);
        } else if (netType_ == NetworkType::SERVER) {
            updateForServer(time);
        } else {
            updateForClient(time);
        }
    }
}

void GameScenario::updateForOffline(float time) {
    for (int i = 0; i < circles_.size(); i++) {
        circles_[i]->update(time);
        char out = gameBounds_.isOut(circles_[i]->getPosition(), circles_[i]->getRad());
        if (out != 'i') {
            sf::Vector2f newPosition = circles_[i]->getPosition();
            if (out == 'l') {
                newPosition.x += circles_[i]->getSpeed()*time / 1000.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'r') {
                newPosition.x -= circles_[i]->getSpeed()*time / 1000.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'u') {
                newPosition.y += circles_[i]->getSpeed()*time / 1000.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'l') {
                newPosition.y -= circles_[i]->getSpeed()*time / 1000.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
        }
        if (circles_[i]->isPlayable()) {
            sf::Vector2f playerPos = circles_[i]->getPosition();
            if (playerPos.x < 960.f) {
                playerPos.x = 960.f;
            }
            if (playerPos.x > gameBounds_.getWidth() - 960.f) {
                playerPos.x = gameBounds_.getWidth() - 960.f;
            }
            if (playerPos.y < 540.f) {
                playerPos.y = 540.f;
            }
            if (playerPos.y > gameBounds_.getHeight() - 540.f) {
                playerPos.y = gameBounds_.getHeight() - 540.f;
            }
            view.setCenter(playerPos);
            window_->setView(view);
        }
    }
    for (int i = 0; i < coins_.size(); i++) {
        coins_[i]->update(time);
    }
    for (int i = 0; i < circles_.size(); i++) {
        for (int j = 0; j < coins_.size(); j++) {
            if (circles_[i]->isInside(coins_[j]->getPosition()) && coins_[j]->isActive() && circles_[i]->getRad()<MAX_RAD) {
                circles_[i]->catchCoin();
                coins_[j]->sleep();
            }
        }
    }
    for (int i = 0; i < circles_.size(); i++) {
        for (int j = 0; j < circles_.size(); j++) {
            if (i == j) {
                continue;
            }
            if (distanceFromTo(circles_[i]->getPosition(), circles_[j]->getPosition())<circles_[i]->getRad() + circles_[j]->getRad()) {
                circles_[i]->injure();
            }
        }
    }
    int lost = 0;
    for (int i = 0; i < circles_.size(); i++) {
        if (circles_[i]->getPlayState() == playState::lost) {
            lost++;
        }
    }
    if (lost == circles_.size() - 1 && circles_.size()>1 && circles_[0]->getPlayState() != playState::lost) {
        cout << "Winner" << endl;
        endGame = true;
    }
    if (circles_[0]->getPlayState() == playState::lost) {
        cout << "Lost" << endl;
        endGame = true;
    }
    gameBounds_.update(time);
    back_.update(time);
}

void GameScenario::updateForClient(float time) {
    sf::Packet packet;
    sf::Vector2f pos;
    for (int i = 0; i < circles_.size(); i++) {
        circles_[i]->update(time);
        if (circles_[i]->isPlayable()) {
            char out = gameBounds_.isOut(circles_[i]->getPosition(), circles_[i]->getRad());
            if (out != 'i') {
                sf::Vector2f newPosition = circles_[i]->getPosition();
                if (out == 'l') {
                    newPosition.x += circles_[i]->getSpeed()*time / 1000.f;
                    circles_[i]->setPosition(newPosition);
                    circles_[i]->setToGoPoint(newPosition);
                }
                if (out == 'r') {
                    newPosition.x -= circles_[i]->getSpeed()*time / 1000.f;
                    circles_[i]->setPosition(newPosition);
                    circles_[i]->setToGoPoint(newPosition);
                }
                if (out == 'u') {
                    newPosition.y += circles_[i]->getSpeed()*time / 1000.f;
                    circles_[i]->setPosition(newPosition);
                    circles_[i]->setToGoPoint(newPosition);
                }
                if (out == 'l') {
                    newPosition.y -= circles_[i]->getSpeed()*time / 1000.f;
                    circles_[i]->setPosition(newPosition);
                    circles_[i]->setToGoPoint(newPosition);
                }
            }
            pos = circles_[i]->getPosition();
            sf::Vector2f playerPos = circles_[i]->getPosition();
            if (playerPos.x < 960.f) {
                playerPos.x = 960.f;
            }
            if (playerPos.x > gameBounds_.getWidth() - 960.f) {
                playerPos.x = gameBounds_.getWidth() - 960.f;
            }
            if (playerPos.y < 540.f) {
                playerPos.y = 540.f;
            }
            if (playerPos.y > gameBounds_.getHeight() - 540.f) {
                playerPos.y = gameBounds_.getHeight() - 540.f;
            }
            view.setCenter(playerPos);
            window_->setView(view);
        }
    }
    for (int i = 0; i < coins_.size(); i++) {
        coins_[i]->updateAnimation(time);
    }
    int lost = 0;
    bool isLost = false;
    for (int i = 0; i < circles_.size(); i++) {
        if (circles_[i]->getPlayState() == playState::lost) {
            lost++;
            if (circles_[i]->isPlayable()) {
                isLost = true;
            }
        }
    }
    if (lost == circles_.size() - 1 && circles_.size()>1 && !isLost) {
        cout << "Winner" << endl;
        endGame = true;
    }
    if (isLost) {
        cout << "Lost" << endl;
        endGame = true;
    }
    gameBounds_.update(time);
    back_.update(time);
    
    packet.clear();
    packet << pos.x << pos.y;
    mainSocket_.send(packet);

    for (size_t i = 0; i < circles_.size(); i++) {


        float x;
        float y;
        float rad;
        float speed;
        int ps;
        mainSocket_.receive(packet);
        packet >> x >> y >> rad >> speed >> ps;
        circles_[i]->setPosition(sf::Vector2f(x, y));
        circles_[i]->setRad(rad);
        circles_[i]->setSpeed(speed);
        circles_[i]->setPlayState(static_cast<playState>(ps));

    }

    for (size_t i = 0; i < coins_.size(); i++) {
        float x;
        float y;
        bool visible;
        bool active;
        mainSocket_.receive(packet);

        packet >> x >> y >> visible >> active;

        coins_[i]->setPosition(sf::Vector2f(x, y));
        coins_[i]->setVisible(visible);
        coins_[i]->setActive(active);

    }
}

void GameScenario::updateForServer(float time) {
    sf::Packet packet;
    for (int i = 0; i < circles_.size(); i++) {
        circles_[i]->update(time);
        char out = gameBounds_.isOut(circles_[i]->getPosition(), circles_[i]->getRad());
        if (out != 'i') {
            sf::Vector2f newPosition = circles_[i]->getPosition();
            if (out == 'l') {
                newPosition.x += 1.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'r') {
                newPosition.x -= 1.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'u') {
                newPosition.y += 1.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
            if (out == 'l') {
                newPosition.y -= 1.f;
                circles_[i]->setPosition(newPosition);
                circles_[i]->setToGoPoint(newPosition);
            }
        }
        if (circles_[i]->isPlayable()) {
            sf::Vector2f playerPos = circles_[i]->getPosition();
            if (playerPos.x < 960.f) {
                playerPos.x = 960.f;
            }
            if (playerPos.x > gameBounds_.getWidth() - 960.f) {
                playerPos.x = gameBounds_.getWidth() - 960.f;
            }
            if (playerPos.y < 540.f) {
                playerPos.y = 540.f;
            }
            if (playerPos.y > gameBounds_.getHeight() - 540.f) {
                playerPos.y = gameBounds_.getHeight() - 540.f;
            }
            view.setCenter(playerPos);
            window_->setView(view);
        }
        

    }


    for (int i = 0; i < coins_.size(); i++) {
        coins_[i]->update(time);
    }
    for (int i = 0; i < circles_.size(); i++) {
        for (int j = 0; j < coins_.size(); j++) {
            if (circles_[i]->isInside(coins_[j]->getPosition()) && coins_[j]->isActive() && circles_[i]->getRad()<MAX_RAD) {
                circles_[i]->catchCoin();
                coins_[j]->sleep();
            }
        }
    }
    for (int i = 0; i < circles_.size(); i++) {
        for (int j = 0; j < circles_.size(); j++) {
            if (i == j) {
                continue;
            }
            if (distanceFromTo(circles_[i]->getPosition(), circles_[j]->getPosition())<circles_[i]->getRad() + circles_[j]->getRad()) {
                circles_[i]->injure();
            }
        }
    }
    int lost = 0;
    for (int i = 0; i < circles_.size(); i++) {
        if (circles_[i]->getPlayState() == playState::lost) {
            lost++;
        }
    }
    if (lost == circles_.size() - 1 && circles_.size()>1 && circles_[0]->getPlayState() != playState::lost) {
        cout << "Winner" << endl;
        endGame = true;
    }
    if (circles_[0]->getPlayState() == playState::lost) {
        cout << "Lost" << endl;
        endGame = true;
    }
    gameBounds_.update(time);
    back_.update(time);

    for (int i = 0; i < sockets_.size(); i++) {

        sockets_[i]->receive(packet);
        float x;
        float y;
        packet >> x >> y;
        circles_[i+1]->attachTo(sf::Vector2f(x, y));

        for (size_t j = 0; j < circles_.size(); j++) {
            packet.clear();
            sf::Vector2f pos = circles_[j]->getPosition();
            float rad = circles_[j]->getRad();
            float speed = circles_[j]->getSpeed();
            playState ps = circles_[j]->getPlayState();
            packet << pos.x << pos.y << rad << speed << static_cast<int>(ps);
            sockets_[i]->send(packet);

        }

        for (size_t j = 0; j < coins_.size(); j++) {
            packet.clear();
            sf::Vector2f pos = coins_[j]->getPosition();
            bool visible = coins_[j]->isVisible();
            bool active = coins_[j]->isActive();
            packet << pos.x << pos.y << visible << active;
            sockets_[i]->send(packet);

        }

    }
}
