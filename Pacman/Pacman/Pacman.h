#include <iostream>
#pragma once
#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define POWERUPCOUNT 3

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player
{
	// Data to represent Pacman
	bool dead;
	Vector2* Position;
	Rect* SourceRect;
	Texture2D* Texture;
	int Direction;
	int Frame;
	int CurrentFrameTime;
	int lives;
	float speedMultiplier;
};
struct Object
{
	Vector2* Position;
	Rect* SourceRect;
	Texture2D* Texture;
	int Frame;
	int CurrentFrameTime;
	bool eaten;
	bool okay;
};
struct Enemy
{
	bool dead;
	Vector2* Position;
	Texture2D* Texture;
	Rect* SourceRect;
	int Direction;
	float Speed;
	int Frame;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:	
	Player* _pacman;
	Object* _munchie[MUNCHIECOUNT];
	Object* _cherry;
	Object* _powerUp[POWERUPCOUNT];
	Enemy* _ghost[GHOSTCOUNT];


	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPause(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart();
	void CheckDeath();
	void CheckViewportCollision();
	void CheckGhostCollision();
	void CheckCherryCollision();
	void CheckMunchieCollision();
	void CheckPowerUpCollision(int elapsedTime);
	void CheckWin();
	
	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void UpdateGhost(Enemy*, int elapsedTime);

	//consts
	const int _cFrameTime;
	const float _cSpeed;

	
	//ints for score
	int Score;
	int time;
	int eaten;

	//allows pacman to go in one direction 
	bool dDown;
	bool wDown;
	bool aDown;
	bool sDown;

	//loads sounds 
	SoundEffect* _pop;
	SoundEffect* _pauseSound;
	SoundEffect* _deathSound;
	SoundEffect* _scoreSound;

	// Position for String
	Vector2* _stringPosition1;
	Vector2* _stringPosition2;
	Vector2* _stringPosition3;
	
	// data for Scan lines 
	Texture2D* _overlayBackground;
	Rect* _overlayRect;
	bool _overlay;

	//Data for menu 
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _loop;
	
	//Data for start screen 
	Texture2D* _startBackground;
	Rect* _startRectangle;
	Vector2* _startStringPosition;
	bool _started;

	//Data for win screen 
	Texture2D* _winBackground;
	Rect* _winRectangle;
	Vector2* _winStringPosition;
	Vector2* _winStringPosition2;
	bool _win;

	//Data for start screen 
	Texture2D* _deathBackground;
	Rect* _deathRectangle;
	Vector2* _deathStringPosition;
	bool _death;

	//data for lives image
	Texture2D* _livesTexture;
	Rect* _livesRect;
	Vector2* _livesPosition;

	Vector2* distance;

	//checks for new key strokes 
	bool _pKeyDown;
public:
	
	
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};