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

    refresh_screen();
}

void place_ships(char field[SIZE_FIELD_Y][SIZE_FIELD_Y]) {
    for (int i = 0; i < SIZE_FIELD_Y; i++) {
        for (int j = 0; j < SIZE_FIELD_Y; j++) {
            if (field[i][j] == '*') {
                mvwaddstr(my_field, i, j * 3, SHIP);
            }
        }
    }

    refresh_screen();
}

void set_move_state_label(enum move_state state) {
    int max_x, max_y;
    
    getmaxyx(stdscr, max_y, max_x);

    switch (state) {
        case MY_MOVE: {
            wattron(stdscr, COLOR_PAIR(MY_MOVE));
            mvaddstr(max_y - 7, (max_x / 2) - 5, "Ваш ход");
            wattroff(stdscr, COLOR_PAIR(MY_MOVE));
        } break;
        case ENEMY_MOVE: {
            wattron(stdscr, COLOR_PAIR(ENEMY_MOVE));
            mvaddstr(max_y - 7, (max_x / 2) - 9, "Ход противника");
            wattroff(stdscr, COLOR_PAIR(ENEMY_MOVE));
            mvaddstr(max_y - 5, (max_x / 2) - 9, "Ожидание ответа...");
        } break;
    }

    refresh_screen();
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

void enter_coords(int *v_coord, int *h_coord) {
    int max_x, max_y;
    char coord;

    getmaxyx(stdscr, max_y, max_x);

    do {
        mvaddstr(max_y - 5, (max_x / 2) - 18, "Введите вертикальную координату (0-9)");
        refresh_screen();
        do {
            coord = getch();
        } while (coord < '0' || coord > '9');
        *v_coord = coord - '0';

        move(max_y - 5, (max_x / 2) - 18);
        clrtoeol();

        mvaddstr(max_y - 5, (max_x / 2) - 18, "Введите горизонтальную координату (0-9)");
        refresh_screen();
        do {
            coord = getch();
        } while (coord < '0' || coord > '9');
        *h_coord = coord - '0';

        move(max_y - 5, (max_x / 2) - 18);
        clrtoeol();

    } while (is_markered(*v_coord, *h_coord));

    refresh_screen();
}

void enter_ship_param(struct player *ship_inf, int ship_num) {
    int max_x, max_y;
    char input_data;

    getmaxyx(stdscr, max_y, max_x);

    print_ship_length(ship_inf->boat[ship_num].length);

    mvaddstr(max_y - 5, (max_x / 2) - 18, "Введите вертикальную координату (0-9)");
    refresh_screen();
    do {
        input_data = getch();
    } while (input_data < '0' || input_data > '9');
    ship_inf->boat[ship_num].start_y = input_data - '0';

    move(max_y - 5, (max_x / 2) - 18);
    clrtoeol();

    mvaddstr(max_y - 5, (max_x / 2) - 18, "Введите горизонтальную координату (0-9)");
    refresh_screen();
    do {
        input_data = getch();
    } while (input_data < '0' || input_data > '9');
    ship_inf->boat[ship_num].start_x = input_data - '0';

    move(max_y - 5, (max_x / 2) - 18);
    clrtoeol();

    mvaddstr(max_y - 5, (max_x / 2) - 27, "Введите направление (Горизонтальное [0], Вертикальное [1])");
    refresh_screen();
    do {
        input_data = getch();
    } while (input_data < '0' || input_data > '1');
    ship_inf->boat[ship_num].orientation = input_data - '0';

    move(max_y - 5, (max_x / 2) - 18);
    clrtoeol();

    refresh_screen();
}

void mark_my_field(int x, int y, enum reply_state rep) {
    switch (rep) {
        case DAMAGE:
            mvwaddstr(my_field, y, x * 3, HIT);
            break;
        case KILL:
            mvwaddstr(my_field, y, x * 3, HIT);
            break;
        case MISS:
            mvwaddstr(my_field, y, x * 3, WATER);
            break;
    }

    refresh_screen();
}

void mark_enemy_field(int x, int y, enum reply_state rep) {
    switch (rep) {
        case DAMAGE:
            mvwaddstr(enemy_field, y, x * 3, HIT);
            break;
        case KILL:
            mvwaddstr(enemy_field, y, x * 3, HIT);
            break;
        case MISS:
            mvwaddstr(enemy_field, y, x * 3, WATER);
            break;
    }

    refresh_screen();
}

int is_markered(int x, int y) {
    int symbol = mvwinch(enemy_field, y, x * 3);

    if (symbol == 8729 || symbol == 10061) return 1;
    else return 0;
}

void print_reply_label(enum reply_state rep) {
    int max_x, max_y;

    getmaxyx(stdscr, max_y, max_x);

    clear_label();

    switch (rep) {
        case DAMAGE:
            mvaddstr(max_y - 5, (max_x / 2) - 3, "Ранил!");
            break;
        case KILL:
            mvaddstr(max_y - 5, (max_x / 2) - 3, "Убил!");
            break;
        case MISS:
            mvaddstr(max_y - 5, (max_x / 2) - 6, "Промахнулся!");
            break;
    }

    refresh_screen();
}

void print_ship_length(int length) {
    int max_x, max_y;

    getmaxyx(stdscr, max_y, max_x);

    switch (length) {
        case 1:
            mvaddstr(max_y - 7, (max_x / 2) - 10, "Однопалубный корабль");
            break;
        case 2:
            mvaddstr(max_y - 7, (max_x / 2) - 10, "Двухпалубный корабль");
            break;
        case 3:
            mvaddstr(max_y - 7, (max_x / 2) - 10, "Трехпалубный корабль");
            break;
        case 4:
            mvaddstr(max_y - 7, (max_x / 2) - 10, "Четырехпалубный корабль");
            break;
    }

    refresh_screen();
}

void print_win_label() {
    int max_x, max_y;

    getmaxyx(stdscr, max_y, max_x);

    clear_label();
    mvaddstr(max_y - 5, (max_x / 2) - 6, "Вы выиграли!");
    refresh_screen();
}

void print_lose_label() {
    int max_x, max_y;

    getmaxyx(stdscr, max_y, max_x);

    clear_label();
    mvaddstr(max_y - 5, (max_x / 2) - 6, "Вы проиграли!");
    refresh_screen();
}

void clear_label() {
    int max_y = getmaxy(stdscr);

    move(max_y - 5, 0);
    clrtoeol();

    move(max_y - 7, 0);
    clrtoeol();

    refresh_screen();
}