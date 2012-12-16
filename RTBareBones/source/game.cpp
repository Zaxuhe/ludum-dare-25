#include "PlatformPrecomp.h"
#include "game.h"

game *g_game;



void enemigo::set_pos(int _x, int _y)
{
	//LogMsg("Quiero poner una posicion");
	x_ = _x;
	y_ = _y;
}

void enemigo::move()
{
	walk_freeze_--;
	if (walk_freeze_ < 0)
	{
		walk_freeze_ = 0;
	}
	if(walk_freeze_ > 0)
	{
		return;
	}

	if (walk_value_ == 0) //horizontal
	{
		if (walk_dir_ == 1)
		{
			if (g_game->is_empty(x_-1, y_,true) && g_game->move_check_wall(x_, y_, 0))
			{
				set_pos(x_-1, y_);
			}
			else
			{
				walk_dir_ = 0;
			}
			
		}
		else
		{
			if (g_game->is_empty(x_+1, y_,true)&& g_game->move_check_wall(x_, y_, 1))
			{
				set_pos(x_+1, y_);
			}
			else
			{
				walk_dir_ = 1;
			}
		}
	}
	else if (walk_value_ == 1) //vertical
	{
		if (walk_dir_ == 1)
		{
			if (g_game->is_empty(x_, y_-1,true) && g_game->move_check_wall(x_, y_, 3))
			{
				set_pos(x_, y_-1);
			}
			else
			{
				walk_dir_ = 0;
			}
			
		}
		else
		{
			if (g_game->is_empty(x_, y_+1,true)&& g_game->move_check_wall(x_, y_, 2))
			{
				set_pos(x_, y_+1);
			}
			else
			{
				walk_dir_ = 1;
			}
		}
	}

	walk_freeze_ = speed_;
}

void enemigo::add_enemigo_render(int _dir)
{
	if (tipo_ == 1)
	{
		_dir+=4; //le agregamos 4 para que dibuje a la seniora
	}
	if (tipo_ == 2)
	{
		_dir+=8; //le agregamos 4 para que dibuje a la cabra
	}
	if (tipo_ == 3)
	{
		_dir+=12; //le agregamos 4 para que dibuje a la ninia
	}
	g_game->layer_enemigos[x_][y_].set(_dir,true);
}

void enemigo::add_to_render()
{
	if (walk_value_ == 0) //horizontal
	{
		for (int i = 0; i < look_dist_; i++)
		{
			int val = i + 1;
			if (walk_dir_ == 1)
			{
				add_enemigo_render(0);
				val *= -1;
				if (g_game->move_check_wall(x_+val, y_, 1) == false)
				{
					break;
				}
			}
			else
			{
				add_enemigo_render(1);
				if (g_game->move_check_wall(x_+val, y_, 0) == false)
				{
					break;
				}
			}

			if (g_game->is_empty(x_+val,y_))
			{
				g_game->layer_vision[x_+val][y_].set(0,true);
			}
			else
			{
				break;
			}
		}
	}
	else if (walk_value_ == 1) //vertical
	{
		for (int i = 0; i < look_dist_; i++)
		{
			int val = i + 1;
			if (walk_dir_ == 1)
			{
				add_enemigo_render(3);
				val *= -1;
				if (g_game->move_check_wall(x_, y_+val, 2) == false)
				{
					break;
				}
			}
			else
			{
				add_enemigo_render(2);
				if (g_game->move_check_wall(x_, y_+val, 3) == false)
				{
					break;
				}
			}
			if (g_game->is_empty(x_,y_+val))
			{
				g_game->layer_vision[x_][y_+val].set(0,true);
			}
			else
			{
				break;
			}
		}
	}
}

game::game()
{

}

//dtor
game::~game()
{
	//spritesheet_.un
	//spritesheet_.InitBlankSurface(0,0);
}

