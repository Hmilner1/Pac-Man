#include "Pacman.h"
#include <iostream>
#include <sstream>
#include <ctime>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cSpeed(0.2f), _cFrameTime(250)
{
	//loads in the diffrent variables from pacman.h
	_pacman = new Player();
	_cherry = new Object();
	//power ups 
	for (int i = 0; i < POWERUPCOUNT; i++)
	{
		_powerUp[i] = new Object();
		_powerUp[i]->eaten = false;
		_powerUp[i]->okay = false;
	}
	//ghost orange
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i] = new Enemy();
		_ghost[i]->Direction = 0;
		_ghost[i]->Speed = 0.2;
		_ghost[i]->Frame = 0;
		_ghost[i]->dead = false;
	}
	//munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i] = new Object();
		_munchie[i]->CurrentFrameTime = 0;
		_munchie[i]->Frame = 0;
		_munchie[i]->eaten = false;
		_munchie[i]->okay = false;
	}
	//sounds
	_pop = new SoundEffect();
	_pauseSound = new SoundEffect();
	_deathSound = new SoundEffect();
	_scoreSound = new SoundEffect();
	//score
	Score = 0;
	time = 0;
	eaten = 0;
	_win = false;
	//screens 
	_paused = false;
	_started = false;
	_pKeyDown = false;
	//pacman
	_pacman->dead = false;
	_pacman->Direction = 0;
	_pacman->CurrentFrameTime = 0;
	_pacman->Frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->lives = 3;
	//cherry
	_cherry->Frame = 0;
	_cherry->CurrentFrameTime = 0;
	_cherry->eaten = false;
	_overlay = true;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _livesTexture;
	delete _livesPosition;
	delete _livesRect;
	delete _pacman;
	delete _pacman->SourceRect;
	delete _pacman->Texture;
	delete _pacman->Position;
	delete _cherry->SourceRect;
	delete _cherry->Texture;
	delete _cherry->Position; 
	delete _cherry;
	delete _pop;
	delete _overlayBackground;
	delete _overlayRect;
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghost[i]->SourceRect;
		delete _ghost[i]->Texture;
		delete _ghost[i]->Position;
		delete _ghost[i];
	}
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie[i]->SourceRect;
		delete _munchie[i]->Texture;
		delete _munchie[i]->Position;
		delete _munchie[i];
	}
}

void Pacman::LoadContent()
{
	// Loads Pacman
	_pacman->Texture = new Texture2D();
	_pacman->Texture->Load("Textures/Pacman.png", false);
	_pacman->Position = new Vector2(500.0f, 370.0f);
	_pacman->SourceRect = new Rect(0.0f, 0.0f, 32, 32);
	//loads ghost 
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (i > 1)
		{
			_ghost[i]->Texture = new Texture2D();
			_ghost[i]->Texture->Load("Textures/GhostOrange.png", false);
			_ghost[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			_ghost[i]->SourceRect = new Rect(0.0f, 0.0f, 32, 32);
		}
		else
		{
			_ghost[i]->Texture = new Texture2D();
			_ghost[i]->Texture->Load("Textures/GhostBlue.png", false);
			_ghost[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			_ghost[i]->SourceRect = new Rect(0.0f, 0.0f, 32, 32);
		}
	}

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		// Loads munchie
		_munchie[i]->Texture = new Texture2D();
		_munchie[i]->Texture->Load("Textures/Munchie.png", false);
		while (_munchie[i]->okay == false)
		{
			_munchie[i]->okay = true;
			_munchie[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			_munchie[i]->SourceRect = new Rect(0.0f, 0.0f, 12, 12), Graphics::GetViewportWidth(), Graphics::GetViewportHeight();
				if (_munchie[i]->Position = _munchie[i]->Position)
				{
					_munchie[i]->okay == false;
					break;
				}
		}
	}
	for (int i = 0; i < POWERUPCOUNT; i++)
	{
		_powerUp[i]->Texture = new Texture2D();
		_powerUp[i]->Texture->Load("Textures/PowerUp.png", false);
		_powerUp[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_powerUp[i]->SourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	//Loads Overlay 
	_overlayBackground = new Texture2D();
	_overlayBackground->Load("Textures/ScanLines.png", false);
	_overlayRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	// Loads cherry
	_cherry->Texture = new Texture2D();
	_cherry->Texture->Load("Textures/Cherry.png", false);
	_cherry->Position = new Vector2(400.0f, 450.0f);
	_cherry->SourceRect = new Rect(0,0,32,32);	
	
	// set Death paramiters 
	_deathBackground = new Texture2D();
	_deathBackground->Load("Textures/Transparency.png", false);
	_deathRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_deathStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.5f, Graphics::GetViewportHeight() / 2.0f);

	// loads lives image
	_livesTexture = new Texture2D();
	_livesTexture->Load("Textures/LivesSprites.png", false);
	_livesRect = new Rect;
	_livesRect = new Rect(800.0f,0.0f, 48,20);
	_livesPosition = new Vector2(810.0f, 10.0f);

	// Set string positions for on screen text such as score and position 
	_stringPosition1 = new Vector2(10.0f, 25.0f);
	_stringPosition2 = new Vector2(900.0f, 25.0f);
	_stringPosition3 = new Vector2(750.0f, 25.0f);

	//loads the sounds files into the game 
	_pop->Load("Sound Effects/pop.wav");
	_pauseSound->Load("Sound Effects/Pause.wav");
	_deathSound->Load("Sound Effects/GameOver.wav");
	_scoreSound->Load("Sound Effects/Score.wav");
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	CheckStart();
	if (!_started)
	{
		//check for start
		if (keyboardState->IsKeyDown(Input::Keys::RETURN))
		{
			_started = true;
		}
	}
	else
	{
		//checks for pause 
		CheckPause(keyboardState, Input::Keys::P);
		CheckDeath();
		if (!_paused)
		{
			Input(elapsedTime, keyboardState, mouseState);
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
			UpdateCherry(elapsedTime);
			CheckViewportCollision();
			CheckGhostCollision();
			CheckCherryCollision();
			CheckMunchieCollision();
			CheckPowerUpCollision(elapsedTime);
			Draw(elapsedTime);
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				UpdateGhost(_ghost[i], elapsedTime);
			}
			CheckWin();
		}
	}
}

void Pacman::CheckStart()
{
	// set start paramiters of start sceen 
	_startBackground = new Texture2D();
	_startBackground->Load("Textures/Transparency.png", false);
	_startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::CheckPause(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// set menu paramiters 
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.1f, Graphics::GetViewportHeight() / 2.0f);

	// checks if keys are down 
	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
	// controls menu
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
		Audio::Play(_pauseSound);
	}
}

