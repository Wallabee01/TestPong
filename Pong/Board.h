#pragma once
#include "Entity.h"

class Board : public Entity
{
public:
	Board(SDL_Renderer* renderer);
	~Board();

	void Update(float delta);
	void Render(float delta);

	float boundTop, boundBottom;

private:
	SDL_Texture* m_sideTexture;
};

