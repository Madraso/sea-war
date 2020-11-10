#include "../include/graphic.h"

WINDOW *my_field, *enemy_field;

void init_screen() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(MY_MOVE, COLOR_GREEN, COLOR_BLACK);
    init_pair(ENEMY_MOVE, COLOR_RED, COLOR_BLACK);
    init_pair(LABEL_COLOR, COLOR_CYAN, COLOR_BLACK);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
}

void deinit_screen() {
    endwin();
    delwin(my_field);
    delwin(enemy_field);
}

void refresh_screen() {
    refresh();
    wrefresh(my_field);
    wrefresh(enemy_field);
}

void draw_fields() {
    int max_x = getmaxx(stdscr);
    int offset_x = (max_x / 2) - (SIZE_FIELD_X + 4) - 2, offset_y = 3;

    mvaddstr(1, (max_x / 2) - 6, "Морской бой");

    draw_field(offset_x, offset_y, "Моё поле");
    my_field = newwin(SIZE_FIELD_Y, SIZE_FIELD_X, offset_y + 2, offset_x + 2);
    
    offset_x = (max_x / 2) + 2;

    draw_field(offset_x, offset_y, "Поле врага");
    enemy_field = newwin(SIZE_FIELD_Y, SIZE_FIELD_X, offset_y + 2, offset_x + 2);
}

void place_ships(char field[SIZE_FIELD_Y][SIZE_FIELD_Y]) {
    for (int i = 0; i < SIZE_FIELD_Y; i++) {
        for (int j = 0; j < SIZE_FIELD_Y; j++) {
            if (field[i][j] == '*') {
                mvwaddstr(my_field, i, j * 3, SHIP);
            }
            else if (field[i][j] == ' ') {
                mvwaddstr(my_field, i, j * 3, WATER);
            }
        }
    }
}

void change_move_state(MOVE_STATE state) {
    int max_x = getmaxx(stdscr);

    switch (state) {
        case MY_MOVE: {
            wattron(stdscr, COLOR_PAIR(MY_MOVE));
            mvaddstr(17, (max_x / 2) - 5, "Ваш ход");
            wattroff(stdscr, COLOR_PAIR(MY_MOVE));
        } break;
        case ENEMY_MOVE: {
            wattron(stdscr, COLOR_PAIR(ENEMY_MOVE));
            mvaddstr(17, (max_x / 2) - 9, "Ход противника");
            wattroff(stdscr, COLOR_PAIR(ENEMY_MOVE));
        } break;
    }
}

void draw_field(int offset_x, int offset_y, const char field_name[]) {
    mvaddstr(offset_y, offset_x + 11, field_name);

    wattron(stdscr, COLOR_PAIR(LABEL_COLOR));

    mvaddstr(offset_y + 1, offset_x + 2, "А  Б  В  Г  Д  Е  Ж  З  И  К");
    mvaddstr(offset_y + 12, offset_x + 2, "А  Б  В  Г  Д  Е  Ж  З  И  К");
    
    int row_num = 0;
    for (int y = offset_y + 2; y < offset_y + 12; y++, row_num++) {
        mvprintw(y, offset_x, "%d", row_num);
        mvprintw(y, offset_x + SIZE_FIELD_X + 3, "%d", row_num);
    }

    wattroff(stdscr, COLOR_PAIR(LABEL_COLOR));
}