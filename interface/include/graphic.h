#ifndef GRAPHIC
#define GRAPHIC

#include <curses.h>
#include <locale.h>

#define SIZE_FIELD_Y 10
#define SIZE_FIELD_X 28
#define LABEL_COLOR 3

#define WATER "\u2219"
#define SHIP "\u25A0"
#define HIT "\u274C"

enum MOVE_STATE { MY_MOVE = 1, ENEMY_MOVE };

void init_screen();
void deinit_screen();
void refresh_screen();
void draw_fields();
void place_ships(char [SIZE_FIELD_Y][SIZE_FIELD_Y]);
void change_move_state(MOVE_STATE);

void draw_field(int, int, const char []);

#endif