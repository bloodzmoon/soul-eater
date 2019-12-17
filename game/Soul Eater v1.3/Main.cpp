#include <SFML/Graphics.hpp>
#include <vector>
#include "Interface.h"
#include "Player.h"
#include "Enemy.h"
#include "Ground.h"
#include "Particle.h"

#define screenWidth 800
#define screenHeight 512
using namespace sf;
using namespace std;


// Prototypes ===================
void cameraSetting(RenderWindow&, View&, Player&, RectangleShape&, RectangleShape&, Vector2f, float);
void playerGetHit(Player&);
void playerStatus(Player&, Clock&);
void Dashing(Player&);
void createParticles (vector<Particle>&, vector<Particle>&, Player&);
void endParticles    (vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void drawParticles   (vector<Particle>&, vector<Particle>&, vector<Particle>&, Texture&, Texture&, Player&, Vector2f, float, RenderWindow&);
void drawTiny		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawGiant		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawGhost		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawShaman		(vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawDarkSphere (vector<Enemy>&, Texture&, Vector2f&, Vector2f&, RenderWindow&, float);
void drawSoulEater  (vector<Enemy>&, Texture&, Texture&,  Vector2f&, Vector2f&, RenderWindow&, float);
void Tiny		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Giant		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Ghost		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Shaman		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Vector2f&, Clock&, Vector2f&);
void DarkSphere (vector<Enemy>&, Player&, Ground&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void SoulEater	(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void drawGround (RenderWindow&, Ground&, Ground&, Ground&);
void checkPlayerOnGround(Player&, Ground&, Ground&, Ground&, Vector2f&);

// Global variables =============


// Main =========================
int main()
{
	//create Window
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Soul Eater v1.3", Style::Close);
	window.setFramerateLimit(60);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(screenWidth + 100.0f, screenHeight + 100.0f));

	//create Background
	Texture backgroundTexture, midgroundTexture, foregroundTexture;
	RectangleShape background, midground, foreground;
	backgroundTexture.loadFromFile("Res/bg/HallwayBack.png");
	midgroundTexture.loadFromFile ("Res/bg/HallwayMid.png");
	foregroundTexture.loadFromFile("Res/bg/HallwayFront.png");
	background.setTexture   (&backgroundTexture);
	midground.setTexture    (&midgroundTexture);
	foreground.setTexture   (&foregroundTexture);
	background.setSize		(Vector2f(2652.0f * 1.6f, 1145.0f * 1.6f));
	midground.setSize		(Vector2f(1920.0f * 1.6f, 1145.0f * 1.6f));
	foreground.setSize		(Vector2f(2652.0f * 1.6f, 1145.0f * 1.6f));
	background.setPosition	(-700.0f, -820.0f);
	midground.setPosition	(-700.0f, -720.0f);
	foreground.setPosition	(   0.0f, -320.0f);

	//create Interface
	Texture soulTexture, enemyCountTexture;
	soulTexture.loadFromFile("Res/gui/blue.png");
	enemyCountTexture.loadFromFile("Res/gui/enemyCount.png");
	Interface UI(&soulTexture, &enemyCountTexture, Vector2u(11, 1), 0.08f);

	//create Player
	Texture playerTexture, playerCombo;
	playerTexture.loadFromFile("Res/player/blue.png");
	playerCombo.loadFromFile("Res/player/blueATK.png");
	Player player(&playerTexture, Vector2u(10, 7), 0.1f);

	//create Effect
	Texture dustEffect, hitEffect;
	hitEffect.loadFromFile("Res/particles/hit.png");
	dustEffect.loadFromFile("Res/particles/dust.png");
	vector<Particle> hitParticles;
	for (int i = 0; i < 20; i++)
	{
		hitParticles.push_back(Particle(&hitEffect, Vector2u(5, 1), 0.07f, 1));
		Particle& hitParticle = hitParticles[i];
	}
	vector<Particle> dustParticles;
	for (int i = 0; i < 3; i++)
	{
		dustParticles.push_back(Particle(&dustEffect, Vector2u(14, 2), 0.05f, 2));
		Particle& dustParticle = dustParticles[i];
	}
	vector<Particle> dustParticles2;
	for (int i = 0; i < 3; i++)
	{
		dustParticles2.push_back(Particle(&dustEffect, Vector2u(14, 2), 0.05f, 2));
		Particle& dustParticle2 = dustParticles2[i];
	}

	//create Enemy
	Texture tinyTexture, giantTexture, ghostTexture, shamanTexture, darkSphereTexture;
	Texture souleaterTexture, souleaterCast;
	tinyTexture.loadFromFile("Res/enemy/tiny.png");
	giantTexture.loadFromFile("Res/enemy/giant.png");
	ghostTexture.loadFromFile("Res/enemy/ghost.png");
	shamanTexture.loadFromFile("Res/enemy/shaman.png");
	darkSphereTexture.loadFromFile("Res/enemy/darkSphere.png");
	souleaterTexture.loadFromFile("Res/enemy/SoulEater.png");
	souleaterCast.loadFromFile("Res/enemy/SoulEaterCast.png");
	vector<Enemy> tinies;
	for (int i = 0; i < 0; i++)
	{
		tinies.push_back(Enemy(&tinyTexture, Vector2u(11, 5), 500.0f + i * 5, 200.0f, 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 1; i++)
	{
		giants.push_back(Enemy(&giantTexture, Vector2u(13, 5), 500.0f + i * 5, 200.0f, 0.12f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 0; i++)
	{
		ghosts.push_back(Enemy(&ghostTexture, Vector2u(8, 2), 500.0f + i * 5, 200.0f, 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	vector<Enemy> shamans;
	for (int i = 0; i < 0; i++)
	{
		shamans.push_back(Enemy(&shamanTexture, Vector2u(8, 4), 500.0f + i * 5, 200.0f, 0.12f, 3));
		Enemy& shaman = shamans[i];
	} 
	vector<Enemy> darkSpheres;
	for (int i = 0; i < 3; i++)
	{
		darkSpheres.push_back(Enemy(&darkSphereTexture, Vector2u(5, 1), 0.0f, -2000.0f, 0.08f, 4));
		Enemy& darkSphere = darkSpheres[i];
	}
	vector<Enemy> souleaters;
	for (int i = 0; i < 1; i++)
	{
		souleaters.push_back(Enemy(&souleaterTexture, Vector2u(12, 6), 500.0f, 200.0f, 0.08f, 5));
		Enemy& souleater = souleaters[i];
	}

	
	//create Ground
	Ground ground(Vector2f(2500.0f, 20.0f), Vector2f(830.0f, 520.0f));
	Ground wallLeft(Vector2f(20.0f, 240.0f), Vector2f(-180.0f, 380.0f));
	Ground wallRight(Vector2f(20.0f, 240.0f), Vector2f(1920.0f, 380.0f));

	//variables
	float time = 0.0f;

	Vector2f position, tinyPosition, giantPosition, ghostPosition, shamanPosition, direction;
	Vector2f darkSpherePosition, souleaterPosition;
	Clock clock, invincibleTime;


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

		//player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, player);
		endParticles(hitParticles, dustParticles, dustParticles2, player);

		// Setting
		cameraSetting(window, camera, player, background, foreground, position, time);
		if (Keyboard::isKeyPressed(Keyboard::RShift)) player.HP += 5.1f;
		if (Mouse::isButtonPressed(Mouse::Middle))
		{
			window.close();
			main();
		}
		

		// check hitbox
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);

		// Enemy state
		Tiny		(tinies,      hitParticles, player, ground, position, invincibleTime, direction);
		Giant		(giants,      hitParticles, player, ground, position, invincibleTime, direction);
		Ghost		(ghosts,      hitParticles, player, ground, position, invincibleTime, direction);
		Shaman		(shamans,  darkSpheres, hitParticles, player, ground, position, shamanPosition, invincibleTime, direction);
		DarkSphere	(darkSpheres, player, ground, wallLeft, wallRight, position, invincibleTime, direction);
		SoulEater	(souleaters,  player, ground, position, invincibleTime, direction);
		
	

		window.clear(Color(150, 150, 150));
		// Draw here
		window.draw(background);
		window.draw(midground);
		drawGround(window, ground, wallLeft, wallRight);

		drawGiant		(giants,	  giantTexture,		 giantPosition,		 position, window, time);
		player.Update   (&playerTexture, &playerCombo, position, player.Invincible, time);
		player.Draw     (window);
		drawTiny		(tinies,      tinyTexture,       tinyPosition,       position, window, time);
		drawGhost		(ghosts,      ghostTexture,      ghostPosition,      position, window, time);
		drawShaman		(shamans,	  shamanTexture,     shamanPosition,     position, window, time);
		drawDarkSphere	(darkSpheres, darkSphereTexture, darkSpherePosition, position, window, time);
		drawSoulEater	(souleaters, souleaterTexture, souleaterCast, souleaterPosition, position, window, time);
		drawParticles   (hitParticles, dustParticles, dustParticles2, hitEffect, dustEffect, player, position, time, window);

		window.draw(foreground);
		UI.Update(&soulTexture, player, camera, player.HP, player.Energy, time);
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
void cameraSetting(RenderWindow& window, View& camera, Player& player, RectangleShape& back, RectangleShape& front, Vector2f playerPosition, float time)
{
	Vector2f cameraMovement = player.GetPosition() - camera.getCenter();
	cameraMovement += Vector2f(0.0f, -90.0f);

	camera.move(cameraMovement * time * 2.5f);
	window.setView(camera);

	back.move(cameraMovement * time * -0.25f);
	front.move(cameraMovement * time * -3.5f);
}

void playerGetHit(Player& player)
{
	if (player.getHit and player.HP>0)
		player.HP -= 1.0f;
}

void playerStatus(Player& player, Clock& invincibleTime)
{
	if (player.Invincible and invincibleTime.getElapsedTime().asSeconds() > 1.5f)
		player.Invincible = false;
}

void Dashing(Player& player)
{
	if (player.Energy < 100.0f) player.Energy += 0.5f;
	else						player.Energy = 100.0f;

	if (player.Energy >= 50.0f) player.canDash = true;
	else						player.canDash = false;

	if (player.Dash) player.Energy -= 4.5f;
}

void drawTiny(vector<Enemy>& tinies, Texture& tinyTexture, Vector2f& tinyPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < tinies.size(); i++)
	{
		tinyPosition = tinies[i].GetPosition();
		tinies[i].Update(&tinyTexture, &tinyTexture, time, tinyPosition, position, 0);
		tinies[i].Draw(window);
	}
}

void drawGiant(vector<Enemy>& giants, Texture& giantTexture, Vector2f& giantPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < giants.size(); i++)
	{
		giantPosition = giants[i].GetPosition();
		giants[i].Update(&giantTexture, &giantTexture, time, giantPosition, position, 1);
		giants[i].Draw(window);
	}
}

void drawGhost(vector<Enemy>& ghosts, Texture& ghostTexture, Vector2f& ghostPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		ghostPosition = ghosts[i].GetPosition();
		ghosts[i].Update(&ghostTexture, &ghostTexture, time, ghostPosition, position, 2);
		ghosts[i].Draw(window);
	}
}

void drawShaman(vector<Enemy>& shamans, Texture& shamanTexture, Vector2f& shamanPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < shamans.size(); i++)
	{
		shamanPosition = shamans[i].GetPosition();
		shamans[i].Update(&shamanTexture, &shamanTexture, time, shamanPosition, position, 3);
		shamans[i].Draw(window);
	}
}

void drawDarkSphere(vector<Enemy>& darkSpheres, Texture& darkSphereTexture, Vector2f& darkSpherePosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		darkSpherePosition = darkSpheres[i].GetPosition();
		darkSpheres[i].Update(&darkSphereTexture, &darkSphereTexture, time, darkSpherePosition, position, 4);
		darkSpheres[i].Draw(window);
	}
}

void drawSoulEater(vector<Enemy>& souleaters, Texture& souleaterTexture, Texture& souleaterCast, Vector2f& souleaterPosition, Vector2f& position, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < souleaters.size(); i++)
	{
		souleaterPosition = souleaters[i].GetPosition();
		souleaters[i].Update(&souleaterTexture, &souleaterCast, time, souleaterPosition, position, 5);
		souleaters[i].Draw(window);
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
		player.OnCollision(direction);
	if (wallLeft.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction);
	if (wallRight.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction);

}

void Tiny(vector<Enemy>& tinies, vector<Particle>& hitParticles, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < tinies.size(); i++)
	{
	// State
		if (tinies[i].state == 1 and player.GetHitBox().CheckCollide(tinies[i].GetViewBox(), direction, 0.0f, false))
		{
			tinies[i].state = 2;
			tinies[i].wait.restart().asSeconds();
		}
		else if (tinies[i].state == 2 and tinies[i].wait.getElapsedTime().asSeconds() > 1)
			tinies[i].state = 3;
		else if (tinies[i].state == 3 and player.GetHitBox().CheckCollide(tinies[i].GetCollider(), direction, 0.0f, false))
		{
			tinies[i].state = 4;
			tinies[i].animation.playingAnimation = true;
			tinies[i].animation.currentImage.x = 0;
		}
		else if (tinies[i].state == 4 and !tinies[i].animation.playingAnimation)
			tinies[i].state = 1;
		else if (tinies[i].state == 7 and !tinies[i].animation.playingAnimation)
			tinies[i].state = 2;

	// Hit Box
		if (player.GetHitBox().CheckCollide(tinies[i].GetAttackBox(), direction, 0.0f, false) and tinies[i].state == 4 and !player.Invincible and !player.Dash)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.OnCollision(direction);
		}
		if (tinies[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and tinies[i].state != 7)
		{
			tinies[i].state = 7;
			tinies[i].animation.playingAnimation = true;
			tinies[i].animation.OneTimeAnimation = true;
			tinies[i].animation.OneTimeFrame.x = 5;
			tinies[i].animation.currentImage.x = 0;

			for (unsigned int j = 0; j < hitParticles.size(); j++)
			{
				if (!hitParticles[j].isEffect)
				{
					hitParticles[j].isEffect = true;
					hitParticles[j].animation.playingAnimation = true;
					hitParticles[j].animation.OneTimeAnimation = true;
					hitParticles[j].animation.OneTimeFrame.x = 5;
					hitParticles[j].animation.currentImage.x = 0;
					break;
				}
			}
		}
		if (ground.GetCollider().CheckCollide(tinies[i].GetCollider(), direction, 1.0f, true))
			tinies[i].OnCollision(direction, 0);
	}
}

void Giant(vector<Enemy>& giants, vector<Particle>& hitParticles, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < giants.size(); i++)
	{
	// State
		if (giants[i].state == 1 and player.GetHitBox().CheckCollide(giants[i].GetViewBox(), direction, 0.0f, false))
		{
			giants[i].state = 2;
			giants[i].wait.restart().asSeconds();
		}
		else if (giants[i].state == 2 and giants[i].wait.getElapsedTime().asSeconds() > 1)
			giants[i].state = 3;
		else if (giants[i].state == 3 and player.GetHitBox().CheckCollide(giants[i].GetViewBox(), direction, 0.0f, false))
		{
			giants[i].state = 4;
			giants[i].comboWait.restart().asSeconds();
			giants[i].animation.playingAnimation = true;
			giants[i].animation.currentImage.x = 0;
		}
		else if (giants[i].state == 4 and !giants[i].animation.playingAnimation)
			giants[i].state = 1;
		else if (giants[i].state == 7 and !giants[i].animation.playingAnimation)
			giants[i].state = 1;

	// Hit Box
		if (player.GetHitBox().CheckCollide(giants[i].GetAttackBox(), direction, 0.0f, false) and giants[i].state == 4 and !player.Invincible and !player.Dash)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.OnCollision(direction);
		}
		if (giants[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and giants[i].state != 4 and giants[i].state != 7)
		{
			giants[i].state = 7;
			giants[i].animation.playingAnimation = true;
			giants[i].animation.OneTimeAnimation = true;
			giants[i].animation.OneTimeFrame.x = 5;
			giants[i].animation.currentImage.x = 0;

			for (unsigned int j = 0; j < hitParticles.size(); j++)
			{
				if (!hitParticles[j].isEffect)
				{
					hitParticles[j].isEffect = true;
					hitParticles[j].animation.OneTimeAnimation = true;
					hitParticles[j].animation.playingAnimation = true;
					hitParticles[j].animation.OneTimeFrame.x = 5;
					hitParticles[j].animation.currentImage.x = 0;
					break;
				}
			}
		}
		if (ground.GetCollider().CheckCollide(giants[i].GetCollider(), direction, 1.0f, true))
			giants[i].OnCollision(direction, 0);

	}
}

void Ghost(vector<Enemy>& ghosts, vector<Particle>& hitParticles, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{

	// State
		if (ghosts[i].state == 7 and !ghosts[i].animation.playingAnimation)
			ghosts[i].state = 1;

	// Hit Box
		if (player.GetHitBox().CheckCollide(ghosts[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.OnCollision(direction);
		}
		if (ghosts[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and ghosts[i].state != 7)
		{
			ghosts[i].state = 7;
			ghosts[i].animation.playingAnimation = true;
			ghosts[i].animation.OneTimeAnimation = true;
			ghosts[i].animation.OneTimeFrame.x = 8;
			ghosts[i].animation.currentImage.x = 0;

			for (unsigned int j = 0; j < hitParticles.size(); j++)
			{
				if (!hitParticles[j].isEffect)
				{
					hitParticles[j].isEffect = true;
					hitParticles[j].animation.playingAnimation = true;
					hitParticles[j].animation.OneTimeAnimation = true;
					hitParticles[j].animation.OneTimeFrame.x = 5;
					hitParticles[j].animation.currentImage.x = 0;
					break;
				}
			}
		}
		if (ground.GetCollider().CheckCollide(ghosts[i].GetCollider(), direction, 1.0f, true))
			ghosts[i].OnCollision(direction, 0);

	}
}

void Shaman(vector<Enemy>& shamans, vector<Enemy>& darkSpheres, vector<Particle>& hitParticles, Player& player, Ground& ground, Vector2f& position, Vector2f& shamanPosition, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < shamans.size(); i++)
	{
	// State
		if (shamans[i].state == 1 and player.GetHitBox().CheckCollide(shamans[i].GetViewBox(), direction, 0.0f, false))
		{
			shamans[i].state = 2;
			shamans[i].wait.restart().asSeconds();
		}
		else if (shamans[i].state == 2 and shamans[i].wait.getElapsedTime().asSeconds() > 2)
		{
			shamans[i].state = 4;
			shamans[i].wait.restart().asSeconds();
			shamans[i].animation.playingAnimation = true;
			shamans[i].animation.currentImage.x = 0;
		}
		else if (shamans[i].state == 4 and !shamans[i].animation.playingAnimation)
			shamans[i].state = 1;
		else if (shamans[i].state == 7 and !shamans[i].animation.playingAnimation)
			shamans[i].state = 1;
		if (shamans[i].castDarkSphere)
		{
			if (shamans[i].wait.getElapsedTime().asSeconds() > 0.5)
			{
				if (shamans[i].faceRight)
				{
					for (unsigned int j = 0; j < darkSpheres.size(); j++)
					{
						if (darkSpheres[j].state == 1)
						{
							darkSpheres[j].state = 99;
							darkSpheres[j].faceRight = true;
							darkSpheres[j].SetPosition(shamanPosition.x + 40.0f, shamanPosition.y);
							shamans[i].castDarkSphere = false;
							shamans[i].state = 1;
							break;
						}
					}
				}
				else
				{
					for (unsigned int j = 0; j < darkSpheres.size(); j++)
					{
						if (darkSpheres[j].state == 1)
						{
							darkSpheres[j].state = 99;
							darkSpheres[j].faceRight = false;
							darkSpheres[j].SetPosition(shamanPosition.x - 40.0f, shamanPosition.y);
							shamans[i].castDarkSphere = false;
							shamans[i].state = 1;
							break;
						}
					}
				}
			}
		}

	// Hit Box
		if (shamans[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and shamans[i].state != 7)
		{
			shamans[i].state = 7;
			shamans[i].animation.playingAnimation = true;
			shamans[i].animation.OneTimeAnimation = true;
			shamans[i].animation.OneTimeFrame.x = 4;
			shamans[i].animation.currentImage.x = 0;

			for (unsigned int j = 0; j < hitParticles.size(); j++)
			{
				if (!hitParticles[j].isEffect)
				{
					hitParticles[j].isEffect = true;
					hitParticles[j].animation.playingAnimation = true;
					hitParticles[j].animation.OneTimeAnimation = true;
					hitParticles[j].animation.OneTimeFrame.x = 5;
					hitParticles[j].animation.currentImage.x = 0;
					break;
				}
			}
		}
		if (ground.GetCollider().CheckCollide(shamans[i].GetCollider(), direction, 1.0f, true))
			shamans[i].OnCollision(direction, 0);
	}
}

void DarkSphere(vector<Enemy>& darkSpheres, Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		if (player.GetHitBox().CheckCollide(darkSpheres[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.OnCollision(direction);
		}
		if (wallLeft.GetCollider().CheckCollide(darkSpheres[i].GetCollider(), direction, 1.0f, true))
			darkSpheres[i].state = 1;
		if (wallRight.GetCollider().CheckCollide(darkSpheres[i].GetCollider(), direction, 1.0f, true))
			darkSpheres[i].state = 1;
	}
}

void SoulEater(vector<Enemy>& souleaters, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < souleaters.size(); i++)
	{
		// State
		if (souleaters[i].state == 1 and player.GetHitBox().CheckCollide(souleaters[i].GetViewBox(), direction, 0.0f, false))
		{
			souleaters[i].state = 2;
			souleaters[i].animation.OneTimeAnimation = true;
			souleaters[i].animation.playingAnimation = true;
			souleaters[i].animation.OneTimeFrame.x = 12;
			souleaters[i].animation.currentImage.x = 0;
		}
		else if (souleaters[i].state == 2 and !souleaters[i].animation.playingAnimation)
		{
			souleaters[i].state = 3;
			souleaters[i].wait.restart().asSeconds();
		}
		else if (souleaters[i].state == 3 and souleaters[i].wait.getElapsedTime().asSeconds() > 1.5)
		{
			souleaters[i].state = 4;
			souleaters[i].wait.restart().asSeconds();
		}
		else if (souleaters[i].state == 4 and souleaters[i].wait.getElapsedTime().asSeconds() > 1.5)
		{
			souleaters[i].state = 5;
			souleaters[i].wait.restart().asSeconds();
		}
		else if (souleaters[i].state == 5 and souleaters[i].wait.getElapsedTime().asSeconds() > 1.5)
		{
			souleaters[i].state = 6;
			souleaters[i].animation.OneTimeAnimation = true;
			souleaters[i].animation.playingAnimation = true;
			souleaters[i].animation.OneTimeFrame.x = 42;
			souleaters[i].animation.currentImage.x = 0;
		}
		else if (souleaters[i].state == 6 and !souleaters[i].animation.playingAnimation)
			souleaters[i].state = 7;
			
		
		// Hit Box
		if (player.GetHitBox().CheckCollide(souleaters[i].GetAttackBox(), direction, 0.0f, false) and souleaters[i].state == 4 and !player.Invincible and !player.Dash)
		{
			player.getHit = true;
			invincibleTime.restart().asSeconds();
			player.OnCollision(direction);
		}
		if (ground.GetCollider().CheckCollide(souleaters[i].GetHitBox(), direction, 1.0f, true))
			souleaters[i].OnCollision(direction, 0);
	}
}

void createParticles(vector<Particle>& dustParticles, vector<Particle>& dustParticles2, Player& player)
{
	if (player.Dash and !player.dashDust)
	{
		for (unsigned int i = 0; i < dustParticles.size(); i++)
		{
			if (!dustParticles[i].isEffect)
			{
				player.dashDust = true;
				dustParticles[i].isEffect = true;
				dustParticles[i].animation.OneTimeAnimation = true;
				dustParticles[i].animation.playingAnimation = true;
				dustParticles[i].animation.OneTimeFrame.x = 14;
				dustParticles[i].row = 0;
				dustParticles[i].animation.currentImage.x = 0;
				break;
			}
		}
	}
	if (player.jumping and !player.jumpDust)
	{
		for (unsigned int i = 0; i < dustParticles.size(); i++)
		{
			if (!dustParticles[i].isEffect)
			{
				player.jumpDust = true;
				dustParticles[i].isEffect = true;
				dustParticles[i].animation.OneTimeAnimation = true;
				dustParticles[i].animation.playingAnimation = true;
				dustParticles[i].animation.OneTimeFrame.x = 14;
				dustParticles[i].row = 1;
				dustParticles[i].animation.currentImage.x = 0;
				break;
			}
		}
	}
	if (player.impactGround and player.canJump and !player.fallDust)
	{
		for (unsigned int i = 0; i < dustParticles2.size(); i++)
		{
			if (!dustParticles2[i].isEffect)
			{
				player.impactGround = false;
				player.fallDust = true;
				dustParticles2[i].isEffect = true;
				dustParticles2[i].animation.OneTimeAnimation = true;
				dustParticles2[i].animation.playingAnimation = true;
				dustParticles2[i].animation.OneTimeFrame.x = 14;
				dustParticles2[i].row = 1;
				dustParticles2[i].animation.currentImage.x = 0;
				break;
			}
		}
	}
}

void endParticles(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, Player& player)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
	{
		if (hitParticles[i].isEffect and !hitParticles[i].animation.OneTimeAnimation)
		{
			hitParticles[i].isEffect = false;
			hitParticles[i].setPosition = false;
		}
	}
	for (unsigned int i = 0; i < dustParticles.size(); i++)
	{
		if (dustParticles[i].isEffect and !dustParticles[i].animation.OneTimeAnimation)
		{
			dustParticles[i].setPosition = false;
			dustParticles[i].isEffect = false;
			if (dustParticles[i].row == 0)
				player.dashDust = false;
			else
				player.jumpDust = false;
		}
	}
	for (unsigned int i = 0; i < dustParticles2.size(); i++)
	{
		if (dustParticles2[i].isEffect and !dustParticles2[i].animation.OneTimeAnimation)
		{
			dustParticles2[i].setPosition = false;
			dustParticles2[i].isEffect = false;
			player.fallDust = false;
		}
	}
}

void drawParticles(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, Texture& hitEffect, Texture& dustEffect, Player& player, Vector2f position, float time, RenderWindow& window)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
	{
		hitParticles[i].Update(&hitEffect, player, position, time, 1);
		hitParticles[i].Draw(window);
	}
	for (unsigned int i = 0; i < dustParticles.size(); i++)
	{
		dustParticles[i].Update(&dustEffect, player, position, time, 2);
		dustParticles[i].Draw(window);
	}
	for (unsigned int i = 0; i < dustParticles2.size(); i++)
	{
		dustParticles2[i].Update(&dustEffect, player, position, time, 2);
		dustParticles2[i].Draw(window);
	}
}