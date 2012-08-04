//Zunanje datoteke
#include "Classes.h"

//Konstruktor
Player::Player()
{
	//Nastavitve
	b_x = b_left = b_right = b_jump = b_doubleJump = b_nextLevel = b_xReleased = b_dead = false;
	b_direction = true;

	colRect = new Rect();
	drawRect = new Rect();
	walkAnimation = new Animation();
	standAnimation = new Animation();
	jumpAnimation = new Animation();

	curAnimation = walkAnimation;

	//Hitrosti
	f_xSpeed = 0;
	f_ySpeed = 0;
	f_jumpSpeed = 350;

	drawRect->set(0, 0, 64, 64);
	colRect->set(28, 2, 10, 62);

	//Zacetne
	f_startX = f_startY = 0;

	//Smrt
	f_deadColor = 1;

	//Tocke
	f_score = 0;
}

//Destruktor
Player::~Player()
{
	delete colRect;
	delete drawRect;
	curAnimation = NULL;
	delete walkAnimation;
	delete standAnimation;
	delete jumpAnimation;
}

//Vnosi
void Player::handleInput()
{
	//Pritisk tipke
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_x: b_x = true; break;
		case SDLK_UP: b_x = true; break;
		case SDLK_LEFT: b_left = true; break;
		case SDLK_RIGHT: b_right = true; break;
		default: break;
		}
	}

	//Spust tipke
	if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_x: b_x = false; break;
		case SDLK_UP: b_x = false; break;
		case SDLK_LEFT: b_left = false; break;
		case SDLK_RIGHT: b_right = false; break;
		default: break;
		}
	}
}