void game::init()
{
	GetAudioManager()->Play("interface/korra.mp3", true, true);
	moving_dir_ = -1;
	world_size_ = 12;
	tile_width_ = 80;
	tile_height_ = 40;
	cant_move_ = 0;
	game_state_ = 0;
	if (!menu_bg_.IsLoaded())
	{
		menu_bg_.LoadFile("interface/menu.rttex");
	}
	if (!menu_bg_btn1_.IsLoaded())
	{
		menu_bg_btn1_.LoadFile("interface/start.rttex");
	}
	if (!menu_bg_btn2_.IsLoaded())
	{
		menu_bg_btn2_.LoadFile("interface/music_off.rttex");
	}
	if (!menu_bg_btn3_.IsLoaded())
	{
		menu_bg_btn3_.LoadFile("interface/music_on.rttex");
	}

	if (!mundo_.IsLoaded())
	{
		mundo_.LoadFile("interface/mundo.rttex");
	}
	if (!candado_.IsLoaded())
	{
		candado_.LoadFile("interface/candado.rttex");
	}
	if (!game_over_.IsLoaded())
	{
		game_over_.LoadFile("interface/game_over.rttex");
	}
	if (!congrats_.IsLoaded())
	{
		congrats_.LoadFile("interface/congrats.rttex");
	}
	if (!spritesheet_.IsLoaded())
	{
		spritesheet_.LoadFile("interface/export_test.rttex");
		spritesheet_.AddFrame("char_3",CL_Rect(0,0,80,80));
		spritesheet_.AddFrame("char_1",CL_Rect(80,0,160,80));
		spritesheet_.AddFrame("char_2",CL_Rect(160,0,240,80));
		spritesheet_.AddFrame("char_0",CL_Rect(240,0,320,80));
		spritesheet_.AddFrame("tv",CL_Rect(320,0,400,80));
		spritesheet_.AddFrame("floor_default",CL_Rect(400,0,480,80));
		spritesheet_.AddFrame("muebledoble_1",CL_Rect(480,0,560,80));

		spritesheet_.AddFrame("cuadro_1",CL_Rect(560,0,640,80));
		spritesheet_.AddFrame("cuadro_1r",CL_Rect(640,0,720,80));
		spritesheet_.AddFrame("silla2_1",CL_Rect(720,0,800,80));
		spritesheet_.AddFrame("silla2_2",CL_Rect(800,0,880,80));
		spritesheet_.AddFrame("silla2_3",CL_Rect(880,0,960,80));


		spritesheet_.AddFrame("muebledoble_2",CL_Rect(0,80,80,160));
		spritesheet_.AddFrame("tv_1",CL_Rect(80,80,160,160));
		spritesheet_.AddFrame("tv_2",CL_Rect(160,80,240,160));
		spritesheet_.AddFrame("enemigo_0",CL_Rect(240,80,320,160));
		spritesheet_.AddFrame("enemigo_1",CL_Rect(320,80,400,160));
		spritesheet_.AddFrame("enemigo_2",CL_Rect(400,80,480,160));
		spritesheet_.AddFrame("enemigo_3",CL_Rect(480,80,560,160));

		spritesheet_.AddFrame("silla2_4",CL_Rect(560,80,640,160));
		spritesheet_.AddFrame("mueble_1",CL_Rect(640,80,720,160));
		spritesheet_.AddFrame("mueble_1r",CL_Rect(720,80,800,160));
		spritesheet_.AddFrame("muebledbl_1",CL_Rect(800,80,880,160));
		spritesheet_.AddFrame("muebledbl_2",CL_Rect(880,80,960,160));


		spritesheet_.AddFrame("vision",CL_Rect(0,160,80,240));
		spritesheet_.AddFrame("tvr_1",CL_Rect(80,160,160,240));
		spritesheet_.AddFrame("tvr_2",CL_Rect(160,160,240,240));
		spritesheet_.AddFrame("silla_r1_1",CL_Rect(240,160,320,240));
		spritesheet_.AddFrame("silla_r2_1",CL_Rect(320,160,400,240));
		spritesheet_.AddFrame("silla_r3_1",CL_Rect(400,160,480,240));
		spritesheet_.AddFrame("silla_r4_1",CL_Rect(480,160,560,240));

		spritesheet_.AddFrame("muebledbl_1r",CL_Rect(560,160,640,240));
		spritesheet_.AddFrame("muebledbl_2r",CL_Rect(640,160,720,240));
		spritesheet_.AddFrame("micro",CL_Rect(720,160,800,240));
		spritesheet_.AddFrame("micro_r",CL_Rect(800,160,880,240));
		spritesheet_.AddFrame("mama_1",CL_Rect(880,160,960,240));

		spritesheet_.AddFrame("muebledoble_1r",CL_Rect(0,240,80,320));
		spritesheet_.AddFrame("muebledoble_2r",CL_Rect(80,240,160,320));
		spritesheet_.AddFrame("compu_desk_1",CL_Rect(160,240,240,320));
		spritesheet_.AddFrame("compu_desk_2",CL_Rect(240,240,320,320));
		spritesheet_.AddFrame("compu_desk_1r",CL_Rect(320,240,400,320));
		spritesheet_.AddFrame("compu_desk_2r",CL_Rect(400,240,480,320));
		spritesheet_.AddFrame("buro",CL_Rect(480,240,560,320));

		spritesheet_.AddFrame("mama_2",CL_Rect(560,240,640,320));
		spritesheet_.AddFrame("mama_3",CL_Rect(640,240,720,320));
		spritesheet_.AddFrame("mama_4",CL_Rect(720,240,800,320));
		spritesheet_.AddFrame("mesa",CL_Rect(800,240,880,320));
		spritesheet_.AddFrame("refri_1",CL_Rect(880,240,960,320));


		spritesheet_.AddFrame("buro_r",CL_Rect(0,320,80,400));
		spritesheet_.AddFrame("pc_2",CL_Rect(80,320,160,400));
		spritesheet_.AddFrame("pc",CL_Rect(160,320,240,400));
		spritesheet_.AddFrame("pc_r_2",CL_Rect(240,320,320,400));
		spritesheet_.AddFrame("pc_r",CL_Rect(320,320,400,400));
		spritesheet_.AddFrame("pared_1",CL_Rect(400,320,480,400));
		spritesheet_.AddFrame("pared_2",CL_Rect(480,320,560,400));

		spritesheet_.AddFrame("refri_2",CL_Rect(560,320,640,400));
		spritesheet_.AddFrame("refri_1r",CL_Rect(640,320,720,400));
		spritesheet_.AddFrame("refri_2r",CL_Rect(720,320,800,400));
		spritesheet_.AddFrame("ventana",CL_Rect(800,320,880,400));
		spritesheet_.AddFrame("ventana_r",CL_Rect(880,320,960,400));

		spritesheet_.AddFrame("goat_1",CL_Rect(0,400,80,480));
		spritesheet_.AddFrame("goat_2",CL_Rect(80,400,160,480));
		spritesheet_.AddFrame("goat_3",CL_Rect(160,400,240,480));
		spritesheet_.AddFrame("goat_4",CL_Rect(240,400,320,480));
		spritesheet_.AddFrame("nina_1",CL_Rect(320,400,400,480));
		spritesheet_.AddFrame("nina_2",CL_Rect(400,400,480,480));
		spritesheet_.AddFrame("nina_3",CL_Rect(480,400,560,480));
		spritesheet_.AddFrame("nina_4",CL_Rect(560,400,640,480));
	}


	level_ = 1;
}

