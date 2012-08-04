//Zunanje datoteke
#include "Classes.h"

//Konstruktor
EditorState::EditorState()
{
	//Ozadje
	glClearColor(1, 1, 1, 1);

	//Zacetne vrednosti
	b_mouseClick = false;
	b_mouseRightClick = false;
	b_workState = true;
	b_menuOpen = false;
	b_saving = false;
	b_loading = false;
	b_charAdded = false;
	b_backspace = false;
	b_enter = false;
	b_escape = false;
	b_mouseScrollDown = false;
	b_mouseScrollUp = false;
	curType = BLOCK;
	b_down = b_up = b_left = b_right = false;
	b_testing = false;

	c_tipka = '.';
	s_file = "";

	//Kamera
	cameraX = cameraY = 0;

	//State
	nextState = NONE;

}

//Destruktor
EditorState::~EditorState()
{
	//Pobrisemo trenutni level
	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		delete o_tiles.at(n);
	}

	o_tiles.clear();
}

//Dogodki
void EditorState::events()
{
	//Ce je bila klikjena miska
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		b_mouseClick = true;
	}

	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
	{
		b_mouseClick = false;
	}

	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
	{
		b_mouseRightClick = true;
	}

	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
	{
		b_mouseRightClick = false;
	}

	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELDOWN)
	{
		b_mouseScrollDown = true;
	}

	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELUP)
	{
		b_mouseScrollUp = true;
	}

	else if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_RETURN: b_enter = true; break;
		case SDLK_BACKSPACE: b_backspace = true; break;
		case SDLK_ESCAPE: b_escape = true; break;
		case SDLK_UP: b_up = true; break;
		case SDLK_DOWN: b_down = true; break;
		case SDLK_LEFT: b_left = true; break;
		case SDLK_RIGHT: b_right = true; break;
		case SDLK_r: b_r = true; break;
		}

		c_tipka = event.key.keysym.unicode;
		b_charAdded = true;
	}

	else if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_UP: b_up = false; break;
		case SDLK_DOWN: b_down = false; break;
		case SDLK_LEFT: b_left = false; break;
		case SDLK_RIGHT: b_right = false; break;
		case SDLK_r: b_r = false; break;
		}
	}

	//izhod
	if (event.type == SDL_QUIT)
	{
		nextState = EXIT;
	}
}