//Logika
void Player::logic(vector<class Tile*> *tiles)
{
	//Delta
	float k = getFPS();
	float speed = 150;

	//Omejitev
	if (k > 0.3)
	{
		k = 0.3;
	}

	//Tipke levo desno
	if (!b_dead)
	{
		if (b_right == true && b_left == false)
		{
			f_xSpeed = speed;
			b_direction = true;
			if (!b_jump)
			{
				curAnimation = walkAnimation;
			}
		}
		else if (b_left == true && b_right == false)
		{
			f_xSpeed = -speed;
			b_direction = false;
			if (!b_jump)
			{
				curAnimation = walkAnimation;
			}
		}
		else
		{
			f_xSpeed = 0;
			if (!b_jump)
			{
				curAnimation = standAnimation;
			}
		}

		//Skok
		if (b_x && !b_jump)
		{
			f_ySpeed = -(200+(250*k));
			b_jump = true;
			curAnimation = jumpAnimation;

			//Zvok
			sound_engine->play2D(sound_jump);
		}

		//Podaljsek skoka
		else if (b_x && b_jump && f_ySpeed < 0)
		{
			f_ySpeed -= f_jumpSpeed*k;
			f_jumpSpeed -= 100*k;

			if (f_jumpSpeed < 0)
			{
				f_jumpSpeed = 0;
			}
		}
		//Dvojni skok
		else if (!b_x && b_jump && !b_xReleased)
		{
			b_xReleased = true;
		}

		if (b_x && b_jump && b_xReleased && !b_doubleJump)
		{
			f_ySpeed = -(200+(250*k));
			f_jumpSpeed = 250+(250*k);
			b_doubleJump = true;

			//Zvok
			sound_engine->play2D(sound_jump);
		}

	}
	//Gravitacija
	f_ySpeed += 600*k;

	
	if (f_ySpeed > 10)
	{
		b_jump = true;
	}
	
	//Omejitev
	if (f_ySpeed > 500)
	{
		f_ySpeed = 500;
	}

	//Fake collision box
	Rect* box = new Rect(*colRect);

	//PREMIK PO X
	float xMove = 0;
	
	//Premaknemo fake box
	box->x += f_xSpeed*k;

	//Preverimo kolizijo
	for (int n = 0; n < (signed)tiles->size(); n++)
	{
		//Block
		if (tiles->at(n)->type == BLOCK)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				if (f_xSpeed > 0)
				{
					box->x = tiles->at(n)->colRect->x-box->w;
				}
				else
				{
					box->x = tiles->at(n)->colRect->x+tiles->at(n)->colRect->w;
				}

				break;
			}
		}

		//Spikes
		if (tiles->at(n)->type == SPIKES || tiles->at(n)->type == SPIKESDOWN)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				if (!b_dead)
				{
					//Zvok
					sound_engine->play2D(sound_dead);
				}

				b_dead = true;
			}
		}

		//Coin
		if (tiles->at(n)->type == COIN)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				//Zvok
				sound_engine->play2D(sound_coin);

				f_score += 10;
				delete tiles->at(n);
				tiles->erase(tiles->begin()+n);
				break;
			}
		}

		//END
		if (tiles->at(n)->type == DOOR)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				b_nextLevel = true;
				break;
			}
		}
	}

	//PREMIK PO Y
	float yMove = 0;

	//Premaknemo
	box->y += f_ySpeed*k;
	

	//Preverimo kolizijo
	for (int n = 0; n < (signed)tiles->size(); n++)
	{
		//Block
		if (tiles->at(n)->type == BLOCK)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{	
				if (f_ySpeed > 0)
				{
					box->y = tiles->at(n)->colRect->y-box->h;
					f_ySpeed = 0;
					b_jump = false;
					f_jumpSpeed = 350+(350*k);
					b_doubleJump = false;
					b_xReleased = false;
					b_x = false;
				}
				else
				{
					box->y = tiles->at(n)->colRect->y+tiles->at(n)->colRect->h;
					f_ySpeed = 0;
				}
				break;
			}
		}

		//Spikes
		if (tiles->at(n)->type == SPIKES || tiles->at(n)->type == SPIKESDOWN)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				if (!b_dead)
				{
					//Zvok
					sound_engine->play2D(sound_dead);
				}

				b_dead = true;
			}
		}

		//Coin
		if (tiles->at(n)->type == COIN)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				//Zvok
				sound_engine->play2D(sound_coin);

				f_score += 10;
				delete tiles->at(n);
				tiles->erase(tiles->begin()+n);
				break;
			}
		}

		//End
		//END
		if (tiles->at(n)->type == DOOR)
		{
			if (checkCollision(box, tiles->at(n)->colRect))
			{
				b_nextLevel = true;
			}
		}
	}

	//Koncni premik
	xMove = box->x - colRect->x;
	yMove = box->y - colRect->y;

	if (!b_dead)
	{
		drawRect->x += xMove;
		drawRect->y += yMove;
		colRect->x += xMove;
		colRect->y += yMove;

		//Spreminjamo animacijo
		curAnimation->tick();
	}

	delete box;

	//Ven iz ekrana spodaj
	if (colRect->y > 480*3)
	{
		b_dead = true;
		f_deadColor = 0;
	}

	//Smrt
	if (b_dead)
	{
		b_direction = true;
		f_ySpeed = 0;
		f_deadColor -= 1*k;
	}

	
}

//Izris
void Player::render()
{
	//Izris
	glEnable(GL_TEXTURE_2D);
	glColor4f(f_deadColor, f_deadColor, f_deadColor, 1);
	glBindTexture(GL_TEXTURE_2D, curAnimation->curTexture);

	//Zavrtimo sliko ce se premikamo levo
	if (!b_direction)
	{
		glPushMatrix();
		glTranslatef((drawRect->x+drawRect->w/2),(drawRect->y+drawRect->h/2), 0);
		glRotatef(180, 0, 1, 0);
		glTranslatef(-(drawRect->x+drawRect->w/2)-2,-(drawRect->y+drawRect->h/2), 0);

	}

	//Izris
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(drawRect->x, drawRect->y);
	glTexCoord2f(1, 0); glVertex2f(drawRect->x+drawRect->w, drawRect->y);
	glTexCoord2f(1, 1); glVertex2f(drawRect->x+drawRect->w, drawRect->y+drawRect->h);
	glTexCoord2f(0, 1); glVertex2f(drawRect->x, drawRect->y+drawRect->h);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	if (!b_direction)
	{
		glPopMatrix();
	}

	//Colision box
	/*
	glLineWidth(2);
	glColor4f(1, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(colRect->x, colRect->y);
	glVertex2f(colRect->x+colRect->w, colRect->y);
	glVertex2f(colRect->x+colRect->w, colRect->y+colRect->h);
	glVertex2f(colRect->x, colRect->y+colRect->h);
	glEnd();
	*/
}