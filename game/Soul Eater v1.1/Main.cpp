#include <SFML/Graphics.hpp>
#include <vector>
#include "Interface.h"
#include "Player.h"
#include "Enemy.h"
#include "Ground.h"

#define screenWidth 800.0f
#define screenHeight 512.0f
using namespace sf;
using namespace std;


// Prototypes ===================
void cameraSetting(RenderWindow&, View, Vector2f);
void playerGetHit();
void playerDash(Player&);

// Global variables =============
bool InterfaceStopMoving;
bool getHit, Invincible;
bool tinyState[5], giantState[2], ghostState[3];

int lostHP, InvincibleTime;
float HP = 200.0f, Energy = 100.0f;

// Main =========================
int main()
{
	//create Window
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Soul Eater v1.1", Style::Close);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(screenWidth, screenHeight));

	//create Background
	Texture backgroundTexture, foregroundTexture, backbackTexture;
	RectangleShape background, foreground, backback;
	backgroundTexture.loadFromFile("Res/background.png");
	foregroundTexture.loadFromFile("Res/foreground.png");
	backbackTexture.loadFromFile("Res/Castle5.jpg");
	background.setSize(Vector2f(3800.0f, 950.0f));
	background.setPosition(-200.0f, -300.0f);
	background.setTexture(&backgroundTexture);
	foreground.setSize(Vector2f(2800.0f, 950.0f));
	foreground.setPosition(-200.0f, -300.0f);
	foreground.setTexture(&foregroundTexture);
	backback.setSize(Vector2f(4800.0f, 950.0f));
	backback.setPosition(-200.0f, -300.0f);
	backback.setTexture(&backbackTexture);

	//create Interface
	Texture soulTexture, enemyCountTexture;
	soulTexture.loadFromFile("Res/soul.png");
	enemyCountTexture.loadFromFile("Res/enemyCount.png");
	Interface UI(&soulTexture, &enemyCountTexture, Vector2u(8, 1), 0.2f);

	//create Player
	Texture playerTexture, playerCombo;
	playerTexture.loadFromFile("Res/katana.png");
	playerCombo.loadFromFile("Res/katanaCombo.png");
	Player player(&playerTexture, Vector2u(4, 4), 0.1f);

	//create Enemy
	Texture tinyTexture, giantTexture, ghostTexture;
	tinyTexture.loadFromFile("Res/tiny.png");
	giantTexture.loadFromFile("Res/giant.png");
	ghostTexture.loadFromFile("Res/ghost.png");
	vector<Enemy> tinies;
	for (int i = 0; i < 5; i++)
	{
		tinies.push_back(Enemy(&tinyTexture, Vector2u(8, 4), 500.0f + i * 5, 200.0f, 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 2; i++)
	{
		giants.push_back(Enemy(&giantTexture, Vector2u(11, 4), 500.0f + i * 5, 200.0f, 0.15f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 3; i++)
	{
		ghosts.push_back(Enemy(&ghostTexture, Vector2u(8, 4), 500.0f + i * 5, 200.0f, 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	
	//create Ground
	Ground ground(Vector2f(3900.0f, 20.0f), Vector2f(1600.0f, 520.0f));

	//variables
	float time = 0.0f;
	Vector2f position, tinyPosition, giantPosition, ghostPosition, direction;
	Clock clock, invincibleTime;

	// Main game =====================================
	while (window.isOpen())
	{
		Event button;
		while (window.pollEvent(button))
			if (button.type == Event::Closed)
				window.close();


		position = player.GetPosition();
		if (player.velocity.x < 0.0f)
		{
			backback.move(+0.1f, 0.0f); foreground.move(+0.05f, 0.0f);
		}
		if (player.velocity.x > 0.0f)
		{
			backback.move(-0.1f, 0.0f); foreground.move(-0.05f, 0.0f);
		}

		time = clock.restart().asSeconds();
		if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;

		//player status
		playerDash(player);
		playerGetHit();
		if (Invincible and invincibleTime.getElapsedTime().asSeconds() > 1.5f)
			Invincible = false;

		// Setting
		cameraSetting(window, camera, position);
		if (Mouse::isButtonPressed(Mouse::Middle)) HP += 0.08f;
		

		// check hitbox
		for (int i = 0; i < tinies.size(); i++)
		{
			if (player.GetCollider().CheckCollide(tinies[i].GetCollider(), direction, 0.0f, false) and !Invincible and !player.Dash)
			{
				getHit = true;
				invincibleTime.restart().asSeconds();
				Invincible = true;
				player.OnCollision(direction);
			}
			if (tinies[i].GetCollider().CheckCollide(player.GetAttackBox(), direction, -10.0f, false))
			{
				tinies[i].OnCollision(direction);
				tinyState[i] = false;
			}
			if (tinyState[i] == false)
			{
				tinyState[i] = true;
				float random = (rand() % 500) + position.x;
				tinies[i].SetPosition(random + 200.0f, 200.0f);
			}
			if (ground.GetCollider().CheckCollide(tinies[i].GetCollider(), direction, 1.0f, true))
				tinies[i].OnCollision(direction);
		}
		for (int i = 0; i < giants.size(); i++)
		{
			if (player.GetCollider().CheckCollide(giants[i].GetCollider(), direction, 0.0f, false) and !Invincible and !player.Dash)
			{
				getHit = true;
				invincibleTime.restart().asSeconds();
				Invincible = true;
				player.OnCollision(direction);
			}
			if (giants[i].GetCollider().CheckCollide(player.GetAttackBox(), direction, -10.0f, false))
			{
				giants[i].OnCollision(direction);
				giantState[i] = false;
			}
			if (giantState[i] == false)
			{
				giantState[i] = true;
				float random = (rand() % 500) + position.x;
				giants[i].SetPosition(random + 200.0f, 200.0f);
			}
			if (ground.GetCollider().CheckCollide(giants[i].GetCollider(), direction, 1.0f, true))
				giants[i].OnCollision(direction);
		}
		for (int i = 0; i < ghosts.size(); i++)
		{
			if (player.GetCollider().CheckCollide(ghosts[i].GetCollider(), direction, 0.0f, false) and !Invincible and !player.Dash)
			{
				getHit = true;
				invincibleTime.restart().asSeconds();
				Invincible = true;
				player.OnCollision(direction);
			}
			if (ghosts[i].GetCollider().CheckCollide(player.GetAttackBox(), direction, -10.0f, false))
			{
				ghosts[i].OnCollision(direction);
				ghostState[i] = false;
			}
			if (ghostState[i] == false)
			{
				ghostState[i] = true;
				float random = (rand() % 500) + position.x;
				ghosts[i].SetPosition(random + 200.0f, 200.0f);
			}
			if (ground.GetCollider().CheckCollide(ghosts[i].GetCollider(), direction, 1.0f, true))
				ghosts[i].OnCollision(direction);
		}
		if(ground.GetCollider().CheckCollide(player.GetCollider(), direction, 1.0f, true))
			player.OnCollision(direction);


		window.clear(Color(150, 150, 150));
		// Draw here
		window.draw(backback);
		window.draw(background);
		ground.Draw(window);

		for (int i = 0; i < giants.size(); i++)
		{
			giantPosition = giants[i].GetPosition();
			giants[i].Update(&giantTexture, time, position, giantState[i], 1);
			giants[i].Draw(window);
		}
		player.Update(&playerTexture, &playerCombo, position, getHit, time);
		player.Draw(window);
		for (int i = 0; i < tinies.size(); i++)
		{
			tinyPosition = tinies[i].GetPosition();
			tinies[i].Update(&tinyTexture, time, position, tinyState[i], 0);
			tinies[i].Draw(window);
		}
		for (int i = 0; i < ghosts.size(); i++)
		{
			ghostPosition = ghosts[i].GetPosition();
			ghosts[i].Update(&ghostTexture, time, position, ghostState[i], 2);
			ghosts[i].Draw(window);
		}


		window.draw(foreground);
		UI.Update(&soulTexture, position, HP, Energy, time, InterfaceStopMoving);
		UI.Draw(window);
		//                                
		window.display();
	}

	return 0;
}

// Functions =================================
void cameraSetting(RenderWindow& window, View camera, Vector2f player)
{
	if (player.x > 200 and player.x < 3200)
	{
		camera.setCenter(player.x, player.y - 80);
		InterfaceStopMoving = false;
	}
	else if (player.x <= 200)
	{
		camera.setCenter(200, player.y - 80);
		InterfaceStopMoving = true;
	}
	else if (player.x >= 3200)
	{
		camera.setCenter(3200, player.y - 80);
		InterfaceStopMoving = true;
	}
	window.setView(camera);
}

void playerGetHit()
{
	if (getHit and lostHP < 300 and HP>0)
	{
		HP -= 0.08f;
		lostHP++;
	}
	if (lostHP == 300)
	{
		getHit = false;
		lostHP = 0;
	}
}

void playerDash(Player& player)
{
	if (Energy < 100.0f) Energy += 0.03f;
	else Energy = 100.0f;

	if (Energy >= 50.0f) player.canDash = true;
	else player.canDash = false;

	if (player.Dash) Energy -= 0.5f;
}