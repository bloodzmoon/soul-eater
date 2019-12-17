#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Ground.h"

#define screenWidth 800.0f
#define screenHeight 512.0f
using namespace sf;
using namespace std;


// Prototypes ===================

// Global variables =============


// Main =========================
int main()
{
	//create Window
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Soul Eater v1.0", Style::Close);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(screenWidth, screenHeight));


	//create Player
	Texture playerTexture;
	playerTexture.loadFromFile("Res/katana.png");
	Player player(&playerTexture, Vector2u(4, 4), 0.1f);

	
	//create Ground
	Ground ground(Vector2f(3900.0f, 20.0f), Vector2f(1600.0f, 520.0f));

	//variables
	float time = 0.0f;
	Vector2f position, direction;
	Clock clock;

	// Main game =====================================
	while (window.isOpen())
	{
		Event button;
		while (window.pollEvent(button))
			if (button.type == Event::Closed)
				window.close();


		position = player.GetPosition();

		time = clock.restart().asSeconds();
		if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;

		// Setting
		camera.setCenter(position.x, position.y - 80);
		window.setView(camera);
	
		// check hitbox
		if(ground.GetCollider().CheckCollide(player.GetCollider(), direction, 1.0f, true))
			player.OnCollision(direction);


		window.clear(Color(150, 150, 150));
		// Draw here
		player.Update(&playerTexture, position, time); 
		player.Draw(window);
		ground.Draw(window);

		//                                
		window.display();
	}

	return 0;
}

// Functions =================================