void Pacman::CheckDeath()
{
	// set Death paramiters 
	_deathBackground = new Texture2D();
	_deathBackground->Load("Textures/Transparency.png", false);
	_deathRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_deathStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.1f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	//gets keybaord state
	float pacmanSpeed = _cSpeed * elapsedTime * _pacman->speedMultiplier;
	for (int i = 0; i < POWERUPCOUNT; i++)
	{
		if (_powerUp[i]->okay == true)
		{
			//increases speed when shift is down 
			_pacman->speedMultiplier = 2.0f;
			time += elapsedTime;
			if (time > 6000)
			{
				_powerUp[i]->okay = false;
				_pacman->speedMultiplier = 1.0f;
				time = 0;
			}
		}
		
	}
	//changes direction and frame of animation based off the input 
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->Position->X += pacmanSpeed;
		_pacman->Direction = 0;
	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->Position->Y -= pacmanSpeed;
		_pacman->Direction = 3;
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->Position->X -= pacmanSpeed;
		_pacman->Direction = 2;
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->Position->Y += pacmanSpeed;
		_pacman->Direction = 1;
	}
	if (mouseState->LeftButton == Input::ButtonState::PRESSED && _cherry->eaten == false)
	{
		_cherry->Position->X = mouseState->X;
		_cherry->Position->Y = mouseState->Y;
	}
}

