//Zunanje datoteke
#include "Classes.h"

LevelState::LevelState(bool mode)
{
	nextState = NONE;

	//Objekti
	//Igralec
	o_player = new Player();

	//Hoja Animacija
	o_player->walkAnimation->textures.push_back(1);
	o_player->walkAnimation->textures.push_back(2);
	o_player->walkAnimation->textures.push_back(3);
	o_player->walkAnimation->textures.push_back(4);
	o_player->walkAnimation->textures.push_back(5);
	o_player->walkAnimation->textures.push_back(6);
	o_player->walkAnimation->textures.push_back(7);
	o_player->walkAnimation->textures.push_back(8);
	o_player->walkAnimation->curTexture = 1;

	o_player->walkAnimation->frameTime = 5;

	//Stand animacija
	o_player->standAnimation->textures.push_back(16);
	o_player->standAnimation->textures.push_back(17);

	o_player->standAnimation->curTexture = 16;
	o_player->standAnimation->frameTime = 50;


	//Jump animacija
	o_player->jumpAnimation->textures.push_back(18);
	o_player->jumpAnimation->curTexture = 18;

	//kamera
	cameraY = 0;
	cameraX = 0;

	//Bools
	b_escape = b_r = b_test = false;

	//Preskok
	f_switch = 1;

	//Tocke
	f_score = 0;

	//Zmaga
	b_win = false;

	//Ce je continue
	if (mode)
	{
		//Odpremo datoteko
		ifstream file;
		file.open("Data/Levels/game.save");

		//Ce je odpreto
		if (file.is_open())
		{
			//Pobrisemo trenutni level
			for (int n = 0; n < (signed)o_tiles.size(); n++)
			{
				delete o_tiles.at(n);
			}

			o_tiles.clear();

			string line;

			//Beremo score
			file >> o_player->f_score;
			f_score = o_player->f_score;

			//Beremo
			while (true)
			{
				file >> line;

				if (line == "EOF")
				{
					break;
				}
				else
				{
					s_levels.push_back(line);
				}
			}
		}

		//Ce ni datoteke, potem nastavimo novo igro
		else
		{
			mode = false;
		}

		file.close();
	}

	//Ce je nova igra
	if (!mode)
	{
		//Imena sob
		//Odpremo datoteko
		ifstream file;
		file.open("Data/Levels/levels.levels");

		//Ce je odpreto
		if (file.is_open())
		{
			//Pobrisemo trenutni level
			for (int n = 0; n < (signed)o_tiles.size(); n++)
			{
				delete o_tiles.at(n);
			}

			o_tiles.clear();

			string line;

			//Beremo
			while (true)
			{
				file >> line;

				if (line == "EOF")
				{
					break;
				}
				else
				{
					s_levels.push_back(line);
				}
			}
		}

		file.close();
		////////////////
	}
}

//Konstuktor za test level
LevelState::LevelState(vector<class Tile*> *tiles)
{
	nextState = NONE;

	//Objekti
	//Igralec
	o_player = new Player();

	//Hoja Animacija
	o_player->walkAnimation->textures.push_back(1);
	o_player->walkAnimation->textures.push_back(2);
	o_player->walkAnimation->textures.push_back(3);
	o_player->walkAnimation->textures.push_back(4);
	o_player->walkAnimation->textures.push_back(5);
	o_player->walkAnimation->textures.push_back(6);
	o_player->walkAnimation->textures.push_back(7);
	o_player->walkAnimation->textures.push_back(8);
	o_player->walkAnimation->curTexture = 1;

	o_player->walkAnimation->frameTime = 5;

	//Stand animacija
	o_player->standAnimation->textures.push_back(16);
	o_player->standAnimation->textures.push_back(17);

	o_player->standAnimation->curTexture = 16;
	o_player->standAnimation->frameTime = 50;


	//Jump animacija
	o_player->jumpAnimation->textures.push_back(18);
	o_player->jumpAnimation->curTexture = 18;

	//kamera
	cameraY = 0;
	cameraX = 0;

	//Bools
	b_escape = b_r = false;
	b_test = true;
	b_win = false;

	//Kopiramo tile z level editora
	for (int n = 0; n < (signed)tiles->size(); n++)
	{
		o_tiles.push_back(new Tile(tiles->at(n)->drawRect->x, tiles->at(n)->drawRect->y, tiles->at(n)->type));
	}

	//najdemo start block in end block
	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		if (o_tiles.at(n)->type == START)
		{
			o_player->drawRect->set(o_tiles.at(n)->drawRect->x, o_tiles.at(n)->drawRect->y, 64, 64);
			o_player->colRect->set(o_tiles.at(n)->drawRect->x+28, o_tiles.at(n)->drawRect->y+2, 10, 62);
			o_player->f_startX = o_tiles.at(n)->drawRect->x;
			o_player->f_startY = o_tiles.at(n)->drawRect->y;

			delete o_tiles.at(n);
			o_tiles.erase(o_tiles.begin()+n);
			n--;
		}
		else if (o_tiles.at(n)->type == END)
		{
			o_tiles.at(n)->type = DOOR;
			o_tiles.at(n)->texture = 15;
		}
	}
}

