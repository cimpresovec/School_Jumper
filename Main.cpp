/*
Izdelek za 4. izpitno nalogo; Igra
Avtor: Luka Horvat
*/

//Zunanje datoteke
#include "Classes.h"

//Globalni objekt za delo z dogodki
SDL_Event event;

//Za FPS
float FPS_A = 0;

//Globalni zvoki in engine
irrklang::ISoundEngine *sound_engine = NULL;
irrklang::ISoundSource *sound_jump = NULL;
irrklang::ISoundSource *sound_coin = NULL;
irrklang::ISoundSource *sound_dead = NULL;
irrklang::ISoundSource *sound_select = NULL;
irrklang::ISoundSource *sound_music = NULL;

FTGLPixmapFont* font = NULL;

//Zacetek programa
int main(int argc, char* args[])
{
	//Postavimo okno
	if (!initilize())
	{
		cout << "Napaka pri zagonu\n";
		return 1;
	}

	//Nalozimo zvoke
	sound_coin = sound_engine->addSoundSourceFromFile("Data/Sfx/coin.wav", irrklang::ESM_AUTO_DETECT, true);
	sound_jump = sound_engine->addSoundSourceFromFile("Data/Sfx/jump.wav", irrklang::ESM_AUTO_DETECT, true);
	sound_dead = sound_engine->addSoundSourceFromFile("Data/Sfx/dead.wav", irrklang::ESM_AUTO_DETECT, true);
	sound_select = sound_engine->addSoundSourceFromFile("Data/Sfx/select.wav", irrklang::ESM_AUTO_DETECT, true);
	sound_music = sound_engine->addSoundSourceFromFile("Data/Music/Offenbach_Project-Paris_at_night.mp3", irrklang::ESM_AUTO_DETECT, true);

	//Nalozimo slike
	loadTexture("Data/Images/drofWalk0000.png"); //1
	loadTexture("Data/Images/drofWalk0001.png");
	loadTexture("Data/Images/drofWalk0002.png");
	loadTexture("Data/Images/drofWalk0003.png");
	loadTexture("Data/Images/drofWalk0004.png");
	loadTexture("Data/Images/drofWalk0005.png");
	loadTexture("Data/Images/drofWalk0006.png");
	loadTexture("Data/Images/drofWalk0007.png"); //8
	loadTexture("Data/Images/tile0000.png"); //9
	loadTexture("Data/Images/editormenu.png"); //10
	loadTexture("Data/Images/spikes.png"); //11
	loadTexture("Data/Images/playerstart.png"); //12
	loadTexture("Data/Images/playerend.png"); //13
	loadTexture("Data/Images/delete.png"); //14
	loadTexture("Data/Images/door.png"); //15
	loadTexture("Data/Images/drofStand0000.png"); //16
	loadTexture("Data/Images/drofStand0001.png"); //17
	loadTexture("Data/Images/jump0000.png"); //18
	loadTexture("Data/Images/coin0000.png"); //19
	loadTexture("Data/Images/Ozadje.png"); //20
	loadTexture("Data/Images/win.png"); //21
	loadTexture("Data/Images/info.png"); //22

	//Nalozimo font
	font = new FTGLPixmapFont("Data/Font/FreeMono.ttf");

	//Kreiramo stanje igre
	GameState* state = new MenuState();

	//Nastavimo timer za FPS
	setFPS();

	//Zazenemo glasbo
	sound_engine->play2D(sound_music, true, false, false, true);
	sound_engine->setSoundVolume(sound_engine->getSoundVolume()-0.5);

	//Glavna zanga igre
	bool b_play = true;
	while (b_play)
	{
		//Menjamo state
		switch (state->nextState)
		{
		case GAME:
			{
				delete state;
				state = new LevelState();
				break;
			}
		case CONTINUE:
			{
				delete state;
				state = new LevelState(true);
				break;
			}
		case EDITOR:
			{
				delete state;
				state = new EditorState();
				break;
			}
		case MENU:
			{
				delete state;
				state = new MenuState();
				break;
			}
		case EXIT: b_play = false; break;
		default: break;
		}

		//Events, dogodki
		while (SDL_PollEvent(&event))
		{
			state->events();
		}

		//Logika
		state->logic();

		//Ponastavimo timer za FPS
		setFPS();

		//Risanje
		//Pocistimo okno
		glClear(GL_COLOR_BUFFER_BIT);

		state->render();

		SDL_GL_SwapBuffers();

		//Pocakamo za FPS
		delayFPS();
	}
	
	//Zapremo SDL
	SDL_Quit();

	//Zbrisemo zvoke
	sound_engine->removeAllSoundSources();
	delete sound_engine;

	//Zbrisemo texture
	unsigned int i = 1;
	for (int n = 1; n <= 22; n++)
	{
		i = n;
		glDeleteTextures(1, &i);
	}

	//Konec programa
	return 0;
}