void Pacman::CheckViewportCollision()
{
	//allows pacman to go from one side of the screen to the other 
	if (_pacman->Position->X + _pacman->SourceRect->Width > Graphics::GetViewportWidth())
	{
		_pacman->Position->X = 0 - _pacman->SourceRect->Width;
	}
	else if (_pacman->Position->X + _pacman->SourceRect->Width < 0)
	{
		_pacman->Position->X = Graphics::GetViewportWidth() - _pacman->SourceRect->Width;
	}
	//stops pacman from going throught the top
	else if (_pacman->Position->Y + _pacman->SourceRect->Height > Graphics::GetViewportHeight())
	{
		_pacman->Position->Y = Graphics::GetViewportHeight() - _pacman->SourceRect->Height;
	}
	else if (_pacman->Position->Y + _pacman->SourceRect->Height < 32)
	{
		_pacman->Position->Y = 32 - _pacman->SourceRect->Height;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	//changes imagine on sprite sheet for animation 
	_pacman->SourceRect->X = _pacman->SourceRect->Width * _pacman->Frame;
	//pacman animation 
	_pacman->SourceRect->Y = _pacman->SourceRect->Height * _pacman->Direction;
	_pacman->CurrentFrameTime += elapsedTime;
	if (_pacman->CurrentFrameTime > _cFrameTime)
	{
		_pacman->Frame++;
		if (_pacman->Frame >= 2)
		{
			_pacman->Frame = 0;
		}
		_pacman->CurrentFrameTime = 0;
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		//changes imagine on sprite sheet for animation 
		_munchie[i]->SourceRect->X = _munchie[i]->SourceRect->Width * _munchie[i]->Frame;
		_munchie[i]->CurrentFrameTime += elapsedTime;
		if (_munchie[i]->CurrentFrameTime > 500)
		{
			_munchie[i]->Frame++;
			if (_munchie[i]->Frame >= 2)
			{
				_munchie[i]->Frame = 0;
			}
			_munchie[i]->CurrentFrameTime = 0;
		}
	}
}

void Pacman::CheckMunchieCollision()
{
	//checks to see if pacman has collided with munchie 
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		bottom2 = _munchie[i]->Position->Y + _munchie[i]->SourceRect->Height;
		left2 = _munchie[i]->Position->X;
		right2 = _munchie[i]->Position->X + _munchie[i]->SourceRect->Width;
		top2 = _munchie[i]->Position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2)
			&& (left1 < right2))
		{
			//unloads munchie position and plays sound effect 
			_munchie[i]->eaten = true;
			_munchie[i]->Position->X = -100;
			_munchie[i]->Position->Y = -100;
			eaten = eaten + 1;
			Score = Score + 100;
			Audio::Play(_pop);
		}
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	//changes imagine on sprite sheet for animation 
	_cherry->SourceRect->X = _cherry->SourceRect->Width * _cherry->Frame;
	_cherry->CurrentFrameTime += elapsedTime;
	if (_cherry->CurrentFrameTime > 500)
	{
		_cherry->Frame++;
		if (_cherry->Frame >= 2)
		{
			_cherry->Frame = 0;
		}
		_cherry->CurrentFrameTime = 0;
	}
}

void Pacman::CheckCherryCollision()
{
	//checks if pacman has collided with cherry
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	bottom2 = _cherry->Position->Y + _cherry->SourceRect->Height;
	left2 = _cherry->Position->X;
	right2 = _cherry->Position->X + _cherry->SourceRect->Width;
	top2 = _cherry->Position->Y;
	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2)
		&& (left1 < right2))
	{
		//unloads the cherry then plays sound effect
		_cherry->eaten = true;
		delete _cherry->Position;
		Score = Score + 250;
		Audio::Play(_scoreSound);
	}
}

void Pacman::CheckPowerUpCollision(int elapsedTime)
{
	//checks if pacman has collided with PowerUp
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	for (int i = 0; i < POWERUPCOUNT; i++)
	{
		bottom2 = _powerUp[i]->Position->Y + _powerUp[i]->SourceRect->Height;
		left2 = _powerUp[i]->Position->X;
		right2 = _powerUp[i]->Position->X + _powerUp[i]->SourceRect->Width;
		top2 = _powerUp[i]->Position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2)
			&& (left1 < right2))
		{
			//unloads the cherry then plays sound effect
			_powerUp[i]->eaten = true;
			_powerUp[i]->okay = true;
			delete _powerUp[i]->Position;
			Audio::Play(_scoreSound);
		}
	}

}

void Pacman::UpdateGhost(Enemy* ghost, int elapsedTime)
{
	//changes the ghost direction based off diffrent factors 
	if (ghost->Direction == 0)
	{
		ghost->Position->X += ghost->Speed * elapsedTime;
	}
	else if (ghost->Direction == 1)
	{
		ghost->Position->X -= ghost->Speed * elapsedTime;
	}
	if (ghost->Position->X + ghost->SourceRect->Width >= Graphics::GetViewportWidth())
	{
		ghost->Direction = 1;
		ghost->Frame = 1;
	}
	else if (ghost->Position->X <= 0)
	{
		ghost->Direction = 0;
		ghost->Frame = 0;
	}

	if (ghost->Position->X <= _pacman->Position->X)
	{
		ghost->Position->X += ghost->Speed * elapsedTime;
	}
	else if (ghost->Position->X >= _pacman->Position->X)
	{
		ghost->Position->X -= ghost->Speed * elapsedTime;
	}
	//changes imagine on sprite sheet for animation 
	ghost->SourceRect->X = ghost->SourceRect->Width * ghost->Frame;
	//pacman animation 
	ghost->SourceRect->Y = ghost->SourceRect->Height * ghost->Direction;
}

