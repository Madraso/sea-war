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
void place_ships(struct field [SIZE_FIELD_Y][SIZE_FIELD_Y]);
void set_move_state_label(enum move_state);
void enter_coords(int *, int *);
void enter_ship_param(struct player *, int);
void mark_my_field(int, int, enum reply_state);
void mark_enemy_field(int, int, enum reply_state);
int is_markered(int, int);
void print_reply_label(enum reply_state);
void print_ship_length(int);
void print_win_label();
void print_lose_label();
void clear_label();
void draw_field(int, int, const char []);

#endif