void game::start_level(int lvl_)
{
	clean();
	if (lvl_ == 1)
	{
		level_1();
	}
	else if (lvl_ == 2)
	{
		level_2();
	}
	else if (lvl_ == 3)
	{
		level_3();
	}
	else if (lvl_ == 4)
	{
		level_4();
	}
	else if (lvl_ == 5)
	{
		level_5();
	}
	else if (lvl_ == 6)
	{
		level_6();
	}
	current_level_ = lvl_;
	game_state_ = 1;
}

void game::clean()
{
	layer_map.clear();
	layer_walls.clear();
	layer_enemigos.clear();
	layer_top.clear();
	only_render.clear(); //dosen't cause collision or that you can get it
	layer_vision.clear(); //dosen't cause collision or that you can get it

	enemigos_.clear();
}

void game::set_player(int _x, int _y, int _dir)
{
	player_.x_ = _x;
	player_.y_ = _y;
	player_.looking_direction_ = _dir;
}

void game::default_level()
{
	set_player(0,0);


	//buro grande
	add_buro_grande(true,9,7);
	add_buro_grande(false,2,2);

	//escritorio pc
	add_escritorio_pc(true,8,8);
	add_escritorio_pc(false,10,10);

	//buro chico normal y r
	add_buro(true,3,10);
	add_buro(false,4,10);

	//sillas
	add_silla_compu(0,4,2);
	add_silla_compu(1,4,3);
	add_silla_compu(2,4,4);
	add_silla_compu(3,4,5);

	//sillas
	add_silla(0,5,2);
	add_silla(1,5,3);
	add_silla(2,5,4);
	add_silla(3,5,5);

	//muebles metal
	add_mueble_metal(true, 0, 4);
	add_mueble_metal(false, 6, 5);

	//mueble doble metal
	add_mueble_doble_metal(true,7,3);
	add_mueble_doble_metal(false,8,3);

	//mesa
	add_mesa(true,11,9);

	//refri
	add_refri(true,9,2);
	add_refri(false,11,2);

	// paredes 
	add_wall(true,0,1);
	add_wall(false,0,1);
	add_wall(false,0,2);
	add_wall(true,7,5);
	add_wall(false,5,7);

	//ventanas
	add_window(true,7,5);
	add_window(false,5,7);

	//microhondas
	add_micro(true,7,3);
	add_micro(false,8,2);

	//computadora
	add_pc(true,10,9);
	add_pc(false,8,8);
	
	//tv
	add_tv(true, 2, 1);
	add_tv(false, 9, 7);

	add_enemy(0,2,7,0,3,10,0);
	add_enemy(1,7,7,1,3,10,0);
	add_enemy(2,3,4,1,3,5,0);
	add_enemy(3,0,3,1,3,5,0);
}

void game::add_enemy(int _type, int _x, int _y, int _dir, int _view_distance, int _speed, int _start_dir)
{
	enemigos_.push_back(enemigo(_type,_x,_y,_dir,_view_distance,_speed,_start_dir));
}

void game::add_buro_grande(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		//mueble doble metal
		layer_map[_x][_y].set(0,true);
		layer_map[_x][_y-1].set(1,true);
	}
	else
	{
		//mueble doble metal
		layer_map[_x][_y].set(6,true);
		layer_map[_x-1][_y].set(7,true);
	}

}

void game::add_escritorio_pc(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		//mueble doble metal
		layer_map[_x][_y].set(8,true);
		layer_map[_x][_y-1].set(9,true);
	}
	else
	{
		//mueble doble metal
		layer_map[_x][_y].set(10,true);
		layer_map[_x-1][_y].set(11,true);
	}

}

void game::add_buro(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		//mueble doble metal
		layer_map[_x][_y].set(12,true);
	}
	else
	{
		//mueble doble metal
		layer_map[_x][_y].set(13,true);
	}
}