void Pacman::CheckGhostCollision()
{
	// checks if pacman and ghosts have collided 
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	// Populate variables with Ghost data
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		bottom2 = _ghost[i]->Position->Y + _ghost[i]->SourceRect->Height;
		left2 = _ghost[i]->Position->X;
		right2 = _ghost[i]->Position->X + _ghost[i]->SourceRect->Width;
		top2 = _ghost[i]->Position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2)
			&& (left1 < right2))
		{
			if (_pacman->lives == 1)
			{
				_pacman->dead = true;
				_paused = !_paused;
				delete _pacman->Position;
				delete _pacman->SourceRect;
				delete _livesPosition;
			}
			else
			{
				_pacman->lives--;
				_pacman->Position = new Vector2(350.0f, 350.0f);
			}

			//plays sound effect 
			Audio::Play(_deathSound);
			i = GHOSTCOUNT;
		}
	}
}

void Pacman::CheckWin()
{
	// set Win paramiters 
	_winBackground = new Texture2D();
	_winBackground->Load("Textures/Transparency.png", false);
	_winRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_winStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.2f, Graphics::GetViewportHeight() / 2.0f);
	_winStringPosition2 = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	//Checks to see if all the munchies have been eaten 
	if (eaten == 50)
	{
		_win = true;
		_paused = true;
	}
	
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	std::stringstream scoreTxt;
	std::stringstream livestxt;
	stream << "Pacman X: " << _pacman->Position->X << " Y: " << _pacman->Position->Y;
	scoreTxt << "Score:" << Score;
	livestxt << "Lives :";

	//starts drawing 
	SpriteBatch::BeginDraw(); 
	
	if (!_pacman->dead)
	{
		//draws pacman
		SpriteBatch::Draw(_pacman->Texture, _pacman->Position, _pacman->SourceRect); 
	} 

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (!_ghost[i]->dead)
		//draws ghosts 
		SpriteBatch::Draw(_ghost[i]->Texture, _ghost[i]->Position, _ghost[i]->SourceRect);
	}

	if (!_cherry->eaten)
	{
		//drwas cherry
		SpriteBatch::Draw(_cherry->Texture, _cherry->Position, _cherry->SourceRect);
	}

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if(!_munchie[i]->eaten)
		{
			//draws munchies 
			SpriteBatch::Draw(_munchie[i]->Texture, _munchie[i]->Position, _munchie[i]->SourceRect);
		}
	}
	for (int i = 0; i < POWERUPCOUNT; i++)
	{
		if (!_powerUp[i]->eaten)
		{
			//draws munchies 
			SpriteBatch::Draw(_powerUp[i]->Texture, _powerUp[i]->Position, _powerUp[i]->SourceRect);
		}
	}
	//draws diffrent game screens 
	if (!_started)
	{
		std::stringstream menuStream;
		menuStream << "READY!";
		SpriteBatch::Draw(_startBackground, _startRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _startStringPosition, Color::Yellow);
	}
	if (_paused && _pacman->dead == false && _win == false)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED...";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Yellow);
	}
	if (_pacman->dead == true)
	{
		std::stringstream menuStream;
		menuStream << "Game Over!";
		SpriteBatch::Draw(_deathBackground, _deathRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _deathStringPosition, Color::Red);
	}
	if (_win == true)
	{
		std::stringstream menuStream;
		menuStream << "Your ";
		SpriteBatch::Draw(_winBackground, _winRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _winStringPosition, Color::Yellow);
		SpriteBatch::DrawString(scoreTxt.str().c_str(), _winStringPosition2, Color::Yellow);
	}
	//draws the lives on to the screen 
	if (_pacman->lives == 3)
	{
		SpriteBatch::Draw(_livesTexture, _livesPosition, _livesRect);
	}
	else if(_pacman->lives == 2)
	{
		_livesRect = new Rect (800.0f,0.0f,30,20);
		SpriteBatch::Draw(_livesTexture, _livesPosition, _livesRect);
	}
	else if (_pacman->lives == 1)
	{
		_livesRect = new Rect(800.0f, 0.0f, 18, 20);
		SpriteBatch::Draw(_livesTexture, _livesPosition, _livesRect);
	}
	//draws arcade scanlines 
	SpriteBatch::Draw(_overlayBackground, _overlayRect);

	// Draws position 
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition1, Color::Green);
	//draws score 
	SpriteBatch::DrawString(scoreTxt.str().c_str(), _stringPosition2, Color::Blue);
	SpriteBatch::DrawString(livestxt.str().c_str(), _stringPosition3, Color::Blue);
	SpriteBatch::EndDraw(); 
}