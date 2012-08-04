//Zunanje datoteke
#include "Classes.h"

//Konstruktor
Rect::Rect()
{
	//Default koordinate
	x = y = w = h = 0;
}

//Nastavlanje
void Rect::set(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

//Animacije
Animation::Animation()
{
	curTime = 0;
	frameTime = 0;
	curTexture = 0;
}

//Logika animacije
void Animation::tick()
{
	curTime += 60*getFPS();

	//Spremenimo texturo, ce je doloceno FPS poteklo
	if (curTime >= frameTime)
	{
		if (curTexture++ >= textures.size()+textures.at(0)-1)
		{
			curTexture = textures.at(0);
		}

		curTime = 0;
	}
}