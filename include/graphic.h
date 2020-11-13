#ifndef GRAPHIC
#define GRAPHIC

#include <curses.h>
#include <locale.h>
#include <stdlib.h>

#include "../include/logic.h"

#define SIZE_FIELD_Y 10
#define SIZE_FIELD_X 28
#define LABEL_COLOR 3

#define WATER "\u2219" /* 8729 */
#define SHIP "\u25A0"
#define HIT "\u274C" /* 10061 */

void init_screen();
void deinit_screen();
void refresh_screen();
void draw_fields();
void place_ships(char [SIZE_FIELD_Y][SIZE_FIELD_Y]);
void set_move_state_label(enum move_state);
void enter_coords(int *, int *);
void mark_my_field(int, int, enum reply_state);
void mark_enemy_field(int, int, enum reply_state);
int is_markered(int, int);
void print_reply_label(enum reply_state);
void clear_label();
void enter_coords_param(int *v_coord, int *h_coord, int *distr, int *len);
int test_coords_param(int v_coord, int h_coord, int distr, int len, struct player *player_str );
int who_win(enum move_state state, int health);
void draw_field(int, int, const char []);

#endif