void game::add_silla_compu(int dir, int _x, int _y)
{
	if (dir == 0)
	{
		layer_map[_x][_y].set(3,true);
	}
	else if (dir == 1)
	{
		layer_map[_x][_y].set(2,true);
	}
	else if (dir == 2)
	{
		layer_map[_x][_y].set(4,true);
	}
	else if (dir == 3)
	{
		layer_map[_x][_y].set(5,true);
	}
}

void game::add_silla(int dir, int _x, int _y)
{
	if (dir == 0)
	{
		layer_map[_x][_y].set(15,true);
	}
	else if (dir == 1)
	{
		layer_map[_x][_y].set(14,true);
	}
	else if (dir == 2)
	{
		layer_map[_x][_y].set(16,true);
	}
	else if (dir == 3)
	{
		layer_map[_x][_y].set(17,true);
	}
}

void game::add_mueble_metal(bool _rotated, int _x, int _y)
{
	if (_rotated)
	{
		//mueble doble metal
		layer_map[_x][_y].set(18,true);
	}
	else
	{
		//mueble doble metal
		layer_map[_x][_y].set(19,true);
	}
}

void game::add_mueble_doble_metal(bool _rotated, int _x, int _y)
{
	if (_rotated)
	{
		//mueble doble metal
		layer_map[_x][_y].set(20,true);
		layer_map[_x-1][_y].set(21,true);
	}
	else
	{
		//mueble doble metal
		layer_map[_x][_y].set(22,true);
		layer_map[_x][_y-1].set(23,true);
	}
}

void game::add_mesa(bool _rotated, int _x, int _y)
{
	layer_map[_x][_y].set(24,true);
}

void game::add_refri(bool _rotated, int _x, int _y)
{
	if (_rotated)
	{
		layer_map[_x][_y].set(25,true);
		only_render[_x-1][_y+1].set(4,true);
	}
	else
	{
		layer_map[_x][_y].set(26,true);
		only_render[_x-1][_y+1].set(5,true);
	}
}

void game::add_wall(bool _rotated, int _x, int _y)
{
	if (_rotated)
	{
		layer_walls[_x][_y].set(0,true);
	}
	else
	{
		layer_walls[_x][_y].set(1,true);
	}
}

void game::add_window(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		layer_walls[_x][_y].set(2,true);
	}
	else
	{
		layer_walls[_x][_y].set(3,true);
	}
}

void game::add_micro(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		layer_top[_x][_y].set(4,true);
	}
	else
	{
		layer_top[_x][_y].set(5,true);
	}
}

void game::add_pc(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		layer_top[_x][_y].set(2,true);
		only_render[_x-2][_y+2].set(2,true);
	}
	else
	{
		layer_top[_x][_y].set(3,true);
		only_render[_x-2][_y+2].set(3,true);
	}
}

void game::add_tv(bool _rotated, int _x, int _y)
{
	if (_rotated == false)
	{
		//tv
		layer_top[_x][_y].set(0,true);
		only_render[_x-2][_y+2].set(0,true);
	}
	else
	{
		//tv r
		layer_top[_x][_y].set(1,true);
		only_render[_x-2][_y+2].set(1,true);
	}
}

void game::level_1()
{
	//default_level();
	set_player(0,3,1);

	add_enemy(0,5,5,0,3,20,1);
	add_buro_grande(false,11,6);
	add_escritorio_pc(true,6,8);
	add_pc(false,6,8);
	add_buro(false,5,2);
	add_refri(true,2,5);
}

void game::level_2()
{
	set_player(0,0,1);

	add_enemy(0,7,9,0,3,20,1);
	add_enemy(1,5,11,1,3,15,1);
	add_buro_grande(false,0,6);
	add_buro(true,5,2);
	add_escritorio_pc(true,11,4);
	add_pc(false,11,4);
	add_refri(true,11,10);
}

void game::level_3()
{
	set_player(0,0,0);

	add_enemy(0,3,8,1,3,20,1);
	add_enemy(1,10,6,0,4,15,1);
	add_enemy(3,6,8,1,3,10,0);
	add_buro(false,4,8);
	add_tv(false,4,8);
	add_refri(true,9,10);
	add_mueble_metal(false,3,9);
	add_micro(false,3,9);
	add_buro_grande(true,4,1);
	add_escritorio_pc(false,8,2);
	add_pc(true,8,1);
	add_silla_compu(3,9,1);
}

void game::level_4()
{
	set_player(0,0,2);

	add_wall(false,2,0);
	add_wall(false,2,1);
	add_wall(false,2,2);

	add_enemy(0,2,7,0,3,20,1);
	add_enemy(1,8,8,0,4,15,1);
	add_enemy(3,5,6,1,3,10,0);
	add_enemy(3,3,2,0,3,10,0);
	add_silla(false,3,1);
	add_buro_grande(true,4,0);
	add_tv(true,4,0);
	add_mueble_doble_metal(true,10,11);
	add_buro(false,8,1);
	add_mesa(true,5,5);
	add_buro_grande(true,10,0);
	add_silla(2,5,4);
	add_silla(0,5,6);
	add_silla(1,4,5);
	add_silla(3,6,5);
	add_micro(true,10,11);
	add_buro_grande(false,0,9);
	add_escritorio_pc(true,3,9);
	add_pc(false,3,9);
	add_silla_compu(2,3,8);
	add_wall(0,8,8);
	add_wall(0,8,9);
	add_wall(0,8,10);
	add_wall(0,8,11);
	add_wall(1,10,8);
	add_wall(1,11,8);
	add_mesa(1,9,9);
	add_silla(2,9,8);
	add_refri(true,10,10);
}

