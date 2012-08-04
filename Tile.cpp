//Zunanje datoteke
#include "Classes.h"

//Konstruktor
Tile::Tile(float x, float y, TILE_TYPE type)
{
	colRect = new Rect();
	drawRect = new Rect();
	this->type = type;
	
	//Nastavimo zacetne vrednosti odvisno od tipa
	if (type == BLOCK)
	{
		texture = 9;
		drawRect->set(x, y, 64, 64);
		colRect->set(x, y, 64, 64);
	}
	else if (type == SPIKES)
	{
		texture = 11;
		drawRect->set(x, y, 64, 64);
		colRect->set(x+6,y+52,52,10);
	}
	else if (type == SPIKESDOWN)
	{
		texture = 11;
		drawRect->set(x, y, 64, 64);
		colRect->set(x+6,y,52,10);
	}
	else if (type == START)
	{
		texture = 12;
		drawRect->set(x, y, 64, 64);
		colRect->set(x, y, 64, 64);
	}
	else if (type == END)
	{
		texture = 13;
		drawRect->set(x, y, 64, 64);
		colRect->set(x, y, 64, 64);
	}
	else if (type == COIN)
	{
		texture = 19;
		drawRect->set(x, y, 64, 64);
		colRect->set(x+18, y+18, 26, 26);
	}

	this->type = type;
}

//Destruktor
Tile::~Tile()
{
	delete colRect;
	delete drawRect;
}

//Risanje
void Tile::render(float a, bool box)
{
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, a);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Obrnjeni spikes
	if (type == SPIKESDOWN)
	{
		glPushMatrix();
		glTranslatef((drawRect->x+drawRect->w/2), (drawRect->y+drawRect->h/2), 0);
		glRotatef(180, 1, 0, 0);
		glTranslatef(-(drawRect->x+drawRect->w/2), -(drawRect->y+drawRect->h/2), 0);
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(drawRect->x, drawRect->y);
	glTexCoord2f(1, 0); glVertex2f(drawRect->x+drawRect->w, drawRect->y);
	glTexCoord2f(1, 1); glVertex2f(drawRect->x+drawRect->w, drawRect->y+drawRect->h);
	glTexCoord2f(0, 1); glVertex2f(drawRect->x, drawRect->y+drawRect->h);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//
	if (type == SPIKESDOWN)
	{
		glPopMatrix();
	}

	if (box)
	{
		//Colision boxes
		glLineWidth(2);
		glColor4f(1, 0, 0, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(colRect->x, colRect->y);
		glVertex2f(colRect->x+colRect->w, colRect->y);
		glVertex2f(colRect->x+colRect->w, colRect->y+colRect->h);
		glVertex2f(colRect->x, colRect->y+colRect->h);
		glEnd();
	}
}