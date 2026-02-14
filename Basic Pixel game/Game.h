#pragma once
#include <iostream> 
#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <random>  
#include "Game.h"
#include "Print.h"

class Game {
public: 
	Game();
	~Game();

	enum GameMode {
		Play,
		LevelEditor,
	};

	 
	void Run();
	
private: 
	void UpdatePlay(); 
	void UpdateLevelEditor();
	void Draw();

private:
	sf::RenderWindow m_Window; 
	GameMode m_eGameMode; 

	 
};