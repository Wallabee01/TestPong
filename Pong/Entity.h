#pragma once

class Entity
{
public:
	Entity(SDL_Renderer* renderer);
	virtual ~Entity();

	float x, y, width, height;

	virtual void Update(float delta);
	virtual void Render(float delta);

	bool Collides(Entity* other);
protected:
	SDL_Renderer* m_renderer;
};

