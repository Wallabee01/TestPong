#include "stdafx.h"
#include "Game.h"
#include "sError.h"

Game::Game() :
	m_window(nullptr),
    m_renderer(nullptr)
{
}


Game::~Game()
{
}

void Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	m_audioEngine.init();

	m_window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

	if (m_window == nullptr) {
		sError("Game::Init : Failed to load window.");
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (m_renderer == nullptr) {
		sError("Game::Init : Failed to load renderer.");
	}

	m_lastTick = SDL_GetTicks();
	m_fpsTick = m_lastTick;
	m_fps = 0;
	m_frameCount = 0;
}

void Game::Run()
{
	Music music = m_audioEngine.loadMusic("Sounds/overworld.wav");
	music.play(-1);

	m_ballSound = m_audioEngine.loadSoundEffect("Sounds/shot.wav");
	m_scoreSound = m_audioEngine.loadSoundEffect("Sounds/coin.wav");
	m_scoredOnSound = m_audioEngine.loadSoundEffect("Sounds/bulletbill.wav");

	m_board = new Board(m_renderer);
	m_ball = new Ball(m_renderer);

	m_paddles.push_back(new Paddle(m_renderer));
	m_paddles.push_back(new Paddle(m_renderer));

	NewGame();

	while (true) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		unsigned int crntTick = SDL_GetTicks();
		float delta = (crntTick - m_lastTick) / 1000.0f;
		if (crntTick - m_fpsTick >= FPS_DELAY) {
			m_fps = m_frameCount * (1000.0f / (crntTick - m_fpsTick));
			m_fpsTick = crntTick;
			m_frameCount = 0;
			char buf[100];
			snprintf(buf, 100, "FPS: %u", m_fps);
			SDL_SetWindowTitle(m_window, buf);
		}
		else {
			m_frameCount++;
		}
		m_lastTick = crntTick;

		Update(delta);
		Render(delta);
	}

	Clean();

	SDL_Quit();
}

void Game::Clean()
{
	delete m_board;
	delete m_ball;

	for (unsigned int i = 0; i < m_paddles.size(); i++) {
		delete m_paddles[i];
	}
	m_paddles.clear();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

void Game::Update(float delta)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SetPaddleY(0, mouseY - m_paddles[0]->height / 2.0f);

	CheckCollisions();
	AI(delta);

	m_board->Update(delta);
	m_ball->Update(delta);

	for (unsigned int i = 0; i < m_paddles.size(); i++) {
		m_paddles[i]->Update(delta);
	}
}

void Game::Render(float delta)
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	m_board->Render(delta);
	m_ball->Render(delta);

	for (unsigned int i = 0; i < m_paddles.size(); i++) {
		m_paddles[i]->Render(delta);
	}

	SDL_RenderPresent(m_renderer);
}

void Game::NewGame()
{
	SetPaddleY(0, m_board->height / 2 - m_paddles[0]->height / 2);
	SetPaddleY(1, m_board->height / 2 - m_paddles[1]->height / 2);

	m_paddles[0]->x = 12;
	m_paddles[1]->x = m_board->width - m_paddles[1]->width - 12;

	m_ball->x = m_board->width / 2 - m_ball->width / 2;
	m_ball->y = m_board->height / 2 - m_ball->height / 2;

	m_ball->SetDirection(1, -1);
}

void Game::SetPaddleY(int index, float y)
{
	float newY;
	if (y < m_board->boundTop) {
		newY = m_board->boundTop;
	}
	else if (y + m_paddles[index]->height > m_board->boundBottom) {
		newY = m_board->boundBottom - m_paddles[index]->height;
	}
	else {
		newY = y;
	}

	m_paddles[index]->y = newY;
}

void Game::CheckCollisions()
{
	//Top & Bottom
	if (m_ball->y < m_board->boundTop) {
		m_ball->y = m_board->boundTop;
		m_ball->SetDirection(m_ball->dirX, -1 * m_ball->dirY);
		//m_ballSound.play();
	}
	else if (m_ball->y + m_ball->height > m_board->boundBottom) {
		m_ball->y = m_board->boundBottom - m_ball->height;
		m_ball->SetDirection(m_ball->dirX, -1 * m_ball->dirY);
		//m_ballSound.play();
	}

	//Left Paddle
	if (m_ball->Collides(m_paddles[0])) {
		m_ball->x = m_paddles[0]->x + m_paddles[0]->width;
		m_ball->SetDirection(1, GetReflection(0, (m_ball->y + m_ball->height / 2) - m_paddles[0]->y));
		m_ballSound.play();
	}

	//Right Paddle
	if (m_ball->Collides(m_paddles[1])) {
		m_ball->x = m_paddles[1]->x - m_ball->width;
		m_ball->SetDirection(-1, GetReflection(1, (m_ball->y + m_ball->height / 2) - m_paddles[1]->y));
		m_ballSound.play();
	}

	//Off-screen
	if (m_ball->x + m_ball->width <= m_board->x) {
		//Player 2 scores
		m_scoredOnSound.play();

		m_ball->x = m_board->width / 2 - m_ball->width / 2 + 250;
		m_ball->y = m_board->height / 2 - m_ball->height / 2;

		m_ball->SetDirection(-1, -1);
	}
	else if (m_ball->x >= m_board->x + m_board->width) {
		//Player 1 scores
		m_scoreSound.play();

		m_ball->x = m_board->width / 2 - m_ball->width / 2 - 250;
		m_ball->y = m_board->height / 2 - m_ball->height / 2;

		m_ball->SetDirection(1, -1);
	}
}

float Game::GetReflection(int index, float hitY)
{
	if (hitY < 0) {
		hitY = 0;
	}
	else if (hitY > m_paddles[index]->height) {
		hitY = m_paddles[index]->height;
	}

	hitY -= m_paddles[index]->height / 2.0f;

	return 2.0f * (hitY / (m_paddles[index]->height / 2.0f));
}

void Game::AI(float delta)
{
	float dirY = AI_SPEED * delta;

	if (m_ball->x > m_board->width / 2 && m_ball->dirX > 0) {
		//Lag behind ball, when ball is in sight
		if (m_paddles[1]->y + m_paddles[1]->height / 2 < m_ball->y + m_ball->height / 2 - dirY) {
			SetPaddleY(1, m_paddles[1]->y + dirY);
		}
		else if (m_paddles[1]->y + m_paddles[1]->height / 2 > m_ball->y + m_ball->height / 2 + dirY) {
			SetPaddleY(1, m_paddles[1]->y - dirY);
		}
	}
	else {
		//Move to center when ball is far away
		if (m_paddles[1]->y + m_paddles[1]->height / 2 < m_board->height / 2 - dirY) {
			SetPaddleY(1, m_paddles[1]->y + dirY);
		}
		else if (m_paddles[1]->y + m_paddles[1]->height / 2 > m_board->height / 2 + dirY) {
			SetPaddleY(1, m_paddles[1]->y - dirY);
		}
	}
}
