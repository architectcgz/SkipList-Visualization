
#include "SkipList.h"

int main()
{

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SkipList visualization");
    window.setPosition(sf::Vector2i(0, 0));
    SkipList skipList(&window);
    skipList.operate();

    return 0;
}
