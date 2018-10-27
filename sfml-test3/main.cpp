#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Circle.h"
#include "Coin.h"
#include "CoinTracker.h"
#include "GameScenario.h"


using namespace std;

int main()
{


    char menu='\0';
    while (menu != 'o' && menu != 'c' && menu != 's') {
        cout << "Type \'o\', if you want offline, or \'m\' if you want multiplayer" << endl;
        cin >> menu;
        if (menu == 'm') {
            cout << "Type \'s\', if you want to be a server, or \'c\' if you want to be a client" << endl;
            cin >> menu;
        }
    }


    srand(static_cast<unsigned int>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "C!rcles");


    GameScenario gs(&window, "resources/coin.png", "resources/bounds.png", "resources/space.png");

    switch (menu) {
    case('o'):
        gs.addNewPlayable(100.0f, 1.0f);
        gs.addNewCircle(1, 100.0f, 1.0f);
        gs.addNewCoin(5);
        break;
    case('s'):
        gs.setUpNetwork(NetworkType::SERVER);
        break;
    case('c'):
        gs.setUpNetwork(NetworkType::CLIENT);
        break;
    }
    sf::Clock clock;
    sf::Clock gameTime;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
            if (event.type == sf::Event::Closed) {
                window.close();
            }
		}
        window.clear();
        if (!gs.getEndGame()) {
            float time;
            while ((time = static_cast<float>(clock.getElapsedTime().asMicroseconds())) == 0.f) {}
            clock.restart();
            if (window.hasFocus()) {
                if (time <= 1.f) {
                    time = 0.2f;
                }
                gs.update(time);
                gs.draw();
            }
        }
		window.display();
	}

	return 0;
}