//Destruktor
LevelState::~LevelState()
{
	delete o_player;

	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		delete o_tiles.at(n);
	}

	o_tiles.clear();

	for (int n = 0; n < s_levels.size(); n++)
	{
		s_levels.at(n).clear();
	}
	s_levels.clear();
}

//Dogodki
void LevelState::events()
{
	//izhod
	if (event.type == SDL_QUIT)
	{
		nextState = EXIT;
	}

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
	{
		b_escape = true;
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
	{
		b_r = true;
	}

	//Igralec
	o_player->handleInput();
}

//Logika
void LevelState::logic()
{
	//Nalozimo prvi level
	if (o_tiles.size() == 0)
	{
		if (s_levels.size() >= 1)
		{
			loadLevel(s_levels.at(0));

			//najdemo start block in end block
			for (int n = 0; n < (signed)o_tiles.size(); n++)
			{
				if (o_tiles.at(n)->type == START)
				{
					o_player->drawRect->set(o_tiles.at(n)->drawRect->x, o_tiles.at(n)->drawRect->y, 64, 64);
					o_player->colRect->set(o_tiles.at(n)->drawRect->x+28, o_tiles.at(n)->drawRect->y+2, 10, 62);
					o_player->f_startX = o_tiles.at(n)->drawRect->x;
					o_player->f_startY = o_tiles.at(n)->drawRect->y;

					delete o_tiles.at(n);
					o_tiles.erase(o_tiles.begin()+n);
					n--;
				}
				else if (o_tiles.at(n)->type == END)
				{
					o_tiles.at(n)->type = DOOR;
					o_tiles.at(n)->texture = 15;
				}
			}
		}
	}

	//Igralec
	if (!o_player->b_nextLevel || b_test)
	{
		o_player->logic(&o_tiles);
	}

	//Ce je igralec na koncu
	if (o_player->b_nextLevel)
	{
		f_switch -= 1 * getFPS();
	}

	//Ce je nasledni level
	if (o_player->b_nextLevel && f_switch <= 0)
	{
		//Prekinemo prehod
		f_switch = 1;

		if (s_levels.size() >= 2)
		{
			//Pobrisemo trenutni level
			for (int n = 0; n < (signed)o_tiles.size(); n++)
			{
				delete o_tiles.at(n);
			}

			o_tiles.clear();

			o_player->b_nextLevel = false;
			//Shranimo tocke
			f_score = o_player->f_score;

			//Nalozimo nasledni level
			s_levels.erase(s_levels.begin());
			loadLevel(s_levels.at(0));

			//najdemo start block in end block
			for (int n = 0; n < (signed)o_tiles.size(); n++)
			{
				if (o_tiles.at(n)->type == START)
				{
					//nastavimo igralca
					o_player->drawRect->set(o_tiles.at(n)->drawRect->x, o_tiles.at(n)->drawRect->y, 64, 64);
					o_player->colRect->set(o_tiles.at(n)->drawRect->x+28, o_tiles.at(n)->drawRect->y+2, 10, 62);
					o_player->f_startX = o_tiles.at(n)->drawRect->x;
					o_player->f_startY = o_tiles.at(n)->drawRect->y;

					delete o_tiles.at(n);
					o_tiles.erase(o_tiles.begin()+n);
					n--;
				}
				else if (o_tiles.at(n)->type == END)
				{
					o_tiles.at(n)->type = DOOR;
					o_tiles.at(n)->texture = 15;
				}
			}

			//Shranimo napredek
			//Odpremo datoteko
			ofstream file;
			file.open("Data/Levels/game.save");

			//Zapisemo score
			file << o_player->f_score << '\n';

			//Zapisemo preistale sobe
			for (int n = 0; n < (signed)s_levels.size(); n++)
			{
				file << s_levels.at(n) << '\n';
			}

			//Konec datoteke
			file << "EOF";

			//Zapremo
			file.close();
		}

		//Zmaga
		else if (!b_test)
		{
			b_win = true;
		}
	}

	//Ce je igralec umrl
	if (o_player->b_dead && o_player->f_deadColor <= 0)
	{
		b_r = true;
		o_player->b_dead = false;
		o_player->f_deadColor = 1;
	}

	//Ce je restart
	if (b_r && !b_test && !b_win)
	{
		b_r = false;

		//Pobrisemo trenutni level
		for (int n = 0; n < (signed)o_tiles.size(); n++)
		{
			delete o_tiles.at(n);
		}

		o_tiles.clear();

		

		//Nalozimo trenutni level ponovno
		loadLevel(s_levels.at(0));

		//najdemo start block in end block
		for (int n = 0; n < (signed)o_tiles.size(); n++)
		{
			if (o_tiles.at(n)->type == START)
			{
				//nastavimo igralca
				o_player->drawRect->set(o_tiles.at(n)->drawRect->x, o_tiles.at(n)->drawRect->y, 64, 64);
				o_player->colRect->set(o_tiles.at(n)->drawRect->x+28, o_tiles.at(n)->drawRect->y+2, 10, 62);
				o_player->f_startX = o_tiles.at(n)->drawRect->x;
				o_player->f_startY = o_tiles.at(n)->drawRect->y;

				delete o_tiles.at(n);
				o_tiles.erase(o_tiles.begin()+n);
				n--;
			}
			else if (o_tiles.at(n)->type == END)
			{
				o_tiles.at(n)->type = DOOR;
				o_tiles.at(n)->texture = 15;
			}
		}

		//Znizamo score za 10
		f_score -= 10;

		//Ponastavimo tocke
		o_player->f_score = f_score;

	}
	else if (b_r)
	{
		nextState = MENU;
	}

	//Ce je escape
	if (b_escape)
	{
		b_escape = false;
		nextState = MENU;
	}

	//Premaknemo kamero
	cameraX = (o_player->drawRect->x - 320 + 32);
	cameraY = (o_player->drawRect->y - 240 + 32);
}

//Render
void LevelState::render()
{
	glPushMatrix();
	glOrtho(0, 640, 480, 0, -1, 1);

	//Kamera
	glTranslatef(-cameraX, -cameraY, 0);

	//Ozadje
	glColor4f(0.3,0.8,0.4,1);
	glBegin(GL_QUADS);
	glVertex2f(0+cameraX, 0+cameraY);
	glVertex2f(640+cameraX, 0+cameraY);
	glColor4f(0.8,0.7,0.8,1);
	glVertex2f(640+cameraX, 480+cameraY);
	glVertex2f(0+cameraX, 480+cameraY);
	glEnd();

	//Izris objektov
	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		o_tiles.at(n)->render();
	}

	//izris igralca
	o_player->render();

	//Tocke
	//Rect za tockami
	glColor4f(0.9, 0.9, 0.9, 0.7);
	glBegin(GL_QUADS);
	glVertex2f(10+cameraX, 6+cameraY);
	glVertex2f(210+cameraX, 6+cameraY);
	glVertex2f(210+cameraX, 35+cameraY);
	glVertex2f(10+cameraX, 35+cameraY);
	glEnd();

	stringstream score;
	score << "Score: " << o_player->f_score;
	renderText(score.str(), font, 30, 10+cameraX, 10+cameraY);

	//Med leveli
	if (o_player->b_nextLevel && !b_test)
	{
		glColor4f(f_switch, f_switch, f_switch, 1-f_switch);
		glBegin(GL_QUADS);
		glVertex2f(0+cameraX, 0+cameraY);
		glVertex2f(640+cameraX, 0+cameraY);
		glVertex2f(640+cameraX, 480+cameraY);
		glVertex2f(0+cameraX, 480+cameraY);
		glEnd();
	}

	//Win screen
	if (b_win)
	{
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 21);

		//Narisemo ozadje
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0+cameraX, 0+cameraY);
		glTexCoord2f(1, 0); glVertex2f(640+cameraX, 0+cameraY);
		glTexCoord2f(1, 1); glVertex2f(640+cameraX, 480+cameraY);
		glTexCoord2f(0, 1); glVertex2f(0+cameraX, 480+cameraY);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		//Score
		stringstream tocke;
		tocke << o_player->f_score;

		renderText(tocke.str(), font, 40, 400+cameraX, 340+cameraY);
		
	}

	glPopMatrix();
}

//Load level
void LevelState::loadLevel(const string levelName)
{
	//Odpremo datoteko
	ifstream file;
	file.open("Data/Levels/"+levelName);

	//Ce je odpreto
	if (file.is_open())
	{
		//Pobrisemo trenutni level
		for (int n = 0; n < (signed)o_tiles.size(); n++)
		{
			delete o_tiles.at(n);
		}

		o_tiles.clear();

		string type;

		//Beremo
		while(true)
		{
			//Preberemo tip
			file >> type;

			//Shranimo odvisno od tipa
			if (type == "BLOCK")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, BLOCK));
			}
			else if (type == "SPIKES")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, SPIKES));
			}
			else if (type == "SPIKESDOWN")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, SPIKESDOWN));
			}
			else if (type == "START")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, START));
			}
			else if (type == "END")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, END));
			}
			else if (type == "COIN")
			{
				float x,y;
				file >> x;
				file >> y;

				o_tiles.push_back(new Tile(x, y, COIN));
			}
			else if (type == "EOF")
			{
				break;
			}
		}
	}

	//Zapremo datoteko
	file.close();
}