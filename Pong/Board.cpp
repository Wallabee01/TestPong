#include "stdafx.h"
#include "Board.h"

Board::Board(SDL_Renderer* renderer) : Entity(renderer)
{
	m_sideTexture = IMG_LoadTexture(renderer, "Textures/side.png");

	srand(time(0));

	x = 0;
	y = 0;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	boundTop = 21;
	boundBottom = SCREEN_HEIGHT - 21;
}


Board::~Board()
{
	SDL_DestroyTexture(m_sideTexture);
}

void Board::Update(float delta)
{

}

void Board::Render(float delta)
{
	SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = SCREEN_WIDTH;
	destRect.h = 21;
	SDL_RenderCopy(m_renderer, m_sideTexture, 0, &destRect);

	destRect.x = 0;
	destRect.y = SCREEN_HEIGHT - 21;
	destRect.w = SCREEN_WIDTH;
	destRect.h = 21;
	SDL_RenderCopy(m_renderer, m_sideTexture, 0, &destRect);
}
