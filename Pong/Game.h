#pragma once
#include "Board.h"
#include "Paddle.h"
#include "Ball.h"
#include "AudioEngine.h"

const int FPS_DELAY = 500;
const float AI_SPEED = 400.0f;

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Run();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	unsigned int m_lastTick, m_fpsTick, m_fps, m_frameCount;

	Board* m_board;
	std::vector<Paddle*> m_paddles;
	Ball* m_ball;
	AudioEngine m_audioEngine;
	SoundEffect m_ballSound;
	SoundEffect m_scoreSound;
	SoundEffect m_scoredOnSound;

	void Clean();

	void Update(float delta);
	void Render(float delta);

	void NewGame();

	void SetPaddleY(int index, float y);
	void CheckCollisions();
	float GetReflection(int index, float hitY);

	void AI(float delta);
};

