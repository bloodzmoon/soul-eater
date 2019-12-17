#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Interface.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Ground.h"
#include "Particle.h"

#define WINDOW_W 800.f
#define WINDOW_H 512.f
#define CAM_W 900.f
#define CAM_H 612.f
using namespace std;
using namespace sf;

// Prototypes ===================
void animate (Texture*, Vector2u, short, float, float);
void loadFile();
void EventAndTime (RenderWindow&, Clock&, Player&, float&);

void showTutorial  (RenderWindow&, Particle&, Particle&, Texture&, Texture&, Player&, Ground&, Ground&, Ground&, float);
void cameraSetting (RenderWindow&, View&, Player&, RectangleShape&, RectangleShape&, RectangleShape&, Vector2f,  float);
bool CutScene      (bool, RectangleShape&, unsigned int&);

void createFX (vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void endFX    (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Player&);
void drawFX   (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, RenderWindow&);
void updateFX (vector<Particle>&, vector<Particle>&, vector<Particle>&, vector<Particle>&, Texture&, Texture&, Texture&, Player&, float);

void drawTiny		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawGiant		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawGhost		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawShaman		(vector<Enemy>&, Player&, RenderWindow&, float);
void drawDarkSphere (vector<Enemy>&, Player&, RenderWindow&, float);
void Tiny		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&);
void Giant		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&);
void Ghost		(vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&);
void Shaman		(vector<Enemy>&, vector<Enemy>&, vector<Enemy>&, vector<Particle>&, Player&, Ground&);
void DarkSphere (vector<Enemy>&, Player&);
void HomuraAtk  (Enemy&, vector<Enemy>&, Enemy&, Player&, vector<Particle>&, RenderWindow&, float);

void drawGround (RenderWindow&, Ground&, Ground&, Ground&);
void checkPlayerOnGround(Player&, Ground&, Ground&, Ground&);


// Prepare to load file =========================================================================
bool fileLoaded = false;
Thread loading(&loadFile);
	// Theme song
	Music titleTheme, firstTheme, forestTheme, bossTheme;
	// Title soundFX
	SoundBuffer selectSoundFX, enterSoundFX, cancleSoundFX;
	Sound selectFX, enterFX, cancleFX;
	// Title
	Texture Title01, Title11, logoTex, pressSpace, scoreBG;
	Texture menu1Tex, menu2Tex, menu3Tex, menu1Tex2, menu2Tex2, menu3Tex2, menu4Tex2;
	Texture blueSel, pinkSel, yellowSel, blackSel;
	// GUI
	Font font;
	Texture guiTex, blackHpTex, redHpTex, playerHpTex;
	Texture soulTex, counterTex, wideScreenTex;
	// Particles
	Texture dustFX, hitFX, atkFX, orbTex;
	// Enemy
	Texture tinyTex, giantTex, ghostTex, shamanTex, darkSphereTex;
	Texture homuraTexture, homuraATK;
	Texture bulletTex, rocketTex;
	SoundBuffer tinySoundFX, giantSoundFX, ghostSoundFX, shamanSoundFX;
	Sound tinyFX, giantFX, ghostFX, shamanFX;
	//Player
	Texture playerTexture, playerCombo;
	SoundBuffer pokeSoundFX;
	Sound pokeFX;
	// Misc
	Texture moveKey, actKey, portalTex;
	// Global Sound
	SoundBuffer getHitSoundFX, deadSoundFX, pickSoundFX, dropSoundFX, warpSoundFX;
	Sound getHitFX, deadFX, pickFX, dropFX, warpFX;
	// Saved game
	ofstream saved;
	ifstream loaded;

// Global variables ====================================================================================
	// animate function
	IntRect uvRect;
	Vector2u currentImage;
	float totalTime = 0.f;
	
	// Game
	Clock wait;
	Vector2f direction;
	bool isPressed;

	uint16_t selectedMenu  = 0;
	uint16_t counter = 0;
	uint16_t score = 0;
	short charactor = 0;

	uint16_t stage = 0;
	uint16_t screen = 0;
	uint16_t tutorial = 0;
	uint16_t cutSceneState = 0;
	bool showUI = false;
	bool shakeFX = false;
	bool unlockedSoul = false;


	// Fade
	uint16_t fadeVol = 100;
	uint16_t opacity1 = 0, opacity2 = 0, opacity3 = 0, opacity4 = 0, opacity5 = 0;
	bool blink = false;
	bool continueSound = false;


