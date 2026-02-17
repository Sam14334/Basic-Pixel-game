//refactor/convert-to-oop BRANCH
#include "Game.h"
#include "MathHelpers.h"

Game::Game()
    :m_Window(sf::VideoMode({ 1920, 1080 }), "Basic Pixel Game!")
    ,m_eGameMode(Play)
{
}

Game::~Game()
{
}

void Game::Run()
{  
    m_Window.setFramerateLimit(60);

    switch (m_eGameMode) {
    case Play:
        UpdatePlay();
    case LevelEditor:
        UpdateLevelEditor();

    }

}

void Game::UpdatePlay()
{

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

    //Clock for delta time
    sf::Clock clock;

    std::vector<sf::Sprite> enemies;
    enemies.push_back(enemy);

    sf::Font font("Fonts/pixelFont.ttf");
    sf::Text text(font);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    text.setPosition({ 15,2 });
    text.setString("Level: 1");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("Sprites/slv_bullet.png");
    sf::Sprite projectile(bulletTexture);
    projectile.setPosition({ 100,200 });
    projectile.setOrigin({ 8.0f ,8.0f });
    projectile.setScale({ 5,5 });

    std::vector < sf::Sprite > projectiles;
    projectiles.push_back(projectile);

    std::vector<float> angles = { 0 };
    sf::Vector2f angles2;

    float playerMovespeed = 300.0f;
    float enemyMoveSpeed = 100.0f;
    float playerHealth = 3;
    float shootTimer = 0;
    short level = 1;


    while (m_Window.isOpen())
    {
        //Restart the clock and get time as delta
        sf::Time delta = clock.restart();


        while (const std::optional event = m_Window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                m_Window.close();
            }

            //This type of event is for one time keys
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    m_Window.close();
                }

            }

        }


        //Player movement
        //movePlayer::movePlayer(player, 200.0f, delta);
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

        //This type of event is for held keys


        sf::Vector2f vMousePosition = (sf::Vector2f)sf::Mouse::getPosition(m_Window);
        sf::Vector2f vPlayerToMouse = vMousePosition - player.getPosition();

        sf::Vector2f vPlayerToMouseNormalize = MathHelpers::Normalize(vPlayerToMouse);
        weapon.setPosition(player.getPosition() + vPlayerToMouseNormalize * 160.0f);

        //for loop for each enemy sprite in vector enemies
        for (int i = 0; i < enemies.size(); i++) {


            //Did the weapon hit the enemy  
            sf::Vector2f vWeaponToEnemy = enemies[i].getPosition() - weapon.getPosition();
            float fLengthFromWeaponToEnemy = MathHelpers::Length(vWeaponToEnemy);

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
            vEnemyToPlayer = MathHelpers::Normalize(vEnemyToPlayer);

            enemies[i].move(vEnemyToPlayer * delta.asSeconds() * enemyMoveSpeed);

            //game over
            sf::Vector2f vPlayerToEnemy = enemies[i].getPosition() - player.getPosition();
            float fLengthPlayerToEnemy = MathHelpers::Length(vPlayerToEnemy);

            if (fLengthPlayerToEnemy < 110.0f) {
                sf::Vector2f vRandomPosition(std::rand() % 1920, std::rand() % 1080); //random pos based on screen size
                enemies[i].setPosition(vRandomPosition);
                player.setColor(sf::Color(255 - playerHealth * 15, 10, 10));
                playerHealth--;
                if (playerHealth <= 0) {
                    m_Window.close();
                }

            }

        }


        //trying to make a bullet 
        /*for (int i = 0; i < projectiles.size();i++) {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                projectiles[i].setPosition(player.getPosition());
            }

            sf::Vector2f vBulletToMouse = vMousePosition - projectiles[i].getPosition();
            vBulletToMouse = Normalize(vBulletToMouse);
            projectiles[i].move(vBulletToMouse * delta.asSeconds() * 400.0f);
        }
         */

        shootTimer += 1.0f * delta.asSeconds();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTimer >= 0.2) {
            projectile.setPosition(player.getPosition());
            projectiles.push_back(projectile);

            shootTimer = 0;
            angles.push_back(atan2(vMousePosition.y - player.getPosition().y, vMousePosition.x - player.getPosition().x));
        }


        for (int i = 0; i < projectiles.size(); i++) {

            sf::Vector2f vBulletToMouse = MathHelpers::Normalize(vMousePosition - projectiles[i].getPosition());
            projectiles[i].move({ 15 * cos(angles[i]),15 * sin(angles[i]) });
            for (int j = 0; j < enemies.size(); j++) {

                sf::Vector2f vBulletToEnemy = enemies[j].getPosition() - projectiles[i].getPosition();
                float fLengthFromBulletToEnemy = MathHelpers::Length(vBulletToEnemy);

                if (fLengthFromBulletToEnemy <= 7.5 + 60) {
                    sf::Vector2f vRandomPosition(std::rand() % 1920, std::rand() % 1080); //random pos based on screen size
                    enemies[j].setPosition(vRandomPosition);

                    if (enemies.size() < 8 && level % 3 == 0) {//spawns enemies 
                        enemies.push_back(enemy);
                        enemyMoveSpeed += 40.0f;
                    }
                    level++;

                    text.setString("Level: " + std::to_string(level));
                }
            }

        }

        //oop headers
        //aabb collision
        //tileset/maps
        //health bar/slots
        //bullet cooldown reload
        //potion regen / slot
        //dash
        //test comment remote commit

        m_Window.clear();
        for (const auto& enemy1 : enemies) {
            m_Window.draw(enemy1);
        }
        for (const auto& projectile : projectiles) {
            m_Window.draw(projectile);
        }
        m_Window.draw(player);
        m_Window.draw(weapon);
        m_Window.draw(text);

        m_Window.display();
    }
}

void Game::UpdateLevelEditor()
{
}

void Game::Draw()
{
}
