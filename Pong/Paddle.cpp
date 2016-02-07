#include "stdafx.h"
#include "Paddle.h"


Paddle::Paddle(SDL_Renderer* renderer) : Entity(renderer)
{
	m_texture = IMG_LoadTexture(renderer, "Textures/paddle.png");

	width = 32;
	height = 128;

	x = 0;
	y = 0;
}


Paddle::~Paddle()
{
	SDL_DestroyTexture(m_texture);
}

void Paddle::Update(float delta)
{

}

void Paddle::Render(float delta)
{
	SDL_Rect rect;
	rect.x = (int)(x + 0.5f);
	rect.y = (int)(y + 0.5f);
	rect.w = width;
	rect.h = height;
	SDL_RenderCopy(m_renderer, m_texture, 0, &rect);
}
