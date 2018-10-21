#include <SFML/Graphics.hpp>
#include <vector>
#include "Circle.h"
#include "Coin.h"
#include "CoinTracker.h"
#include "GameScenario.h"


using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    /*Circle c(&window, 100.0f,1.0f);
    Coin coin(&window,"resources/coin.png");
    vector<Coin*> coins;
    coins.push_back(&coin);
    vector<Circle*> circles;
    circles.push_back(&c);

    CoinTracker ct(&circles,&coins);


    coin.spawn();
    coin.setPosition(sf::Vector2f(960,540));*/

    GameScenario gs(&window, "resources/coin.png", "resources/bounds.png", "resources/space.png");


    gs.addNewPlayable(100.0f,1.0f);
    gs.addNewCircle(1, 100.0f, 1.0f);
    gs.addNewCoin(5);
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
            float time = clock.restart().asMilliseconds();
            if (window.hasFocus()) {
                gs.update(time);
                gs.draw();
            }
        }
		window.display();
	}

	return 0;
}