void game::level_5()
{
	set_player(0,0,2);

	add_wall(false,2,0);
	add_wall(false,2,1);
	add_wall(false,2,2);

	add_enemy(0,2,7,0,3,20,1);
	add_enemy(1,8,8,0,4,15,1);
	add_enemy(1,2,8,1,4,15,1);
	add_enemy(3,5,6,1,3,10,0);
	add_enemy(2,3,2,0,3,5,0);
	add_enemy(2,9,2,1,3,5,0);
	add_silla(false,3,1);
	add_buro_grande(true,4,0);
	add_tv(true,4,0);
	add_mueble_doble_metal(true,10,11);
	add_buro(false,8,1);
	add_mesa(true,5,5);
	add_buro_grande(true,10,0);
	add_silla(2,5,4);
	add_silla(0,5,6);
	add_silla(1,4,5);
	add_silla(3,6,5);
	add_micro(true,10,11);
	add_buro_grande(false,0,9);
	add_escritorio_pc(true,3,9);
	add_pc(false,3,9);
	add_silla_compu(2,3,8);
	add_wall(0,8,8);
	add_wall(0,8,9);
	add_wall(0,8,10);
	add_wall(0,8,11);
	add_wall(1,10,8);
	add_window(true,10,8);
	add_wall(1,11,8);
	add_mesa(1,9,9);
	add_silla(2,9,8);
	add_refri(true,10,10);
}

void game::level_6()
{
	set_player(0,0,2);

	add_wall(false,2,0);
	add_wall(false,2,1);
	add_wall(false,2,2);

	add_enemy(0,2,7,0,3,20,1);
	add_enemy(1,8,8,0,4,15,1);
	add_enemy(1,2,8,1,4,15,1);
	add_enemy(3,5,6,1,3,10,0);
	add_enemy(0,3,2,0,3,20,0);
	add_enemy(2,9,2,1,3,5,0);
	add_enemy(2,6,2,1,3,5,0);
	add_enemy(0,4,3,0,3,20,0);
	add_enemy(2,2,11,0,3,5,0);
	add_silla(false,3,1);
	add_buro_grande(true,4,0);
	add_tv(true,4,0);
	add_mueble_doble_metal(true,10,11);
	add_buro(false,8,1);
	add_mesa(true,5,5);
	add_buro_grande(true,10,0);
	add_silla(2,5,4);
	add_silla(0,5,6);
	add_silla(1,4,5);
	add_silla(3,6,5);
	add_micro(true,10,11);
	add_buro_grande(false,0,9);
	add_escritorio_pc(true,3,9);
	add_pc(false,3,9);
	add_silla_compu(2,3,8);
	add_wall(0,8,8);
	add_wall(0,8,9);
	add_wall(0,8,10);
	add_wall(0,8,11);
	add_wall(1,10,8);
	add_window(true,10,8);
	add_wall(1,11,8);
	add_mesa(1,9,9);
	add_silla(2,9,8);
	add_refri(true,10,10);
}

void game::draw_main_menu()
{
	//LogMsg("Dibujar Menu");
	if (game_state_ == 0)
	{
		menu_bg_.Blit(0,0);
		menu_bg_btn1_.Blit(310,260);
		if (GetAudioManager()->GetMusicEnabled())
		{
			menu_bg_btn3_.Blit(310,420);
		}
		else
		{
			menu_bg_btn2_.Blit(310,420);
		}
	}
	else if (game_state_ == 4)
	{
		mundo_.Blit(0,0);
		if (level_ < 2)
		{
			candado_.Blit(498,80);
		}
		if (level_ < 3)
		{
			candado_.Blit(592,116);
		}
		if (level_ < 4)
		{
			candado_.Blit(671,161);
		}
		if (level_ < 5)
		{
			candado_.Blit(760,205);
		}
		if (level_ < 6)
		{
			candado_.Blit(852,249);
		}
	}
	else if (game_state_ == 2) //game_over
	{

		game_over_.Blit(0,0);
	}
	else if (game_state_ == 3) //win
	{

		congrats_.Blit(0,0);
	}
}

