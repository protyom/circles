#include "GameScenario.h"





GameScenario::GameScenario(sf::RenderWindow* window, const string &coinFilename, const string &boundsFilename, const string &backFilename) :
    GameObject(window), coinFilename_(coinFilename),
    gameBounds_(window, boundsFilename,
        static_cast<float>(2 * window->getSize().x), static_cast<float>(2 * window->getSize().y)),
    back_(window, backFilename,
        static_cast<float>(2 * window->getSize().x), static_cast<float>(2 * window->getSize().y)) {
    view.reset(sf::FloatRect(0, 0, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
    endGame = false;
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
    for (int i = 0; i < coins_.size(); i++) {
        coins_[i]->draw();
    }

    for (int i = 0; i < circles_.size(); i++) {
        circles_[i]->draw();
    }

    gameBounds_.draw();

}

bool GameScenario::getEndGame() const {
    return endGame;
}

void GameScenario::setUpNetwork(NetworkType newNetType) {
    netType_ = newNetType;
    if (netType_ == NetworkType::SERVER) {
        sf::TcpListener listener;
        listener.listen(2000);
        sf::Packet packet;
        cout << "Your IP adress is: " << sf::IpAddress::getLocalAddress() << endl;
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
        }
        addNewPlayable(100.0f, 1.0f);
        addNewCircle(sockets_.size(), 100.0f, 1.0f);
        for (int i = 0; i < sockets_.size(); i++) {

            for (int j = 0; j < circles_.size(); j++){
                packet.clear();
                sf::Color cl(circles_[i]->getColor());
                packet << cl.r << cl.g << cl.b << cl.a;
                sf::Vector2f pos=circles_[i]->getPosition();
                packet<<pos.x<<pos.y
            }
        }

    } else if (netType_ = NetworkType::CLIENT) {
        cout << "Enter server\'s IP adress" << endl;
        sf::IpAddress serverIp;
        cin >> serverIp;
        mainSocket_.connect(serverIp, 2000);
        sf::Packet packet;
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
    if (netType_ == NetworkType::OFFLINE)         {
        updateForOffline(time);
    } else if (netType_ == NetworkType::SERVER) {
        updateForServer(time);
    } else {
        updateForClient(time);
    }
}

void GameScenario::updateForOffline(float time) {
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
}

void GameScenario::updateForClient(float time) {
    sf::Packet packet;
    mainSocket_.receive(packet);
    
}

void GameScenario::updateForServer(float time) {
}