//Zunanje datoteke
#include "Classes.h"

//Konstruktor
MenuState::MenuState()
{
	nextState = NONE;

	b_info = false;
	b_mouseClick = false;

	//Objekti
	tipke.push_back(Rect());
	tipke.push_back(Rect());
	tipke.push_back(Rect());
	tipke.push_back(Rect());

	//Koordiante gumbov
	tipke[0].set(230, 200, 180, 30);
	tipke[1].set(230, 270, 180, 30);
	tipke[2].set(220, 340, 200, 35);
	tipke[3].set(230, 400, 180, 30);
}

//Destruktor
MenuState::~MenuState()
{

}

//Dogodki
void MenuState::events()
{
	//Levi klik miske
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		b_mouseClick = true;
	}

	//kaj koli sprosceno
	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
	{
		if (b_info)
		{
			nextState = GAME;
		}
	}

	//izhod
	if (event.type == SDL_QUIT)
	{
		nextState = EXIT;
	}
}

//Logika
void MenuState::logic()
{
	//Klik miske
	if (b_mouseClick)
	{
		b_mouseClick = false;	

		//Pozicija miske
		int x,y;
		SDL_GetMouseState(&x, &y);
		Rect mouse;
		mouse.set((float)x, (float)y, 1, 1);

		//Check menus
		for (int n = 0; n < 4; n++)
		{
			if (checkCollision(&mouse, &tipke.at(n)))
			{
				//Zvok
				sound_engine->play2D(sound_select);

				switch (n)
				{
				case 0: b_info = true; break;
				case 1: nextState = CONTINUE; break;
				case 2: nextState = EDITOR; break;
				case 3: nextState = EXIT; break;
				}

				break;
			}
		}
	}
}

//Risanje
void MenuState::render()
{
	glPushMatrix();

	glOrtho(0, 640, 480, 0, -1, 1);

	glPushMatrix();

	//Background
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 20);

	//Narisemo ozadje
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(640, 0);
	glTexCoord2f(1, 1); glVertex2f(640, 480);
	glTexCoord2f(0, 1); glVertex2f(0, 480);
	glEnd();

	glPopMatrix();

	//Moje ime
	renderText("by Luka Horvat", font, 30, 380, 450);

	if (b_info)
	{
		glPushMatrix();

		glBindTexture(GL_TEXTURE_2D, 22);
		glColor4d(1, 1, 1, 1);

		//Narisemo info po potrebi
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(640, 0);
		glTexCoord2f(1, 1); glVertex2f(640, 480);
		glTexCoord2f(0, 1); glVertex2f(0, 480);
		glEnd();

		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}