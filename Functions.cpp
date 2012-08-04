//Zunanje datoteke
#include "Classes.h"

//Funkcija za postavitev okna in zacetne nastavitve
bool initilize()
{
	//Zazenemo SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	//Nastavimo OpenGL parametre za okno
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Nastavimo ime okna
	SDL_WM_SetCaption("School Jumper", NULL);

	//Kreiramo okno
	SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

	//OpenGL
	//Clear barva (bela)
	glClearColor(0.7,0.7,0.7,1);

	//Shader
	glShadeModel(GL_SMOOTH);

	//ViewPort - kaj prikazujemo
	glViewport(0, 0, 640, 480);

	//Matrica (koordinatni sistem)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Blendanje
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	SDL_EnableUNICODE( SDL_ENABLE ); 

	//Zazenemo sound engine
	sound_engine = irrklang::createIrrKlangDevice();

	return true;
}

//Funkcije za delo z FPS
//Pridobitev kolicnika za mnozenje za gibanje na slicico
float getFPS()
{
	return ( ( SDL_GetTicks() - FPS_A ) / 1000.f );
}

//Reset FPS timera
void setFPS()
{
	FPS_A = (float)SDL_GetTicks();
}

//Zaustavimo program, da se drzimo standarda 60 FPS
void delayFPS()
{
	if ( (SDL_GetTicks() - FPS_A) < 1000.00/60.00 )
	{
		SDL_Delay( (Uint32)(( (1000.f/60.f) ) - ((float)SDL_GetTicks() - FPS_A )) );
	}
}

unsigned int loadTexture(const string fileName)
{
	//Load surface
	SDL_Surface *image = IMG_Load( fileName.c_str() );

	if ( !image ){
		std::cout << "Problem reading image " << fileName.c_str() << "!\n";
		return 0;
	}

	//Set display format
	SDL_DisplayFormatAlpha(image);

	//Create texture
	unsigned object(0);

	glGenTextures(1, &object);

	glBindTexture(GL_TEXTURE_2D, object);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

	//Free surface
	SDL_FreeSurface(image);

	return object;
}

//Funkcija za preverjanje prekrivanja
bool checkCollision(Rect* A, Rect* B)
{
	if (A->x + A->w <= B->x) return false; //Prvi pravokotnik bolj levo od drugega
	else if (A->x >= B->x + B->w) return false; //Prvi pravokotnik bolj desno od drugega
	else if (A->y + A->h <= B->y) return false; //Prvi pravokotnik višje od drugega
	else if (A->y >= B->y + B->h) return false; //Prvi pravokotnik nižje od drugega
	return true; //Pravokotnika se prekrivata
}

//Funkcija za prikaz texta
void renderText(const std::string text, FTGLPixmapFont* font, int size, float x, float y)
{
	if ( font->FaceSize() != size )
		font->FaceSize(size);

	glPushMatrix();

	glColor4ub(0, 0, 0, 255);
		
	float height = (float)(font->BBox(text.c_str()).Upper().Y() - font->BBox(text.c_str()).Lower().Y());

	glRasterPos2f(x,y+height);

	font->Render(text.c_str());

	glPopMatrix();
}