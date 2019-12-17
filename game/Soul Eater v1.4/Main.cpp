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
void keyEvent (RenderWindow&);
void drawTutorial(RenderWindow&, Particle&, Particle&, Texture&, Texture&, Player&, Ground&, Ground&, Ground&, float);
void cameraSetting(RenderWindow&, View&, Player&, RectangleShape&, RectangleShape&, RectangleShape&, Vector2f, float);
void playerGetHit (Player&);
void playerStatus (Player&, Clock&);
void Dashing      (Player&);
void createParticles (vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void endParticles    (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void drawParticles   (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Texture&, Texture&, Texture&, Player&, RenderWindow&, float);
void drawTiny		(vector<Enemy>&, Texture&, Player&, RenderWindow&, float);
void drawGiant		(vector<Enemy>&, Texture&, Player&, RenderWindow&, float);
void drawGhost		(vector<Enemy>&, Texture&, Player&, RenderWindow&, float);
void drawShaman		(vector<Enemy>&, Texture&, Player&, RenderWindow&, float);
void drawDarkSphere (vector<Enemy>&, Texture&, Player&, RenderWindow&, float);
void drawSoulEater  (vector<Enemy>&, Texture&, Texture&, Player&, RenderWindow&, float);
void Tiny		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Giant		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Ghost		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Shaman		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void DarkSphere (vector<Enemy>&, Player&, Ground&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void SoulEater	(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void drawGround (RenderWindow&, Ground&, Ground&, Ground&);
void checkPlayerOnGround(Player&, Ground&, Ground&, Ground&, Vector2f&);

// Global variables =============
unsigned int screen = 0;          
unsigned int SelectedMenu = 0;    // 0 = START GAME, 1 = CONTINUE, 2 = EXIT
bool pressed;

unsigned int charactor = 0;

bool showMove = false, showJump = false, showDash = false, showAttack = false, showUI = false;
bool showCropScreen = false, changeStage = false;
unsigned int counter;


// Main =========================
int main()
{
	//create Window and Camera
	Image icon;
	icon.loadFromFile("Res/icon.png");
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Soul Eater v1.4", Style::Default);
	window.setIcon(32, 32, icon.getPixelsPtr());
	window.setFramerateLimit(60);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(screenWidth + 100.0f, screenHeight + 100.0f));

	//create Interface
	Texture soulTexture, enemyCountTexture, healthBarTexture, energy;
	soulTexture.loadFromFile("Res/gui/blue.png");
	enemyCountTexture.loadFromFile("Res/gui/enemyCount.png");
	energy.loadFromFile("Res/gui/EnergyBar.png");
	Interface UI(&soulTexture, &enemyCountTexture,Vector2u(11, 1), 0.08f);
	UI.energyBar.setTexture(&energy);
	UI.EnemyIMG.setPosition(0.f, 2000.f);
	UI.healthBar.setPosition(0.f, 2000.f);
	UI.energyBar.setPosition(0.f, 2000.f);
	UI.font.loadFromFile("Res/Font/CreditValley.ttf");
	UI.enemyCount.setFont(UI.font);
	UI.enemyCount.setStyle(Text::Bold | Text::Italic);
	UI.enemyCount.setFillColor(Color::White);
	UI.enemyCount.setCharacterSize(36);

	//create Player
	Texture playerTexture, playerCombo;
	Player player(&playerTexture, Vector2u(0, 0), 0.1f, charactor);

	//create Effect
	Texture dustEffect, hitEffect, atkEffect;
	hitEffect.loadFromFile("Res/particles/hit.png");
	dustEffect.loadFromFile("Res/particles/dust.png");
	vector<Particle> attackParticles;
	for (int i = 0; i < 4; i++)
	{
		attackParticles.push_back(Particle(&atkEffect, Vector2u(0, 0), 0.1f, 3));
		Particle& attackParticle = attackParticles[i];
	}
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
	for (int i = 0; i < 15; i++)
	{
		tinies.push_back(Enemy(&tinyTexture, Vector2u(11, 5), 0.f, 2000.0f, 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 1; i++)
	{
		giants.push_back(Enemy(&giantTexture, Vector2u(13, 5), 0.f, 2000.0f, 0.12f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 1; i++)
	{
		ghosts.push_back(Enemy(&ghostTexture, Vector2u(8, 2), 0.f, 2000.0f, 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	vector<Enemy> shamans;
	for (int i = 0; i < 1; i++)
	{
		shamans.push_back(Enemy(&shamanTexture, Vector2u(8, 4), 0.f, 2000.0f, 0.12f, 3));
		Enemy& shaman = shamans[i];
	} 
	vector<Enemy> darkSpheres;
	for (int i = 0; i < 3; i++)
	{
		darkSpheres.push_back(Enemy(&darkSphereTexture, Vector2u(5, 1), 0.f, 2000.0f, 0.08f, 4));
		Enemy& darkSphere = darkSpheres[i];
	}
	vector<Enemy> souleaters;
	for (int i = 0; i < 0; i++)
	{
		souleaters.push_back(Enemy(&souleaterTexture, Vector2u(12, 6), 0.f, 2000.0f, 0.08f, 5));
		Enemy& souleater = souleaters[i];
	}

	////////////////
	Vector2f position, direction;
	Clock clock, wait, invincibleTime;


	// create object in Title screen
	Texture Title01, Title11, LogoTexture, pressSpace, menuTexture, selectedTexture, blueSel, pinkSel, yellowSel;;
	RectangleShape title, whiteScreen, logo, button0, menu1, selectedMenu, charSel;
	Title01.loadFromFile        ("Res/title/Title.png");
	Title11.loadFromFile        ("Res/title/TitleBlur.png");
	pressSpace.loadFromFile     ("Res/title/pressSpace.png");
	LogoTexture.loadFromFile    ("Res/title/logo.png");
	menuTexture.loadFromFile    ("Res/title/menu.png");
	selectedTexture.loadFromFile("Res/title/selected menu.png");
	blueSel.loadFromFile        ("Res/title/blueSelected.png");
	pinkSel.loadFromFile        ("Res/title/pinkSelected.png");
	yellowSel.loadFromFile      ("Res/title/yellowSelected.png");
	title.setSize       (Vector2f(screenWidth, screenHeight));
	whiteScreen.setSize (Vector2f(screenWidth + 100.f, screenHeight + 100.f));
	logo.setSize        (Vector2f(screenWidth, screenHeight));
	menu1.setSize       (Vector2f(screenWidth, screenHeight));
	button0.setSize     (Vector2f(116.5f, 21.f));
	selectedMenu.setSize(Vector2f(100.f, 16.5f));
	charSel.setSize     (Vector2f(500.f, 512.f));
	button0.setPosition      ( 350.f,  450.f);
	selectedMenu.setPosition (-100.f, -100.f);
	charSel.setPosition      ( 150.f,    0.f);
	title.setTexture       (&Title01);
	button0.setTexture     (&pressSpace);
	logo.setTexture        (&LogoTexture);
	menu1.setTexture       (&menuTexture);
	selectedMenu.setTexture(&selectedTexture);
	title.setFillColor(Color(0, 0, 0));
	logo.setFillColor (Color(0, 0, 0, 0));

	//variables
	bool  blink = false, blink2 = false;
	float time    = 0.0f;
	int   opacity1 = 0, opacity2 = 0, opacity3 = 0;

	// Title Screen ===================================================================
	while (window.isOpen())
	{
		keyEvent(window);

		// Press Space Screen ---------
		if (screen == 0)
		{
			if (opacity1 != 255)						      opacity1++;
			else if (opacity1 == 255 and opacity2 != 255)     opacity2 += 3;
			if (opacity1 == 255 and !blink and opacity3 > 0)  opacity3 -= 3;
			else                     				          blink = true;
			if (opacity1 == 255 and blink and opacity3 < 255) opacity3 += 3;
			else 										      blink = false;

			title.setFillColor   (Color(opacity1, opacity1, opacity1));
			logo.setFillColor    (Color(opacity2, opacity2, opacity2, opacity2));
			button0.setFillColor (Color(255, 255, 255, opacity3));

			if (opacity1 == 255 and Keyboard::isKeyPressed(Keyboard::Space))
			{
				title.setFillColor (Color(255, 255, 255, 255));
				logo.setFillColor(Color(255, 255, 255, 255));
				opacity1  = 255;
				opacity2 = 255;
				screen  = 1;
				pressed = true;
			}
		}
		
		// Menu Screen -----------------
		else if (screen == 1)
		{
			if      (SelectedMenu == 0) selectedMenu.setPosition(350.f, 392.f);
			else if (SelectedMenu == 1)	selectedMenu.setPosition(350.f, 415.f);
			else if (SelectedMenu == 2)	selectedMenu.setPosition(350.f, 438.f);

			if (Keyboard::isKeyPressed(Keyboard::Up) and SelectedMenu != 0 and !pressed)
				{ SelectedMenu--; pressed = true; }
			else if (Keyboard::isKeyPressed(Keyboard::Down) and SelectedMenu != 2 and !pressed)
				{ SelectedMenu++; pressed = true; }
			else if (Keyboard::isKeyPressed(Keyboard::Space) and SelectedMenu == 0 and !pressed)
				{ screen = 2; title.setTexture(&Title11); }
			else if (Keyboard::isKeyPressed(Keyboard::Space) and SelectedMenu == 2 and !pressed)
				{ screen = 99; window.close(); }
		}
		else if (screen == 2)
		{
			if      (opacity1 != 0)	                  opacity1 -= 3;
			else if (opacity1 == 0 and opacity2 != 0) opacity2 -= 3;
			else if (opacity1 == 0 and opacity2 == 0) screen    = 3;

			whiteScreen.setFillColor (Color(255, 255, 255, opacity1));
			title.setFillColor       (Color(opacity2, opacity2, opacity2, opacity2));
		}

		// Charactor Selection ------
		else if (screen == 3)
		{
			if      (charactor == 0) charSel.setTexture(&blueSel);
			else if (charactor == 1) charSel.setTexture(&pinkSel);
			else if (charactor == 2) charSel.setTexture(&yellowSel);
				

			if (Keyboard::isKeyPressed(Keyboard::Up) and charactor != 0 and !pressed)
				{ charactor--; pressed = true; }
			else if (Keyboard::isKeyPressed(Keyboard::Down) and charactor != 2 and !pressed)
				{ charactor++; pressed = true; }

			else if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
			{
				player.charactor = charactor;
				opacity1 = 255;
				opacity2 = 255;
				screen   = 4;
				if (charactor == 0)
				{
					soulTexture.loadFromFile("Res/gui/blue.png");
					healthBarTexture.loadFromFile("Res/gui/BlueHealthBar.png");
					playerTexture.loadFromFile("Res/player/blue.png");
					playerCombo.loadFromFile("Res/player/blueATK.png");
					atkEffect.loadFromFile("Res/particles/blueEffect.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(8, 3);
						attackParticles[i].effect.setSize(Vector2f(161.f, 127.f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
					player.animation.imageCount = Vector2u(10, 7);
				}
				else if (charactor == 1)
				{
					soulTexture.loadFromFile("Res/gui/pink.png");
					healthBarTexture.loadFromFile("Res/gui/PinkHealthBar.png");
					playerTexture.loadFromFile("Res/player/blue.png");
					playerCombo.loadFromFile("Res/player/blueATK.png");
					atkEffect.loadFromFile("Res/particles/blueEffect.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(8, 3);
						attackParticles[i].effect.setSize(Vector2f(161.f, 127.f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
					player.animation.imageCount = Vector2u(10, 7);
				}
				else if (charactor == 2)
				{
					soulTexture.loadFromFile("Res/gui/yellow.png");
					healthBarTexture.loadFromFile("Res/gui/YellowHealthBar.png");
					playerTexture.loadFromFile("Res/player/yellow.png");
					playerCombo.loadFromFile("Res/player/yellowATK.png");
					atkEffect.loadFromFile("Res/particles/yellowEffect.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(14, 3);
						attackParticles[i].animation.switchTime = 0.06f;
						attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
						
					player.animation.imageCount = Vector2u(12, 7);
				}
				UI.healthBar.setTexture(&healthBarTexture);	
			}
		}
		// Show Soul -----------------
		else if (screen == 4)
		{
			if (opacity1 != 0)
			{
				opacity1 -= 3;
				whiteScreen.setFillColor(Color(255, 255, 255, opacity1));
				wait.restart().asSeconds();
			}
			else if (wait.getElapsedTime().asSeconds() > 2)
			{
				UI.soul.setPosition(-380.0f, -210.0f);
				screen = 5;
				break;
			}
		}


		// Draw Screen -------------------
		if (screen == 0)
		{
			window.clear();
			window.draw(title);
			window.draw(logo);
			window.draw(button0);
			window.display();
		}
		else if (screen == 1)
		{
			window.clear();
			window.draw(title);
			window.draw(logo);
			window.draw(selectedMenu);
			window.draw(menu1);
			window.display();
		}
		else if (screen == 2)
		{
			window.clear();
			window.draw(title);
			window.draw(whiteScreen);
			window.display();
		}
		else if (screen == 3)
		{
			window.clear();
			window.draw(charSel);
			window.display();
		}
		else if (screen == 4)
		{
			time = clock.restart().asSeconds();
			if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;

			window.clear();
			UI.Update(&soulTexture, player, camera, player.HP, player.Energy, time, true);
			UI.Draw(window);
			window.draw(whiteScreen);
			window.display();
		}
	}

	//create Background
	Texture backgroundTexture, midgroundTexture, foregroundTexture;
	RectangleShape background, midground, foreground;
	backgroundTexture.loadFromFile("Res/bg/village1.png");
	midgroundTexture.loadFromFile ("Res/bg/village2.png");
	foregroundTexture.loadFromFile("Res/bg/village3.png");
	background.setTexture(&backgroundTexture);
	midground.setTexture (&midgroundTexture);
	foreground.setTexture(&foregroundTexture);

	// create Environments
	Ground ground   (Vector2f(5000.f, 20.f),  Vector2f(1550.f, 1050.f));
	Ground wallLeft (Vector2f(20.0f, 240.0f), Vector2f(5.f, 400.f));
	Ground wallRight(Vector2f(20.0f, 240.0f), Vector2f(3400.f, 900.f));
	Ground barrel   (Vector2f(100.f, 40.f),   Vector2f(1480.f, 1020.f));
	Ground barrel2  (Vector2f(55.f, 40.f),    Vector2f(1475.f, 970.f));
	Ground fire     (Vector2f(40.f, 200.f), Vector2f(2210.f, 900.f));
	background.setSize(Vector2f(1699.f * 2.f, 552.f * 2.f));
	midground.setSize (Vector2f(1699.f * 2.f, 552.f * 2.f));
	foreground.setSize(Vector2f(6048.f * 1.f, 552.f * 2.f));
	foreground.setPosition(-20.f, 40.f);
	Texture cutScene, textSplash;
	RectangleShape wideScreen, TextSplash;
	cutScene.loadFromFile("Res/gui/cropScreen.png");
	textSplash.loadFromFile("Res/bg/01TEXT.png");
	wideScreen.setSize(Vector2f(screenWidth + 120.f, screenHeight + 100.f));
	wideScreen.setOrigin(wideScreen.getSize() / 2.f);
	wideScreen.setTexture(&cutScene);
	TextSplash.setSize(Vector2f(screenWidth, screenHeight));
	TextSplash.setOrigin(wideScreen.getSize() / 2.f);
	TextSplash.setTexture(&textSplash);

	// create Tutorial
	Texture moveKey, actKey;
	moveKey.loadFromFile("Res/particles/button2.png");
	actKey.loadFromFile ("Res/particles/button.png");
	Particle movementKey(&moveKey, Vector2u(4, 1), 0.5, 0);
	Particle actionKey  (&actKey, Vector2u(2, 4), 0.5, -1);
	Ground moveZone(Vector2f(600.f, 200.f), Vector2f(700.f, 900.f));
	Ground jumpZone(Vector2f(300.f, 300.f), Vector2f(1480.f, 900.f));
	Ground dashZone(Vector2f(200.f, 200.f), Vector2f(2115.f, 900.f));
	Ground endZone (Vector2f(300.f, 300.f), Vector2f(3320.f, 900.f));

	// set object
	blink = 0;
	blink2 = 0;
	opacity1 = 0;
	opacity2 = 0;
	camera.setCenter(460.f, 800.f);
	player.hitBox.setPosition(-50.f, 1000.f);
	tinies[0].state = 98;
	tinies[0].SetPosition(2700.f, 900.f);
	counter = 1;

	int size = screenHeight + 100.f;
	
	// Tutorial =========================================================================
	while (window.isOpen())
	{
		//break;
		keyEvent(window);
		position = player.GetPosition();
		cameraSetting(window, camera, player, background, background, foreground, position, time);
		wideScreen.setPosition(camera.getCenter());
		TextSplash.setPosition(camera.getCenter());
		
		UI.enemyCount.setString(to_string(counter));

		//Text splash
		if (opacity1 < 255 and !blink)
		{
			opacity1 += 1;
			TextSplash.setFillColor(Color(255, 255, 255, opacity1));
		}
		else blink = true;
		if (opacity1 > 0 and blink)
		{
			opacity1 -= 1;
			TextSplash.setFillColor(Color(255, 255, 255, opacity1));
		}

		// auto Move Player and Tiny
		if (player.cutScene and position.x < 600.f)
			player.velocity.x += 300.f;
		else if (player.cutScene and position.x >= 600.f and position.x < 2200.f)
		{
			wideScreen.setSize(Vector2f(screenWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < screenHeight + 300.f)
				size += 5;
			else
			{
				wallLeft.SetPosition(5.f, 900.f);
				player.cutScene = false;
			}
		}
		else if (!player.cutScene and position.x >= 2200.f and tinies[0].state == 98)
		{
			player.cutScene = true;
			tinies[0].state = 1;
		}
		else if (player.cutScene and tinies[0].state == 1)
		{
			wideScreen.setSize(Vector2f(screenWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size > screenHeight + 100.f)
				size -= 5;
			else
			{
				wideScreen.setSize(Vector2f(screenWidth + 120.f, screenHeight + 100.f));
				wideScreen.setOrigin(wideScreen.getSize() / 2.f);
				size = screenHeight + 100.f;
			}
			camera.move((tinies[0].GetPosition().x - camera.getCenter().x) * time * 2.5f, 0.f);
		}

		if (player.cutScene and player.getHit)
		{
			showAttack = true;
			showUI = true;
			blink2 = true;
		}
		if (blink2)
		{
			wideScreen.setSize(Vector2f(screenWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < screenHeight + 300.f)
				size += 5;
			else
			{
				wideScreen.setSize(Vector2f(screenWidth + 120.f, screenHeight + 100.f));
				wideScreen.setOrigin(wideScreen.getSize() / 2.f);
				size = screenHeight + 100.f;
				blink2 = false;
				player.cutScene = false;
			}
		}

		if (changeStage and opacity2 != 255)
		{
			player.Dead = true;
			whiteScreen.setPosition(camera.getCenter().x - 450.f, camera.getCenter().y - 306.f);
			whiteScreen.setFillColor(Color(0, 0, 0, opacity2));
			opacity2 += 1;
		}
		else if (changeStage)
		{
			changeStage = false;
			screen = 6;
			player.Dead = false;

			darkSpheres[0].hitBox.setRotation(-90.f);
			darkSpheres[0].sprite.setRotation(-90.f);
			darkSpheres[0].sprite.setSize(Vector2f(190.0f, 40.0f));
			darkSpheres[0].sprite.setFillColor(Color(255, 255, 255, 255));
			darkSpheres[0].state = 1;
			break;
		}


	// Show Arrow button
		if (moveZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !showAttack) showMove = true;
		else showMove = false;
	// Show Jump button
		if (jumpZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !showAttack)
		{
			showJump = true;
			actionKey.row = 3;
		}
		else showJump = false;
	// Show X button
		if (dashZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !showAttack)
		{
			showDash = true;
			actionKey.row = 1;
		}
		else showDash = false;
	// Show Z button
		if (showAttack) actionKey.row = 0;
	// End map
		if (endZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !changeStage and counter == 0)
			changeStage = true;


		// Check collide with object
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);
		if (ground.GetCollider().CheckCollide(tinies[0].GetHitBox(), direction, 1.f, true))
			tinies[0].OnCollision(direction);
		if (barrel.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.f, true))
			player.OnCollision(direction);
		if (barrel2.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.f, true))
			player.OnCollision(direction);
		if (fire.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.0f, false) and !player.Dash)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
		}
		darkSpheres[0].SetPosition(2220.f, 860.f);
		darkSpheres[0].hitBox.setRotation(90.f);
		darkSpheres[0].sprite.setRotation(90.f);
		darkSpheres[0].sprite.setSize(Vector2f(190.f*1.5, 40.f*1.5));
		darkSpheres[0].sprite.setFillColor(Color(255, 100, 0, 255));
		darkSpheres[0].state = 98;
			
		// Tiny Attack!!!
		Tiny(tinies, hitParticles, player, ground, position, invincibleTime, direction);
		player.HP = 100.f;
		
		// player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles, player);
		
		// Draw Every things
		time = clock.restart().asSeconds();
		if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;
		window.clear();
		window.draw(background);
		window.draw(midground);
		player.Update(&playerTexture, &playerCombo, position, player.Invincible, time, charactor);
		player.Draw(window);
		tinies[0].Update(&tinyTexture, &tinyTexture, time, tinies[0].GetPosition(), player, 0);
		tinies[0].Draw(window);
		drawParticles(hitParticles, dustParticles, dustParticles2, attackParticles, hitEffect, dustEffect, atkEffect,player, window, time);
		drawTutorial(window, movementKey, actionKey, moveKey, actKey, player, moveZone, jumpZone, dashZone, time);
		darkSpheres[0].Update(&darkSphereTexture, &darkSphereTexture, time, darkSpheres[0].GetPosition(), player, 4);
		darkSpheres[0].Draw(window);
		UI.Update(&soulTexture, player, camera, player.HP, player.Energy, time, false);
		window.draw(TextSplash);
		if (showUI)
			UI.Draw(window);
		window.draw(foreground);
		if (player.cutScene)
			window.draw(wideScreen);
		window.draw(whiteScreen);
		window.display();

	}
	

	// set Background
	Texture moonTexture;
	RectangleShape moon;
	moonTexture.loadFromFile("Res/bg/forest0.png");
	backgroundTexture.loadFromFile("Res/bg/forest1.png");
	midgroundTexture.loadFromFile("Res/bg/forest2.png");
	foregroundTexture.loadFromFile("Res/bg/forest3.png");
	moon.setTexture(&moonTexture);
	background.setTexture(&backgroundTexture);
	midground.setTexture(&midgroundTexture);
	foreground.setTexture(&foregroundTexture);
	moon.setSize(Vector2f(1139.f*2, 552.f * 2));
	background.setSize(Vector2f(3685.f*1.2, 552.f*2));
	midground.setSize(Vector2f(3618.f*1.2, 552.f*2));
	foreground.setSize(Vector2f(6762.f*1.2, 552.f*2));
	moon.setPosition(-1000.f, 120.f);

	//set object
	Texture homuraTexture;
	homuraTexture.loadFromFile("Res/player/black.png");
	//Enemy Homura(&homuraTexture, Vector2u(13, 7), 3010.f, 900.f, 0.1f, 6);
	ground.SetPosition(1700.f, 1050.f);
	wallRight.SetPosition(4200.f,900.f);
	textSplash.loadFromFile("Res/bg/02TEXT.png");
	blink = false;
	opacity1 = 0;
	opacity2 = 255;
	player.hitBox.setPosition(41.f, 1000.f);
	camera.setCenter(460.f, 800.f);
	player.cutScene = true;
	counter = 20;
	int spawn = 0;
	screen = 6;

	// Stage 1 ============================================================================
	while (window.isOpen())
	{
		keyEvent(window);
		cameraSetting(window, camera, player, moon, background, foreground, position, time);

		position = player.GetPosition();
		TextSplash.setPosition(camera.getCenter());
		wideScreen.setPosition(camera.getCenter());
		whiteScreen.setPosition(camera.getCenter().x - 450.f, camera.getCenter().y - 306.f);

		UI.enemyCount.setString(to_string(counter));

		//Text splash
		if (opacity1 < 255 and !blink)
		{
			opacity1 += 1;
			TextSplash.setFillColor(Color(255, 255, 255, opacity1));
		}
		else blink = true;
		if (opacity1 > 0 and blink)
		{
			opacity1 -= 1;
			TextSplash.setFillColor(Color(255, 255, 255, opacity1));
		}
		// Fade Screen
		if (opacity2 > 0) 
		{
			opacity2--;
			whiteScreen.setFillColor(Color(0, 0, 0, opacity2));
		}

		// auto Move Player
		if (player.cutScene and position.x < 600.f)
			player.velocity.x += 300.f;
		else if (player.cutScene and position.x >= 600.f and position.x < 2200.f)
		{
			wideScreen.setSize(Vector2f(screenWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < screenHeight + 300.f)
				size += 5;
			else
			{
				wallLeft.SetPosition(5.f, 900.f);
				showUI = true;
				player.cutScene = false;
			}
		}

		// Spawn ------
		if (position.x > 1500.f and spawn == 0)
		{
			spawn = 1;
			tinies[0].SetPosition(400.f, 800.f);
			tinies[0].state = 3;
			tinies[1].SetPosition(800.f, 800.f);
			tinies[1].state = 3;
			tinies[2].SetPosition(2200.f, 800.f);
			tinies[2].state = 1;
			tinies[3].SetPosition(2400.f, 800.f);
			tinies[3].state = 1;
			tinies[4].SetPosition(100.f, 800.f);
			tinies[4].state = 1;
			tinies[5].SetPosition(2800.f, 800.f);
			tinies[5].state = 1;
			tinies[6].SetPosition(3000.f, 800.f);
			tinies[6].state = 3;
			tinies[7].SetPosition(600.f, 800.f);
			tinies[7].state = 1;
		}
		else if (position.x > 2000.f and spawn == 1)
		{
			tinies[0].SetPosition(400.f, 800.f);
			tinies[0].state = 3;
			tinies[1].SetPosition(800.f, 800.f);
			tinies[1].state = 1;
			tinies[2].SetPosition(1600.f, 800.f);
			tinies[2].state = 1;
			tinies[3].SetPosition(1000.f, 800.f);
			tinies[3].state = 1;
			tinies[4].SetPosition(100.f, 800.f);
			tinies[4].state = 3;
			tinies[5].SetPosition(500.f, 800.f);
			tinies[5].state = 1;
			tinies[6].SetPosition(3000.f, 800.f);
			tinies[6].state = 3;
			tinies[7].SetPosition(1200.f, 800.f);
			tinies[7].state = 1;
			spawn = 2;
		}
		else if (position.x > 2000.f and spawn == 1)
		{
			tinies[0].SetPosition(400.f, 800.f);
			tinies[0].state = 3;
			tinies[1].SetPosition(800.f, 800.f);
			tinies[1].state = 1;
			tinies[2].SetPosition(1600.f, 800.f);
			tinies[2].state = 1;
			tinies[3].SetPosition(1000.f, 800.f);
			tinies[3].state = 1;
			tinies[4].SetPosition(100.f, 800.f);
			tinies[4].state = 3;
			tinies[5].SetPosition(500.f, 800.f);
			tinies[5].state = 1;
			tinies[6].SetPosition(3000.f, 800.f);
			tinies[6].state = 3;
			tinies[7].SetPosition(1200.f, 800.f);
			tinies[7].state = 1;
			spawn = 2;
		}



		
		Tiny(tinies, hitParticles, player, ground, position, invincibleTime, direction);
		Giant(giants, hitParticles, player, ground, position, invincibleTime, direction);

		// Check collide with object
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);

		// player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles, player);

		// Draw Every things
		time = clock.restart().asSeconds();
		if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;
		window.clear();
		window.draw(moon);
		window.draw(background);
		window.draw(midground);
		drawGiant(giants, giantTexture, player, window, time);
		player.Update(&playerTexture, &playerCombo, position, player.Invincible, time, charactor);
		player.Draw(window);
		drawTiny(tinies, tinyTexture, player, window, time);
		drawParticles(hitParticles, dustParticles, dustParticles2, attackParticles, hitEffect, dustEffect, atkEffect, player, window, time);
		window.draw(foreground);
		UI.Update(&soulTexture, player, camera, player.HP, player.Energy, time, false);
		if (showUI and ! player.cutScene)
			UI.Draw(window);
		if (player.cutScene)
			window.draw(wideScreen);
		window.draw(whiteScreen);
		window.draw(TextSplash);
		window.display();

	}
	
	// Main game ========================================================================
	while (window.isOpen() and screen != 99)
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
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles,player);

		// Setting
		cameraSetting(window, camera, player, background, background, foreground, position, time);
		if (Keyboard::isKeyPressed(Keyboard::RShift)) player.HP += 5.1f;
		if (Mouse::isButtonPressed(Mouse::Middle))
		{
			window.close();
			screen = 0;
			main();
		}
		

		// check hitbox
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);

		// Enemy state
		Tiny		(tinies,      hitParticles, player, ground, position, invincibleTime, direction);
		Giant		(giants,      hitParticles, player, ground, position, invincibleTime, direction);
		Ghost		(ghosts,      hitParticles, player, ground, position, invincibleTime, direction);
		Shaman		(shamans,  darkSpheres, hitParticles, player, ground, position, invincibleTime, direction);
		DarkSphere	(darkSpheres, player, ground, wallLeft, wallRight, position, invincibleTime, direction);
		SoulEater	(souleaters,  player, ground, position, invincibleTime, direction);
		
	

		window.clear(Color(150, 150, 150));
		// Draw here
		window.draw(background);
		window.draw(midground);
		drawGround(window, ground, wallLeft, wallRight);

		drawGiant		(giants,	  giantTexture,	     player, window, time);
		player.Update   (&playerTexture, &playerCombo,   position, player.Invincible, time, charactor);
		player.Draw     (window);
		drawTiny		(tinies,      tinyTexture,       player, window, time);
		drawGhost		(ghosts,      ghostTexture,      player, window, time);
		drawShaman		(shamans,	  shamanTexture,     player, window, time);
		drawDarkSphere	(darkSpheres, darkSphereTexture, player, window, time);
		drawSoulEater	(souleaters,  souleaterTexture, souleaterCast,  player, window, time);
		drawParticles   (hitParticles, dustParticles, dustParticles2, attackParticles, hitEffect, dustEffect, atkEffect, player, window, time);

		window.draw(foreground);
		UI.Update(&soulTexture, player, camera, player.HP, player.Energy, time, false);
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
void keyEvent(RenderWindow& window)
{
	Event button;
	while (window.pollEvent(button))
		if (button.type == Event::Closed)
			window.close();
	if (button.type == Event::KeyReleased)
		pressed = false;
}

void drawTutorial(RenderWindow& window, Particle& movementKey, Particle& actionKey, Texture& moveKey, Texture& actKey, Player& player, Ground& moveZone, Ground& jumpZone, Ground& dashZone, float time)
{
	movementKey.Update(&moveKey, player, time, 0);
	if (showMove and !showAttack)
		movementKey.Draw(window);
	actionKey.Update(&actKey, player, time, -1);
	if (showJump or showDash or showAttack)
		actionKey.Draw(window);

	//moveZone.Draw(window);
	//jumpZone.Draw(window);
	//dashZone.Draw(window);
}

void cameraSetting(RenderWindow& window, View& camera, Player& player, RectangleShape& preBack, RectangleShape& back, RectangleShape& front, Vector2f playerPosition, float time)
{
	Vector2f cameraMovement = player.GetPosition() - camera.getCenter();
	cameraMovement += Vector2f(0.0f, -90.0f);

	window.setView(camera);
	if (screen == 5) // cutscene
	{
		if (playerPosition.x > 460.f and playerPosition.x < 2720.f)
		{
			camera.move (cameraMovement.x * time *  2.5f , 0.f);
			back  .move (cameraMovement.x * time * -0.15f, 0.f);
			front .move (cameraMovement.x * time * -1.5f , 0.f);
		}
	}
	else if (screen == 6) // stage 1
	{
		if (playerPosition.x > 460.f and playerPosition.x < 3620.f)
		{
			camera.move(cameraMovement.x * time *  2.5f, 0.f);
			preBack.move(cameraMovement.x* time * 2.5, 0.f);
			back. move(cameraMovement.x * time * -0.5f, 0.f);
			front.move(cameraMovement.x * time * -1.5f, 0.f);
		}
	}
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

	if (player.Dash)            player.Energy -= 4.5f;
}

void drawTiny(vector<Enemy>& tinies, Texture& tinyTexture, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < tinies.size(); i++)
	{
		tinies[i].Update(&tinyTexture, &tinyTexture, time, tinies[i].GetPosition(), player, 0);
		tinies[i].Draw(window);
	}
}

void drawGiant(vector<Enemy>& giants, Texture& giantTexture, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < giants.size(); i++)
	{
		giants[i].Update(&giantTexture, &giantTexture, time, giants[i].GetPosition(), player, 1);
		giants[i].Draw(window);
	}
}

void drawGhost(vector<Enemy>& ghosts, Texture& ghostTexture, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		ghosts[i].Update(&ghostTexture, &ghostTexture, time, ghosts[i].GetPosition(), player, 2);
		ghosts[i].Draw(window);
	}
}

void drawShaman(vector<Enemy>& shamans, Texture& shamanTexture, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < shamans.size(); i++)
	{
		shamans[i].Update(&shamanTexture, &shamanTexture, time, shamans[i].GetPosition(), player, 3);
		shamans[i].Draw(window);
	}
}

void drawDarkSphere(vector<Enemy>& darkSpheres, Texture& darkSphereTexture, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		darkSpheres[i].Update(&darkSphereTexture, &darkSphereTexture, time, darkSpheres[i].GetPosition(), player, 4);
		darkSpheres[i].Draw(window);
	}
}

void drawSoulEater(vector<Enemy>& souleaters, Texture& souleaterTexture, Texture& souleaterCast, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < souleaters.size(); i++)
	{
		souleaters[i].Update(&souleaterTexture, &souleaterCast, time, souleaters[i].GetPosition(), player, 5);
		souleaters[i].Draw(window);
	}
}

void drawGround(RenderWindow& window, Ground& ground, Ground& wallLeft, Ground& wallRight)
{
	ground   .Draw(window);
	wallLeft .Draw(window);
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
		if (tinies[i].HP <= 0.f and !tinies[i].Dead)
			tinies[i].state = 100;
		else if (tinies[i].Dead and tinies[i].state != 101)
		{ 
			tinies[i].state = 101;
			counter--;
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(tinies[i].GetAttackBox(), direction, 0.0f, false) and tinies[i].state == 4 and !player.Invincible and !player.Dash and !player.Dead)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
		}
		if (tinies[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and tinies[i].state != 7 and tinies[i].state != 100 and tinies[i].state != 101)
		{
			tinies[i].HP -= 10;
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
		if (ground.GetCollider().CheckCollide(tinies[i].GetHitBox(), direction, 1.0f, true))
			tinies[i].OnCollision(direction);
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
		if (giants[i].HP <= 0.f and !giants[i].Dead)
			giants[i].state = 100;
		else if (giants[i].Dead and giants[i].state != 101)
		{
			giants[i].state = 101;
			counter--;
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(giants[i].GetAttackBox(), direction, 0.0f, false) and giants[i].state == 4 and !player.Invincible and !player.Dash and !player.Dead)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
		}
		if (giants[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and giants[i].state != 4 and giants[i].state != 7 and giants[i].state != 100 and giants[i].state != 101)
		{
			giants[i].HP -= 10.f;
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
		if (ground.GetCollider().CheckCollide(giants[i].GetHitBox(), direction, 1.0f, true))
			giants[i].OnCollision(direction);

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
		if (player.GetHitBox().CheckCollide(ghosts[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.Dead)
		{
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
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
		if (ground.GetCollider().CheckCollide(ghosts[i].GetHitBox(), direction, 1.0f, true))
			ghosts[i].OnCollision(direction);

	}
}

void Shaman(vector<Enemy>& shamans, vector<Enemy>& darkSpheres, vector<Particle>& hitParticles, Player& player, Ground& ground, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
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
							darkSpheres[j].SetPosition(shamans[i].GetPosition().x + 40.0f, shamans[i].GetPosition().y);
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
							darkSpheres[j].SetPosition(shamans[i].GetPosition().x - 40.0f, shamans[i].GetPosition().y);
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
		if (ground.GetCollider().CheckCollide(shamans[i].GetHitBox(), direction, 1.0f, true))
			shamans[i].OnCollision(direction);
	}
}

void DarkSphere(vector<Enemy>& darkSpheres, Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		if (player.GetHitBox().CheckCollide(darkSpheres[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.Dead)
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
		if (wallLeft.GetCollider().CheckCollide(darkSpheres[i].GetHitBox(), direction, 1.0f, true))
			darkSpheres[i].state = 1;
		if (wallRight.GetCollider().CheckCollide(darkSpheres[i].GetHitBox(), direction, 1.0f, true))
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
		if (player.GetHitBox().CheckCollide(souleaters[i].GetAttackBox(), direction, 0.0f, false) and souleaters[i].state == 4 and !player.Invincible and !player.Dash and !player.Dead)
		{
			player.getHit = true;
			invincibleTime.restart().asSeconds();
		}
		if (ground.GetCollider().CheckCollide(souleaters[i].GetHitBox(), direction, 1.0f, true))
			souleaters[i].OnCollision(direction);
	}
}

void createParticles(vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles,Player& player)
{
	if (player.Dash and !player.dashDust)
	{
		for (unsigned int i = 0; i < dustParticles.size(); i++)
		{
			if (!dustParticles[i].isEffect)
			{
				player.dashDust = true;
				dustParticles[i].isEffect = true;
				dustParticles[i].animation.playingAnimation = true;
				dustParticles[i].animation.OneTimeAnimation = true;
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
				dustParticles[i].animation.playingAnimation = true;
				dustParticles[i].animation.OneTimeAnimation = true;
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
				dustParticles2[i].animation.playingAnimation = true;
				dustParticles2[i].animation.OneTimeAnimation = true;
				dustParticles2[i].animation.OneTimeFrame.x = 14;
				dustParticles2[i].row = 1;
				dustParticles2[i].animation.currentImage.x = 0;
				break;
			}
		}
	}
	if (player.isAttack)
	{

		for (unsigned int i = 0; i < attackParticles.size(); i++)
		{
			if (!attackParticles[i].isEffect and player.combo == i+1)
			{
				player.attackBoxON = true;
				attackParticles[i].isEffect = true;
				attackParticles[i].animation.playingAnimation = true;
				attackParticles[i].animation.OneTimeAnimation = true;
				if (charactor == 0 or charactor == 1)
				{
					if (player.combo != 3)
						attackParticles[i].animation.OneTimeFrame.x = 4;
					else
						attackParticles[i].animation.OneTimeFrame.x = 8;
					if (player.combo == 1 or player.combo == 4)
						attackParticles[i].row = 0;
					else if (player.combo == 2)
						attackParticles[i].row = 1;
					else if (player.combo == 3)
						attackParticles[i].row = 2;
				}
				else if (charactor == 2)
				{
					if (player.combo != 3)
						attackParticles[i].animation.OneTimeFrame.x = 14;
					else
						attackParticles[i].animation.OneTimeFrame.x = 14;
					if (player.combo == 1 or player.combo == 4)
						attackParticles[i].row = 0;
					else if (player.combo == 2)
						attackParticles[i].row = 1;
					else if (player.combo == 3)
						attackParticles[i].row = 2;
				}
				attackParticles[i].animation.currentImage.x = 0;
				break;
			}
		}
	}
}

void endParticles(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles, Player& player)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
	{
		if (hitParticles[i].isEffect and !hitParticles[i].animation.OneTimeAnimation)
		{
			hitParticles[i].isEffect    = false;
			hitParticles[i].setPosition = false;
		}
	}
	for (unsigned int i = 0; i < dustParticles.size(); i++)
	{
		if (dustParticles[i].isEffect and !dustParticles[i].animation.OneTimeAnimation)
		{
			dustParticles[i].setPosition = false;
			dustParticles[i].isEffect    = false;

			if (dustParticles[i].row == 0) player.dashDust = false;
			else                           player.jumpDust = false;
		}
	}
	for (unsigned int i = 0; i < dustParticles2.size(); i++)
	{
		if (dustParticles2[i].isEffect and !dustParticles2[i].animation.OneTimeAnimation)
		{
			dustParticles2[i].setPosition = false;
			dustParticles2[i].isEffect    = false;
			player.fallDust               = false;
		}
	}
	for (unsigned int i = 0; i <attackParticles.size(); i++)
	{
		if (attackParticles[i].isEffect and !attackParticles[i].animation.OneTimeAnimation and !player.animation.playingAnimation)
		{
			player.attackBoxON = false;
			attackParticles[i].isEffect = false;
			attackParticles[i].setPosition = false;
		}
		else if (attackParticles[i].isEffect and !attackParticles[i].animation.OneTimeAnimation and player.animation.playingAnimation)
			attackParticles[i].effect.setPosition(0.f, 2000.f);
	}
}

void drawParticles(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles, Texture& hitEffect, Texture& dustEffect, Texture& atkEffect, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
	{
		hitParticles[i].Update(&hitEffect, player, time, 1);
		hitParticles[i].Draw(window);
	}
	for (unsigned int i = 0; i < dustParticles.size(); i++)
	{
		dustParticles[i].Update(&dustEffect, player, time, 2);
		dustParticles[i].Draw(window);
	}
	for (unsigned int i = 0; i < dustParticles2.size(); i++)
	{
		dustParticles2[i].Update(&dustEffect, player, time, 2);
		dustParticles2[i].Draw(window);
	}
	for (unsigned int i = 0; i < attackParticles.size(); i++)
	{
		if (i == 2)
			attackParticles[i].Update(&atkEffect, player, time, 4);
		else
			attackParticles[i].Update(&atkEffect, player, time, 3);
		attackParticles[i].Draw(window);
	}
}