//Logika
void EditorState::logic()
{
	//Ce smo kliknoli
	if (b_mouseClick)
	{
		if (!b_menuOpen)
		{
			b_mouseClick = false;
			//Pozicija miske
			int x,y;
			SDL_GetMouseState(&x, &y);
			x += (int)cameraX;
			y += (int)cameraY;

			//Dodajamo objekt
			if (b_workState)
			{
				//Na grid
				x = x - x%64;
				y = y - y%64;


				Tile* obj = new Tile((float)x, (float)y, curType);

				//Preverimo ce ze obstaja objekt na tej poziciji
				for (int n = 0; n < (signed)o_tiles.size(); n++)
				{
					if (checkCollision(obj->colRect, o_tiles.at(n)->colRect))
					{
						delete obj;
						obj = NULL;
						break;
					}
				}

				//Ce ni bilo objekta
				if (obj != NULL)
				{
					//Preverimo za objekte, ki so samo lahko 1x
					if (curType == START || curType == END)
					{
						bool b_any = false;
						for (int n = 0; n < (signed)o_tiles.size(); n++)
						{
							if (o_tiles.at(n)->type == curType)
							{
								b_any = true;
								//Pobrisemo starega
								delete o_tiles.at(n);
								o_tiles.erase(o_tiles.begin()+n);

								//Dodamo novega
								o_tiles.push_back(obj);

								break;
							}
						}

						//Ce ni bilo nobenega
						if (!b_any)
						{
							o_tiles.push_back(obj);
						}
					}
					else
					{
						o_tiles.push_back(obj);
					}
				}
			}
			//Odstranimo objekt
			else
			{
				//Na grid
				x = x - x%64;
				y = y - y%64;

				Rect mouse;
				mouse.set((float)x, (float)y, 64, 64);

				//Preverimo kolizijo
				for (int n = 0; n < (signed)o_tiles.size(); n++)
				{
					if (checkCollision(&mouse, o_tiles.at(n)->colRect))
					{
						delete o_tiles.at(n);
						//Zbrisemo objekt
						o_tiles.erase(o_tiles.begin()+n);
					}
				}
			}
		}
		else
		{
			b_mouseClick = false;
			//Pozicija miske
			int x,y;
			SDL_GetMouseState(&x, &y);
			x += (int)cameraX;
			y += (int)cameraY;

			//Mozne tipke
			Rect tipke[5];
			for (int x = 0+(int)cameraX, y = 448+(int)cameraY, n = 0; n < 5; x+=128, n++)
			{
				tipke[n].x = (float)x;
				tipke[n].y = (float)y;
				tipke[n].w = 128;
				tipke[n].h = 32;
			}

			//Preverimo kam smo kliknili
			Rect miska;
			miska.set((float)x, (float)y, 1, 1);

			for (int n = 0; n < 5; n++)
			{
				if (checkCollision(&miska, &tipke[n]))
				{
					//Zvok
					sound_engine->play2D(sound_select);

					//Delo odvisno od tipke
					switch(n)
					{
					case 0: b_workState = true; b_menuOpen = false; break;
					case 1: b_workState = false; curType = BLOCK; b_menuOpen = false; break;
					case 2: b_testing = true; b_menuOpen = false; break;
					case 3: b_saving = true; break;
					case 4: b_loading = true; break;
					default: break;
					}
				}
			}
		}
	}

	//Ce shranjujemo ali loadamo
	if (b_saving || b_loading)
	{	
		//Preprecimo reset
		b_r = false;

		//Ce je tipka ok
		if ((c_tipka & (0xFF80)) == 0 && b_charAdded)
		{
			char tipka = (char)(c_tipka & 0x7F);

			//Ce je trenutnoa tipka stevilka ali crka
			if ((tipka >= 'a' && tipka <= 'z') || (tipka >= '0' && tipka <= '9')) 
			{
				//Ce je beseda dovolj dolga
				if (s_file.size() < 9)
				{				
					//Dodamu k imenu
					string temp = " ";
					temp.at(0) = tipka;
		
					s_file += temp;
				}
			}
		}

		//Ce brisemo znak
		if (b_backspace)
		{
			b_backspace = false;

			//Brisemo samo ce imamo vsaj 1 znak
			if (s_file.size() >= 1)
			{
				s_file.pop_back();
			}
		}

		//Koncano
		else if (b_enter)
		{
			b_enter = false;

			//Shranimo / nalozimo
			if (b_saving)
			{
				saveLevel(s_file);
			}
			else
			{
				loadLevel(s_file);
			}

			s_file = "";

			//Koncamo
			b_saving = false;
			b_loading = false;
		}
		
		//Prekinitev
		else if (b_escape)
		{
			b_escape = false;

			s_file = "";

			//Koncamo
			b_saving = false;
			b_loading = false;
		}
	}
	b_charAdded = false;

	//Ce je desni klik
	if (b_mouseRightClick)
	{
		b_mouseRightClick = false;

		//Odpremo/zapremo meni
		if (b_menuOpen && (!b_loading && !b_saving))
			b_menuOpen = false;
		else
			b_menuOpen = true;
	}

	//Ce je scroll
	if (b_workState)
	{
		if (b_mouseScrollDown)
		{
			b_mouseScrollDown = false;

			int t = (int)curType;
			t++;

			if (t > (int)END)
			{
				curType = BLOCK;
			}
			else
			{
				curType = (TILE_TYPE)t;
			}
		}
		else if (b_mouseScrollUp)
		{
			b_mouseScrollUp = false;

			int t = (int)curType;
			t--;

			if (t < (int)BLOCK)
			{
				curType = END;
			}
			else
			{
				curType = (TILE_TYPE)t;
			}
		}
	}

	//Pritisk smernih tipk za premik kamere
	float cameraMove = 200, k = getFPS();
	if (b_left)
	{
		cameraX -= cameraMove*k;
	}
	if (b_right)
	{
		cameraX += cameraMove*k;
	}
	if (b_up)
	{
		cameraY -= cameraMove*k;
	}
	if (b_down)
	{
		cameraY += cameraMove*k;
	}

	//Limit kamere
	if (cameraX < 0)
	{
		cameraX = 0;
	}
	else if (cameraX > 640*2)
	{
		cameraX = 640*2;
	}
	if (cameraY < 0)
	{
		cameraY = 0;
	}
	else if (cameraY > 480*2)
	{
		cameraY = 480*2;
	}

	//Pritisk R, za brisanje levela
	if (b_r)
	{
		for (int n = 0; n < (signed)o_tiles.size(); n++)
		{
			delete o_tiles.at(n);
		}

		o_tiles.clear();

		b_r = false;
	}

	//Ce testiramo level
	if (b_testing)
	{
		LevelState* test = new LevelState(&o_tiles);

		//Ponastavimo fps
		setFPS();

		//Simuliramo igranje
		while (test->nextState == NULL)
		{
			while (SDL_PollEvent(&event))
			{
				test->events();
			}

			test->logic();

			setFPS();

			glClear(GL_COLOR_BUFFER_BIT);

			test->render();

			SDL_GL_SwapBuffers();

			delayFPS();
		}

		//Preverimo X
		if (test->nextState == EXIT)
		{
			nextState = EXIT;
		}

		//Pobrisemo
		delete test;

		b_testing = false;
	}

	//Preverimo Escape
	if (b_escape)
	{
		//Vrnemo v meni
		nextState = MENU;
		b_escape = false;
	}
}