bool game::pick_top_item(int _x, int _y, bool no_pick /* = false*/)
{
	if (layer_top[_x][_y] == 0)
	{
		return false; // no hay item
	}

	if (no_pick) //we don't pick up but we know there is something
	{
		return true;
	}

	std::bitset<64> *_set = &layer_top[_x][_y];
	if (_set->test(0))//television
	{
		layer_top[_x][_y].set(0,false);
		only_render[_x-2][_y+2].set(0,false);
	}
	if (_set->test(1))//television_r
	{
		layer_top[_x][_y].set(1,false);
		only_render[_x-2][_y+2].set(1,false);
	}
	if (_set->test(2))//pc
	{
		layer_top[_x][_y].set(2,false);
		only_render[_x-2][_y+2].set(2,false);
	}
	if (_set->test(3))//pc
	{
		layer_top[_x][_y].set(3,false);
		only_render[_x-2][_y+2].set(3,false);
	}
	if (_set->test(4))//micro
	{
		layer_top[_x][_y].set(4,false);
	}
	if (_set->test(5))//micro
	{
		layer_top[_x][_y].set(5,false);
	}

	return true;

	//recojemos el item
}

void game::check_for_win()
{
	bool found_item = false;
	for (int i = 0; i < world_size_; i++)
	{
		for (int j = world_size_; j > 0; j--)
		{
			if (g_game->layer_map[i][j-1] != 0)
			{
				g_game->draw_item(i,j);
				 found_item = true;
				 break;
			}
			if (g_game->layer_top[i][j-1] != 0)
			{
				g_game->draw_item_top(i,j);
				 found_item = true;
				 break;
			}
		}
	}
	if (found_item == false)
	{
		game_state_ = 3;
		if (current_level_ == level_)
		{
			level_++;
		}
	}
}


void game::pick_up_item()
{
	if (cant_steal_ > 0)
	{
		return;
	}

	int look_x = player_.x_;
	int look_y = player_.y_;
	if (player_.looking_direction_ == 0)
	{
		look_x-=1;
	}
	else if (player_.looking_direction_ == 1)
	{
		look_x+=1;
	}
	else if (player_.looking_direction_ == 2)
	{
		look_y+=1;
	}
	else if (player_.looking_direction_ == 3)
	{
		look_y-=1;
	}

	if (pick_top_item(look_x,look_y)) //ya recogimos el item
	{

		cant_steal_ = 30;
		check_for_win();
		return;
	}

	std::vector<pos2> posiciones;
	//we look for what type of item it is
	if (layer_map[look_x][look_y].test(0)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,0));
		posiciones.push_back(pos2(look_x,look_y-1,1));
	}
	else if (layer_map[look_x][look_y].test(1)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y+1,0));
		posiciones.push_back(pos2(look_x,look_y,1));
	}
	else if (layer_map[look_x][look_y].test(2)) //silla_r1_1
	{
		posiciones.push_back(pos2(look_x,look_y,2));
	}
	else if (layer_map[look_x][look_y].test(3)) //silla_r1_2
	{
		posiciones.push_back(pos2(look_x,look_y,3));
	}
	else if (layer_map[look_x][look_y].test(4)) //silla_r1_3
	{
		posiciones.push_back(pos2(look_x,look_y,4));
	}
	else if (layer_map[look_x][look_y].test(5)) //silla_r1_4
	{
		posiciones.push_back(pos2(look_x,look_y,5));
	}
	else if (layer_map[look_x][look_y].test(6)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,6));
		posiciones.push_back(pos2(look_x-1,look_y,7));
	}
	else if (layer_map[look_x][look_y].test(7)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x+1,look_y,6));
		posiciones.push_back(pos2(look_x,look_y,7));
	}
	else if (layer_map[look_x][look_y].test(8)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,8));
		posiciones.push_back(pos2(look_x,look_y-1,9));
	}
	else if (layer_map[look_x][look_y].test(9)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y+1,8));
		posiciones.push_back(pos2(look_x,look_y,9));
	}
	else if (layer_map[look_x][look_y].test(10)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,10));
		posiciones.push_back(pos2(look_x-1,look_y,11));
	}
	else if (layer_map[look_x][look_y].test(11)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x+1,look_y,10));
		posiciones.push_back(pos2(look_x,look_y,11));
	}
	else if (layer_map[look_x][look_y].test(12)) //buro
	{
		posiciones.push_back(pos2(look_x,look_y,12));
	}
	else if (layer_map[look_x][look_y].test(13)) //buro_r
	{
		posiciones.push_back(pos2(look_x,look_y,13));
	}
	else if (layer_map[look_x][look_y].test(14)) //silla_r1_1
	{
		posiciones.push_back(pos2(look_x,look_y,14));
	}
	else if (layer_map[look_x][look_y].test(15)) //silla_r1_2
	{
		posiciones.push_back(pos2(look_x,look_y,15));
	}
	else if (layer_map[look_x][look_y].test(16)) //silla_r1_3
	{
		posiciones.push_back(pos2(look_x,look_y,16));
	}
	else if (layer_map[look_x][look_y].test(17)) //silla_r1_4
	{
		posiciones.push_back(pos2(look_x,look_y,17));
	}
	else if (layer_map[look_x][look_y].test(18)) //mueble
	{
		posiciones.push_back(pos2(look_x,look_y,18));
	}
	else if (layer_map[look_x][look_y].test(19)) //mueble
	{
		posiciones.push_back(pos2(look_x,look_y,19));
	}
	else if (layer_map[look_x][look_y].test(20)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,20));
		posiciones.push_back(pos2(look_x-1,look_y,21));
	}
	else if (layer_map[look_x][look_y].test(21)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x+1,look_y,20));
		posiciones.push_back(pos2(look_x,look_y,21));
	}
	else if (layer_map[look_x][look_y].test(22)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y,22));
		posiciones.push_back(pos2(look_x,look_y-1,23));
	}
	else if (layer_map[look_x][look_y].test(23)) //es mueble doble
	{
		posiciones.push_back(pos2(look_x,look_y+1,22));
		posiciones.push_back(pos2(look_x,look_y,23));
	}
	else if (layer_map[look_x][look_y].test(24)) //mesa
	{
		posiciones.push_back(pos2(look_x,look_y,24));
	}
	else if (layer_map[look_x][look_y].test(25)) //refri
	{
		posiciones.push_back(pos2(look_x,look_y,25));
		posiciones.push_back(pos2(look_x-1,look_y+1,4,true));
	}
	else if (layer_map[look_x][look_y].test(26)) //refri_r
	{
		posiciones.push_back(pos2(look_x,look_y,26));
		posiciones.push_back(pos2(look_x-1,look_y+1,5,true));
	}

	if (posiciones.size() > 0)
	{
		bool delete_top_item = false;
		for (int i = 0; i < posiciones.size(); i++)
		{
			if (pick_top_item(posiciones[i].x_,posiciones[i].y_,true))
			{
				delete_top_item = true;
				return;
			}
		}
		if (delete_top_item == false)
		{
			for (int i = 0; i < posiciones.size(); i++)
			{
				if (posiciones[i].only_render_ == false)
				{
					layer_map[posiciones[i].x_][posiciones[i].y_].set(posiciones[i].bitset_,false);
				}
				else
				{
					only_render[posiciones[i].x_][posiciones[i].y_].set(posiciones[i].bitset_,false);
				}
			}
			cant_steal_ = 30;
			check_for_win();
		}
	}

	//layer_map[player_.x_-1][player_.y_].set(0,0);
}

