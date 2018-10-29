#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Circle.h"
#include "Coin.h"
#include "CoinTracker.h"
#include "GameScenario.h"
#include <TGUI\TGUI.hpp>

using namespace std;


char setMenuChoice(tgui::Button::Ptr but) {
    sf::String name(but->getText());
    if (name == "Offline") {
        return 'o';
    } else if (name == "Multiplayer") {
        return 'm';
    }
    return '\0';
}


void loadWidgets(tgui::Gui& gui) {


    auto button1 = tgui::Button::create("Offline");
    button1->setSize({ "50%", "16.67%" });
    button1->setPosition({ "25%", "50%" });
    gui.add(button1);
    
    auto button2 = tgui::Button::create("Multiplayer");
    button2->setSize({ "50%", "16.67%" });
    button1->setPosition({ "75%", "50%" });
    gui.add(button1);

    button1->connect("pressed", setMenuChoice,button1);
    button2->connect("pressed", setMenuChoice, button2);
}


int main()
{


    /*char menu='\0';
    while (menu != 'o' && menu != 'c' && menu != 's') {
        cout << "Type \'o\', if you want offline, or \'m\' if you want multiplayer" << endl;
        cin >> menu;
        if (menu == 'm') {
            cout << "Type \'s\', if you want to be a server, or \'c\' if you want to be a client" << endl;
            cin >> menu;
        }
    }*/


    srand(static_cast<unsigned int>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "C!rcles");


    GameScenario gs(&window, "resources/coin.png", "resources/bounds.png", "resources/space.png");
    gs.loadMainMenu();

   /*switch (menu) {
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
    }*/
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
            gs.handleEvent(event);
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