//Shranimo sobo
void EditorState::saveLevel(const string levelName)
{
	//Odpremo datoteko
	ofstream file;
	file.open("Data/Levels/"+levelName+".level");

	//Gremo skozi vse tile
	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		switch(o_tiles.at(n)->type)
		{
		case BLOCK: file << "BLOCK "; break;
		case SPIKES: file << "SPIKES "; break;
		case START: file << "START "; break;
		case END: file << "END "; break;
		case COIN: file << "COIN "; break;
		case SPIKESDOWN: file << "SPIKESDOWN "; break;
		}

		file << o_tiles.at(n)->drawRect->x << " " << o_tiles.at(n)->drawRect->y << "\n";
	}

	//Manual EOF
	file << "EOF";

	//Zapremo datoteko
	file.close();
}

//Load level
void EditorState::loadLevel(const string levelName)
{
	//Odpremo datoteko
	ifstream file;
	file.open("Data/Levels/"+levelName+".level");
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

	//Najdemo prvi objekt in postavimo kamero
	if (o_tiles.size() > 0)
	{
		cameraX = o_tiles.at(0)->drawRect->x;
		cameraY = o_tiles.at(0)->drawRect->y;
	}
}

//Risanje
void EditorState::render()
{
	glPushMatrix();

	glOrtho(0, 640, 480, 0, -1, 1);

	//Kamera
	glTranslatef(-cameraX, -cameraY, 0);

	//Grid
	glColor4f(0, 0, 0, 0.3);
	glLineWidth(2);

	glBegin(GL_LINES);
	for (int x = 0; x < 640*3; x += 64)
	{
		glVertex2i(x,0);
		glVertex2i(x,480*3);
	}
	for (int y = 0; y < 480*3; y += 64)
	{
		glVertex2i(0,y);
		glVertex2i(640*3,y);
	}
	glEnd();

	for (int n = 0; n < (signed)o_tiles.size(); n++)
	{
		o_tiles.at(n)->render(1,true);
	}
	
	
	//Ce je menu odprt ga prikazemo
	if (b_menuOpen)
	{
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, 10);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0+cameraX, 448+cameraY);
		glTexCoord2f(1, 0); glVertex2f(640+cameraX, 448+cameraY);
		glTexCoord2f(1, 1); glVertex2f(640+cameraX, 480+cameraY);
		glTexCoord2f(0, 1); glVertex2f(0+cameraX, 480+cameraY);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	//Prikaz objekta
	if (b_workState && !b_menuOpen)
	{
		int x,y;
		SDL_GetMouseState(&x, &y);
		x += (int)cameraX;
		y += (int)cameraY;

		x -= x%64;
		y -= y%64;
		//Temp objekt
		Tile* obj = new Tile((float)x, (float)y, curType);
		obj->render(0.5,true);

		delete obj;
	}

	//Ce brisemo
	if (!b_workState && !b_menuOpen)
	{
		int x,y;
		SDL_GetMouseState(&x, &y);
		x += (int)cameraX;
		y += (int)cameraY;
		
		x -= x%64;
		y -= y%64;
		//Temp objekt
		Tile* obj = new Tile((float)x, (float)y, curType);
		obj->texture = 14;
		obj->render(1,true);

		delete obj;
	}

	//Prikazemo txt ko shranjujemo/loadamo
	if (b_saving || b_loading)
	{
		//Rect
		glColor4ub(200, 200, 100, 255);

		glBegin(GL_QUADS);
		glVertex2f(200+cameraX, 200+cameraY);
		glVertex2f(440+cameraX, 200+cameraY);
		glVertex2f(440+cameraX, 280+cameraY);
		glVertex2f(200+cameraX, 280+cameraY);
		glEnd();

		//Lines
		glColor4f(0, 0, 1, 1);
		glLineWidth(5);

		glBegin(GL_LINE_LOOP);
		glVertex2f(200+cameraX, 200+cameraY);
		glVertex2f(440+cameraX, 200+cameraY);
		glVertex2f(440+cameraX, 280+cameraY);
		glVertex2f(200+cameraX, 280+cameraY);
		glEnd();

		renderText(s_file, font, 40, 210+cameraX, 230+cameraY);
	}

	glPopMatrix();
}