void game::draw_item_top(int i, int j)
{
	std::bitset<64> *_set = &g_game->layer_top[i][j-1];
	if (_set->test(0) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"tv_1");
	}
	if (_set->test(1) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"tvr_1");
	}
	if (_set->test(2) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pc");
	}
	if (_set->test(3) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pc_r");
	}
	if (_set->test(4) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"micro");
	}
	if (_set->test(5) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"micro_r");
	}
}


void game::draw_enemigo(int i, int j)
{
	std::bitset<16> *_set = &g_game->layer_enemigos[i][j-1];
	if (_set->test(0) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"enemigo_0");
	}
	if (_set->test(1) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"enemigo_1");
	}
	if (_set->test(2) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"enemigo_2");
	}
	if (_set->test(3) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"enemigo_3");
	}
	if (_set->test(4) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mama_3");
	}
	if (_set->test(5) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mama_2");
	}
	if (_set->test(6) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mama_4");
	}
	if (_set->test(7) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mama_1");
	}
	if (_set->test(8) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"goat_4");
	}
	if (_set->test(9) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"goat_2");
	}
	if (_set->test(10) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"goat_3");
	}
	if (_set->test(11) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"goat_1");
	}
	if (_set->test(12) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"nina_4");
	}
	if (_set->test(13) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"nina_2");
	}
	if (_set->test(14) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"nina_3");
	}
	if (_set->test(15) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"nina_1");
	}
}

void game::draw_walls(int i, int j)
{
	std::bitset<4> *_set = &g_game->layer_walls[i][j-1];
	if (_set->test(0) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pared_1");
	}
	if (_set->test(1) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pared_2");
	}
	if (_set->test(2) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"ventana");
	}
	if (_set->test(3) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"ventana_r");
	}
}

void game::draw_item(int i, int j)
{
	std::bitset<64> *_set = &g_game->layer_map[i][j-1];
	if (_set->test(0) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledoble_1");
	}
	if (_set->test(1) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledoble_2");
	}
	if (_set->test(2) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla_r1_1");
	}
	if (_set->test(3) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla_r2_1");
	}
	if (_set->test(4) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla_r3_1");
	}
	if (_set->test(5) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla_r4_1");
	}
	if (_set->test(6) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledoble_1r");
	}
	if (_set->test(7) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledoble_2r");
	}
	if (_set->test(8) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"compu_desk_1");
	}
	if (_set->test(9) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"compu_desk_2");
	}
	if (_set->test(10) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"compu_desk_2r");
	}
	if (_set->test(11) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"compu_desk_1r");
	}
	if (_set->test(12) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"buro");
	}
	if (_set->test(13) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"buro_r");
	}
	if (_set->test(14) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla2_1");
	}
	if (_set->test(15) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla2_2");
	}
	if (_set->test(16) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla2_3");
	}
	if (_set->test(17) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"silla2_4");
	}
	if (_set->test(18) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mueble_1");
	}
	if (_set->test(19) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mueble_1r");
	}
	if (_set->test(20) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledbl_1");
	}
	if (_set->test(21) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledbl_2");
	}
	if (_set->test(22) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledbl_2r");
	}
	if (_set->test(23) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"muebledbl_1r");
	}
	if (_set->test(24) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"mesa");
	}
	if (_set->test(25) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"refri_1");
	}
	if (_set->test(26) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"refri_1r");
	}
}

