#ifndef GAME_H
#define GAME_H

#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Renderer/SpriteSheetSurface.h"
#include <bitset>

struct pos2
{
	pos2(int _x, int _y, int _bitset, bool _only_render = false)
	{
		x_ = _x;
		y_ = _y;
		bitset_ = _bitset;
		only_render_ = _only_render;
	}
	int x_;
	int y_;
	int bitset_;
	bool only_render_;
};

class enemigo
{
public:
	enemigo(int _tipo, int _x, int _y, int _walk_value, int _look_dist, int _speed = 30, int _walk_dir = 0)
	{
		x_ = -1;
		y_ = -1;
		walk_value_ = _walk_value;
		look_dist_ = _look_dist;
		walk_dir_ = _walk_dir;
		speed_ = _speed;
	
		set_pos(_x,_y);
		walk_freeze_ = 0;
		tipo_ = _tipo;
	}

	int tipo_; //0 senior, 1 seniora, 2 cabra, 3 ninia

	void add_enemigo_render(int _dir);
	void move();
	void set_pos(int _x, int _y);
	void add_to_render();

	int x_;
	int y_;
	int walk_value_; //0 horizontal 1 vertical 2 diagonal_1 3 diagonal_2
	int walk_dir_; //derecha o izquierda
	int look_dist_; //cantidad que ve el personaje
	int speed_;
	int walk_freeze_;
};

struct player
{
	player()
	{
		x_ = 3;
		y_ = 3;
		looking_direction_ = 3;
	}
	int x_;
	int y_;

	int looking_direction_;
};

struct game_tile
{
	game_tile(int _x, int _y)
	{
		img_ = "default.rttex";
		x_ = _x;
		y_ = _y;
	};
	
	std::string img_;
	int x_;
	int y_;
};

class game
{
public:
    /**
    * Constructor
    */
    game();

    /**
    * Destructor
    */
    virtual ~game();

	/*
	* Initialize the game
	*/
	void init();

	void clean();


	/**
	* We move the player in a dir
	* 0 - Left 
	* 1 - Right
	* 2 - Up
	* 3 - Down
	*/
	void move_player(int dir);

	void pick_up_item();

	/**
	* We check is a map tile is empty (so we can walk there)
	*/
	bool is_empty(int _x, int _y, bool _es_enemigo = false);

	/*
	* Called each update frame
	*/
	void update();

	/**
	* we draw an item and supply _x and _y
	*/
	void draw_item(int _x, int _y);
	void draw_walls(int _x, int _y);
	void draw_enemigo(int _x, int _y);
	void draw_item_top(int i, int j);
	void draw_vision(int i, int j);
	void draw_item_only_render(int i, int j); //dosen't cause collision
	bool pick_top_item(int _x, int _y, bool no_pick = false);
	bool move_check_wall(int _x, int _y, int _dir);
	void draw_main_menu();
	

	SpriteSheetSurface spritesheet_;
	player player_;

	int world_size_; //the size of the world
	int cant_move_; //time that I'm frezzed because moving
	int cant_steal_; //time that I'm frezzed from stealing because stealing
	int moving_dir_;
	int tile_width_;
	int tile_height_;

	int game_state_; // 0 menu, 1 in_game, 2 game_over, 3 next_level, 4 select_level

	Surface menu_bg_;
	Surface menu_bg_btn1_;
	Surface menu_bg_btn2_;
	Surface menu_bg_btn3_;
	Surface mundo_;
	Surface candado_;
	Surface game_over_;
	Surface congrats_;


	//LEVEL EDITOR

	//jugador
	void set_player(int _x, int _y, int _dir = 3);

	//enemigo
	//tipos 0 senior, 1 seniora, 2 cabra, 3 ninia
	//_dir 0 horizontal 1 vertical
	void add_enemy(int _type, int _x, int _y, int _dir, int _view_distance, int _speed, int _start_dir);

	// cosas que pueden estar arriba
	void add_tv(bool _rotated, int _x, int _y);
	void add_pc(bool _rotated, int _x, int _y);
	void add_micro(bool _rotated, int _x, int _y);

	//objetos normales
	void add_refri(bool _rotated, int _x, int _y);
	void add_mesa(bool _rotated, int _x, int _y);
	void add_mueble_doble_metal(bool _rotated, int _x, int _y);
	void add_mueble_metal(bool _rotated, int _x, int _y);
	void add_buro(bool _rotated, int _x, int _y);
	void add_buro_grande(bool _rotated, int _x, int _y);
	void add_escritorio_pc(bool _rotated, int _x, int _y);

	//paredes
	void add_window(bool _rotated, int _x, int _y);
	void add_wall(bool _rotated, int _x, int _y);

	//sillas
	void add_silla(int dir, int _x, int _y);
	void add_silla_compu(int dir, int _x, int _y);

	void check_for_win(); // it sets the game to win state

	int level_;
	int current_level_;

	void default_level();
	void level_1();
	void level_2();
	void level_3();
	void level_4();
	void level_5();
	void level_6();

	void start_level(int lvl_);


	std::map<int, std::map<int, std::bitset<64> > > layer_map;
	std::map<int, std::map<int, std::bitset<4> > > layer_walls;
	std::map<int, std::map<int, std::bitset<16> > > layer_enemigos;
	std::map<int, std::map<int, std::bitset<64> > > layer_top;
	std::map<int, std::map<int, std::bitset<64> > > only_render; //dosen't cause collision or that you can get it
	std::map<int, std::map<int, std::bitset<1> > > layer_vision; //dosen't cause collision or that you can get it

	std::vector<enemigo> enemigos_;
};

/**
* Singleton declaration
*/
extern game *g_game;

#endif // GAME_H