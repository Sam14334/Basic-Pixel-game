#include "Print.h" 


Print::Print()
{
	std::cout << "You printed this using another class";
}

Print::~Print()
{
}

movePlayer::movePlayer(sf::Sprite& player, float playerMovespeed, sf::Time delta) {
    sf::Vector2f vRequestedPlayerMovement({ 0.0f,0.0f });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        vRequestedPlayerMovement += sf::Vector2f({ 0,-playerMovespeed });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
        vRequestedPlayerMovement += sf::Vector2f({ 0.,playerMovespeed });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        vRequestedPlayerMovement += sf::Vector2f({ -playerMovespeed,0.0f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
        vRequestedPlayerMovement += sf::Vector2f({ playerMovespeed,0.0f });
    }
    player.move(vRequestedPlayerMovement * delta.asSeconds());
}
