#include "stdafx.h"
#include "Ball.h"


Ball::Ball(SDL_Renderer* renderer) : Entity(renderer)
{
	m_texture = IMG_LoadTexture(m_renderer, "Textures/ball.png");

	x = 0;
	y = 0;
	width = 24;
	height = 24;

	SetDirection(1, 1);
}


Ball::~Ball()
{
	SDL_DestroyTexture(m_texture);
}

void Ball::Update(float delta)
{
	x += dirX * delta;
	y += dirY * delta;
}

void Ball::Render(float delta)
{
	SDL_Rect rect; 
	rect.x = (int)(x + 0.5f);
	rect.y = (int)(y + 0.5f);
	rect.w = width;
	rect.h = height;
	SDL_RenderCopy(m_renderer, m_texture, 0, &rect);
}

void Ball::SetDirection(float dirX, float dirY)
{
	float length = sqrt(dirX * dirX + dirY * dirY);
	this->dirX = BALL_SPEED * (dirX / length);
	this->dirY = BALL_SPEED * (dirY / length);
}
