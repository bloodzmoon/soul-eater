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
void playerStatus(Player&, Clock&);
void Dashing(Player&);
void drawTiny		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawGiant		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawGhost		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawShaman		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawDarkSphere (vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void Tiny		(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Giant		(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Ghost		(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Shaman		(vector<Enemy>&, vector<Enemy>&, Player&, Ground&, Vector2f&, Vector2f&, Clock&, Vector2f&);
void DarkSphere (vector<Enemy>&, Player&, Ground&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void drawGround(RenderWindow&, Ground&, Ground&, Ground&);
void checkPlayerOnGround(Player&, Ground&, Ground&, Ground&, Vector2f&);

// Global variables =============
bool InterfaceStopMoving;
bool getHit;
int tinyState[5], tinyWait[5], giantState[2], giantWait[2], ghostState[3], shamanState[2], shamanWait[2];
int darkSphereState[10];
int lostHP, InvincibleTime;
float HP = 200.0f, Energy = 100.0f;


// Main =========================
int main()
{
	//create Window
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Soul Eater v1.2", Style::Close);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(screenWidth + 100.0f, screenHeight + 100.0f));

	//create Background
	Texture backgroundTexture, foregroundTexture, backbackTexture;
	RectangleShape background, foreground, backback;
	backgroundTexture.loadFromFile("Res/background.png");
	foregroundTexture.loadFromFile("Res/foreground.png");
	backbackTexture.loadFromFile("Res/Castle5.jpg");
	background.setSize(Vector2f(4200.0f, 1200.0f));
	background.setPosition(-400.0f, -500.0f);
	background.setTexture(&backgroundTexture);
	foreground.setSize(Vector2f(1800.0f, 950.0f));
	foreground.setPosition(900.0f, -100.0f);
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
	Texture playerTexture, playerCombo, playerDash;
	playerTexture.loadFromFile("Res/katana.png");
	playerCombo.loadFromFile("Res/katanaCombo.png");
	playerDash.loadFromFile("Res/katanaDash.png");
	Player player(&playerTexture, Vector2u(4, 4), 0.1f);

	//create Enemy
	Texture tinyTexture, giantTexture, ghostTexture, shamanTexture, darkSphereTexture;
	tinyTexture.loadFromFile("Res/tiny.png");
	giantTexture.loadFromFile("Res/giant.png");
	ghostTexture.loadFromFile("Res/ghost.png");
	shamanTexture.loadFromFile("Res/shaman.png");
	darkSphereTexture.loadFromFile("Res/darkSphere.png");
	vector<Enemy> tinies;
	for (int i = 0; i < 1; i++)
	{
		tinies.push_back(Enemy(&tinyTexture, Vector2u(11, 4), 500.0f + i * 5, 200.0f, 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 1; i++)
	{
		giants.push_back(Enemy(&giantTexture, Vector2u(13, 4), 500.0f + i * 5, 200.0f, 0.12f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 1; i++)
	{
		ghosts.push_back(Enemy(&ghostTexture, Vector2u(8, 1), 500.0f + i * 5, 200.0f, 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	vector<Enemy> shamans;
	for (int i = 0; i < 1; i++)
	{
		shamans.push_back(Enemy(&shamanTexture, Vector2u(8, 4), 500.0f + i * 5, 200.0f, 0.12f, 3));
		Enemy& shaman = shamans[i];
	} 
	vector<Enemy> darkSpheres;
	for (int i = 0; i < 10; i++)
	{
		darkSpheres.push_back(Enemy(&darkSphereTexture, Vector2u(5, 1), 0.0f, -2000.0f, 0.08f, 4));
		Enemy& darkSphere = darkSpheres[i];
	}
	
	//create Ground
	Ground ground(Vector2f(3900.0f, 20.0f), Vector2f(1750.0f, 520.0f));
	Ground ground2(Vector2f(3900.0f, 20.0f), Vector2f(1750.0f, 540.0f));
	Ground wallLeft(Vector2f(20.0f, 2000.0f), Vector2f(-180.0f, 520.0f));
	Ground wallRight(Vector2f(20.0f, 2000.0f), Vector2f(3560.0f, 520.0f));

	//variables
	float time = 0.0f;
	Vector2f position, tinyPosition, giantPosition, ghostPosition, shamanPosition, direction;
	Vector2f darkSpherePosition;
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
			backback.move(+0.1f, 0.0f); foreground.move(+0.05f, 0.0f);
		if (player.velocity.x > 0.0f)
			backback.move(-0.1f, 0.0f); foreground.move(-0.05f, 0.0f);

		time = clock.restart().asSeconds();
		//if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;

		//player status
		Dashing(player);
		playerGetHit();
		playerStatus(player, invincibleTime);

		// Setting
		cameraSetting(window, camera, position);
		if (Keyboard::isKeyPressed(Keyboard::RShift)) HP += 0.1f;
		if (Mouse::isButtonPressed(Mouse::Middle))
		{
			window.close();
			main();
		}
		

		// check hitbox
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);

		// Enemy state
		Tiny		(tinies,  player, ground, position, invincibleTime, direction);
		Giant		(giants,  player, ground, position, invincibleTime, direction);
		Ghost		(ghosts,  player, ground, position, invincibleTime, direction);
		Shaman		(shamans, darkSpheres, player, ground, position, shamanPosition, invincibleTime, direction);
		DarkSphere	(darkSpheres, player, ground, wallLeft, wallRight, position, invincibleTime, direction);
		

		window.clear(Color(150, 150, 150));
		// Draw here
		window.draw(backback);
		window.draw(background);
		drawGround(window, ground, wallLeft, wallRight);

		drawGiant		(giants,	  giantTexture,		 giantPosition,		 position, window, time);
		player.Update(&playerTexture, &playerCombo, &playerDash, position, player.Invincible, time);
		player.Draw(window);
		drawTiny		(tinies,      tinyTexture,       tinyPosition,       position, window, time);
		drawGhost		(ghosts,      ghostTexture,      ghostPosition,      position, window, time);
		drawShaman		(shamans,	  shamanTexture,     shamanPosition,     position, window, time);
		drawDarkSphere	(darkSpheres, darkSphereTexture, darkSpherePosition, position, window, time);


		window.draw(foreground);
		UI.Update(&soulTexture, position, HP, Energy, time, InterfaceStopMoving);
		UI.Draw(window);
		//                                
		window.display();
	}
	return 0;
}

// ==========================================
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
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

void playerStatus(Player& player, Clock& invincibleTime)
{
	if (player.Invincible and invincibleTime.getElapsedTime().asSeconds() > 1.5f)
		player.Invincible = false;
}

void Dashing(Player& player)
{
	if (Energy < 100.0f) Energy += 0.05f;
	else Energy = 100.0f;

	if (Energy >= 50.0f) player.canDash = true;
	else player.canDash = false;

	if (player.Dash) Energy -= 0.5f;
}

void drawTiny(vector<Enemy>& tinies, Texture& tinyTexture, Vector2f& tinyPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (int i = 0; i < tinies.size(); i++)
	{
		tinyPosition = tinies[i].GetPosition();
		tinies[i].Update(&tinyTexture, time, tinyPosition, position, tinyState[i], 0);
		tinies[i].Draw(window);
	}
}

void drawGiant(vector<Enemy>& giants, Texture& giantTexture, Vector2f& giantPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (int i = 0; i < giants.size(); i++)
	{
		giantPosition = giants[i].GetPosition();
		giants[i].Update(&giantTexture, time, giantPosition, position, giantState[i], 1);
		giants[i].Draw(window);
	}
}

void drawGhost(vector<Enemy>& ghosts, Texture& ghostTexture, Vector2f& ghostPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (int i = 0; i < ghosts.size(); i++)
	{
		ghostPosition = ghosts[i].GetPosition();
		ghosts[i].Update(&ghostTexture, time, ghostPosition, position, ghostState[i], 2);
		ghosts[i].Draw(window);
	}
}

void drawShaman(vector<Enemy>& shamans, Texture& shamanTexture, Vector2f& shamanPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (int i = 0; i < shamans.size(); i++)
	{
		shamanPosition = shamans[i].GetPosition();
		shamans[i].Update(&shamanTexture, time, shamanPosition, position, shamanState[i], 3);
		shamans[i].Draw(window);
	}
}

void drawDarkSphere(vector<Enemy>& darkSpheres, Texture& darkSphereTexture, Vector2f& darkSpherePosition, Vector2f& position, RenderWindow& window, float time)
{
	for (int i = 0; i < darkSpheres.size(); i++)
	{
		darkSpherePosition = darkSpheres[i].GetPosition();
		darkSpheres[i].Update(&darkSphereTexture, time, darkSpherePosition, position, darkSphereState[i], 4);
		darkSpheres[i].Draw(window);
	}
}

void drawGround(RenderWindow& window, Ground& ground, Ground& wallLeft, Ground& wallRight)
{
	ground.Draw(window);
	wallLeft.Draw(window);
	wallRight.Draw(window);
}

void checkPlayerOnGround(Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight, Vector2f& direction)
{
	if (ground.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction, 0);
	if (wallLeft.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction, 0);
	if (wallRight.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction, 0);

}

void Tiny(vector<Enemy>& tinies, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (int i = 0; i < tinies.size(); i++)
	{
	// State
		if (player.GetHitBox().CheckCollide(tinies[i].GetViewBox(), direction, 0.0f, false) and tinyState[i] == 1)
			tinyState[i] = 2;
		if (tinyState[i] == 2) tinyWait[i]++;
		if (tinyState[i] == 2 and tinyWait[i] >= 500)
		{
			tinyState[i] = 3;
			tinyWait[i] = 0;
		}
		if (player.GetHitBox().CheckCollide(tinies[i].GetCollider(), direction, 0.0f, false) and tinyState[i] == 3)
		{
			tinyState[i] = 4;
			tinies[i].animation.isCombo = true;
			tinies[i].animation.currentImage.x = 0;
		}
		if (tinyState[i] == 4 and !tinies[i].animation.isCombo)
			tinyState[i] = 1;

	// Hit Box
		if (player.GetHitBox().CheckCollide(tinies[i].GetAttackBox(), direction, 0.0f, false) and tinyState[i] == 4 and !player.Invincible and !player.Dash)
		{
			getHit = true;
			invincibleTime.restart().asSeconds();
			player.OnCollision(direction, getHit);
		}
		if (tinies[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false))
		{
			tinies[i].OnCollision(direction);
			tinyState[i] = 0;
		}
		if (ground.GetCollider().CheckCollide(tinies[i].GetCollider(), direction, 1.0f, true))
			tinies[i].OnCollision(direction);

	// Respawn
		if (tinyState[i] == 0)
		{
			tinyState[i] = 1;
			float random = (rand() % 500) + position.x;
			tinies[i].SetPosition(random + 200.0f, 200.0f);
		}
	}
}

void Giant(vector<Enemy>& giants, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (int i = 0; i < giants.size(); i++)
	{
	// State
		if (player.GetHitBox().CheckCollide(giants[i].GetViewBox(), direction, 0.0f, false) and giantState[i] == 1)
			giantState[i] = 2;
		if (giantState[i] == 2) giantWait[i]++;
		if (giantState[i] == 2 and giantWait[i] >= 500)
		{
			giantState[i] = 3;
			giantWait[i] = 0;
		}
		if (player.GetHitBox().CheckCollide(giants[i].GetViewBox(), direction, 0.0f, false) and giantState[i] == 3)
		{
			giantState[i] = 4;
			giants[i].animation.isCombo = true;
			giants[i].animation.currentImage.x = 0;
		}
		if (giantState[i] == 4 and !giants[i].animation.isCombo)
			giantState[i] = 1;

	// Hit Box
		if (player.GetHitBox().CheckCollide(giants[i].GetAttackBox(), direction, 0.0f, false) and giantState[i] == 4 and !player.Invincible and !player.Dash)
		{
			getHit = true;
			invincibleTime.restart().asSeconds();
			player.OnCollision(direction, getHit);
		}
		if (giants[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false))
		{
			giants[i].OnCollision(direction);
			giantState[i] = 0;
		}
		if (ground.GetCollider().CheckCollide(giants[i].GetCollider(), direction, 1.0f, true))
			giants[i].OnCollision(direction);

	// Respawn
		if (giantState[i] == 0)
		{
			giantState[i] = 1;
			float random = (rand() % 500) + position.x;
			giants[i].SetPosition(random + 200.0f, 200.0f);
		}
	}
}

void Ghost(vector<Enemy>& ghosts, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (int i = 0; i < ghosts.size(); i++)
	{
	// Hit Box
		if (player.GetHitBox().CheckCollide(ghosts[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash)
		{
			getHit = true;
			invincibleTime.restart().asSeconds();
			player.OnCollision(direction, getHit);
		}
		if (ghosts[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false))
		{
			ghosts[i].OnCollision(direction);
			ghostState[i] = 0;
		}
		if (ground.GetCollider().CheckCollide(ghosts[i].GetCollider(), direction, 1.0f, true))
			ghosts[i].OnCollision(direction);

	// Respawn
		if (ghostState[i] == 0)
		{
			ghostState[i] = 1;
			float random = (rand() % 500) + position.x;
			ghosts[i].SetPosition(random + 200.0f, 200.0f);
		}
	}
}

void Shaman(vector<Enemy>& shamans, vector<Enemy>& darkSpheres, Player& player, Ground& ground, Vector2f& position, Vector2f& shamanPosition, Clock& invincibleTime, Vector2f& direction)
{
	for (int i = 0; i < shamans.size(); i++)
	{
	// State
		if (player.GetHitBox().CheckCollide(shamans[i].GetViewBox(), direction, 0.0f, false) and shamanState[i] == 1)
			shamanState[i] = 2;
		if (shamanState[i] == 2) shamanWait[i]++;
		if (shamanState[i] == 2 and shamanWait[i] >= 1000)
		{
			shamanState[i] = 4;
			shamanWait[i] = 0;
			shamans[i].animation.isCombo = true;
			shamans[i].animation.currentImage.x = 0;
		}
		if (shamanState[i] == 4 and !shamans[i].animation.isCombo)
			shamanState[i] = 1;
		if (shamans[i].castDarkSphere)
		{
			shamanWait[i]++;
			if (shamanWait[i] >= 320)
			{
				if (shamans[i].faceRight)
				{
					for (int j = 0; j < darkSpheres.size(); j++)
					{
						if (darkSphereState[j] == 9)
						{
							darkSphereState[j] = 1;
							darkSpheres[j].faceRight = true;
							darkSpheres[j].SetPosition(shamanPosition.x + 40.0f, shamanPosition.y);
							shamans[i].castDarkSphere = false;
							shamanState[i] = 1;
							break;
						}
					}
				}
				else
				{
					for (int j = 0; j < darkSpheres.size(); j++)
					{
						if (darkSphereState[j] == 9)
						{
							darkSphereState[j] = 1;
							darkSpheres[j].faceRight = false;
							darkSpheres[j].SetPosition(shamanPosition.x - 40.0f, shamanPosition.y);
							shamans[i].castDarkSphere = false;
							shamanState[i] = 1;
							break;
						}
					}
				}
			}
		}

	// Hit Box
		if (shamans[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false))
		{
			shamans[i].OnCollision(direction);
			shamanState[i] = 0;
		}
		if (ground.GetCollider().CheckCollide(shamans[i].GetCollider(), direction, 1.0f, true))
			shamans[i].OnCollision(direction);

	// Respawn
		if (shamanState[i] == 0)
		{
			shamanState[i] = 1;
			float random = (rand() % 500) + position.x;
			shamans[i].SetPosition(random + 200.0f, 200.0f);
		}
	}
}

void DarkSphere(vector<Enemy>& darkSpheres, Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (int i = 0; i < darkSpheres.size(); i++)
	{
		if (player.GetHitBox().CheckCollide(darkSpheres[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash)
		{
			getHit = true;
			invincibleTime.restart().asSeconds();
			player.OnCollision(direction, getHit);
		}
		if (darkSphereState[i] == 0)
			darkSphereState[i] = 9; // standby
		if (wallLeft.GetCollider().CheckCollide(darkSpheres[i].GetCollider(), direction, 1.0f, true))
			darkSphereState[i] = 9;
		if (wallRight.GetCollider().CheckCollide(darkSpheres[i].GetCollider(), direction, 1.0f, true))
			darkSphereState[i] = 9;
		if (darkSphereState[i] == 9)
			darkSpheres[i].SetPosition(0.0f, 2000.0f);
	}
}