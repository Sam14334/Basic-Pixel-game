#include <iostream> 
#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <random> 
#include <string> 

float Length(const sf::Vector2f& rVector) {
    float fLength = sqrt(rVector.x * rVector.x + rVector.y * rVector.y);
    return fLength;
}

sf::Vector2f Normalize(const sf::Vector2f& rVector) {

    float fLength = Length(rVector);
    sf::Vector2f vNormalizedVector = { rVector.x / fLength , rVector.y / fLength };
    return vNormalizedVector;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Basic Pixel Game!");
    window.setFramerateLimit(60);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("Sprites/Player.png");
    sf::Sprite player(playerTexture);
    player.setPosition({ 100,100 });
    player.setOrigin({ 8,8 });
    player.setScale({ 10,10 });

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("Sprites/Enemy.png");
    sf::Sprite enemy(enemyTexture);
    enemy.setPosition({ 900,100 });
    enemy.setOrigin({ 8,8 });
    enemy.setScale({ 10,10 });

    sf::Texture weaponTexture;
    weaponTexture.loadFromFile("Sprites/Weapon.png");
    sf::Sprite weapon(weaponTexture);
    weapon.setPosition({ 500,100 });
    weapon.setOrigin({ 7.5f,7.5f });
    weapon.setScale({ 10,10 });

    sf::CircleShape circle(30.0f);
    circle.setPosition({ 100,200 });
    circle.setFillColor(sf::Color::Red);
    circle.setOrigin({ 30.0f / 2,30.0f / 2 });

    //Clock for delta time
    sf::Clock clock;

    std::vector<sf::Sprite> enemies;
    for (int i = 0; i < 1; i++) {
        enemies.push_back(enemy);
    }


    sf::Font font("Fonts/pixelFont.ttf");
    sf::Text text(font);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    text.setPosition({ 15,2 });
    text.setString("Level: 1");

    float playerMovespeed = 300.0f;
    float enemyMoveSpeed = 100.0f;
    short level = 1;
    while (window.isOpen())
    {
        //Restart the clock and get time as delta
        sf::Time delta = clock.restart();


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            //This type of event is for one time keys
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    player.move({ 0,-playerMovespeed });
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Tab) {
                    circle.setPosition(player.getPosition());

                }

            }

        }


        //Player movement
        sf::Vector2f vRequestedPlayerMovement({ 0.0f,0.0f });

        //This type of event is for held keys
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

        sf::Vector2f vMousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
        sf::Vector2f vPlayerToMouse = vMousePosition - player.getPosition();

        sf::Vector2f vPlayerToMouseNormalize = Normalize(vPlayerToMouse);
        weapon.setPosition(player.getPosition() + vPlayerToMouseNormalize * 160.0f);

        //for loop for each enemy sprite in vector enemies
        for (int i = 0; i < enemies.size(); i++) {


            //Did the weapon hit the enemy  
            sf::Vector2f vWeaponToEnemy = enemies[i].getPosition() - weapon.getPosition();
            float fLengthFromWeaponToEnemy = Length(vWeaponToEnemy);

            if (fLengthFromWeaponToEnemy < 160.0f) {
                sf::Vector2f vRandomPosition(std::rand() % 1920, std::rand() % 1080); //random pos based on screen size
                enemies[i].setPosition(vRandomPosition);

                if (enemies.size() < 6 && level % 3 == 0) {//spawns enemies 
                    enemies.push_back(enemy);
                    enemyMoveSpeed += 40.0f;
                }
                level++;

                text.setString("Level: " + std::to_string(level));
            }

            //move enemy to player
            sf::Vector2f vEnemyToPlayer = player.getPosition() - enemies[i].getPosition();
            vEnemyToPlayer = Normalize(vEnemyToPlayer);

            enemies[i].move(vEnemyToPlayer * delta.asSeconds() * enemyMoveSpeed);

            //game over
            sf::Vector2f vPlayerToEnemy = enemies[i].getPosition() - player.getPosition();
            float fLengthPlayerToEnemy = Length(vPlayerToEnemy);

            if (fLengthPlayerToEnemy < 110.0f) {
                window.close();
            }

        }


        //trying to make a bullet 
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            circle.setPosition(player.getPosition());
        }

        sf::Vector2f vBulletToMouse = vMousePosition - circle.getPosition();
        vBulletToMouse = Normalize(vBulletToMouse);
        circle.move(vBulletToMouse * delta.asSeconds() * 400.0f);


        /*std::cout << "player pos: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
        std::cout << "player distance to weapon: " << vPlayerToMouse.x << ", " << vPlayerToMouse.y << std::endl;
        std::cout << "weapon position: " << sf::Vector2f(player.getPosition() + vPlayerToMouse).x << ", " << sf::Vector2f(player.getPosition() + vPlayerToMouse).y << std::endl;
        std::cout << "mouse position: " << vMousePosition.x << ", " << vMousePosition.y << std::endl;*/



        window.clear();
        for (const auto& enemy1 : enemies) {
            window.draw(enemy1);
        }
        window.draw(circle);
        window.draw(player);
        window.draw(weapon);
        window.draw(text);

        window.display();
    }
}