// Main ===================================================================================================================
int main()
{
	// begin Randomizer
	srand(time(NULL));

	// create Window and Camera //
	RenderWindow window(VideoMode(WINDOW_W, WINDOW_H), "Soul Eater v1.7", Style::Default);
	Image icon; icon.loadFromFile("Res/misc/icon.png");
	window.setIcon(64, 64, icon.getPixelsPtr());
	window.setFramerateLimit(60);
	View camera(Vector2f(0.0f, 0.0f), Vector2f(CAM_W, CAM_H));


	////////////  Load Game Data ////////////////////
	loaded.open("saved.txt");
	int tmp = 0, tmp2 = 0;
	if (loaded.is_open())
	{
		int line = 0;
		while (!loaded.eof())
		{
			if (line == 0) loaded >> unlockedSoul;
			else if (line == 1) loaded >> tmp;
			else if (line == 2) loaded >> tmp2;
			else if (line == 3) loaded >> stage;
			else if (line == 4) loaded >> charactor;
			else if (line == 5) loaded >> score;
			line++;
		}
	}
	loaded.close();
	
	////////////////////////////////////////////////
	// Loading Screen object //
	Texture loadingScrTex;  loadingScrTex.loadFromFile("Res/misc/loadingScreen.png"); 
	Texture tinyLoadingTex; tinyLoadingTex.loadFromFile("Res/misc/tinyLoading2.png"); 
	Sprite  loadingScr;     loadingScr.setTexture(loadingScrTex); loadingScrTex.setSmooth(true);

	RectangleShape tinyLoading;
	tinyLoading.setTexture(&tinyLoadingTex); tinyLoading.setSize(Vector2f(75, 55)); 
	tinyLoading.setOrigin(tinyLoading.getSize() / 2.f);	tinyLoading.setPosition(WINDOW_W / 2, WINDOW_H / 2 - 30);
	Clock clock;  float time = 0;
	//////////////////////////////////////////////// LOADING ////////////////////////////////////////////////////////////////
	loading.launch();
	while (window.isOpen())
	{
		time = clock.restart().asSeconds();
		if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;

		if (fileLoaded) { fileLoaded = false; break; }

		window.clear();
		window.draw(loadingScr);
		animate(&tinyLoadingTex, Vector2u(11, 1), 0, 0.06f, time);
		tinyLoading.setTextureRect(uvRect);
		window.draw(tinyLoading);
		window.display();
	}

	// create object //
	Texture   prebackTex, backgroundTex, midgroundTex, foregroundTex, staticBGTex, pbTex, bTex, mTex, fTex, stTex;
	Interface UI(&soulTex, &guiTex, &blackHpTex, &redHpTex, &counterTex, &font, Vector2u(11, 1), 0.08f);
	Player    player(&playerTexture, &playerCombo, Vector2u(0, 0), 0.1f, charactor);	player.HP = tmp; player.redHP = tmp2;

	// create FX //
	vector<Particle> attackParticles;
	for (int i = 0; i < 5; i++)
	{
		attackParticles.push_back(Particle(&atkFX, Vector2u(0, 0), 0.1f, 3));
		Particle& attackParticle = attackParticles[i];
	}
	vector<Particle> hitParticles;
	for (int i = 0; i < 8; i++)
	{
		hitParticles.push_back(Particle(&hitFX, Vector2u(4, 1), 0.07f, 1));
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

	// create Enemy //
	vector<Enemy> tinies;
	for (int i = 0; i < 10; i++)
	{
		tinies.push_back(Enemy(&tinyTex, nullptr, Vector2u(11, 5), 0.1f, 0));
		Enemy& tiny = tinies[i];
	}
	vector<Enemy> giants;
	for (int i = 0; i < 10; i++)
	{
		giants.push_back(Enemy(&giantTex, nullptr, Vector2u(13, 5), 0.12f, 1));
		Enemy& giant = giants[i];
	}
	vector<Enemy> ghosts;
	for (int i = 0; i < 10; i++)
	{
		ghosts.push_back(Enemy(&ghostTex, nullptr, Vector2u(8, 2), 0.08f, 2));
		Enemy& ghost = ghosts[i];
	}
	vector<Enemy> shamans;
	for (int i = 0; i < 10; i++)
	{
		shamans.push_back(Enemy(&shamanTex, nullptr, Vector2u(8, 4), 0.12f, 3));
		Enemy& shaman = shamans[i];
	}
	vector<Enemy> darkSpheres;
	for (int i = 0; i < 10; i++)
	{
		darkSpheres.push_back(Enemy(&darkSphereTex, nullptr, Vector2u(5, 1), 0.08f, 4));
		Enemy& darkSphere = darkSpheres[i];
	}

	// create bullet //
	vector<Weapon> pistol;
	for (int i = 0; i < 15; i++)
	{
		pistol.push_back(Weapon(&bulletTex, Vector2u(1, 1), 0.1f, 0));
		Weapon& weapons = pistol[i];
	}
	vector<Weapon> rifle;
	for (int i = 0; i < 15; i++)
	{
		rifle.push_back(Weapon(&bulletTex, Vector2u(1, 1), 0.1f, 1));
		Weapon& weapons = rifle[i];
	}
	vector<Weapon> bazooka;
	for (int i = 0; i < 5; i++)
	{
		bazooka.push_back(Weapon(&rocketTex, Vector2u(4, 1), 0.06f, 2));
		Weapon& weapon = bazooka[i];
	}

	// create heal orb //
	vector<Enemy> orbs;
	for (int i = 0; i < 7; i++)
	{
		orbs.push_back(Enemy(&orbTex, nullptr, Vector2u(11, 1), 0.04f, 44));
		Enemy& orb = orbs[i];
	}
	
	// Score Board //
	vector<pair<int, string>> scoreboard;
	loaded.open("score.txt");
	while (!loaded.eof()) 
	{
		string tempName; int tempScore;
		loaded >> tempName >> tempScore;
		scoreboard.push_back({ tempScore,tempName });
	}
	sort(scoreboard.begin(), scoreboard.end(), greater<pair<int, string>>());
	loaded.close();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Create object in Title screen //
	Sprite title;  title .setScale(WINDOW_W / (float)Title01.getSize().x, WINDOW_H / (float)Title01.getSize().y);
	Sprite logo;   logo  .setScale(WINDOW_W / (float)Title01.getSize().x, WINDOW_H / (float)Title01.getSize().y);
	Sprite menu;   menu  .setScale(WINDOW_W / (float)Title01.getSize().x, WINDOW_H / (float)Title01.getSize().y);
	Sprite keyBtn; keyBtn.setScale(WINDOW_W / (float)Title01.getSize().x, WINDOW_H / (float)Title01.getSize().y);
	title.setTexture(Title01); title.setColor(Color(0, 0, 0, 0));
	logo .setTexture(logoTex); logo .setColor(Color(0, 0, 0, 0));
	keyBtn.setTexture(pressSpace); keyBtn.setOrigin(pressSpace.getSize().x / 2, pressSpace.getSize().y / 2);
	keyBtn.setPosition(WINDOW_W / 2, WINDOW_H / 2 + 120);
	RectangleShape charSel, flash;
	flash.setSize (Vector2f(CAM_W, CAM_H)); flash.setOrigin(flash.getSize() / 2.f); flash.setPosition(WINDOW_W / 2, WINDOW_H / 2);
	charSel.setSize(Vector2f(500.f, 512.f )); charSel.setPosition(150.f, 0.f);

	// set variable //
	fadeVol = 0;
	opacity1 = 0, opacity2 = 0, opacity3 = 0, opacity4 = 0, opacity5 = 0;
	blink = false;
	
	/////////////////////////////////////////////////  Title Screen ///////////////////////////////////////////////////////////
	titleTheme.play();
	while (window.isOpen())
	{
		EventAndTime(window, clock, player, time);

		// fade sound in
		if (fadeVol != 100) titleTheme.setVolume(fadeVol++);

		// Press Space Screen 
		if (screen == 0)
		{
			// fade screen in
			if (opacity1 != 255) opacity1++;
			else if (opacity1 == 255 and opacity2 != 255) opacity2 += 3;

			// press space blink
			if (opacity1 == 255 and !blink and opacity3 > 0)  opacity3 -= 3;
			else blink = true;
			if (opacity1 == 255 and blink and opacity3 < 255) opacity3 += 3;
			else blink = false;

			title  .setColor (Color(opacity1, opacity1, opacity1, opacity1));
			logo   .setColor (Color(opacity2, opacity2, opacity2, opacity2));
			keyBtn .setColor (Color(255, 255, 255, opacity3));

			if (opacity1 == 255 and Keyboard::isKeyPressed(Keyboard::Space) and !isPressed)
			{
				selectFX.play();
				title.setColor (Color(255, 255, 255, 255));
				logo.setColor  (Color(255, 255, 255, 255));
				opacity1  = 255;
				opacity2 = 255;
				screen  = 1;
				isPressed = true;
			}
		}
		
		// Menu Screen 
		else if (screen == 1)
		{
			if (stage == 0)
			{
				if      (selectedMenu == 0) menu.setTexture(menu1Tex);
				else if (selectedMenu == 1) menu.setTexture(menu2Tex);
				else if (selectedMenu == 2)	menu.setTexture(menu3Tex);

				if (Keyboard::isKeyPressed(Keyboard::Up) and selectedMenu > 0 and !isPressed)
				{

					isPressed = true;
					selectedMenu--;
					selectFX.play();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) and selectedMenu < 2 and !isPressed)
				{
					isPressed = true;
					selectedMenu++;
					selectFX.play();
				}
			}
			else
			{
				if      (selectedMenu == 0) menu.setTexture(menu1Tex2);
				else if (selectedMenu == 1) menu.setTexture(menu2Tex2);
				else if (selectedMenu == 2)	menu.setTexture(menu3Tex2);
				else if (selectedMenu == 3)	menu.setTexture(menu4Tex2);

				if (Keyboard::isKeyPressed(Keyboard::Up) and selectedMenu > 0 and !isPressed)
				{
					isPressed = true;
					selectedMenu--;
					selectFX.play();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) and selectedMenu < 3 and !isPressed)
				{
					isPressed = true;
					selectedMenu++;
					selectFX.play();
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Space) and !isPressed)
			{
				enterFX.play();
				if (stage == 0)
				{
					if (selectedMenu == 0)
					{
						screen = 2;
						title.setTexture(Title11);
					}
					else if (selectedMenu == 1)
					{
						screen = 50;
						opacity5 = 0;
						title.setTexture(scoreBG);
					}
					else if (selectedMenu == 2)
					{
						window.close();
						exit(0);
					}
				}
				else
				{
					if (selectedMenu == 0)
					{
						screen = 2;
						title.setTexture(Title11);
					}
					else if (selectedMenu == 1)
					{
						screen = 4;
						player.loadFX(charactor);
						if (charactor == 0)
						{
							soulTex.loadFromFile("Res/gui/blue.png");
							playerHpTex.loadFromFile("Res/gui/blueHP.png");
							playerTexture.loadFromFile("Res/player/blue.png");
							playerCombo.loadFromFile("Res/player/blueATK.png");
							atkFX.loadFromFile("Res/particles/blueEffect.png");
							pokeSoundFX.loadFromFile("Res/sound/blueHit.wav");
							orbTex.loadFromFile("Res/gui/blue.png");
							hitFX.loadFromFile("Res/particles/hit.png");
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
							playerHpTex.loadFromFile("Res/gui/pinkHP.png");
							playerTexture.loadFromFile("Res/player/pink.png");
							playerCombo.loadFromFile("Res/player/pinkATK.png");
							atkFX.loadFromFile("Res/particles/pinkEffect.png");
							pokeSoundFX.loadFromFile("Res/sound/pinkHit.ogg");
							orbTex.loadFromFile("Res/gui/pink.png");
							hitFX.loadFromFile("Res/particles/hit2.png");
							for (int i = 0; i < attackParticles.size(); i++)
							{
								attackParticles[i].animation.imageCount = Vector2u(8, 3);
								attackParticles[i].animation.switchTime = 0.1;
								attackParticles[i].effect.setSize(Vector2f(280.f, 250.f));
								attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
							}
							player.animation.imageCount = Vector2u(10, 7);
						}
						else if (charactor == 2)
						{
							soulTex.loadFromFile("Res/gui/yellow.png");
							playerHpTex.loadFromFile("Res/gui/yellowHP.png");
							playerTexture.loadFromFile("Res/player/yellow.png");
							playerCombo.loadFromFile("Res/player/yellowATK.png");
							atkFX.loadFromFile("Res/particles/yellowEffect.png");
							pokeSoundFX.loadFromFile("Res/sound/pinkHit.ogg");
							orbTex.loadFromFile("Res/gui/yellow.png");
							hitFX.loadFromFile("Res/particles/hit3.png");
							for (int i = 0; i < attackParticles.size(); i++)
							{
								attackParticles[i].animation.imageCount = Vector2u(10, 3);
								attackParticles[i].animation.switchTime = 0.06f;
								attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
								attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
							}
							player.animation.imageCount = Vector2u(12, 7);
						}
						else if (charactor == 3)
						{
							soulTex.loadFromFile("Res/gui/black.png");
							playerHpTex.loadFromFile("Res/gui/blackHP.png");
							playerTexture.loadFromFile("Res/player/black.png");
							playerCombo.loadFromFile("Res/player/blackATK.png");
							atkFX.loadFromFile("Res/particles/yellowEffect.png");
							pokeSoundFX.loadFromFile("Res/sound/pinkHit.ogg");
							orbTex.loadFromFile("Res/gui/black.png");
							hitFX.loadFromFile("Res/particles/hit3.png");
							for (int i = 0; i < attackParticles.size(); i++)
							{
								attackParticles[i].animation.imageCount = Vector2u(14, 3);
								attackParticles[i].animation.switchTime = 0.06f;
								attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
								attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
							}
							player.animation.imageCount = Vector2u(13, 7);
						}
						UI.playerHP.setTexture(&playerHpTex);
						title.setTexture(Title11);
					}
					else if (selectedMenu == 2) 
					{
						screen = 50;
						opacity5 = 0;
						title.setTexture(scoreBG);
					}
					else if (selectedMenu == 3)
					{
						window.close();
						exit(0);
					}
				}
				
			}
		}
		else if (screen == 2)
		{
			if      (opacity1 != 0)	                  opacity1 -= 3;
			else if (opacity1 == 0 and opacity2 != 0) opacity2 -= 3;
			else if (opacity1 == 0 and opacity2 == 0) screen    = 3;

			flash.setFillColor (Color(255, 255, 255, opacity1));
			title.setColor (Color(opacity2, opacity2, opacity2, opacity2));
		}

		// Charactor Selection 
		else if (screen == 3)
		{
			if      (charactor == 0) charSel.setTexture(&blueSel);
			else if (charactor == 1) charSel.setTexture(&pinkSel);
			else if (charactor == 2) charSel.setTexture(&yellowSel);
			else if (charactor == 3) charSel.setTexture(&blackSel);
				
			if (unlockedSoul)
			{
				if (Keyboard::isKeyPressed(Keyboard::Up) and charactor != 0 and !isPressed)
				{
					charactor--;
					isPressed = true;
					selectFX.play();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) and charactor != 3 and !isPressed)
				{
					charactor++;
					isPressed = true;
					selectFX.play();
				}
			}
			else
			{
				if (Keyboard::isKeyPressed(Keyboard::Up) and charactor != 0 and !isPressed)
				{
					charactor--;
					isPressed = true;
					selectFX.play();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) and charactor != 2 and !isPressed)
				{
					charactor++;
					isPressed = true;
					selectFX.play();
				}
			}
			
			if (Keyboard::isKeyPressed(Keyboard::Escape) and !isPressed)
			{
				isPressed = true;
				screen = 1;
				charactor = 0;
				opacity1 = 255;
				opacity2 = 255;
				title.setTexture(Title01);
				title.setColor(Color(255, 255, 255, 255));
				cancleFX.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Space) and !isPressed)
			{
				enterFX.play();
				player.charactor = charactor;
				player.loadFX(charactor);
				opacity1 = 255;
				opacity2 = 255;
				screen   = 4;
				stage = 0;

				if (charactor == 0)
				{
					soulTex      .loadFromFile("Res/gui/blue.png");           
					playerHpTex  .loadFromFile("Res/gui/blueHP.png");     
					playerTexture.loadFromFile("Res/player/blue.png");  
					playerCombo  .loadFromFile("Res/player/blueATK.png"); 
					atkFX        .loadFromFile("Res/particles/blueEffect.png"); 
					pokeSoundFX  .loadFromFile("Res/sound/blueHit.wav");
					orbTex       .loadFromFile("Res/gui/blue.png");
					hitFX        .loadFromFile("Res/particles/hit.png");
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
					soulTex      .loadFromFile("Res/gui/pink.png");
					playerHpTex  .loadFromFile("Res/gui/pinkHP.png");
					playerTexture.loadFromFile("Res/player/pink.png");
					playerCombo  .loadFromFile("Res/player/pinkATK.png");
					atkFX        .loadFromFile("Res/particles/pinkEffect.png");
					pokeSoundFX  .loadFromFile("Res/sound/pinkHit.ogg");
					orbTex       .loadFromFile("Res/gui/pink.png");
					hitFX        .loadFromFile("Res/particles/hit2.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(8, 3);
						attackParticles[i].animation.switchTime = 0.1;
						attackParticles[i].effect.setSize(Vector2f(280.f, 250.f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
					player.animation.imageCount = Vector2u(10, 7);
				}
				else if (charactor == 2)
				{
					soulTex      .loadFromFile("Res/gui/yellow.png");
					playerHpTex  .loadFromFile("Res/gui/yellowHP.png");
					playerTexture.loadFromFile("Res/player/yellow.png");
					playerCombo  .loadFromFile("Res/player/yellowATK.png");
					atkFX        .loadFromFile("Res/particles/yellowEffect.png");
					pokeSoundFX  .loadFromFile("Res/sound/pinkHit.ogg");
					orbTex       .loadFromFile("Res/gui/yellow.png");
					hitFX        .loadFromFile("Res/particles/hit3.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(10, 3);
						attackParticles[i].animation.switchTime = 0.06f;
						attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
					player.animation.imageCount = Vector2u(12, 7);
				}
				else if (charactor == 3)
				{
					soulTex      .loadFromFile("Res/gui/black.png");
					playerHpTex  .loadFromFile("Res/gui/blackHP.png");
					playerTexture.loadFromFile("Res/player/black.png");
					playerCombo  .loadFromFile("Res/player/blackATK.png");
					atkFX        .loadFromFile("Res/particles/yellowEffect.png");
					pokeSoundFX  .loadFromFile("Res/sound/pinkHit.ogg");
					orbTex       .loadFromFile("Res/gui/black.png");
					hitFX		 .loadFromFile("Res/particles/hit3.png");
					for (int i = 0; i < attackParticles.size(); i++)
					{
						attackParticles[i].animation.imageCount = Vector2u(14, 3);
						attackParticles[i].animation.switchTime = 0.06f;
						attackParticles[i].effect.setSize(Vector2f(594.f / 1.5f, 146.f / 1.5f));
						attackParticles[i].effect.setOrigin(attackParticles[i].effect.getSize() / 2.f);
					}
					player.animation.imageCount = Vector2u(13, 7);
				}
				UI.playerHP.setTexture(&playerHpTex);
				soulTex.setSmooth(true); playerHpTex.setSmooth(true); atkFX.setSmooth(true);
			}
		}

		// Show Soul 
		else if (screen == 4)
		{
			if (opacity1 != 0)
			{
				opacity1 -= 3;
				flash.setFillColor(Color(255, 255, 255, opacity1));
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

		// Sanctuary
		else if (screen == 50)
		{
			if (opacity5 != 255) opacity5 += 3;
			title.setColor(Color(opacity5, opacity5, opacity5, opacity5));
			window.draw(title);

			int cnt = 0;
			for (vector<pair<int, string>>::iterator i = scoreboard.begin(); i != scoreboard.end(); ++i) {
				++cnt;
				if (cnt > 5) break;                         
				Text a, b;                                
				a.setString(to_string(i->first));         
				a.setFont(font);                        
				a.setCharacterSize(24);                   
				a.setPosition(WINDOW_W / 2 + 120, 40 + (60 * cnt));
				window.draw(a);                           
				b.setString(i->second);            
				b.setFont(font);                        
				b.setCharacterSize(24);                   
				b.setPosition(WINDOW_W / 2 - 160, 40 + (60 * cnt));
				window.draw(b);                           
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape) and !isPressed)
			{
				isPressed = true;
				screen = 1;
				opacity1 = 255;
				opacity2 = 255;
				title.setTexture(Title01);
				title.setColor(Color(255, 255, 255, 255));
				cancleFX.play();
			}
		}

		// Draw Screen -----------------------------------
		if (screen == 0)
		{
			window.clear();
			window.draw(title);
			window.draw(logo);
			window.draw(keyBtn);
			window.display();
		}
		else if (screen == 1)
		{
			window.clear();
			window.draw(title);
			window.draw(menu);
			window.draw(logo);
			window.display();
		}
		else if (screen == 2)
		{
			window.clear();
			window.draw(title);
			window.draw(flash);
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
			UI.Update(counter, score, &soulTex, camera, player, time, true);
			UI.Draw(window);
			window.draw(flash);
			window.display();
		}
		else if (screen == 50)
		{
			window.display();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	


	 ///////////////////////////////
	// create object in Tutorial //
	RectangleShape staticBG, preback; stTex.setSmooth(true); prebackTex.setSmooth(true);
	RectangleShape background, midground, foreground; backgroundTex.setSmooth(true); midgroundTex.setSmooth(true); foregroundTex.setSmooth(true);
	staticBGTex  .loadFromFile("Res/bg/village-1.png"); staticBG  .setTexture(&staticBGTex);   staticBG  .setSize(Vector2f(1699.f * 2.f, 552.f * 2.f));
	prebackTex   .loadFromFile("Res/bg/village0.png");   preback  .setTexture(&prebackTex);    preback   .setSize(Vector2f(1699.f * 2.f, 552.f * 2.f));
	backgroundTex.loadFromFile ("Res/bg/village1.png"); background.setTexture(&backgroundTex); background.setSize(Vector2f(1699.f * 2.f, 552.f * 2.f));
	midgroundTex .loadFromFile ("Res/bg/village2.png"); midground .setTexture(&midgroundTex);  midground .setSize(Vector2f(1699.f * 2.f, 552.f * 2.f));
	foregroundTex.loadFromFile ("Res/bg/village3.png"); foreground.setTexture(&foregroundTex); foreground.setSize(Vector2f(6048.f * 1.f, 552.f * 2.f));
	midground.setPosition(0, 100); foreground.setPosition(-20.f, 100.f);
	bTex.loadFromFile("Res/bg/village11.png"); 	mTex.loadFromFile("Res/bg/village22.png"); 	pbTex.loadFromFile("Res/bg/village00.png"); stTex.loadFromFile("Res/bg/village-11.png");

	// platform
	Ground ground (Vector2f(5000.f,  20.f), Vector2f(1550.f, 1050.f));
	Ground wall_L (Vector2f(  20.f, 240.f), Vector2f(   5.f,  400.f));
	Ground wall_R (Vector2f(  20.f, 240.f), Vector2f(3400.f,  900.f));
	// obstruct
	Ground cart   (Vector2f(150.f,  100.f), Vector2f(1510.f, 960.f));
	Ground fire     (Vector2f( 40.f, 200.f), Vector2f(2210.f,  900.f));
	// overlay screen
	Texture splashTex, flameTex; splashTex.loadFromFile("Res/bg/01TEXT.png"); splashTex.setSmooth(true); flameTex.loadFromFile("Res/misc/fire.png"); flameTex.setSmooth(true);
	RectangleShape wideScreen, Splash, flame, flame2, flame3, flame4, flame5;
	flame     .setSize(Vector2f(252/2, 144/2)); flame.setOrigin(flame.getSize() / 2.f); flame.setTexture(&flameTex); 
	flame2    .setSize(Vector2f(252 / 3, 144 / 2)); flame2.setOrigin(flame2.getSize() / 2.f); flame2.setTexture(&flameTex);
	flame3    .setSize(Vector2f(252 / 4, 144)); flame3.setOrigin(flame3.getSize() / 2.f); flame3.setTexture(&flameTex);
	flame4    .setSize(Vector2f(252 / 3, 144*2)); flame4.setOrigin(flame4.getSize() / 2.f); flame4.setTexture(&flameTex);
	flame5    .setSize(Vector2f(252 *2, 144*2)); flame5.setOrigin(flame5.getSize() / 2.f); flame5.setTexture(&flameTex);
	flame     .setPosition(840, 800); flame2.setPosition(1510,1020); flame3.setPosition(1390,520); flame4.setPosition(2210,920); flame5.setPosition(2000,1100);
	Splash    .setSize(Vector2f(WINDOW_W, WINDOW_H)); Splash.setTexture(&splashTex); Splash.setOrigin(Splash.getSize() / 2.f);
	wideScreen.setSize(Vector2f(WINDOW_W + 120.f, WINDOW_H + 100.f)); wideScreen.setTexture(&wideScreenTex);
	wideScreen.setOrigin(wideScreen.getSize() / 2.f);
	// tutorial button
	Particle movementKey(&moveKey, Vector2u(4, 1), 0.5,  0);
	Particle actionKey  (&actKey , Vector2u(2, 4), 0.5, -1);
	Ground moveZone(Vector2f(600.f, 200.f), Vector2f(700.f, 900.f));
	Ground jumpZone(Vector2f(300.f, 300.f), Vector2f(1480.f, 900.f));
	Ground dashZone(Vector2f(200.f, 200.f), Vector2f(2115.f, 900.f));
	Ground endZone (Vector2f(300.f, 300.f), Vector2f(3170.f, 900.f));
	// portal
	Enemy portal(&portalTex, nullptr, Vector2u(9, 3), 0.1f, 55);
	portal.SetPosition(3000.f, 910.f);
	portal.SetColor(0, 0, 0, 0);
	portal.faceRight = false;

	// set object
	player.SetColor(235, 200, 130, 255);
	blink = 0;
	opacity1 = 0;
	opacity2 = 255;
	opacity3 = 255;
	opacity4 = 0;
	opacity5 = 0;
	cutSceneState = 0;
	counter = 1;

	if (stage == 0)
	{
		camera.setCenter(460.f, 800.f);
		player.SetPosition(-50.f, 1000.f);
		tinies[0].state = 98;
		tinies[0].SetPosition(2700.f, 900.f);
	}

	unsigned int size = CAM_H;


	////////////////////////////////////////////////  Tutorial ///////////////////////////////////////////////////////////////////
	firstTheme.play();
	while (window.isOpen() and stage == 0)
	{
		EventAndTime(window, clock, player, time);
		tinies[0].SetColor(235, 200, 130, 255);

		cameraSetting(window, camera, player, preback, background, foreground, player.GetPosition(), time);
		wideScreen.setPosition(camera.getCenter());
		Splash.setPosition(camera.getCenter());

		// Text splash
		if (opacity1 < 255 and !blink)
		{
			opacity1 += 1;
			Splash.setFillColor(Color(255, 255, 255, opacity1));
		}
		else blink = true;
		if (opacity1 > 0 and blink)
		{
			opacity1 -= 1;
			Splash.setFillColor(Color(255, 255, 255, opacity1));
		}

		// cut scene
			// start
			if (player.cutScene and player.GetPosition().x < 600.f) player.velocity.x += 300.f;
			else if (player.cutScene and player.GetPosition().x >= 600.f and cutSceneState == 0)
			{
				if (CutScene(false, wideScreen, size))
				{
					wall_L.SetPosition(5.f, 900.f);
					player.cutScene = false;
					cutSceneState = 1;
				}
			}
			// found tiny
			else if (!player.cutScene and player.GetPosition().x >= 2350.f and tinies[0].state == 98 and cutSceneState == 1)
			{
				player.cutScene = true;
				tinies[0].state = 1;
				cutSceneState = 2;
			}
			else if (player.cutScene and tinies[0].state == 1)
			{
				player.SetColor(235, 200, 130, 255);
				CutScene(true, wideScreen, size);
				camera.move((tinies[0].GetPosition().x - camera.getCenter().x) * time * 2.5f, 0.f);
			}
			// get hit
			if (player.cutScene and player.getHit and cutSceneState == 2)
			{
				tutorial = 4;
				showUI = true;
				cutSceneState = 3;
			}
			else if (cutSceneState == 3)
			{
				if (CutScene(false, wideScreen, size))
				{
					player.cutScene = false;
					cutSceneState = 4;
				}
			}
			// portal open
			if (counter == 0 and opacity5 != 255)
			{
				portal.SetColor(235, 200, 130, opacity5);
				opacity5 += 3;
				tutorial = 0;
			}
			// warp to next map
			if (endZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and cutSceneState == 4 and counter == 0)
			{
				warpFX.play();
				cutSceneState = 5;
			}
			if (cutSceneState == 5 and opacity2 != 0)
			{
				CutScene(true, wideScreen, size);

				player.cutScene = true;
				player.SetColor(235, 200, 130, opacity2);
				opacity2 -= 3;
			}
			else if (cutSceneState == 5 and opacity3 != 0)
			{
				portal.sprite.setFillColor(Color(235,200,130, opacity3));
				opacity3 -= 3;
			}
			else if (cutSceneState == 5 and opacity4 != 255)
			{
				flash.setPosition(camera.getCenter());
				flash.setFillColor(Color(0, 0, 0, opacity4));
				opacity4+=3;
			}
			else if (cutSceneState == 5)
			{
				screen = 6;
				player.cutScene = false;
				tinies[0].SetColor(255, 255, 255, 255);

				firstTheme.stop();
				stage = 1;
				break;
			}
			
		// show tutorial button
		if (cutSceneState == 0 or cutSceneState == 1)
		{
			
			// Show Arrow button
			if (moveZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0) tutorial = 1;
			
			// Show Jump button
			else if (jumpZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0)
			{
				tutorial = 2;
				actionKey.row = 3;
			}
			
			// Show X button
			else if (dashZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and tutorial == 0)
			{
				tutorial = 3;
				actionKey.row = 1;
			}
			
			if (!moveZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !jumpZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !dashZone.GetCollider().CheckCollide(player.GetHitBox(), direction, 0.f, false))
				tutorial = 0;
		}
		else
		{
			// Show Z button
			if (tutorial == 4) actionKey.row = 0;
		}
		
		// Check collide
		checkPlayerOnGround(player, ground, wall_L, wall_R);
		if (cart.GetCollider().CheckCollide(player.GetHitBox()   , direction, 1.f, true)) player.OnCollision(direction);
		if (fire.GetCollider().CheckCollide(player.GetHitBox()     , direction, 0.0f, false) and !player.Dash and !player.STOP and !player.getHit)
		{
			getHitFX.play();
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
		}

		// Tiny Attack!!!
		Tiny(tinies, orbs, hitParticles, player, ground);
		// Fix HP and SCORE
		player.HP = 100; player.redHP = 100;
		score = 0;
		// createFX
		createFX(dustParticles, dustParticles2, attackParticles, player);
		endFX(hitParticles, dustParticles, dustParticles2, attackParticles, player);
		// Homura's weapon
		if (player.fire == 1)
		{
			for (int i = 0; i < pistol.size(); i++)
			{
				if (pistol[i].state == 99)
				{
					pistol[i].state = 1;
					pistol[i].wait.restart().asSeconds();
					pistol[i].faceRight = player.faceRight;
					if (player.faceRight) pistol[i].SetPosition(player.GetPosition().x + 50, player.GetPosition().y - 15);
					else pistol[i].SetPosition(player.GetPosition().x -50, player.GetPosition().y -15);

					pistol[i].Update(time, 0);
					player.fire = 0;
					break;
				}
			}
		}
		// bullet damage
		for (int i = 0; i < pistol.size(); i++)
		{
			for (int j = 0; j < tinies.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(tinies[j].GetHitBox(), direction, 0.f, false) and tinies[j].state != 7 and tinies[j].state != 98 and tinies[j].state != 100 and tinies[j].state != 101)
				{
					tinies[j].HP -= player.damage;
					tinies[j].state = 7;
					tinies[j].animation.playingAnimation = true;
					tinies[j].animation.OneTimeAnimation = true;
					tinies[j].animation.OneTimeFrame.x = 5;
					tinies[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(tinies[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < giants.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(giants[j].GetHitBox(), direction, 0.f, false) and giants[j].state != 7 and giants[j].state != 100 and giants[j].state != 101)
				{
					giants[j].HP -= player.damage;
					giants[j].state = 7;
					giants[j].animation.playingAnimation = true;
					giants[j].animation.OneTimeAnimation = true;
					giants[j].animation.OneTimeFrame.x = 5;
					giants[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(giants[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < ghosts.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(ghosts[j].GetHitBox(), direction, 0.f, false) and ghosts[j].state != 7 and ghosts[j].state != 100 and ghosts[j].state != 101)
				{
					ghosts[j].HP -= player.damage;
					ghosts[j].state = 7;
					ghosts[j].animation.playingAnimation = true;
					ghosts[j].animation.OneTimeAnimation = true;
					ghosts[j].animation.OneTimeFrame.x = 5;
					ghosts[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(ghosts[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < shamans.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(shamans[j].GetHitBox(), direction, 0.f, false) and shamans[j].state != 7 and shamans[j].state != 100 and shamans[j].state != 101)
				{
					shamans[j].HP -= player.damage;
					shamans[j].state = 7;
					shamans[j].animation.playingAnimation = true;
					shamans[j].animation.OneTimeAnimation = true;
					shamans[j].animation.OneTimeFrame.x = 5;
					shamans[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(shamans[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
		}

		// Draw Every things
		window.clear();
		window.draw(staticBG);
		window.draw(preback);
		window.draw(background);
		window.draw(midground);
		if (player.STOP)
		{
			continueSound = true;
			firstTheme.pause();
			staticBG.setTexture(&stTex);
			preback.setTexture(&pbTex);
			background.setTexture(&bTex);
			midground.setTexture(&mTex);
		}
		else
		{
			if (continueSound)
			{
				continueSound = false;
				firstTheme.play();
			}
			staticBG.setTexture(&staticBGTex);
			preback.setTexture(&prebackTex);
			background.setTexture(&backgroundTex);
			midground.setTexture(&midgroundTex);

			portal.Update(time, player, 55);
			tinies[0].Update(time, player, 0);
			updateFX(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX, player, time);
			animate(&flameTex, Vector2u(13, 1), 0, 0.06f, time);
		}
		if (counter == 0)
			portal.Draw(window);
		window.draw(flame);
		player.Update(stage, time, charactor);
		player.Draw(window);
		tinies[0].Draw(window);
		for (int i = 0; i < pistol.size(); i++)
		{
			if (!player.STOP)
			pistol[i].Update(time, 0);
			pistol[i].Draw(window);
		}
		drawFX(hitParticles, dustParticles, dustParticles2, attackParticles, window);
		showTutorial(window, movementKey, actionKey, moveKey, actKey, player, moveZone, jumpZone, dashZone, time);
		window.draw(flame2);
		window.draw(flame3);
		window.draw(flame4);
		window.draw(flame5);
		window.draw(foreground);
		UI.Update(counter, score, &soulTex, camera, player, time, false);
		if (showUI and !player.cutScene)
			UI.Draw(window);
		if (player.cutScene)
			window.draw(wideScreen);
		window.draw(flash);
		window.draw(Splash);
		flame.setTextureRect(uvRect);
		flame2.setTextureRect(uvRect);
		flame3.setTextureRect(uvRect);
		flame4.setTextureRect(uvRect);
		flame5.setTextureRect(uvRect);
		window.display();
	}

	////////////  Save Game Data ////////////////////
	saved.open("saved.txt");
	if (saved.is_open())
	{
		int line = 0;
		while (line < 6)
		{
			if (line == 0) saved << unlockedSoul << endl;
			else if (line == 1) saved << player.HP << endl;
			else if (line == 2) saved << (int)player.redHP << endl;
			else if (line == 3) saved << stage << endl;
			else if (line == 4) saved << charactor << endl;
			else if (line == 5) saved << score;
			line++;
		}
	}
	saved.close();
	firstTheme.stop();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 ////////////////////////////
	// create object in woods //
	splashTex    .loadFromFile("Res/bg/02TEXT.png");
	backgroundTex.loadFromFile("Res/bg/forest1.png"); background.setSize(Vector2f(4422.f, 1104.f));
	midgroundTex .loadFromFile("Res/bg/forest2.png"); midground .setSize(Vector2f(4341.6f, 1104.f));
	foregroundTex.loadFromFile("Res/bg/forest3.png"); foreground.setSize(Vector2f(8114.4f, 1104.f));
	bTex.loadFromFile("Res/bg/forest11.png"); 	mTex.loadFromFile("Res/bg/forest22.png"); 	fTex.loadFromFile("Res/bg/forest33.png");
	Texture moonTexture, moTex; moonTexture.loadFromFile("Res/bg/forest0.png"); moTex.loadFromFile("Res/bg/forest00.png");
	RectangleShape moon; moon.setTexture(&moonTexture); moon.setSize(Vector2f(2278.f, 1104.f)); moon.setPosition(-1000.f, 120.f);

	//set object
	camera.setCenter  (600.f, 600.f);
	player.SetPosition(600.f, 600.f); player.canJump = false; player.falling = true; player.cutScene = true;
	portal.SetPosition(600.f, 600.f); portal.SetColor(0, 0, 0, 0); portal.row = 1;
	ground.SetPosition(1700.f, 1050.f);
	wall_L.SetPosition(5.f, 900.f);
	wall_R.SetPosition(4000.f,900.f);
	
	blink = false;
	opacity1 = 0;
	opacity2 = 255;
	opacity3 = 0;
	opacity4 = 255;
	cutSceneState = 0;
	counter = 25;
	screen = 6;
	Clock spawnDelay;
	int   spawn = 0, spawnCount = 0;

	//////////////////////////////////////////////////// Woods /////////////////////////////////////////////////////////////////
	forestTheme.play();
	while (window.isOpen() and stage == 1)
	{
		//break;
		EventAndTime(window, clock, player, time);
		cameraSetting(window, camera, player, moon, background, foreground, player.GetPosition(), time);

		flash     .setPosition(camera.getCenter());
		Splash    .setPosition(camera.getCenter());
		wideScreen.setPosition(camera.getCenter());


		// Text splash
		if (opacity1 < 255 and !blink)
		{
			opacity1 += 1;
			Splash.setFillColor(Color(255, 255, 255, opacity1));
		}
		else blink = true;
		if (opacity1 > 0 and blink)
		{
			opacity1 -= 1;
			Splash.setFillColor(Color(255, 255, 255, opacity1));
		}
		
		// Fade up Screen
		if (opacity2 > 0 and cutSceneState != 67) 
		{
			opacity2--;
			flash.setFillColor(Color(0, 0, 0, opacity2));
		}

		// cut scene
			// fall from portal
			if (cutSceneState == 0)
			{
				portal.SetColor(opacity3, opacity3, opacity3, opacity3);
				player.SetColor(opacity3, opacity3, opacity3, opacity3);
				player.SetPosition(600.f, 600.f);
				player.velocity.y = 0;
				if (opacity3 != 255) opacity3 += 1;
				else if (opacity3 == 255 and cutSceneState == 0)
				{
					player.SetPosition(600.f, 600.f);
					player.canJump = false;
					player.falling = true;
					player.velocity.y = 0;
					opacity3 = 0;
					cutSceneState = 1;
					size = CAM_H;
				}
			}
			else if (cutSceneState == 1)
			{
				if (CutScene(false, wideScreen, size))
				{
					showUI = true;
					player.cutScene = false;
					camera.setCenter(500.f, 800.f);
					screen = 7;
					opacity3 = 255;
					cutSceneState = 2;
				}
			}
			if (screen == 7 and cutSceneState == 2)
			{
				portal.SetColor(opacity3, opacity3, opacity3, opacity3);
				if (opacity3 != 0) opacity3 -= 3;
				else cutSceneState = 3;
			}
			// warp to boss fight
			if (spawn == 4 and counter == 1 and !player.cutScene)
			{
				warpFX.play();
				player.cutScene = true;
			}
			if (player.cutScene and cutSceneState == 3)
			{
				if (CutScene(true, wideScreen, size))
					cutSceneState = 4;
			}
			else if (player.cutScene and cutSceneState == 4)
			{
				if (player.GetPosition().x < 1550)
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
				player.SetColor(opacity4, opacity4, opacity4, 255);
				moon.setFillColor(Color(255, 255, 255, opacity4));
				background.setFillColor(Color(opacity4, opacity4, opacity4, opacity4));
				midground.setFillColor(Color(opacity4, opacity4, opacity4, opacity4));
				if (opacity4 != 0) opacity4--;
				else
				{
					wait.restart().asSeconds();
					cutSceneState = 5;
				}
			}
			else if (player.cutScene and cutSceneState == 5)
			{
				player.SetColor(0, 0, 0, 255);
				moon.setFillColor(Color(255, 255, 255, 0));
				background.setFillColor(Color(0, 0, 0, 0));
				if (wait.getElapsedTime().asSeconds() < 1)
				{
					if (player.GetPosition().x < 1550)
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
					stage = 2;
					break;
				}
			}
			// player Dead X.X
			if (player.Dead and cutSceneState != 66 and cutSceneState != 67)
			{
				forestTheme.stop();
				getHitFX.stop();
				deadFX.play();
				player.cutScene = true;
				wait.restart().asSeconds();
				cutSceneState = 66;
				player.SetColor(0, 0, 0, 255);
				for (int i = 0; i < tinies     .size(); i++) tinies[i]     .SetColor(0, 0, 0, 255);
				for (int i = 0; i < giants     .size(); i++) giants[i]     .SetColor(0, 0, 0, 255);
				for (int i = 0; i < ghosts     .size(); i++) ghosts[i]     .SetColor(0, 0, 0, 255);
				for (int i = 0; i < shamans    .size(); i++) shamans[i]    .SetColor(0, 0, 0, 255);
				for (int i = 0; i < darkSpheres.size(); i++) darkSpheres[i].SetColor(0, 0, 0, 255);
			}
			else if (cutSceneState == 66 and wait.getElapsedTime().asSeconds() > 2)
			{
				cutSceneState = 67;
				player.SetColor(255, 255, 255, 255);
				for (int i = 0; i < tinies     .size(); i++) tinies[i]     .SetColor(255, 255, 255, 255);
				for (int i = 0; i < giants     .size(); i++) giants[i]     .SetColor(255, 255, 255, 255);
				for (int i = 0; i < ghosts     .size(); i++) ghosts[i]     .SetColor(255, 255, 255, 255);
				for (int i = 0; i < shamans    .size(); i++) shamans[i]    .SetColor(255, 255, 255, 255);
				for (int i = 0; i < darkSpheres.size(); i++) darkSpheres[i].SetColor(255, 255, 255, 255);
			}
			else if (cutSceneState == 67)
			{
				flash.setFillColor(Color(0, 0, 0, opacity2));
				if (opacity2 != 255) opacity2++;
				else break;
			}

		// Spawn monster
			// tiny only
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
							if (player.GetPosition().x < 2500.f) 
								 tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
							else tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
		
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds(); spawnCount++;
						}
						else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
						{
							if (player.GetPosition().x > 1000.f)
								 tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
							else tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
					
							tinies[spawnCount].state = 1;
							spawnDelay.restart().asSeconds(); spawnCount++;
						}
					}
				}
			}
			// tiny + ghost
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
								if (player.GetPosition().x < 2500.f)
									 ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
							
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
							
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else
						{
							if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
					
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
							
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
					}
				}
			}
			// tiny + ghost + shaman
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
								if (player.GetPosition().x < 2500.f)
							 		 ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
							
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
							
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else if (spawnCount == 1 or spawnCount == 4 )
						{
							if (spawnCount % 2 == 0 and shamans[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
									 shamans[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else shamans[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
							
								shamans[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and shamans[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 shamans[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
								else shamans[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								
								shamans[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else
						{
							if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
							
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
						
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
					}
				}
			}
			// tiny + ghost + shaman + giant
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
								if (player.GetPosition().x < 2500.f)
									 ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
							
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and ghosts[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 ghosts[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else ghosts[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
				
								ghosts[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else if (spawnCount == 1)
						{
							if (spawnCount % 2 == 0 and shamans[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
									 shamans[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else shamans[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
						
								shamans[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and shamans[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 shamans[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
								else shamans[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
							
								shamans[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else if (spawnCount == 5)
						{
							if (spawnCount % 2 == 0 and giants[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
								 	 giants[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
								else giants[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
							
								giants[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and giants[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 giants[spawnCount].SetPosition(player.GetPosition().x - 600.f, 800.f);
								else giants[spawnCount].SetPosition(player.GetPosition().x + 600.f, 800.f);
							
								giants[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
						else
						{
							if (spawnCount % 2 == 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x < 2500.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
							
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
							else if (spawnCount % 2 != 0 and tinies[spawnCount].state == 98)
							{
								if (player.GetPosition().x > 1000.f)
									 tinies[spawnCount].SetPosition(player.GetPosition().x - 600.f, 600.f);
								else tinies[spawnCount].SetPosition(player.GetPosition().x + 600.f, 600.f);
							
								tinies[spawnCount].state = 1;
								spawnDelay.restart().asSeconds(); spawnCount++;
							}
						}
					}
				}
			}


		// Monster Attack!!!
		Tiny      (tinies, orbs, hitParticles, player, ground);
		Giant     (giants, orbs, hitParticles, player, ground);
		Ghost     (ghosts, orbs, hitParticles, player, ground);
		Shaman    (shamans, darkSpheres, orbs, hitParticles, player, ground);
		DarkSphere(darkSpheres, player);

		// Homura's weapon
		if (player.fire == 1)
		{
			for (int i = 0; i < pistol.size(); i++)
			{
				if (pistol[i].state == 99)
				{
					pistol[i].state = 1;
					pistol[i].wait.restart().asSeconds();
					pistol[i].faceRight = player.faceRight;
					if (player.faceRight) pistol[i].SetPosition(player.GetPosition().x + 50, player.GetPosition().y - 15);
					else pistol[i].SetPosition(player.GetPosition().x - 50, player.GetPosition().y - 15);

					pistol[i].Update(time, 0);
					player.fire = 0;
					break;
				}
			}
		}
		// bullet damage
		for (int i = 0; i < pistol.size(); i++)
		{
			for (int j = 0; j < tinies.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(tinies[j].GetHitBox(), direction, 0.f, false) and tinies[j].state != 7 and tinies[j].state != 98 and tinies[j].state != 100 and tinies[j].state != 101)
				{
					tinies[j].HP -= player.damage;
					tinies[j].state = 7;
					tinies[j].animation.playingAnimation = true;
					tinies[j].animation.OneTimeAnimation = true;
					tinies[j].animation.OneTimeFrame.x = 5;
					tinies[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(tinies[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < giants.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(giants[j].GetHitBox(), direction, 0.f, false) and giants[j].state != 7 and giants[j].state != 100 and giants[j].state != 101)
				{
					giants[j].HP -= player.damage;
					giants[j].state = 7;
					giants[j].animation.playingAnimation = true;
					giants[j].animation.OneTimeAnimation = true;
					giants[j].animation.OneTimeFrame.x = 5;
					giants[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(giants[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < ghosts.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(ghosts[j].GetHitBox(), direction, 0.f, false) and ghosts[j].state != 7 and ghosts[j].state != 100 and ghosts[j].state != 101)
				{
					ghosts[j].HP -= player.damage;
					ghosts[j].state = 7;
					ghosts[j].animation.playingAnimation = true;
					ghosts[j].animation.OneTimeAnimation = true;
					ghosts[j].animation.OneTimeFrame.x = 5;
					ghosts[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(ghosts[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
			for (int j = 0; j < shamans.size(); j++)
			{
				if (pistol[i].GetCollider().CheckCollide(shamans[j].GetHitBox(), direction, 0.f, false) and shamans[j].state != 7 and shamans[j].state != 100 and shamans[j].state != 101)
				{
					shamans[j].HP -= player.damage;
					shamans[j].state = 7;
					shamans[j].animation.playingAnimation = true;
					shamans[j].animation.OneTimeAnimation = true;
					shamans[j].animation.OneTimeFrame.x = 5;
					shamans[j].animation.currentImage.x = 0;

					for (unsigned int k = 0; k < hitParticles.size(); k++)
					{
						if (!hitParticles[k].isEffect)
						{
							pokeFX.play();
							hitParticles[k].isEffect = true;
							hitParticles[k].effect.setPosition(shamans[j].GetPosition());
							hitParticles[k].animation.playingAnimation = true;
							hitParticles[k].animation.OneTimeAnimation = true;
							hitParticles[k].animation.OneTimeFrame.x = 5;
							hitParticles[k].animation.currentImage.x = 0;
							break;
						}
					}
				}
			}
		}

		// Check collide
		checkPlayerOnGround(player, ground, wall_L, wall_R);
		for (int i = 0; i < orbs.size(); i++)
		{
			if (orbs[i].GetHitBox().CheckCollide(player.GetHitBox(), direction, 0.f, false) and orbs[i].state == 1)
			{
				pickFX.play();
				orbs[i].state = 98;
				player.HP += 10;
				player.redHP += 10;
			}
		}

		// createFX
		createFX(dustParticles, dustParticles2, attackParticles, player);
		endFX(hitParticles, dustParticles, dustParticles2, attackParticles, player);


		// Draw Every things
		window.clear(Color::White);
		if (cutSceneState != 66)
		{
			window.draw(moon);
			window.draw(background);
			window.draw(midground);
		}
		if (player.STOP)
		{
			continueSound = true;
			firstTheme.pause();
			moon.setTexture(&moTex);
			background.setTexture(&bTex);
			midground.setTexture(&mTex);
			foreground.setTexture(&fTex);
		}
		else
		{
			if (continueSound)
			{
				continueSound = false;
				firstTheme.play();
			}
			moon.setTexture(&moonTexture);
			background.setTexture(&backgroundTex);
			midground.setTexture(&midgroundTex);
			foreground.setTexture(&foregroundTex);

			portal.Update(time, player, 55);
			updateFX(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX, player, time);
		}
		portal.Draw(window);
		drawGiant(giants, player, window, time);
		drawShaman(shamans, player, window, time);
		if (cutSceneState != 66)
		player.Update(stage, time, charactor);
		player.Draw(window);
		drawGhost(ghosts, player, window, time);
		drawTiny(tinies, player, window, time);
		drawDarkSphere(darkSpheres,  player, window, time);
		drawFX(hitParticles, dustParticles, dustParticles2, attackParticles, window);
		for (int i = 0; i < pistol.size(); i++)
		{
			if (!player.STOP)
				pistol[i].Update(time, 0);
			pistol[i].Draw(window);
		}
		for (int i = 0; i < orbs.size(); i++)
		{
			if (orbs[i].GetHitBox().CheckCollide(ground.GetCollider(), direction, 0.f, true))
				orbs[i].OnCollision(direction);
			if (!player.STOP)
			orbs[i].Update(time, player, 44);
			orbs[i].Draw(window);
		}
		if (cutSceneState != 66)
		window.draw(foreground);
		if (cutSceneState != 66)
		UI.Update(counter, score, &soulTex, camera, player, time, false);
		if (showUI and ! player.cutScene)
			UI.Draw(window);
		if (player.cutScene)
			window.draw(wideScreen);
		window.draw(flash);
		window.draw(Splash);
		window.display();
	}

	//// summit name ////
	camera.setCenter(CAM_W / 2.f, CAM_H / 2.f);
	Sprite end;	backgroundTex.loadFromFile("Res/misc/over.png");  end.setTexture(backgroundTex);
	end.setPosition(0, 0);
	string enterName;
	Text Name; Name.setFont(font); Name.setCharacterSize(36);
	while (window.isOpen() and player.Dead and cutSceneState != 69)
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
				isPressed = false;
			if (button.type == Event::TextEntered)
			{
				if (button.text.unicode == 8) // BACK SPACE
				{
					if (enterName.size() != 0) enterName.pop_back();
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
				else if (button.text.unicode == 13 and enterName.size() != 0) // RETURN
				{
					window.clear(Color::White);
					enterFX.play();
					stage = 0;
					break;
				}
				else if (button.text.unicode < 128 and button.text.unicode != 32 and button.text.unicode != 13 and enterName.size() < 15)
				{
					enterName.push_back((char)button.text.unicode);
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
			}
		}
		window.setView(camera);
		window.clear();
		window.draw(end);
		window.draw(Name);
		window.display();
	}
	
	if (player.Dead)
	{
		stage = 0;
		screen = 0;
		////////////  Save Game Data ////////////////////
		saved.open("saved.txt");
		if (saved.is_open())
		{
			int line = 0;
			while (line < 6)
			{
				if (line == 0) saved << unlockedSoul << endl;
				else if (line == 1) saved << player.HP << endl;
				else if (line == 2) saved << (int)player.redHP << endl;
				else if (line == 3) saved << stage << endl;
				else if (line == 4) saved << charactor << endl;
				else if (line == 5) saved << score;
				line++;
			}
		}
		saved.close();

		saved.open("score.txt", ios::app);
		if (saved.is_open())
		{
			saved << "\n" << enterName << " " << score;
		}
		saved.close();
		player.Dead = false;
		window.close();
		main();
	}
	

	////////////  Save Game Data ////////////////////
	saved.open("saved.txt");
	if (saved.is_open())
	{
		int line = 0;
		while (line < 6)
		{
			if (line == 0) saved << unlockedSoul << endl;
			else if (line == 1) saved << player.HP << endl;
			else if (line == 2) saved << (int)player.redHP << endl;
			else if (line == 3) saved << stage << endl;
			else if (line == 4) saved << charactor << endl;
			else if (line == 5) saved << score;
			line++;
		}
	}
	saved.close();
	forestTheme.stop();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 ///////////////////
	// create object //
	Texture bossBGTex, bossStopTex; bossBGTex.loadFromFile("Res/bg/bridge.png"); bossStopTex.loadFromFile("Res/bg/bridgeSTOP.png");
	Sprite bossBG; bossBG.setTexture(bossBGTex); bossBG.setColor(Color(255, 255, 255, 0)); bossBG.setPosition(-50, -50);
	bossBG.setScale(CAM_W / (float)bossBGTex.getSize().x, (CAM_H / (float)bossBGTex.getSize().y)*1.01);
	ground.SetPosition(400.f, 512.f);
	wall_L.SetPosition(-40.f, 400.f);
	wall_R.SetPosition(840.f, 400.f);
	player.SetPosition(400.f, 500.f);
	wideScreen.setPosition(CAM_W / 2 - 50, CAM_H / 2 - 50);

	Enemy rocket(&rocketTex, nullptr, Vector2u(4, 1), 0.08f, 8);
	vector<Enemy> bullets;
	for (int i = 0; i < 6; i++)
	{
		bullets.push_back(Enemy(&bulletTex, nullptr, Vector2u(1, 1), 0.08f, 7));
		Enemy& bullet = bullets[i];
	}
	for (int i = 0; i < bullets.size(); i++) bullets[i].state = 1;
	rocket.state = 1;
	
	Enemy Homura(&homuraTexture, &homuraATK, Vector2u(13, 7), 0.1f, 6); Homura.faceRight = false;
	Homura.SetPosition(600.f, 500.f); Homura.SetColor(255, 255, 255, 0);

	opacity1 = 0;
	opacity2 = 0;
	opacity3 = 255;
	opacity4 = 0;
	screen = 8;
	counter = 1;
	cutSceneState = 0;

	
	/////////////////////////////////////////// Homura Fight /////////////////////////////////////////////////////////
	while (window.isOpen() and stage == 2)
	{
		EventAndTime(window, clock, player, time);
		cameraSetting(window, camera, player, moon, background, foreground, player.GetPosition(), time);

		//cut scene
			// fade up
			if (cutSceneState == 0)
			{
				player.SetColor(opacity1, opacity1, opacity1, 255);
				if (opacity1 != 255) opacity1++;
				else cutSceneState = 1;
			}
			else if (cutSceneState == 1)
			{
				bossBG.setColor(Color(255, 255, 255, opacity2));
				Homura.SetColor(255, 255, 255, opacity2);
				if (opacity2 != 255) opacity2++;
				else cutSceneState = 2;
			}
			else if (cutSceneState == 2)
			{
				if (CutScene(false, wideScreen, size))
				{
					showUI = true;
					player.cutScene = false;
					cutSceneState = 3;
					Homura.state = 1;
					bossTheme.play();
				}
			}
			// homura DEAD!!
			if (Homura.Dead)
			{
				if (cutSceneState == 3)
				{
					bossTheme.stop();
					player.cutScene = true;
					flash.setFillColor(Color(255, 255, 255, opacity4));
					if (opacity4 != 255) opacity4++;
					else cutSceneState = 4;
				}
				else if (cutSceneState == 4)
				{
					stage = 3;
					score += 3000;
					break;
				}
			}
			// player Dead X.X
			if (player.Dead and cutSceneState != 66 and cutSceneState != 67)
			{	
				bossTheme.stop();
				getHitFX.stop();
				deadFX.play();
				player.cutScene = true; opacity5 = 0;
				wait.restart().asSeconds();
				cutSceneState = 66;
				player.SetColor(0, 0, 0, 255);
				Homura.SetColor(0, 0, 0, 255);
				rocket.SetColor(0, 0, 0, 255);
				for (int i = 0; i < bullets.size(); i++) bullets[i].SetColor(0, 0, 0, 255);
			}
			else if (cutSceneState == 66 and wait.getElapsedTime().asSeconds() > 2)
			{
				cutSceneState = 67;
				player.SetColor(255, 255, 255, 255);
				Homura.SetColor(255, 255, 255, 255);
				rocket.SetColor(255, 255, 255, 255);
				for (int i = 0; i < bullets.size(); i++) bullets[i].SetColor(255, 255, 255, 255);
			}
			else if (cutSceneState == 67)
			{
				flash.setFillColor(Color(0, 0, 0, opacity5));
				if (opacity5 != 255) opacity5++;
				else break;
			}

		// createFX
		createFX(dustParticles, dustParticles2, attackParticles, player);
		endFX(hitParticles, dustParticles, dustParticles2, attackParticles, player);

		// Homura Fight!!
		HomuraAtk(Homura, bullets, rocket, player, hitParticles, window, time);

		// Homura's weapon (Player)
		if (player.fire == 1)
		{
			for (int i = 0; i < pistol.size(); i++)
			{
				if (pistol[i].state == 99)
				{
					pistol[i].state = 1;
					pistol[i].wait.restart().asSeconds();
					pistol[i].faceRight = player.faceRight;
					if (player.faceRight) pistol[i].SetPosition(player.GetPosition().x + 50, player.GetPosition().y - 15);
					else pistol[i].SetPosition(player.GetPosition().x - 50, player.GetPosition().y - 15);

					pistol[i].Update(time, 0);
					player.fire = 0;
					break;
				}
			}
		}
		// bullet damage
		for (int i = 0; i < pistol.size(); i++)
		{
			if (pistol[i].GetCollider().CheckCollide(Homura.GetHitBox(), direction, 0.f, false) and !Homura.invincible)
			{
				Homura.HP -= player.damage;
				Homura.invincible = true;
				Homura.invincibleTime.restart().asSeconds();

				for (unsigned int k = 0; k < hitParticles.size(); k++)
				{
					if (!hitParticles[k].isEffect)
					{
						pokeFX.play();
						hitParticles[k].isEffect = true;
						hitParticles[k].effect.setPosition(Homura.GetPosition());
						hitParticles[k].animation.playingAnimation = true;
						hitParticles[k].animation.OneTimeAnimation = true;
						hitParticles[k].animation.OneTimeFrame.x = 5;
						hitParticles[k].animation.currentImage.x = 0;
						break;
					}
				}
			}
		}

		// check collide
		checkPlayerOnGround(player, ground, wall_L, wall_R);
		if (ground.GetCollider().CheckCollide(Homura.GetHitBox(), direction, 1.f, true))
			Homura.OnCollision(direction);

		// Draw Every Things
		window.clear(Color::White);
		if (Homura.STOP or player.STOP)
		{
			bossBG.setTexture(bossStopTex);
		}
		else
		{
			bossBG.setTexture(bossBGTex);
			if (cutSceneState != 66)
			{
				rocket.Update(time, player, 8);
				UI.Update(counter, score, &soulTex, camera, player, time, false);
				updateFX(hitParticles, dustParticles, dustParticles2, attackParticles, hitFX, dustFX, atkFX, player, time);
			}
		}
		if (!Homura.STOP and cutSceneState != 66)
		player.Update(stage, time, charactor);
		if (cutSceneState != 66)
		{
			window.draw(bossBG);
			if (!player.STOP and cutSceneState != 66)
			Homura.Update(time, player, 6);
		}
		Homura.Draw(window);
		player.Draw(window);
		for (int i = 0; i < bullets.size(); i++)
		{
			if (!player.STOP and !Homura.STOP and cutSceneState != 66) bullets[i].Update(time,player,7);
			bullets[i].Draw(window);
		}	
		rocket.Draw(window);
		drawFX(hitParticles, dustParticles, dustParticles2, attackParticles, window);
		for (int i = 0; i < pistol.size(); i++)
		{
			if (!player.STOP and !Homura.STOP and cutSceneState != 66)
				pistol[i].Update(time, 0);
			pistol[i].Draw(window);
		}
		if (showUI and !player.cutScene) 
			UI.Draw(window);
		if (player.cutScene and !Homura.Dead)
			window.draw(wideScreen);
		window.draw(flash);
		window.display();
	}

	//// summit name Over////
	camera.setCenter(CAM_W / 2.f, CAM_H / 2.f);
	backgroundTex.loadFromFile("Res/misc/over.png");  end.setTexture(backgroundTex);
	end.setPosition(0, 0);
	while (window.isOpen() and player.Dead and cutSceneState != 69)
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
				isPressed = false;
			if (button.type == Event::TextEntered)
			{
				if (button.text.unicode == 8) // BACK SPACE
				{
					if (enterName.size() != 0) enterName.pop_back();
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
				else if (button.text.unicode == 13 and enterName.size() != 0) // RETURN
				{
					window.clear(Color::White);
					enterFX.play();
					stage = 0;
					break;
				}
				else if (button.text.unicode < 128 and button.text.unicode != 32 and button.text.unicode != 13 and enterName.size() < 15)
				{
					enterName.push_back((char)button.text.unicode);
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
			}
		}
		window.setView(camera);
		window.clear();
		window.draw(end);
		window.draw(Name);
		window.display();
	}

	if (player.Dead)
	{
		stage = 0;
		screen = 0;
		////////////  Save Game Data ////////////////////
		saved.open("saved.txt");
		if (saved.is_open())
		{
			int line = 0;
			while (line < 6)
			{
				if (line == 0) saved << unlockedSoul << endl;
				else if (line == 1) saved << player.HP << endl;
				else if (line == 2) saved << (int)player.redHP << endl;
				else if (line == 3) saved << stage << endl;
				else if (line == 4) saved << charactor << endl;
				else if (line == 5) saved << score;
				line++;
			}
		}
		saved.close();

		saved.open("score.txt", ios::app);
		if (saved.is_open())
		{
			saved << "\n" << enterName << " " << score;
		}
		saved.close();
		player.Dead = false;
		window.close();
		main();
	}


	camera.setCenter(CAM_W / 2.f, CAM_H / 2.f);
	backgroundTex.loadFromFile("Res/misc/end.png");  end.setTexture(backgroundTex);
	end.setPosition(0, 0);
	
	//// summit name End////
	while (window.isOpen() and stage == 3)
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
				isPressed = false;
			if (button.type == Event::TextEntered)
			{
				if (button.text.unicode == 8) // BACK SPACE
				{
					if (enterName.size() != 0) enterName.pop_back();
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
				else if (button.text.unicode == 13 and enterName.size() != 0) // RETURN
				{
					window.clear(Color::White);
					enterFX.play();
					stage = 0;
					break;
				}
				else if (button.text.unicode < 128 and button.text.unicode != 32 and button.text.unicode != 13 and enterName.size() < 15)
				{
					enterName.push_back((char)button.text.unicode);
					Name.setString(enterName);
					Name.setOrigin(Name.getLocalBounds().left + Name.getLocalBounds().width / 2, Name.getLocalBounds().top + Name.getLocalBounds().height / 2);
					Name.setPosition(CAM_W / 2 - 10, CAM_H / 2 + 170);
				}
			}
		}
		window.setView(camera);
		window.clear();
		window.draw(end);
		window.draw(Name);
		window.display();
	}
	unlockedSoul = true;
	charactor = 3;
	screen = 0;
	stage = 0;
	////////////  Save Game Data ////////////////////
	saved.open("saved.txt");
	if (saved.is_open())
	{
		int line = 0;
		while (line < 6)
		{
			if (line == 0) saved << unlockedSoul << endl;
			else if (line == 1) saved << player.HP << endl;
			else if (line == 2) saved << (int)player.redHP << endl;
			else if (line == 3) saved << stage << endl;
			else if (line == 4) saved << charactor << endl;
			else if (line == 5) saved << score;
			line++;
		}
	}
	saved.close();

	saved.open("score.txt", ios::app);
	if (saved.is_open())
	{
		saved << "\n" << enterName << " " << score;
	}
	saved.close();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	window.close();
	main();

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
	if (!unlockedSoul)
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
		if (scoreBG.loadFromFile("Res/title/score.png"))
		if (logoTex.loadFromFile("Res/title/logo.png"))
		if (menu1Tex.loadFromFile("Res/title/menu.png"))
		if (menu2Tex.loadFromFile("Res/title/menu2.png"))
		if (menu3Tex.loadFromFile("Res/title/menu3.png"))
		if (menu1Tex2.loadFromFile("Res/title/menuu.png"))
		if (menu2Tex2.loadFromFile("Res/title/menuu2.png"))
		if (menu3Tex2.loadFromFile("Res/title/menuu3.png"))
		if (menu4Tex2.loadFromFile("Res/title/menuu4.png"))
		if (blueSel.loadFromFile("Res/title/blueSelected.png"))
		if (pinkSel.loadFromFile("Res/title/pinkSelected.png"))
		if (yellowSel.loadFromFile("Res/title/yellowSelected.png"))

		// Tutorial
		if (moveKey.loadFromFile("Res/particles/button2.png"))
		if (actKey.loadFromFile("Res/particles/button.png"))
		if (portalTex.loadFromFile("Res/enemy/portal.png"))

		// GUI
		if (font.loadFromFile("Res/Font/CreditValley.ttf"))
		if (guiTex.loadFromFile("Res/gui/playerBar.png"))
		if (blackHpTex.loadFromFile("Res/gui/playerBar2.png"))
		if (redHpTex.loadFromFile("Res/gui/redHP.png"))
		if (counterTex.loadFromFile("Res/gui/counterBar.png"))
		if (wideScreenTex.loadFromFile("Res/gui/cropScreen.png"))

		// Effect
		if (getHitSoundFX.loadFromFile("Res/sound/getHit.wav"))
		if (deadSoundFX.loadFromFile("Res/sound/playerDie.wav"))
		if (dustFX.loadFromFile("Res/particles/dust.png"))
		if (pickSoundFX.loadFromFile("Res/sound/pick.wav"))
		if (dropSoundFX.loadFromFile("Res/sound/dropOrb.wav"))
		if (warpSoundFX.loadFromFile("Res/sound/changeStage.wav"))

		// Enemy
		if (tinyTex.loadFromFile("Res/enemy/tiny.png"))
		if (tinySoundFX.loadFromFile("Res/sound/tiny.wav"))
		if (giantTex.loadFromFile("Res/enemy/giant.png"))
		if (giantSoundFX.loadFromFile("Res/sound/giant.wav"))
		if (ghostTex.loadFromFile("Res/enemy/ghost.png"))
		if (ghostSoundFX.loadFromFile("Res/sound/ghost.wav"))
		if (shamanTex.loadFromFile("Res/enemy/shaman.png"))
		if (shamanSoundFX.loadFromFile("Res/sound/shaman.wav"))
		if (darkSphereTex.loadFromFile("Res/enemy/shadowBall.png"))
		if (homuraTexture.loadFromFile("Res/enemy/Homura.png"))
		if (homuraATK.loadFromFile("Res/enemy/HomuraATK.png"))
		if (bulletTex.loadFromFile("Res/enemy/bullet.png"))
		if (rocketTex.loadFromFile("Res/enemy/rocket.png"))
		{
			// soundFX
			selectFX.setBuffer(selectSoundFX);
			enterFX.setBuffer(enterSoundFX);
			cancleFX.setBuffer(cancleSoundFX);
			pickFX.setBuffer(pickSoundFX);
			dropFX.setBuffer(dropSoundFX);
			getHitFX.setBuffer(getHitSoundFX);
			warpFX.setBuffer(warpSoundFX);
			deadFX.setBuffer(deadSoundFX);
			pokeFX.setBuffer(pokeSoundFX);
			tinyFX.setBuffer(tinySoundFX);
			giantFX.setBuffer(giantSoundFX);
			ghostFX.setBuffer(ghostSoundFX);
			shamanFX.setBuffer(shamanSoundFX);

			// Music
			titleTheme .setVolume(0);
			firstTheme .setVolume(100);
			forestTheme.setVolume(30);
			bossTheme  .setVolume(80);
			firstTheme .setLoop(true); forestTheme.setLoop(true); bossTheme  .setLoop(true); titleTheme .setLoop(true);

			// Texture
			Title01.setSmooth(true); Title11.setSmooth(true); logoTex.setSmooth(true); pressSpace.setSmooth(true);
			menu1Tex.setSmooth(true); menu2Tex.setSmooth(true); menu3Tex.setSmooth(true); scoreBG.setSmooth(true);
			menu1Tex2.setSmooth(true); menu2Tex2.setSmooth(true); menu3Tex2.setSmooth(true); menu4Tex2.setSmooth(true);
			blueSel.setSmooth(true); pinkSel.setSmooth(true); yellowSel.setSmooth(true);
			moveKey.setSmooth(true); actKey.setSmooth(true); playerTexture.setSmooth(true); playerCombo.setSmooth(true);
			guiTex.setSmooth(true); blackHpTex.setSmooth(true); redHpTex.setSmooth(true); counterTex.setSmooth(true);
			hitFX.setSmooth(true); dustFX.setSmooth(true); orbTex.setSmooth(true);
			giantTex.setSmooth(true); shamanTex.setSmooth(true); darkSphereTex.setSmooth(true);
			homuraTexture.setSmooth(true); homuraATK.setSmooth(true); bulletTex.setSmooth(true); rocketTex.setSmooth(true);

			fileLoaded = true; 
			loading.terminate(); 
		}
	}
	else
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
		if (Title01.loadFromFile("Res/title/Title2.png"))
		if (Title11.loadFromFile("Res/title/Title2Blur.png"))
		if (pressSpace.loadFromFile("Res/title/pressSpace.png"))
		if (logoTex.loadFromFile("Res/title/logo.png"))
		if (scoreBG.loadFromFile("Res/title/score.png"))
		if (menu1Tex.loadFromFile("Res/title/menu.png"))
		if (menu2Tex.loadFromFile("Res/title/menu2.png"))
		if (menu3Tex.loadFromFile("Res/title/menu3.png"))
		if (menu1Tex2.loadFromFile("Res/title/menuu.png"))
		if (menu2Tex2.loadFromFile("Res/title/menuu2.png"))
		if (menu3Tex2.loadFromFile("Res/title/menuu3.png"))
		if (menu4Tex2.loadFromFile("Res/title/menuu4.png"))
		if (blueSel.loadFromFile("Res/title/blueSelected2.png"))
		if (pinkSel.loadFromFile("Res/title/pinkSelected2.png"))
		if (yellowSel.loadFromFile("Res/title/yellowSelected2.png"))
		if (blackSel.loadFromFile("Res/title/blackSelected2.png"))

		// Tutorial
		if (moveKey.loadFromFile("Res/particles/button2.png"))
		if (actKey.loadFromFile("Res/particles/button.png"))
		if (portalTex.loadFromFile("Res/enemy/portal.png"))

		// GUI
		if (font.loadFromFile("Res/Font/CreditValley.ttf"))
		if (guiTex.loadFromFile("Res/gui/playerBar.png"))
		if (blackHpTex.loadFromFile("Res/gui/playerBar2.png"))
		if (redHpTex.loadFromFile("Res/gui/redHP.png"))
		if (counterTex.loadFromFile("Res/gui/counterBar.png"))
		if (wideScreenTex.loadFromFile("Res/gui/cropScreen.png"))

		// Effect
		if (getHitSoundFX.loadFromFile("Res/sound/getHit.wav"))
		if (deadSoundFX.loadFromFile("Res/sound/playerDie.wav"))
		if (dustFX.loadFromFile("Res/particles/dust.png"))
		if (pickSoundFX.loadFromFile("Res/sound/pick.wav"))
		if (dropSoundFX.loadFromFile("Res/sound/dropOrb.wav"))
		if (warpSoundFX.loadFromFile("Res/sound/changeStage.wav"))

		// Enemy
		if (tinyTex.loadFromFile("Res/enemy/tiny.png"))
		if (tinySoundFX.loadFromFile("Res/sound/tiny.wav"))
		if (giantTex.loadFromFile("Res/enemy/giant.png"))
		if (giantSoundFX.loadFromFile("Res/sound/giant.wav"))
		if (ghostTex.loadFromFile("Res/enemy/ghost.png"))
		if (ghostSoundFX.loadFromFile("Res/sound/ghost.wav"))
		if (shamanTex.loadFromFile("Res/enemy/shaman.png"))
		if (shamanSoundFX.loadFromFile("Res/sound/shaman.wav"))
		if (darkSphereTex.loadFromFile("Res/enemy/shadowBall.png"))
		if (homuraTexture.loadFromFile("Res/enemy/Homura.png"))
		if (homuraATK.loadFromFile("Res/enemy/HomuraATK.png"))
		if (bulletTex.loadFromFile("Res/enemy/bullet.png"))
		if (rocketTex.loadFromFile("Res/enemy/rocket.png"))
		{
			// soundFX
			selectFX.setBuffer(selectSoundFX);
			enterFX.setBuffer(enterSoundFX);
			cancleFX.setBuffer(cancleSoundFX);
			pickFX.setBuffer(pickSoundFX);
			dropFX.setBuffer(dropSoundFX);
			getHitFX.setBuffer(getHitSoundFX);
			warpFX.setBuffer(warpSoundFX);
			deadFX.setBuffer(deadSoundFX);
			pokeFX.setBuffer(pokeSoundFX);
			tinyFX.setBuffer(tinySoundFX);
			giantFX.setBuffer(giantSoundFX);
			ghostFX.setBuffer(ghostSoundFX);
			shamanFX.setBuffer(shamanSoundFX);

			// Music
			titleTheme .setVolume(0);
			firstTheme .setVolume(100);
			forestTheme.setVolume(30);
			bossTheme  .setVolume(80);
			firstTheme .setLoop(true);
			forestTheme.setLoop(true);
			bossTheme  .setLoop(true);
			titleTheme .setLoop(true);

			// Texture
			Title01.setSmooth(true); Title11.setSmooth(true); logoTex.setSmooth(true); pressSpace.setSmooth(true);
			menu1Tex.setSmooth(true); menu2Tex.setSmooth(true); menu3Tex.setSmooth(true); scoreBG.setSmooth(true);
			menu1Tex2.setSmooth(true); menu2Tex2.setSmooth(true); menu3Tex2.setSmooth(true); menu4Tex2.setSmooth(true);
			blueSel.setSmooth(true); pinkSel.setSmooth(true); yellowSel.setSmooth(true); blackSel.setSmooth(true);
			moveKey.setSmooth(true); actKey.setSmooth(true); playerTexture.setSmooth(true); playerCombo.setSmooth(true);
			guiTex.setSmooth(true); blackHpTex.setSmooth(true); redHpTex.setSmooth(true); counterTex.setSmooth(true);
			hitFX.setSmooth(true); dustFX.setSmooth(true); orbTex.setSmooth(true);
			giantTex.setSmooth(true); shamanTex.setSmooth(true); darkSphereTex.setSmooth(true);
			homuraTexture.setSmooth(true); homuraATK.setSmooth(true); bulletTex.setSmooth(true); rocketTex.setSmooth(true);

			fileLoaded = true; 
			loading.terminate(); 
		}
	}
}

void EventAndTime(RenderWindow& window, Clock& clock, Player& player, float& time)
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
		{
			player.isPressed = false;
			isPressed = false;
		}		
	}

	time = clock.restart().asSeconds();
	if (time > 1.0f / 20.0f) time = 1.0f / 20.0f;
}

void showTutorial(RenderWindow& window, Particle& movementKey, Particle& actionKey, Texture& moveKey, Texture& actKey, Player& player, Ground& moveZone, Ground& jumpZone, Ground& dashZone, float time)
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

	// Tutorial
	if (screen == 5)
	{
		cameraMovement += Vector2f(0.f, -180.f);
		if (playerPosition.x > 460.f and playerPosition.x < 2720.f)
		{
			camera .move(cameraMovement * time *   2.5f);
			preBack.move(cameraMovement * time * -0.05f);
			back   .move(cameraMovement * time *  0.05f);
			front  .move(cameraMovement * time *  -1.5f);
		}
		else
		{
			camera .move(0,cameraMovement.y * time *   2.5f);
			preBack.move(0,cameraMovement.y * time * -0.05f);
			back   .move(0,cameraMovement.y * time *  0.05f);
			front  .move(0,cameraMovement.y * time *  -1.5f);
		}
	}

	// Woods
	else if (screen == 6)
		camera.move(cameraMovement * time *  2.5f);
	else if (screen == 7)
	{
		cameraMovement += Vector2f(0.f, -180.f);
		if (playerPosition.x > 460.f and playerPosition.x < 3620.f)
		{
			camera .move(cameraMovement.x * time *  2.5f, 0.f);
			preBack.move(cameraMovement.x * time *  2.5f, 0.f);
			back   .move(cameraMovement.x * time *  0.5f, 0.f);
			front  .move(cameraMovement.x * time * -1.5f, 0.f);
		}
	}

	// Boss Fight
	else if (screen == 8) camera.setCenter(WINDOW_W / 2, WINDOW_H / 2);
}

bool CutScene(bool in, RectangleShape& screen, unsigned int& size)
{
	screen.setSize(Vector2f(WINDOW_W + 120.f, size));
	screen.setOrigin(screen.getSize() / 2.f);

	if (in)
	{
		if (size > WINDOW_H + 100.f)
		{ size -= 5; return false; }
		else return true;
	}
	else
	{
		if (size < WINDOW_H + 300.f)
		{ size += 5; return false; }
		else return true;
	}
}

void drawTiny(vector<Enemy>& tinies, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < tinies.size(); i++)
	{
		if (!player.STOP and cutSceneState != 66)
		tinies[i].Update(time, player, 0);
		tinies[i].Draw(window);
	}
}

void drawGiant(vector<Enemy>& giants, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < giants.size(); i++)
	{
		if (!player.STOP and cutSceneState != 66)
		giants[i].Update(time, player, 1);
		giants[i].Draw(window);
	}
}

void drawGhost(vector<Enemy>& ghosts, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		if (!player.STOP and cutSceneState != 66)
		ghosts[i].Update(time, player, 2);
		ghosts[i].Draw(window);
	}
}

void drawShaman(vector<Enemy>& shamans, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < shamans.size(); i++)
	{
		if (!player.STOP and cutSceneState != 66)
		shamans[i].Update(time, player, 3);
		shamans[i].Draw(window);
	}
}

void drawDarkSphere(vector<Enemy>& darkSpheres, Player& player, RenderWindow& window, float time)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		if (!player.STOP and cutSceneState != 66)
		darkSpheres[i].Update(time, player, 4);
		darkSpheres[i].Draw(window);
	}
}

void drawGround(RenderWindow& window, Ground& ground, Ground& wallLeft, Ground& wallRight)
{
	ground   .Draw(window);
	wallLeft .Draw(window);
	wallRight.Draw(window);
}

void checkPlayerOnGround(Player& player, Ground& ground, Ground& wallLeft, Ground& wallRight)
{
	if (ground.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction);
	if (wallLeft.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction);
	if (wallRight.GetCollider().CheckCollide(player.GetHitBox(), direction, 1.0f, true))
		player.OnCollision(direction);

}

void Tiny(vector<Enemy>& tinies, vector<Enemy>& orbs,vector<Particle>& hitParticles, Player& player, Ground& ground)
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
			tinyFX.play();
			tinies[i].state = 4;
			tinies[i].animation.playingAnimation = true;
			tinies[i].animation.currentImage.x = 0;
		}
		else if (tinies[i].state == 4 and !tinies[i].animation.playingAnimation)
			tinies[i].state = 1;
		else if (tinies[i].state == 7 and !tinies[i].animation.playingAnimation)
			tinies[i].state = 2;
		if (tinies[i].HP <= 0.f and !tinies[i].Dead)
		{
			tinies[i].state = 100;
			tinies[i].dropOrb = tinies[i].GetPosition();
		}
		else if (tinies[i].Dead and tinies[i].state == 100)
		{ 
			tinies[i].state = 101;
			counter--;
			score += 100;

			int random = rand() % 100;
			if (random < 45) tinies[i].spawnOrb = true;
		}

	// Spawn Heal Orb
		for (int i = 0; i < tinies.size(); i++)
		{
			for (int j = 0; j < orbs.size(); j++)
			{
				if (tinies[i].spawnOrb and orbs[j].state == 98)
				{
					dropFX.play();
					tinies[i].spawnOrb = false;
					orbs[j].state = 1;
					orbs[j].SetPosition(tinies[i].dropOrb.x, tinies[i].dropOrb.y-20);
				}
			}
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(tinies[i].GetAttackBox(), direction, 0.0f, false) and tinies[i].state == 4 and !player.Invincible and !player.Dash and !player.STOP and !player.Dead and !player.getHit)
		{
			getHitFX.play();
			if (score > 15) score -= 15;
			else score = 0;
			player.lostHP += tinies[i].damage - player.defense;
			player.HP     -= tinies[i].damage - player.defense;
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
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
					pokeFX.play();
					hitParticles[j].isEffect = true;
					hitParticles[j].effect.setPosition(tinies[i].GetPosition());
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

void Giant(vector<Enemy>& giants, vector<Enemy>& orbs, vector<Particle>& hitParticles, Player& player, Ground& ground)
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
			giantFX.play();
			giants[i].state = 4;
			giants[i].comboWait.restart().asSeconds();
			giants[i].animation.playingAnimation = true;
			giants[i].animation.currentImage.x = 0;
		}
		else if (giants[i].state == 4 and !giants[i].animation.playingAnimation)
			giants[i].state = 1;
		else if (giants[i].state == 7 and !giants[i].animation.playingAnimation)
		{
			giantFX.play();
			giants[i].state = 4;
			giants[i].comboWait.restart().asSeconds();
			giants[i].animation.playingAnimation = true;
			giants[i].animation.currentImage.x = 0;
		}
		if (giants[i].HP <= 0.f and !giants[i].Dead)
		{
			giants[i].state = 100;
			giants[i].dropOrb = giants[i].GetPosition();
		}
		else if (giants[i].Dead and giants[i].state != 101 and giants[i].state != 98)
		{
			giants[i].state = 101;
			counter--;
			score += 300;

			int random = rand() % 100;
			if (random < 100) giants[i].spawnOrb = true;
		}

	// Spawn Heal Orb
		for (int i = 0; i < giants.size(); i++)
		{
			for (int j = 0; j < orbs.size(); j++)
			{
				if (giants[i].spawnOrb and orbs[j].state == 98)
				{
					dropFX.play();
					giants[i].spawnOrb = false;
					orbs[j].state = 1;
					orbs[j].SetPosition(giants[i].dropOrb.x, giants[i].dropOrb.y - 20);
				}
			}
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(giants[i].GetAttackBox(), direction, 0.0f, false) and giants[i].state == 4 and !player.Invincible and !player.Dash and !player.STOP and !player.Dead and !player.getHit)
		{
			getHitFX.play();
			if (score > 15) score -= 15;
			else score = 0;
			player.lostHP += giants[i].damage - player.defense;
			player.HP     -= giants[i].damage - player.defense;
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
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
					pokeFX.play();
					hitParticles[j].isEffect = true;
					hitParticles[j].effect.setPosition(giants[i].GetPosition());
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

void Ghost(vector<Enemy>& ghosts, vector<Enemy>& orbs, vector<Particle>& hitParticles, Player& player, Ground& ground)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{

	// State
		if (ghosts[i].state == 7 and !ghosts[i].animation.playingAnimation)
			ghosts[i].state = 1;
		if (ghosts[i].HP <= 0.f and !ghosts[i].Dead)
		{
			ghosts[i].state = 100;
			ghosts[i].dropOrb = ghosts[i].GetPosition();
		}
		else if (ghosts[i].Dead and ghosts[i].state == 100)
		{
			ghosts[i].state = 101;
			counter--;
			score += 125;

			int random = rand() % 100;
			if (random < 70) ghosts[i].spawnOrb = true;
		}

	// Spawn Heal Orb
		for (int i = 0; i < ghosts.size(); i++)
		{
			for (int j = 0; j < orbs.size(); j++)
			{
				if (ghosts[i].spawnOrb and orbs[j].state == 98)
				{
					dropFX.play();
					ghosts[i].spawnOrb = false;
					orbs[j].state = 1;
					orbs[j].SetPosition(ghosts[i].dropOrb.x, ghosts[i].dropOrb.y - 20);
				}
			}
		}

	// Hit Box
		if (player.GetHitBox().CheckCollide(ghosts[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.STOP and !player.Dead and !player.getHit)
		{
			getHitFX.play();
			ghostFX.play();
			if (score > 15) score -= 15;
			else score = 0;
			player.lostHP += ghosts[i].damage - player.defense;
			player.HP     -= ghosts[i].damage - player.defense;
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
		}
		if (ghosts[i].GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.0f, false) and ghosts[i].state != 7 and ghosts[i].state != 101 and ghosts[i].state != 100)
		{
			ghostFX.play();
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
					pokeFX.play();
					hitParticles[j].isEffect = true;
					hitParticles[j].effect.setPosition(ghosts[i].GetPosition());
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

void Shaman(vector<Enemy>& shamans, vector<Enemy>& darkSpheres, vector<Enemy>& orbs, vector<Particle>& hitParticles, Player& player, Ground& ground)
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
		{
			shamans[i].state = 100;
			shamans[i].dropOrb = shamans[i].GetPosition();
		}
		else if (shamans[i].Dead and shamans[i].state == 100)
		{
			shamans[i].state = 101;
			counter--;
			score += 150;

			int random = rand() % 100;
			if (random < 60) shamans[i].spawnOrb = true;
		}
		if (shamans[i].startShoot)
		{
			if (shamans[i].wait.getElapsedTime().asSeconds() > 0.5)
			{
				shamanFX.play();
				if (shamans[i].faceRight)
				{
					for (unsigned int j = 0; j < darkSpheres.size(); j++)
					{
						if (darkSpheres[j].state == 1)
						{
							darkSpheres[j].wait.restart().asSeconds();
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
							darkSpheres[j].wait.restart().asSeconds();
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

	// Spawn Heal Orb
		for (int i = 0; i < shamans.size(); i++)
		{
			for (int j = 0; j < orbs.size(); j++)
			{
				if (shamans[i].spawnOrb and orbs[j].state == 98)
				{
					dropFX.play();
					shamans[i].spawnOrb = false;
					orbs[j].state = 1;
					orbs[j].SetPosition(shamans[i].dropOrb.x, shamans[i].dropOrb.y - 20);
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
					pokeFX.play();
					hitParticles[j].isEffect = true;
					hitParticles[j].effect.setPosition(shamans[i].GetPosition());
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

void DarkSphere(vector<Enemy>& darkSpheres, Player& player)
{
	for (unsigned int i = 0; i < darkSpheres.size(); i++)
	{
		if (player.GetHitBox().CheckCollide(darkSpheres[i].GetAttackBox(), direction, 0.0f, false) and !player.Invincible and !player.Dash and !player.STOP and !player.Dead and !player.getHit)
		{
			getHitFX.play();
			if (score > 15) score -= 15;
			else score = 0;
			player.lostHP += darkSpheres[i].damage - player.defense;
			player.HP     -= darkSpheres[i].damage - player.defense;
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
		}
		if (darkSpheres[i].wait.getElapsedTime().asSeconds() > 8)
			darkSpheres[i].state = 1;
	}
}

void HomuraAtk(Enemy& Homura, vector<Enemy>& bullets, Enemy& rocket, Player& player, vector<Particle>& hitParticles, RenderWindow& window, float time)
{
	// ATK
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

				bullets[i].Update(time, player, 7);
				bullets[i].Draw(window);
				Homura.startShoot = false;
				Homura.startShoot2 = false;
				break;
			}
			if (bullets[i].state == 99 and bullets[i].wait.getElapsedTime().asSeconds() > 1)
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

			rocket.Update(time, player, 8);
			rocket.Draw(window);
			Homura.startShoot3 = false;
		}
	}
	if (rocket.state == 99 and rocket.wait.getElapsedTime().asSeconds() > 8)
		rocket.state = 1;
	if (player.GetHitBox().CheckCollide(Homura.GetAttackBox(), direction, 0.f, false) and !player.Invincible and !player.Dash)
	{
		getHitFX.play();
		if (score > 15) score -= 15;
		else score = 0;
		player.getHit = true;
		player.animation.playingAnimation = true;
		player.animation.OneTimeAnimation = true;
		player.animation.OneTimeFrame.x = 4;
		player.animation.currentImage.x = 0;
		player.HP -= Homura.bodyDamge;
		player.lostHP += Homura.bodyDamge;
	}

	// Get Hit
	if (Homura.GetHitBox().CheckCollide(player.GetAttackBox(), direction, 0.f, false) and !Homura.invincible)
	{
		Homura.invincible = true;
		Homura.HP -= player.damage;
		Homura.invincibleTime.restart().asSeconds();
		for (unsigned int j = 0; j < hitParticles.size(); j++)
		{
			if (!hitParticles[j].isEffect)
			{
				pokeFX.play();
				hitParticles[j].isEffect = true;
				hitParticles[j].effect.setPosition(Homura.GetPosition());
				hitParticles[j].animation.playingAnimation = true;
				hitParticles[j].animation.OneTimeAnimation = true;
				hitParticles[j].animation.OneTimeFrame.x = 5;
				hitParticles[j].animation.currentImage.x = 0;
				break;
			}
		}
	}
	if (Homura.invincible and Homura.invincibleTime.getElapsedTime().asSeconds() > 3)
	{
		Homura.invincible = false;
		Homura.SetColor(255, 255, 255, 255);
	}

	// Projectile
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].GetHitBox().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !player.Invincible and !player.Dash)
		{
			getHitFX.play();
			if (score > 15) score -= 15;
			else score = 0;
			player.getHit = true;
			player.animation.playingAnimation = true;
			player.animation.OneTimeAnimation = true;
			player.animation.OneTimeFrame.x = 4;
			player.animation.currentImage.x = 0;
			player.HP -= Homura.bulletDamage - player.defense;
			player.lostHP += Homura.bulletDamage - player.defense;
		}
	}
	if (rocket.GetHitBox().CheckCollide(player.GetHitBox(), direction, 0.f, false) and !player.getHit and !player.Invincible and !player.Dash)
	{
		getHitFX.play();
		if (score > 15) score -= 15;
		else score = 0;
		player.getHit = true;
		player.animation.playingAnimation = true;
		player.animation.OneTimeAnimation = true;
		player.animation.OneTimeFrame.x = 4;
		player.animation.currentImage.x = 0;
		player.HP -= Homura.bulletDamage * 2 - player.defense;
		player.lostHP += Homura.bulletDamage * 2 - player.defense;
	}
}

void createFX(vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles,Player& player)
{
	// Dash dust
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

	// Jump Dust
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

	// Impact Dust
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

	// atkFX
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
				if (charactor == 0)
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
				else if (charactor == 1)
				{
					if (player.combo != 3)
						attackParticles[i].animation.OneTimeFrame.x = 8;
					else
						attackParticles[i].animation.OneTimeFrame.x = 8;
					if (player.combo == 1 or player.combo == 2)
						attackParticles[i].row = 0;
					else if (player.combo == 4)
						attackParticles[i].row = 1;
					else if (player.combo == 3)
						attackParticles[i].row = 2;
				}
				else if (charactor == 2)
				{
					if (player.combo != 3)
						attackParticles[i].animation.OneTimeFrame.x = 10;
					else
						attackParticles[i].animation.OneTimeFrame.x = 10;
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

void endFX(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles, Player& player)
{
	// hitFX
	for (unsigned int i = 0; i < hitParticles.size(); i++)
	{
		if (hitParticles[i].isEffect and !hitParticles[i].animation.OneTimeAnimation)
		{
			hitParticles[i].isEffect    = false;
			hitParticles[i].setPosition = false;
		}
	}

	// Jump and Dash dust
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

	// Fall dust
	for (unsigned int i = 0; i < dustParticles2.size(); i++)
	{
		if (dustParticles2[i].isEffect and !dustParticles2[i].animation.OneTimeAnimation)
		{
			dustParticles2[i].setPosition = false;
			dustParticles2[i].isEffect    = false;
			player.fallDust               = false;
		}
	}

	// atkFX
	for (unsigned int i = 0; i < attackParticles.size(); i++)
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

void drawFX(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles, RenderWindow& window)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
		hitParticles[i].Draw(window);

	for (unsigned int i = 0; i < dustParticles.size(); i++)
		dustParticles[i].Draw(window);

	for (unsigned int i = 0; i < dustParticles2.size(); i++)
		dustParticles2[i].Draw(window);
	
	for (unsigned int i = 0; i < attackParticles.size(); i++)
		attackParticles[i].Draw(window);
	
}

void updateFX(vector<Particle>& hitParticles, vector<Particle>& dustParticles, vector<Particle>& dustParticles2, vector<Particle>& attackParticles, Texture& hitEffect, Texture& dustEffect, Texture& atkEffect, Player& player, float time)
{
	for (unsigned int i = 0; i < hitParticles.size(); i++)
		hitParticles[i].Update(&hitEffect, player, time, 1);

	for (unsigned int i = 0; i < dustParticles.size(); i++)
		dustParticles[i].Update(&dustEffect, player, time, 2);

	for (unsigned int i = 0; i < dustParticles2.size(); i++)
		dustParticles2[i].Update(&dustEffect, player, time, 2);

	for (unsigned int i = 0; i < attackParticles.size(); i++)
	{
		if (i == 2)
			attackParticles[i].Update(&atkEffect, player, time, 4);
		else
			attackParticles[i].Update(&atkEffect, player, time, 3);
	}

}