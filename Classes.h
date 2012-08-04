#ifndef CLASSES_H
#define CLASSES_H

//Zunanje datoteke
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <FTGL\ftgl.h>
#include <irrKlang.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//Globalni objekt za delo z dogodki
extern SDL_Event event; 

//Globalni objekti za delo z FPS(slicice na sekundo)
extern float FPS_A;

//Globalni font
extern FTGLPixmapFont* font;

//Globalni sistem za svok in zvocni efekti
extern irrklang::ISoundEngine *sound_engine;
extern irrklang::ISoundSource *sound_jump;
extern irrklang::ISoundSource *sound_coin;
extern irrklang::ISoundSource *sound_dead;
extern irrklang::ISoundSource *sound_select;
extern irrklang::ISoundSource *sound_music;

//Razredi
//Definicije
class GameState;
class MenuState;
class Player;
class Tile;
struct Rect;
struct Animation;

//Seznam funkcij
bool initilize();
float getFPS();
void setFPS();
void delayFPS();
unsigned int loadTexture(const string fileName);
bool checkCollision(Rect* A, Rect* B);
void renderText(const std::string text, FTGLPixmapFont* font, int size, float x, float y);

//Enums
enum TILE_TYPE
{
	BLOCK = 0, SPIKES = 1, SPIKESDOWN = 2, START = 4, END = 5, COIN = 3, DOOR = 6
};

enum STATE
{
	NONE = 0, EXIT = 1, MENU = 2, GAME = 3, EDITOR = 4, CONTINUE = 5
};

//Razredi
//Stanje igre, abstrakten razred
class GameState
{
public:
	//Naslednji state
	STATE nextState;

	//Dogodki, eventi
	virtual void events() = 0;
	//Logika
	virtual void logic() = 0;
	//Risanje
	virtual void render() = 0;
};

//Menu
class MenuState : public GameState
{
private:
	//Klik
	bool b_mouseClick;

	//Prikaz info
	bool b_info;

	//Tipke
	vector<Rect> tipke;
public:
	MenuState();
	~MenuState();

	//Dedovane procedure
	void events();
	void logic();
	void render();
};

//Game
class LevelState : public GameState
{
private:
	//Objekti
	Player* o_player;
	vector<class Tile*> o_tiles;

	//Imena sob
	vector<string> s_levels;

	//Spremenljivke za kamero
	float cameraX, cameraY;

	//Tipke
	bool b_escape, b_r;

	//Stanje
	bool b_test;

	//Preskok med leveli
	float f_switch;

	//Tocke na zacetku levela
	float f_score;

	//Konec igre
	bool b_win;
public:
	LevelState(bool mode = false);
	//Konstruktor za test level
	LevelState(vector<class Tile*> *tiles);
	~LevelState();

	//Load level
	void loadLevel(const string levelName);

	//Dedovane
	void events();
	void logic();
	void render();
};

//Level editor
class EditorState : public GameState
{
private:
	//Klik
	bool b_mouseClick;
	bool b_mouseRightClick;
	bool b_mouseScrollUp;
	bool b_mouseScrollDown;

	//Pritisk tipke
	bool b_enter;
	bool b_backspace;
	bool b_escape;
	bool b_down;
	bool b_up;
	bool b_right;
	bool b_left;
	bool b_r;

	//Menu
	bool b_menuOpen;

	//Tip dela
	bool b_workState;
	bool b_testing;

	//Shranjevanje
	bool b_saving;
	bool b_loading;
	bool b_charAdded;

	//Trenutna tipka
	int c_tipka;

	//Ime datoteke
	string s_file;

	//Trenutni tile
	TILE_TYPE curType;

	//Tiles
	vector<class Tile*> o_tiles;

	//kamera
	float cameraX, cameraY;

public:
	EditorState();
	~EditorState();

	//Podedovane procedure
	void events();
	void logic();
	void render();

	//Shraniti mapo 
	void saveLevel(const string levelName);

	//Naloziti mapo
	void loadLevel(const string levelName);
};

//Tile / objekt mape
class Tile
{
private:
	//Recti
	Rect *colRect;
	Rect *drawRect;

	//Slika / texture
	unsigned int texture;

	//Tip
	TILE_TYPE type;

public:
	Tile(float x, float y, TILE_TYPE type = BLOCK);
	~Tile();
	
	//Izris
	void render(float a = 1, bool box = false);

	//Prijatelji
	friend EditorState; 
	friend LevelState;
	friend Player;
};

//Igralec
class Player
{
private:
	//Kolizijski rect
	Rect *colRect;
	//Risalni rect
	Rect *drawRect;
	//Slike
	Animation* curAnimation;
	Animation* walkAnimation;
	Animation* standAnimation;
	Animation* jumpAnimation;

	//Tipke
	bool b_left, b_right, b_x;

	//Stanja
	bool b_jump, b_direction, b_doubleJump, b_nextLevel, b_xReleased, b_dead;

	//Hitrost
	float f_xSpeed, f_ySpeed;
	float f_jumpSpeed;

	//Zacetni polozaj
	float f_startX, f_startY;

	//Smrt
	float f_deadColor;

	//Tocke
	float f_score;

public:
	Player();
	~Player();

	//Procedure
	void handleInput();
	void logic(vector<class Tile*> *tiles);
	void render();

	friend LevelState;
};

//Pravokotnik
struct Rect
{
	//Koordinate in sirina, visina
	float x, y, w, h;

	//Konstruktor
	Rect();

	//Nastavlamo koordinate
	void set(float, float, float, float);
};

//Animacija
struct Animation
{
	//Trenutna slika
	unsigned int curTexture;

	//Vse slike
	vector<unsigned int> textures;

	//Cas med frami
	float frameTime;
	float curTime;

	//Konstruktor
	Animation();

	//Logika
	void tick();
};

#endif