void game::draw_vision(int i, int j)
{
	std::bitset<1> *_set = &g_game->layer_vision[i][j-1];

	spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"vision");
}

void game::draw_item_only_render(int i, int j)
{
	std::bitset<64> *_set = &g_game->only_render[i][j-1];


	if (_set->test(0) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"tv_2");
	}
	if (_set->test(1) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"tvr_2");
	}
	if (_set->test(2) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pc_2");
	}
	if (_set->test(3) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"pc_r_2");
	}
	if (_set->test(4) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"refri_2");
	}
	if (_set->test(5) == true)
	{
		spritesheet_.BlitFrame((j * tile_width_ / 2) + (i * tile_width_ / 2)+10, (i * tile_height_ / 2) - (j * tile_height_ / 2)+GetScreenSizeY()/2,"refri_2r");
	}
}

void game::update()
{
	cant_move_--;
	cant_steal_--;
	if (cant_move_ < 0)
	{
		cant_move_ = 0;
	}
	if (cant_steal_ < 0)
	{
		cant_steal_ = 0;
	}
	move_player(moving_dir_);

	for (int i = 0; i < enemigos_.size(); i++)
	{
		enemigos_[i].move();
	}

	for (int i = 0; i < world_size_; i++)
	{
		for (int j = world_size_; j > 0; j--)
		{
			g_game->layer_enemigos[i][j-1] = 0;
			g_game->layer_vision[i][j-1] = 0;
		}
	}

	for (int i = 0; i < enemigos_.size(); i++)
	{
		enemigos_[i].add_to_render();
	}

	if (g_game->layer_vision[g_game->player_.x_][g_game->player_.y_] != 0)
	{
		game_state_ = 2;
	}

}

bool game::is_empty(int _x, int _y, bool _es_enemigo /* = false*/)
{
	if (layer_map.find(_x) != layer_map.end() && layer_map[_x].find(_y) != layer_map[_x].end())
	{
		if (layer_map[_x][_y] != 0)
		{
			return false;
		}
	}
	if (_es_enemigo == false)
	{
		if (layer_enemigos.find(_x) != layer_enemigos.end() && layer_enemigos[_x].find(_y) != layer_enemigos[_x].end())
		{
			if (layer_enemigos[_x][_y] != 0)
			{
				return false;
			}
		}
	}

	if (_x < 0 || _x >= world_size_ || _y < 0 || _y >= world_size_)
	{
		return false;
	}
	return true;
}

// return true if you can move, false if you cant
bool game::move_check_wall(int _x, int _y, int _dir)
{
	if (_dir == 0) //left
	{
		if (g_game->layer_walls[_x-1][_y].test(1))
		{
			return false;
		}
	}
	else if (_dir == 1) //right
	{
		if (g_game->layer_walls[_x][_y].test(1))
		{
			return false;
		}
	}
	else if (_dir == 2) //up
	{
		if (g_game->layer_walls[_x][_y+1].test(0))
		{
			return false;
		}
	}
	else if (_dir == 3) //down
	{
		if (g_game->layer_walls[_x][_y].test(0))
		{
			return false;
		}
	}

	return true;
}

void game::move_player(int dir)
{
	if (dir == 0)
	{
		g_game->player_.looking_direction_ = 0;
	}
	if (dir == 1)
	{
		g_game->player_.looking_direction_ = 1;
	}
	if (dir == 2)
	{
		g_game->player_.looking_direction_ = 2;
	}
	if (dir == 3)
	{
		g_game->player_.looking_direction_ = 3;
	}


	if (dir == -1 || cant_move_ > 0)
	{
		return;
	}
	bool moved = false;
	if (dir == 0 && is_empty(player_.x_-1,player_.y_) && move_check_wall(player_.x_, player_.y_, 0)) //left
	{
		moved = true;
		player_.x_-=1;
		if (player_.x_ < 0)
		{
			player_.x_ = 0;
		}
	}
	else if (dir == 1 && is_empty(player_.x_+1,player_.y_)  && move_check_wall(player_.x_, player_.y_, 1)) //right
	{
		moved = true;
		player_.x_+=1;
		if (player_.x_ >= world_size_)
		{
			player_.x_ = world_size_-1;
		}
	}
	else if (dir == 2 && is_empty(player_.x_,player_.y_+1)  && move_check_wall(player_.x_, player_.y_, 2)) //up
	{
		moved = true;
		player_.y_+=1;
		if (player_.y_ >= world_size_)
		{
			player_.y_ = world_size_-1;
		}
	}
	else if (dir == 3 && is_empty(player_.x_,player_.y_-1)  && move_check_wall(player_.x_, player_.y_, 3)) //down
	{
		moved = true;
		player_.y_-=1;
		if (player_.y_ < 0)
		{
			player_.y_ = 0;
		}
	}
	if (moved == true)
	{
		cant_move_ = 15;
	}


}