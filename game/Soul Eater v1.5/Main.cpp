#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Interface.h"
#include "Player.h"
#include "Enemy.h"
#include "Ground.h"
#include "Particle.h"

#define windowWidth  800
#define windowHeight 512
#define cameraWidth  900
#define cameraHeight 612
using namespace sf;
using namespace std;


// Prototypes ===================
void animate (Texture*, Vector2u, short, float, float);
void loadFile();
void EventAndTime (RenderWindow&, Clock&, float&); 
void cameraSetting (RenderWindow&, View&, Player&, RectangleShape&, RectangleShape&, RectangleShape&, Vector2f,  float);
void drawTutorial  (RenderWindow&, Particle&, Particle&, Texture&, Texture&, Player&, Ground&, Ground&, Ground&, float);
void playerGetHit (Player&);
void playerStatus (Player&, Clock&);
void Dashing      (Player&);
void createParticles (vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void endParticles    (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void drawParticles   (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Texture&, Texture&, Texture&, Player&, RenderWindow&, float);
void drawTiny		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawGiant		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawGhost		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawShaman		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawDarkSphere (vector<Enemy>&, Player&, RenderWindow&, float);
void drawSoulEater  (vector<Enemy>&, Player&, RenderWindow&, float);
void Tiny		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void Giant		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void Ghost		(vector<Enemy>&, vector<Particle>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void Shaman		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void DarkSphere (vector<Enemy>&, Player&, Ground&, Ground&, Ground&, Vector2f&, Clock&, Vector2f&);
void SoulEater	(vector<Enemy>&, Player&, Ground&, Vector2f&, Clock&, Vector2f&);
void drawGround (RenderWindow&, Ground&, Ground&, Ground&);
void checkPlayerOnGround(Player&, Ground&, Ground&, Ground&, Vector2f&);


// Prepare to load file =========================================================================
bool loaded = false;
Thread loading(&loadFile);
	// Theme song
	Music titleTheme, firstTheme, forestTheme, bossTheme;
	// Title soundFX
	SoundBuffer selectSoundFX, enterSoundFX, cancleSoundFX;
	// Title
	Texture Title01, Title11, LogoTexture, pressSpace, menuTex, selectedMenuTex, blueSel, pinkSel, yellowSel;
	// GUI
	Font font;
	Texture soulTex, counterTex, hpTex, spTex, wideScreenTex;
	// Particles
	Texture dustFX, hitFX, atkFX;
	// Enemy
	Texture tinyTex, giantTex, ghostTex, shamanTex, darkSphereTex;
	// Misc
	Texture moveKey, actKey, portalTex;


// Global variables ====================================================================================
	// animate function
	IntRect uvRect;
	Vector2u currentImage;
	float totalTime = 0.f;

	// Game
	bool showUI = false;
	unsigned short screen = 0;          
	unsigned short cutSceneState = 0;
	unsigned short tutorial = 0;
	
	unsigned short SelectedMenu  = 0;
	unsigned short charactor = 0;
	bool pressed;
	
	unsigned int counter;

SoundBuffer getHitSoundFX;
Sound getHitFX;



// Main ===================================================================================================================
int main()
{
	// create Window and Camera
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Soul Eater v1.5", Style::Default);
	Image icon; icon.loadFromFile("Res/misc/icon.png");
	window.setIcon(32, 32, icon.getPixelsPtr());
	window.setFramerateLimit(60);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(cameraWidth, cameraHeight));

	// Loading Screen object
	Texture loadingScr;     loadingScr.loadFromFile("Res/misc/loadingScreen.png");
	Sprite  loadingScreen;  loadingScreen.setTexture(loadingScr);
	Texture tinyLoadingScr; tinyLoadingScr.loadFromFile("Res/misc/tinyLoading2.png");
	
	RectangleShape tinyLoading; 
	tinyLoading.setTexture(&tinyLoadingScr); tinyLoading.setSize(Vector2f(75, 55)); tinyLoading.setPosition(360, 205);
	Clock clock;  float time = 0;
	loading.launch();
	while (window.isOpen())
	{
		EventAndTime(window, clock, time);

		if (loaded)	{ loaded = false; break; }

		window.clear();
		window.draw(loadingScreen);
		animate(&tinyLoadingScr, Vector2u(11, 1), 0, 0.06f, time);
		tinyLoading.setTextureRect(uvRect);
		window.draw(tinyLoading);
		window.display();
	}
	
	
	
	firstTheme.setVolume(60);
	forestTheme.setVolume(70);
	bossTheme.setVolume(100);
	titleTheme.setLoop(true);
	firstTheme.setLoop(true);
	forestTheme.setLoop(true);
	bossTheme.setLoop(true);

	titleTheme.setVolume(0);
	titleTheme.play();

	getHitSoundFX.loadFromFile("Res/sound/getHit.wav");
	getHitFX.setBuffer(getHitSoundFX);
	
	
	Sound selectFX, enterFX, cancleFX;
	selectFX.setBuffer(selectSoundFX);
	enterFX.setBuffer(enterSoundFX);
	cancleFX.setBuffer(cancleSoundFX);


	//create Interface
	
	

	Interface UI(&soulTex, &counterTex, font, Vector2u(11, 1), 0.08f);
	UI.energyBar.setTexture(&spTex);
	UI.EnemyIMG.setPosition(0.f, 2000.f);
	UI.healthBar.setPosition(0.f, 2000.f);
	UI.energyBar.setPosition(0.f, 2000.f);
	

	//create Player
	Texture playerTexture, playerCombo;
	Player player(&playerTexture, &playerCombo, Vector2u(0, 0), 0.1f, charactor);

	//create Effect
	
	
	vector<Particle> attackParticles;
	for (int i = 0; i < 4; i++)
	{
		attackParticles.push_back(Particle(&atkFX, Vector2u(0, 0), 0.1f, 3));
		Particle& attackParticle = attackParticles[i];
	}
	vector<Particle> hitParticles;
	for (int i = 0; i < 20; i++)
	{
		hitParticles.push_back(Particle(&hitFX, Vector2u(5, 1), 0.07f, 1));
		Particle& hitParticle = hitParticles[i];
	}
	vector<Particle> dustParticles;
	for (int i = 0; i < 3; i++)
	{
		dustParticles.push_back(Particle(&dustFX, Vector2u(14, 2), 0.05f, 2));
		Particle& dustParticle = dustParticles[i];
	}
	vector<Particle> dustParticles2;
	for (int i = 0; i < 3; i++)
	{
		dustParticles2.push_back(Particle(&dustFX, Vector2u(14, 2), 0.05f, 2));
		Particle& dustParticle2 = dustParticles2[i];
	}

	//create Enemy
	
	//Texture souleaterTexture, souleaterCast;
	
	//souleaterTexture.loadFromFile("Res/enemy/SoulEater.png");
	//souleaterCast.loadFromFile("Res/enemy/SoulEaterCast.png");
	vector<Enemy> tinies;
	for (int i = 0; i < 10; i++)
	{
		tinies.push_back(Enemy(&tinyTex, nullptr, Vector2u(11, 5), 0.f, 2000.0f, 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 10; i++)
	{
		giants.push_back(Enemy(&giantTex, nullptr, Vector2u(13, 5), 0.f, 2000.0f, 0.12f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 10; i++)
	{
		ghosts.push_back(Enemy(&ghostTex, nullptr, Vector2u(8, 2), 0.f, 2000.0f, 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	vector<Enemy> shamans;
	for (int i = 0; i < 10; i++)
	{
		shamans.push_back(Enemy(&shamanTex, nullptr, Vector2u(8, 4), 0.f, 2000.0f, 0.12f, 3));
		Enemy& shaman = shamans[i];
	} 
	vector<Enemy> darkSpheres;
	for (int i = 0; i < 10; i++)
	{
		darkSpheres.push_back(Enemy(&darkSphereTex, nullptr, Vector2u(5, 1), 0.f, 2000.0f, 0.08f, 4));
		Enemy& darkSphere = darkSpheres[i];
	}
	/*vector<Enemy> souleaters;
	for (int i = 0; i < 0; i++)
	{
		souleaters.push_back(Enemy(&souleaterTexture, Vector2u(12, 6), 0.f, 2000.0f, 0.08f, 5));
		Enemy& souleater = souleaters[i];
	}*/

	////////////////
	Vector2f position, direction;
	Clock wait, invincibleTime;


	// create object in Title screen
	
	RectangleShape title, whiteScreen, logo, button0, menu1, selectedMenu, charSel;
	
	title.setSize       (Vector2f(windowWidth, windowHeight));
	whiteScreen.setSize (Vector2f(cameraWidth, cameraHeight));
	logo.setSize        (Vector2f(windowWidth, windowHeight));
	menu1.setSize       (Vector2f(windowWidth, windowHeight));
	whiteScreen.setOrigin(whiteScreen.getSize() / 2.f);
	whiteScreen.setPosition(windowWidth / 2.f, windowHeight / 2.f);
	button0.setSize     (Vector2f(116.5f, 21.f));
	selectedMenu.setSize(Vector2f(100.f, 16.5f));
	charSel.setSize     (Vector2f(500.f, 512.f));
	button0.setPosition      ( 350.f,  450.f);
	selectedMenu.setPosition (-100.f, -100.f);
	charSel.setPosition      ( 150.f,    0.f);
	title.setTexture       (&Title01);
	button0.setTexture     (&pressSpace);
	logo.setTexture        (&LogoTexture);
	menu1.setTexture       (&menuTex);
	selectedMenu.setTexture(&selectedMenuTex);
	title.setFillColor(Color(0, 0, 0));
	logo.setFillColor (Color(0, 0, 0, 0));



	//variables
	bool  blink = false, blink2 = false;
	int   opacity1 = 0, opacity2 = 0, opacity3 = 0, opacity4 = 0, opacity5 = 0;
	

	// Title Screen ===================================================================
	while (window.isOpen())
	{
		EventAndTime(window, clock, time);

		// fade in sound
		if (opacity5 != 100) opacity5 += 5;
		titleTheme.setVolume(opacity5);

		// Press Space Screen ---------
		if (screen == 0)
		{
			// fade in
			if (opacity1 != 255) opacity1++;
			else if (opacity1 == 255 and opacity2 != 255) opacity2 += 3;

			// press space blink
			if (opacity1 == 255 and !blink and opacity3 > 0)  opacity3 -= 3;
			else blink = true;
			if (opacity1 == 255 and blink and opacity3 < 255) opacity3 += 3;
			else blink = false;

			title.setFillColor   (Color(opacity1, opacity1, opacity1));
			logo.setFillColor    (Color(opacity2, opacity2, opacity2, opacity2));
			button0.setFillColor (Color(255, 255, 255, opacity3));

			if (opacity1 == 255 and Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
			{
				selectFX.play();
				title.setFillColor (Color(255, 255, 255, 255));
				logo.setFillColor  (Color(255, 255, 255, 255));
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
			{
				SelectedMenu--; 
				pressed = true; 
				selectFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) and SelectedMenu != 2 and !pressed)
			{
				SelectedMenu++; 
				pressed = true; 
				selectFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
			{
				enterFX.play();
				if (SelectedMenu == 0)
				{
					screen = 2; 
					title.setTexture(&Title11);
				}
				else if (SelectedMenu == 2)
				{
					window.close();
					exit(0);
				}
			}
		}
		else if (screen == 2)
		{
			if      (opacity1 != 0)	                  opacity1 -= 3;
			else if (opacity1 == 0 and opacity2 != 0) opacity2 -= 3;
			else if (opacity1 == 0 and opacity2 == 0) screen    = 3;

			whiteScreen.setFillColor (Color(255, 255, 255, opacity1));
			title.setFillColor       (Color(opacity2, opacity2, opacity2, opacity2));
		}

		// Charactor Selection ---------
		else if (screen == 3)
		{
			if      (charactor == 0) charSel.setTexture(&blueSel);
			else if (charactor == 1) charSel.setTexture(&pinkSel);
			else if (charactor == 2) charSel.setTexture(&yellowSel);
				

			if (Keyboard::isKeyPressed(Keyboard::Up) and charactor != 0 and !pressed)
			{ 
				charactor--; 
				pressed = true; 
				selectFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) and charactor != 3 and !pressed)
			{ 
				charactor++; 
				pressed = true; 
				selectFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape) and !pressed)
			{
				pressed = true;
				screen = 1;
				charactor = 0;
				opacity1 = 255;
				opacity2 = 255;
				title.setTexture(&Title01);
				title.setFillColor(Color(255, 255, 255, 255));
				cancleFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
			{
				enterFX.play();
				player.charactor = charactor;
				opacity1 = 255;
				opacity2 = 255;
				screen   = 4;
				if (charactor == 0)
				{
					soulTex.loadFromFile("Res/gui/blue.png");
					hpTex.loadFromFile("Res/gui/BlueHealthBar.png");
					playerTexture.loadFromFile("Res/player/blue.png");
					playerCombo.loadFromFile("Res/player/blueATK.png");
					atkFX.loadFromFile("Res/particles/blueEffect.png");
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
					soulTex.loadFromFile("Res/gui/pink.png");
					hpTex.loadFromFile("Res/gui/PinkHealthBar.png");
					playerTexture.loadFromFile("Res/player/blue.png");
					playerCombo.loadFromFile("Res/player/blueATK.png");
					atkFX.loadFromFile("Res/particles/blueEffect.png");
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
					soulTex.loadFromFile("Res/gui/yellow.png");
					hpTex.loadFromFile("Res/gui/YellowHealthBar.png");
					playerTexture.loadFromFile("Res/player/yellow.png");
					playerCombo.loadFromFile("Res/player/yellowATK.png");
					atkFX.loadFromFile("Res/particles/yellowEffect.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(14, 3);
						attackParticles[i].animation.switchTime = 0.06f;
						attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
						
					player.animation.imageCount = Vector2u(12, 7);
				}
				else if (charactor == 3)
				{
					soulTex.loadFromFile("Res/gui/black.png");
					hpTex.loadFromFile("Res/gui/YellowHealthBar.png");
					playerTexture.loadFromFile("Res/player/black.png");
					playerCombo.loadFromFile("Res/player/yellowATK.png");
					atkFX.loadFromFile("Res/particles/yellowEffect.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(14, 3);
						attackParticles[i].animation.switchTime = 0.06f;
						attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}

					player.animation.imageCount = Vector2u(13, 7);
				}
				UI.healthBar.setTexture(&hpTex);	
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
				titleTheme.stop();
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
			window.clear();
			UI.Update(&soulTex, player, camera, player.HP, player.Energy, time, true);
			UI.Draw(window);
			window.draw(whiteScreen);
			window.display();
		}
	}

	//create Background
	Texture backgroundTex, midgroundTex, foregroundTex;
	RectangleShape background, midground, foreground;
	backgroundTex.loadFromFile("Res/bg/village1.png");
	midgroundTex.loadFromFile ("Res/bg/village2.png");
	foregroundTex.loadFromFile("Res/bg/village3.png");
	background.setTexture(&backgroundTex);
	midground.setTexture (&midgroundTex);
	foreground.setTexture(&foregroundTex);

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
	Texture textSplash;
	RectangleShape wideScreen, TextSplash;
	textSplash.loadFromFile("Res/bg/01TEXT.png");
	wideScreen.setSize(Vector2f(windowWidth + 120.f, windowHeight + 100.f));
	wideScreen.setOrigin(wideScreen.getSize() / 2.f);
	wideScreen.setTexture(&wideScreenTex);
	TextSplash.setSize(Vector2f(windowWidth, windowHeight));
	TextSplash.setOrigin(wideScreen.getSize() / 2.f);
	TextSplash.setTexture(&textSplash);

	// create Tutorial
	
	
	Particle movementKey(&moveKey, Vector2u(4, 1), 0.5, 0);
	Particle actionKey  (&actKey, Vector2u(2, 4), 0.5, -1);
	Ground moveZone(Vector2f(600.f, 200.f), Vector2f(700.f, 900.f));
	Ground jumpZone(Vector2f(300.f, 300.f), Vector2f(1480.f, 900.f));
	Ground dashZone(Vector2f(200.f, 200.f), Vector2f(2115.f, 900.f));
	Ground endZone (Vector2f(300.f, 300.f), Vector2f(3170.f, 900.f));
	Enemy portal(&portalTex, nullptr,Vector2u(9, 3), 3000.f, 910.f, 0.1f, 55);
	Ground prepareEnemy(Vector2f(500.f, 10.f), Vector2f(0.f, 2000.f));
	portal.sprite.setFillColor(Color(opacity5, opacity5, opacity5, opacity5));
	portal.faceRight = false;

	// set object
	blink = 0;
	blink2 = 0;
	opacity1 = 0;
	opacity2 = 255;
	opacity3 = 255;
	opacity4 = 0;
	opacity5 = 0;
	cutSceneState = 0;
	camera.setCenter(460.f, 800.f);
	player.hitBox.setPosition(-50.f, 1000.f);
	tinies[0].state = 98;
	tinies[0].SetPosition(2700.f, 900.f);
	counter = 1;

	darkSpheres[0].SetPosition(2220.f, 860.f);
	darkSpheres[0].sprite.setRotation(90.f);
	darkSpheres[0].sprite.setSize(Vector2f(190.f*1.5, 40.f*1.5));
	darkSpheres[0].sprite.setFillColor(Color(255, 100, 0, 255));
	darkSpheres[0].state = 98;

	int size = cameraHeight;

	// Sound 
	firstTheme.play();
	
	// Tutorial =========================================================================
	while (window.isOpen())
	{
		//break;
		EventAndTime(window, clock, time);
		position = player.GetPosition();
		cameraSetting(window, camera, player, background, background, foreground, position, time);
		wideScreen.setPosition(camera.getCenter());
		TextSplash.setPosition(camera.getCenter());
		
		UI.counter.setString(to_string(counter));

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
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < windowHeight + 300.f)
				size += 5;
			else
			{
				wallLeft.SetPosition(5.f, 900.f);
				player.cutScene = false;
			}
		}
		else if (!player.cutScene and position.x >= 2350.f and tinies[0].state == 98 and cutSceneState == 0)
		{
			player.cutScene = true;
			tinies[0].state = 1;
			cutSceneState = 1;
		}
		else if (player.cutScene and tinies[0].state == 1)
		{
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size > windowHeight + 100.f)
				size -= 5;
			else
			{
				wideScreen.setSize(Vector2f(windowWidth + 120.f, windowHeight + 100.f));
				wideScreen.setOrigin(wideScreen.getSize() / 2.f);
				size = windowHeight + 100.f;
			}
			camera.move((tinies[0].GetPosition().x - camera.getCenter().x) * time * 2.5f, 0.f);
		}

		if (player.cutScene and player.getHit and cutSceneState ==1)
		{
			tutorial = 4;
			showUI = true;
			blink2 = true;
			cutSceneState = 2;
		}
		if (blink2)
		{
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < windowHeight + 300.f)
				size += 5;
			else if (cutSceneState ==2)
			{
				blink2 = false;
				player.cutScene = false;
				cutSceneState = 3;
			}
		}

		if (cutSceneState == 4 and opacity2 != 0)
		{
			player.cutScene = true;
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size > windowHeight + 100.f)
				size -= 5;
			else
			{
				wideScreen.setSize(Vector2f(windowWidth + 120.f, windowHeight + 100.f));
				wideScreen.setOrigin(wideScreen.getSize() / 2.f);
				size = windowHeight + 100.f;
			}
			player.sprite.setFillColor(Color(opacity2, opacity2, opacity2, 255));
			opacity2 -= 3;
		}
		else if (cutSceneState == 4 and opacity3 != 0)
		{
			player.sprite.setFillColor(Color(0, 0, 0, opacity3));
			portal.sprite.setFillColor(Color(255, 255, 255, opacity3));
			opacity3 -= 3;
		}
		else if (cutSceneState == 4 and opacity4 != 255)
		{
			whiteScreen.setPosition(camera.getCenter());
			whiteScreen.setFillColor(Color(0, 0, 0, opacity4));
			opacity4+=3;
		}
		else if (cutSceneState == 4)
		{
			screen = 6;
			player.cutScene = false;

			darkSpheres[0].sprite.setRotation(0);
			darkSpheres[0].sprite.setSize(Vector2f(190.0f, 40.0f));
			darkSpheres[0].sprite.setFillColor(Color(255, 255, 255, 255));
			darkSpheres[0].state = 1;
			firstTheme.stop();
			break;
		}

		if (cutSceneState == 0)
		{
			// Show Arrow button
			if (moveZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0) tutorial = 1;
			else tutorial = 0;
			// Show Jump button
			if (jumpZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0)
			{
				tutorial = 2;
				actionKey.row = 3;
			}
			else tutorial = 0;
			// Show X button
			if (dashZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0)
			{
				tutorial = 3;
				actionKey.row = 1;
			}
			else tutorial = 0;
		}
	
	// Show Z button
		if (tutorial == 4) actionKey.row = 0;
		if (counter == 0 and opacity5 != 255)
		{
			portal.sprite.setFillColor(Color(opacity5, opacity5, opacity5, opacity5));
			opacity5 += 3;
			tutorial = 0;
		}
			

	// End map
		if (endZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and cutSceneState==3 and counter == 0)
			cutSceneState = 4;


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
			getHitFX.play();
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
		}

			
		// Tiny Attack!!!
		Tiny(tinies, hitParticles, player, ground, prepareEnemy, position, invincibleTime, direction);
		player.HP = 100.f;
		
		// player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles, player);
		
		// Draw Every things
		window.clear();
		window.draw(background);
		window.draw(midground);
		portal.Update(time, portal.GetPosition(), player, 55);
		if (counter == 0)
			portal.Draw(window);
		player.Update(position, player.Invincible, time, charactor);
		player.Draw(window);
		tinies[0].Update(time, tinies[0].GetPosition(), player, 0);
		tinies[0].Draw(window);
		drawParticles(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX,player, window, time);
		drawTutorial(window, movementKey, actionKey, moveKey, actKey, player, moveZone, jumpZone, dashZone, time);
		darkSpheres[0].Update(time, darkSpheres[0].GetPosition(), player, 4);
		darkSpheres[0].Draw(window);
		UI.Update(&soulTex, player, camera, player.HP, player.Energy, time, false);
		window.draw(TextSplash);
		if (showUI and !player.cutScene)
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
	backgroundTex.loadFromFile("Res/bg/forest1.png");
	midgroundTex.loadFromFile("Res/bg/forest2.png");
	foregroundTex.loadFromFile("Res/bg/forest3.png");
	moon.setTexture(&moonTexture);
	background.setTexture(&backgroundTex);
	midground.setTexture(&midgroundTex);
	foreground.setTexture(&foregroundTex);
	moon.setSize(Vector2f(1139.f*2, 552.f * 2));
	background.setSize(Vector2f(3685.f*1.2, 552.f*2));
	midground.setSize(Vector2f(3618.f*1.2, 552.f*2));
	foreground.setSize(Vector2f(6762.f*1.2, 552.f*2));
	moon.setPosition(-1000.f, 120.f);

	screen = 6;
	//set object
	portal.SetPosition(2000.f,900.f);
	ground.SetPosition(1700.f, 1050.f);
	wallLeft.SetPosition(5.f, 900.f);
	wallRight.SetPosition(4000.f,900.f);
	textSplash.loadFromFile("Res/bg/02TEXT.png");
	blink = false;
	opacity1 = 0;
	opacity2 = 255;
	opacity3 = 0;
	opacity4 = 255;
	portal.sprite.setFillColor(Color(0, 0, 0, 0));
	portal.hitBox.setPosition(600.f, 600.f);
	portal.row = 1;
	player.hitBox.setPosition(600.f, 600.f);
	player.canJump = false;
	player.falling = true;
	camera.setCenter(600.f, 600.f);
	player.cutScene = true;
	cutSceneState = 0;
	counter = 25;
	int spawn = 0, spawnCount = 0;
	Clock spawnDelay;
	spawnDelay.restart().asSeconds();
	for (int i = 0; i < tinies.size(); i++)
	{
		tinies[i].state = 98;
		tinies[i].SetPosition(0, 1800);
	}
	for (int i = 0; i < ghosts.size(); i++)
	{
		ghosts[i].state = 98;
		ghosts[i].SetPosition(0, 1800);
	}
	for (int i = 0; i < giants.size(); i++)
	{
		giants[i].state = 98;
		giants[i].SetPosition(0, 1800);
	}
	for (int i = 0; i < shamans.size(); i++)
	{
		shamans[i].state = 98;
		shamans[i].SetPosition(0, 1800);
	}

	// Sound 
	forestTheme.play();
		

	// Woods ============================================================================
	while (window.isOpen())
	{
		//break;
		EventAndTime(window,clock,time);
		cameraSetting(window, camera, player, moon, background, foreground, position, time);

		position = player.GetPosition();
		TextSplash.setPosition(camera.getCenter());
		wideScreen.setPosition(camera.getCenter());
		whiteScreen.setPosition(camera.getCenter());

		UI.counter.setString(to_string(counter));

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

		// fall from portal
		if (cutSceneState == 0)
		{
			portal.sprite.setFillColor(Color(opacity3, opacity3, opacity3, opacity3));
			player.sprite.setFillColor(Color(opacity3, opacity3, opacity3, opacity3));
			player.hitBox.setPosition(600.f, 600.f);
			player.velocity.y = 0;
			if (opacity3 != 255)
				opacity3 += 1;
			else if (opacity3 == 255 and cutSceneState == 0)
			{
				player.hitBox.setPosition(600.f, 600.f);
				player.canJump = false;
				player.falling = true;
				player.velocity.y = 0;
				opacity3 = 0;
				cutSceneState = 1;
				size = cameraHeight;
			}
		}
		else if (cutSceneState == 1)
		{
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < windowHeight + 300)
				size += 5;
			else if (screen == 6 and cutSceneState == 1)
			{
				showUI = true;
				player.cutScene = false;
				opacity3 = 255;
				screen = 7;
				camera.setCenter(500.f, 800.f);
				cutSceneState = 2;
			}
		}
		if (screen == 7 and cutSceneState == 2)
		{
			portal.sprite.setFillColor(Color(opacity3, opacity3, opacity3, opacity3));
			if (opacity3 != 0)
				opacity3 -= 3;
			else
			{
				cutSceneState = 3;
				for (int i = 0; i < tinies.size(); i++)
				{
					tinies[i].state = 98;
					tinies[i].SetPosition(0, 1800);
				}
				for (int i = 0; i < ghosts.size(); i++)
				{
					ghosts[i].state = 98;
					ghosts[i].SetPosition(0, 1800);
				}
				for (int i = 0; i < giants.size(); i++)
				{
					giants[i].state = 98;
					giants[i].SetPosition(0, 1800);
				}
				for (int i = 0; i < shamans.size(); i++)
				{
					shamans[i].state = 98;
					shamans[i].SetPosition(0, 1800);
				}
				for (int i = 0; i < darkSpheres.size(); i++)
				{
					darkSpheres[i].state = 1;
					darkSpheres[i].SetPosition(0, 1800);
				}
			}
		}

		// Spawn ------
		if (cutSceneState == 3 and spawn == 0)
		{	
			if (spawnCount >= 6)
			{
				spawnCount = 0;
				spawn = 1;
			}
			else if(spawnCount < 6)
			{
				if (spawnDelay.getElapsedTime().asSeconds() > 1.5f)
				{
					if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
					{
						if (position.x < 2500.f)
							tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
						else
							tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
						tinies[spawnCount].velocity.y = 0;
						tinies[spawnCount].state = 1;
						spawnDelay.restart().asSeconds();
						spawnCount++;
					}
					else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
					{
						if (position.x > 1000.f)
							tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
						else
							tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
						tinies[spawnCount].velocity.y = 0;
						tinies[spawnCount].state = 1;
						spawnDelay.restart().asSeconds();
						spawnCount++;
					}
				}
			}
		}
		else if (cutSceneState == 3 and spawn == 1)
		{
			if (spawnCount >= 6)
			{
				spawnCount = 0;
				spawn = 2;
			}
			else if (spawnCount < 6)
			{
				if (spawnDelay.getElapsedTime().asSeconds() > 1.5f)
				{
					if (spawnCount == 0 or spawnCount == 3)
					{
						if (spawnCount % 2 == 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								ghosts[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								ghosts[spawnCount].SetPosition(position.x - 600.f, 800.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								ghosts[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								ghosts[spawnCount].SetPosition(position.x + 600.f, 600.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else
					{
						if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
				}
			}
		}
		else if (cutSceneState == 3 and spawn == 2)
		{
			if (spawnCount >= 6)
			{
				spawnCount = 0;
				spawn = 3;
			}
			else if (spawnCount < 6)
			{
				if (spawnDelay.getElapsedTime().asSeconds() > 1.5f)
				{
					if (spawnCount == 0 or spawnCount == 3)
					{
						if (spawnCount % 2 == 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								ghosts[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								ghosts[spawnCount].SetPosition(position.x - 600.f, 800.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								ghosts[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								ghosts[spawnCount].SetPosition(position.x + 600.f, 600.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else if (spawnCount == 1 or spawnCount == 4 )
					{
						if (spawnCount % 2 == 0 and shamans[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								shamans[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								shamans[spawnCount].SetPosition(position.x - 600.f, 800.f);
							shamans[spawnCount].velocity.y = 0;
							shamans[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and shamans[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								shamans[spawnCount].SetPosition(position.x - 600.f, 800.f);
							else
								shamans[spawnCount].SetPosition(position.x + 600.f, 800.f);
							shamans[spawnCount].velocity.y = 0;
							shamans[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else
					{
						if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
				}
			}
		}
		else if (cutSceneState == 3 and spawn == 3)
		{
			if (spawnCount >= 6)
			{
				spawnCount = 0;
				spawn = 4;
			}
			else if (spawnCount < 6)
			{
				if (spawnDelay.getElapsedTime().asSeconds() > 1.5f)
				{
					if (spawnCount == 0 or spawnCount == 3)
					{
						if (spawnCount % 2 == 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								ghosts[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								ghosts[spawnCount].SetPosition(position.x - 600.f, 800.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								ghosts[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								ghosts[spawnCount].SetPosition(position.x + 600.f, 600.f);
							ghosts[spawnCount].velocity.y = 0;
							ghosts[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else if (spawnCount == 1)
					{
						if (spawnCount % 2 == 0 and shamans[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								shamans[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								shamans[spawnCount].SetPosition(position.x - 600.f, 800.f);
							shamans[spawnCount].velocity.y = 0;
							shamans[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and shamans[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								shamans[spawnCount].SetPosition(position.x - 600.f, 800.f);
							else
								shamans[spawnCount].SetPosition(position.x + 600.f, 800.f);
							shamans[spawnCount].velocity.y = 0;
							shamans[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else if (spawnCount == 5)
					{
						if (spawnCount % 2 == 0 and giants[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								giants[spawnCount].SetPosition(position.x + 600.f, 800.f);
							else
								giants[spawnCount].SetPosition(position.x - 600.f, 800.f);
							giants[spawnCount].velocity.y = 0;
							giants[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and giants[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								giants[spawnCount].SetPosition(position.x - 600.f, 800.f);
							else
								giants[spawnCount].SetPosition(position.x + 600.f, 800.f);
							giants[spawnCount].velocity.y = 0;
							giants[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
					else
					{
						if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
						{
							if (position.x < 2500.f)
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
						else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
						{
							if (position.x > 1000.f)
								tinies[spawnCount].SetPosition(position.x - 600.f, 600.f);
							else
								tinies[spawnCount].SetPosition(position.x + 600.f, 600.f);
							tinies[spawnCount].velocity.y = 0;
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds();
							spawnCount++;
						}
					}
				}
			}
		}

		// Homura cutscene
		if (spawn == 4 and counter == 1 and !player.cutScene)
		{
			player.cutScene = true;
		}
		if (player.cutScene and cutSceneState == 3)
		{
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size > windowHeight + 100.f)
				size -= 5;
			else
			{
				cutSceneState = 4;
				wideScreen.setSize(Vector2f(windowWidth + 120.f, windowHeight + 100.f));
				wideScreen.setOrigin(wideScreen.getSize() / 2.f);
				size = windowHeight + 100.f;
			}
		}
		else if (player.cutScene and cutSceneState == 4)
		{
			if (position.x < 1550)
			{
				background.move(-5.f, 0.f);
				midground.move(-15.f, 0.f);
				foreground.move(-35.f, 0.f);
			}
			else
			{
				background.move(+5.f, 0.f);
				midground.move(+15.f, 0.f);
				foreground.move(+35.f, 0.f);
			}
			player.sprite.setFillColor(Color(opacity4, opacity4, opacity4, 255));
			background.setFillColor(Color(opacity4, opacity4, opacity4, opacity4));
			midground.setFillColor(Color(opacity4, opacity4, opacity4, opacity4));
			moon.setFillColor(Color(255, 255, 255, opacity4));
			if (opacity4 != 0) opacity4--;
			else 
			{
				tinies[0].wait.restart().asSeconds();
				cutSceneState = 5;
			}
						
		}
		else if (player.cutScene and cutSceneState == 5)
		{
			player.sprite.setFillColor(Color(0, 0, 0, 255));
			background.setFillColor(Color(0, 0, 0, 0));
			moon.setFillColor(Color(255, 255, 255, 0));
			if (tinies[0].wait.getElapsedTime().asSeconds() > 30)
			{
				if (position.x < 1550)
				{
					background.move(-5.f, 0.f);
					midground.move(-15.f, 0.f);
					foreground.move(-35.f, 0.f);
				}
				else
				{
					background.move(+5.f, 0.f);
					midground.move(+15.f, 0.f);
					foreground.move(+35.f, 0.f);
				}
			}
			else
			{
				cutSceneState = 6;
				screen = 8;
				forestTheme.stop();
				break;
			}
		}


		
		Tiny(tinies, hitParticles, player, ground, prepareEnemy, position, invincibleTime, direction);
		Giant(giants, hitParticles, player, ground, prepareEnemy, position, invincibleTime, direction);
		Ghost(ghosts, hitParticles, player, ground, position, invincibleTime, direction);
		Shaman(shamans, darkSpheres, hitParticles, player, ground, prepareEnemy, position, invincibleTime, direction);
		DarkSphere(darkSpheres, player, ground, wallLeft, wallRight, position, invincibleTime, direction);

		// Check collide with object
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);

		// player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles, player);

		// Draw Every things
		window.clear(Color::White);
		window.draw(moon);
		window.draw(background);
		window.draw(midground);
		portal.Update(time, portal.GetPosition(), player, 55);
		portal.Draw(window);
		drawGiant(giants, player, window, time);
		drawShaman(shamans, player, window, time);
		player.Update(position, player.Invincible, time, charactor);
		player.Draw(window);
		drawGhost		(ghosts,         player, window, time);
		drawTiny		(tinies, player, window, time);
		drawDarkSphere(darkSpheres,  player, window, time);
		drawParticles(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX, player, window, time);
		window.draw(foreground);
		UI.Update(&soulTex, player, camera, player.HP, player.Energy, time, false);
		if (showUI and ! player.cutScene)
			UI.Draw(window);
		if (player.cutScene)
			window.draw(wideScreen);
		window.draw(whiteScreen);
		window.draw(TextSplash);
		window.display();

	}

	screen = 8;

	// set backgound
	Texture backgroundStop;
	background.setSize(Vector2f(900.f*2, 612.f*1.02));
	backgroundTex.loadFromFile("Res/bg/bridge.png");
	backgroundStop.loadFromFile("Res/bg/bridgeSTOP.png");
	background.setTexture(&backgroundTex);
	background.setPosition(-50, -42);
	background.setFillColor(Color(255, 255, 255, 0));

	// set object
	Texture bulletTexture,rocketTexture;
	bulletTexture.loadFromFile("Res/enemy/bullet.png");
	rocketTexture.loadFromFile("Res/enemy/rocket.png");
	Enemy rocket(&rocketTexture, nullptr, Vector2u(4, 1), 0.f, 2000.f, 0.08f, 8);
	vector<Enemy> bullets;
	for (int i = 0; i < 10; i++)
	{
		bullets.push_back(Enemy(&bulletTexture, nullptr, Vector2u(1, 1), 0.f, 2000.0f, 0.08f, 7));
		Enemy& bullet = bullets[i];
	}
	for (int i = 0; i < bullets.size(); i++) bullets[i].state = 1;
	rocket.state = 1;
	Texture homuraTexture, homuraATK;
	homuraTexture.loadFromFile("Res/enemy/Homura.png");
	homuraATK.loadFromFile("Res/enemy/HomuraATK.png");
	Enemy Homura(&homuraTexture, &homuraATK, Vector2u(13, 7), 3010.f, 900.f, 0.1f, 6);
	opacity1 = 0;
	opacity2 = 0;
	opacity3 = 255;
	player.hitBox.setPosition(400.f, 500.f);
	Homura.state = 98;
	Homura.faceRight = false;
	Homura.SetPosition(600.f, 500.f);
	Homura.sprite.setFillColor(Color(255, 255, 255, 0));
	wideScreen.setPosition(cameraWidth / 2 - 40, cameraHeight / 2 - 40);
	ground.SetPosition(400.f, 512.f);
	wallLeft.SetPosition(-40.f, 400.f);
	wallRight.SetPosition(840.f, 400.f);
	cutSceneState = 0;

	// Sound 
	bossTheme.play();

	// Homura Fight ======================================================================
	while (window.isOpen())
	{
		EventAndTime(window,clock,time);
		cameraSetting(window, camera, player, moon, background, foreground, position, time);
		position = player.GetPosition();

		//fade in
		if (cutSceneState == 0)
		{
			player.sprite.setFillColor(Color(opacity1, opacity1, opacity1, 255));
			if (opacity1 != 255) opacity1++;
			else cutSceneState = 1;
		}
		else if (cutSceneState == 1)
		{
			background.setFillColor(Color(255, 255, 255, opacity2));
			Homura.sprite.setFillColor(Color(255, 255, 255, opacity2));
			if (opacity2 != 255) opacity2++;
			else cutSceneState = 2;
		}
		else if (cutSceneState == 2)
		{
			wideScreen.setSize(Vector2f(windowWidth + 120.f, size));
			wideScreen.setOrigin(wideScreen.getSize() / 2.f);

			if (size < windowHeight + 300)
				size += 5;
			else
			{
				showUI = true;
				player.cutScene = false;
				cutSceneState = 3;
				Homura.state = 1;
			}
		}

		if (Homura.Dead)
		{
			if (cutSceneState == 3)
			{
				player.cutScene = true;
				background.setFillColor(Color(255, 255, 255, opacity3));

				if (opacity3 != 0) opacity3--;
				else cutSceneState = 4;
			}
			else if (cutSceneState == 4)
			{
				screen = 0;
				bossTheme.stop();
				window.close();
				main();
			}
		}

		// player status
		Dashing(player);
		playerGetHit(player);
		playerStatus(player, invincibleTime);
		createParticles(dustParticles, dustParticles2, attackParticles, player);
		endParticles(hitParticles, dustParticles, dustParticles2, attackParticles, player);

		if (Homura.startShoot or Homura.startShoot2)
		{
			for (unsigned int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i].state == 1)
				{
					bullets[i].wait.restart().asSeconds();
					bullets[i].state = 99;
					
					bullets[i].faceRight = Homura.faceRight;
					if (Homura.startShoot)
					{
						if (Homura.faceRight)
							bullets[i].SetPosition(Homura.GetPosition().x + 40.f, Homura.GetPosition().y - 23.f);
						else
							bullets[i].SetPosition(Homura.GetPosition().x - 40.f, Homura.GetPosition().y - 23.f);
					}
					else if (Homura.startShoot2)
					{
						if (Homura.faceRight)
							bullets[i].SetPosition(Homura.GetPosition().x + 40.f, Homura.GetPosition().y - 3);
						else
							bullets[i].SetPosition(Homura.GetPosition().x - 40.f, Homura.GetPosition().y - 3);
					}
					
					bullets[i].Update(time, bullets[i].GetPosition(), player, 7);
					bullets[i].Draw(window);
					Homura.startShoot = false;
					Homura.startShoot2 = false;
					break;
				}
				if (bullets[i].state == 99 and bullets[i].wait.getElapsedTime().asSeconds() > 5)
					bullets[i].state = 1;
			}
		}
		if (Homura.startShoot3)
		{
			if (rocket.state == 1)
			{
				rocket.wait.restart().asSeconds();
				rocket.state = 99;

				rocket.faceRight = Homura.faceRight;
				if (Homura.faceRight)
					rocket.SetPosition(Homura.GetPosition().x + 40.f, Homura.GetPosition().y - 23.f);
				else
					rocket.SetPosition(Homura.GetPosition().x - 40.f, Homura.GetPosition().y - 23.f);

				rocket.Update(time, rocket.GetPosition(), player, 8);
				rocket.Draw(window);
				Homura.startShoot3 = false;
			}
		}
		if (rocket.state == 99 and rocket.wait.getElapsedTime().asSeconds() > 8)
			rocket.state = 1;

		for (unsigned int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i].GetHitBox().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !player.Invincible and !player.Dash)
			{
				getHitFX.play();
				player.getHit = true;
				player.animation.playingAnimation = true;
				player.animation.OneTimeAnimation = true;
				player.animation.OneTimeFrame.x = 4;
				player.animation.currentImage.x = 0;
				invincibleTime.restart().asSeconds();
				player.Invincible = true;
				player.HP -= Homura.bulletDamage;
			}
		}
		if (rocket.GetHitBox().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !player.Invincible and !player.Dash)
		{
			getHitFX.play();
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.HP -= Homura.bulletDamage;
		}

		if (Homura.GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.f, false) and !Homura.invincible)
		{
			Homura.invincible = true;
			Homura.HP -= player.damage;
			Homura.invincibleTime.restart().asSeconds();
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
		/*
		if (player.GetHitBox().CheckCollide(Homura.GetHitBox(), direction, 0.f, false) and !player.Invincible and !player.Dash)
		{
			getHitFX.play();
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
			player.HP -= Homura.bodyDamge;
		}*/

		if (Homura.invincible and Homura.invincibleTime.getElapsedTime().asSeconds() > 3)
		{
			Homura.invincible = false;
			Homura.sprite.setFillColor(Color(255,255,255,255));
		}


		// check collide
		checkPlayerOnGround(player, ground, wallLeft, wallRight, direction);
		if (ground.GetCollider().CheckCollide(Homura.GetHitBox(), direction, 1.f, true))
			Homura.OnCollision(direction);


		// Draw
		window.clear(Color::White);
		if (!Homura.STOP) background.setTexture(&backgroundTex);
		else background.setTexture(&backgroundStop);
		window.draw(background);
		Homura.Update(time, Homura.GetPosition(), player, 6);
		Homura.Draw(window);
		if (!Homura.STOP)
			player.Update(position, player.Invincible, time, charactor);
		player.Draw(window);
		for (int i = 0; i < bullets.size(); i++)
		{
			if (!Homura.STOP)	
				bullets[i].Update(time, bullets[i].GetPosition(),player,7);
			bullets[i].Draw(window);
		}
		if (!Homura.STOP)
			rocket.Update(time, rocket.GetPosition(), player, 8);
		rocket.Draw(window);
		if(!Homura.STOP)
			drawParticles(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX, player, window, time);
		if (!Homura.STOP)
			UI.Update(&soulTex, player, camera, player.HP, player.Energy, time, false);
		if (showUI and !player.cutScene)
			UI.Draw(window);
		if (player.cutScene)
			window.draw(wideScreen);
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
void animate(Texture* texture, Vector2u imageCount, short row, float switchTime, float time)
{
	uvRect.width  = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

	currentImage.y = row;
	totalTime += time;

	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;
		currentImage.x++;
		if (currentImage.x >= imageCount.x)
			currentImage.x = 0;
	}
	uvRect.top = currentImage.y * uvRect.height;
	uvRect.left = currentImage.x * abs(uvRect.width);
	uvRect.width = abs(uvRect.width);
}

void loadFile()
{
		// Theme song
		if (titleTheme.openFromFile("Res/sound/title.ogg"))
		if (firstTheme.openFromFile("Res/sound/first.ogg"))
		if (forestTheme.openFromFile("Res/sound/woods.ogg"))
		if (bossTheme.openFromFile("Res/sound/boss.ogg"))

		// Title FX
		if (selectSoundFX.loadFromFile("Res/sound/select.ogg"))
		if (enterSoundFX.loadFromFile("Res/sound/enter.ogg"))
		if (cancleSoundFX.loadFromFile("Res/sound/cancle.ogg"))

		// Title
		if (Title01.loadFromFile("Res/title/Title.png"))
		if (Title11.loadFromFile("Res/title/TitleBlur.png"))
		if (pressSpace.loadFromFile("Res/title/pressSpace.png"))
		if (LogoTexture.loadFromFile("Res/title/logo.png"))
		if (menuTex.loadFromFile("Res/title/menu.png"))
		if (selectedMenuTex.loadFromFile("Res/title/selected menu.png"))
		if (blueSel.loadFromFile("Res/title/blueSelected.png"))
		if (pinkSel.loadFromFile("Res/title/pinkSelected.png"))
		if (yellowSel.loadFromFile("Res/title/yellowSelected.png"))

		// Tutorial
		if (moveKey.loadFromFile("Res/particles/button2.png"))
		if (actKey.loadFromFile("Res/particles/button.png"))
		if (portalTex.loadFromFile("Res/enemy/portal.png"))

		// GUI
		if (font.loadFromFile("Res/Font/CreditValley.ttf"))
		if (counterTex.loadFromFile("Res/gui/enemyCount.png"))
		if (spTex.loadFromFile("Res/gui/EnergyBar.png"))
		if (wideScreenTex.loadFromFile("Res/gui/cropScreen.png"))

		// Particles
		if (hitFX.loadFromFile("Res/particles/hit.png"))
		if (dustFX.loadFromFile("Res/particles/dust.png"))

		// Enemy
		if (tinyTex.loadFromFile("Res/enemy/tiny.png"))
		if (giantTex.loadFromFile("Res/enemy/giant.png"))
		if (ghostTex.loadFromFile("Res/enemy/ghost.png"))
		if (shamanTex.loadFromFile("Res/enemy/shaman.png"))
		if (darkSphereTex.loadFromFile("Res/enemy/shadowBall.png"))
		{ loaded = true; loading.terminate(); }
}

void EventAndTime(RenderWindow& window, Clock& clock, float& time)
{
	Event button;
	while (window.pollEvent(button))
	{
		if (button.type == Event::Closed)
		{
			window.close();
			exit(0);
		}
		if (button.type == Event::KeyReleased)
			pressed = false;
	}

	time = clock.restart().asSeconds();
	//if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;
}

void drawTutorial(RenderWindow& window, Particle& movementKey, Particle& actionKey, Texture& moveKey, Texture& actKey, Player& player, Ground& moveZone, Ground& jumpZone, Ground& dashZone, float time)
{
	actionKey.  Update (&actKey, player, time, -1);
	movementKey.Update (&moveKey, player, time, 0);

	if (tutorial == 1)
		movementKey.Draw(window);
	if (tutorial == 2 or tutorial == 3 or tutorial == 4)
		actionKey.Draw(window);

	//moveZone.Draw(window);
	//jumpZone.Draw(window);
	//dashZone.Draw(window);
}

void cameraSetting(RenderWindow& window, View& camera, Player& player, RectangleShape& preBack, RectangleShape& back, RectangleShape& front, Vector2f playerPosition, float time)
{
	Vector2f cameraMovement = player.GetPosition() - camera.getCenter();

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
		camera.move(cameraMovement * time *  2.5f);
	else if (screen == 7)
	{
		cameraMovement += Vector2f(0.0f, -180.0f);
		if (playerPosition.x > 460.f and playerPosition.x < 3620.f)
		{
			camera.move(cameraMovement.x * time *  2.5f,0.f);
			preBack.move(cameraMovement.x* time * 2.5f, 0.f);
			back.move(cameraMovement.x * time * -0.5f, 0.f);
			front.move(cameraMovement.x * time * -1.5f, 0.f);
		}
		
	}
	else if (screen == 8)
	{
		camera.setCenter(windowWidth / 2, windowHeight / 2);
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

void drawTiny(vector<Enemy>& tinies, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < tinies.size(); i++)
	{
		tinies[i].Update(time, tinies[i].GetPosition(), player, 0);
		tinies[i].Draw(window);
	}
}

void drawGiant(vector<Enemy>& giants, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < giants.size(); i++)
	{
		giants[i].Update(time, giants[i].GetPosition(), player, 1);
		giants[i].Draw(window);
	}
}

void drawGhost(vector<Enemy>& ghosts, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		ghosts[i].Update(time, ghosts[i].GetPosition(), player, 2);
		ghosts[i].Draw(window);
	}
}

void drawShaman(vector<Enemy>& shamans, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < shamans.size(); i++)
	{
		shamans[i].Update(time, shamans[i].GetPosition(), player, 3);
		shamans[i].Draw(window);
	}
}

void drawDarkSphere(vector<Enemy>& darkSpheres, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		darkSpheres[i].Update(time, darkSpheres[i].GetPosition(), player, 4);
		darkSpheres[i].Draw(window);
	}
}

void drawSoulEater(vector<Enemy>& souleaters, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < souleaters.size(); i++)
	{
		souleaters[i].Update(time, souleaters[i].GetPosition(), player, 5);
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

void Tiny(vector<Enemy>& tinies, vector<Particle>& hitParticles, Player& player, Ground& ground, Ground& prepare, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
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
		else if (tinies[i].Dead and tinies[i].state == 100)
		{ 
			tinies[i].state = 101;
			counter--;
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(tinies[i].GetAttackBox(), direction, 0.0f, false) and tinies[i].state == 4 and !player.Invincible and !player.Dash and !player.Dead)
		{
			getHitFX.play();
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
			tinies[i].HP -= player.damage;
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
		if (prepare.GetCollider().CheckCollide(tinies[i].GetHitBox(), direction, 1.0f, true))
			tinies[i].OnCollision(direction);
	}
}

void Giant(vector<Enemy>& giants, vector<Particle>& hitParticles, Player& player, Ground& ground, Ground& prepare, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
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
		else if (giants[i].Dead and giants[i].state != 101 and giants[i].state != 98)
		{
			giants[i].state = 101;
			counter--;
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(giants[i].GetAttackBox(), direction, 0.0f, false) and giants[i].state == 4 and !player.Invincible and !player.Dash and !player.Dead)
		{
			getHitFX.play();
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
			giants[i].HP -= player.damage;
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
		if (prepare.GetCollider().CheckCollide(giants[i].GetHitBox(), direction, 1.0f, true))
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
		if (ghosts[i].HP <= 0.f and !ghosts[i].Dead)
			ghosts[i].state = 100;
		else if (ghosts[i].Dead and ghosts[i].state == 100)
		{
			ghosts[i].state = 101;
			counter--;
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(ghosts[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.Dead)
		{
			getHitFX.play();
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			invincibleTime.restart().asSeconds();
			player.Invincible = true;
		}
		if (ghosts[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and ghosts[i].state != 7 and ghosts[i].state != 101 and ghosts[i].state != 100)
		{
			ghosts[i].HP -= player.damage;
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

void Shaman(vector<Enemy>& shamans, vector<Enemy>& darkSpheres, vector<Particle>& hitParticles, Player& player, Ground& ground, Ground& prepare, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
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
		if (shamans[i].HP <= 0.f and !shamans[i].Dead)
			shamans[i].state = 100;
		else if (shamans[i].Dead and shamans[i].state == 100)
		{
			shamans[i].state = 101;
			counter--;
		}
		if (shamans[i].startShoot)
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
							shamans[i].startShoot = false;
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
							shamans[i].startShoot = false;
							shamans[i].state = 1;
							break;
						}
					}
				}
			}
		}

	// Hit Box
		if (shamans[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and shamans[i].state != 7 and shamans[i].state != 4 and shamans[i].state != 101 and shamans[i].state != 100)
		{
			shamans[i].HP -= player.damage;
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
		if (prepare.GetCollider().CheckCollide(shamans[i].GetHitBox(), direction, 1.0f, true))
			shamans[i].OnCollision(direction);
	}
}

void DarkSphere(vector<Enemy>& darkSpheres, Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight, Vector2f& position, Clock& invincibleTime, Vector2f& direction)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		if (player.GetHitBox().CheckCollide(darkSpheres[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.Dead)
		{
			getHitFX.play();
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