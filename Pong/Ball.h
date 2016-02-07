#pragma once
#include "Entity.h"

const float BALL_SPEED = 900.0f;

class Ball : public Entity
{
public:
	Ball(SDL_Renderer* renderer);
	~Ball();

	void Update(float delta);
	void Render(float delta);

	void SetDirection(float dirX, float dirY);

	float dirX, dirY;

private:
	SDL_Texture* m_texture;
};

