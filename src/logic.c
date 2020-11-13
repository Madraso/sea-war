#include "../include/logic.h"
#include "../include/graphic.h"

/* Нужно еще добавить:
    1) (По возможности) Проверка корабля, поврежден он или уже полностью убит
*/

void init_player_str(struct player *player_str) {
    player_str->health = HEALTH;

    player_str->boat[0].length = 1;
    player_str->boat[1].length = 1;
    player_str->boat[2].length = 1;
    player_str->boat[3].length = 1;
    player_str->boat[4].length = 2;
    player_str->boat[5].length = 2;
    player_str->boat[6].length = 2;
    player_str->boat[7].length = 3;
    player_str->boat[8].length = 3;
    player_str->boat[9].length = 4;

    for (int i = 0; i < SIZE_FIELD_Y; i++) {
        for (int j = 0; j < SIZE_FIELD_Y; j++) {
            player_str->field[i][j] = ' ';
        }
    }
}

void generate_ships(struct player *player_str) {
    for (int ship_num = 0; ship_num < 10;) {
        clear_label();
        enter_ship_param(player_str, ship_num);

        struct player str_copy = *player_str;

        if (check_ship_param(str_copy, ship_num) != SUCCESS) continue;

        switch (player_str->boat[ship_num].orientation) {
            case HORIZONTAL:
                for (int j = 1; j <= player_str->boat[ship_num].length; j++) {
                    player_str->field[player_str->boat[ship_num].start_y]
                        [player_str->boat[ship_num].start_x] = '*';
                    player_str->boat[ship_num].start_x++;
                }
                
                place_ships(player_str->field);
                break;
            case VERTICAL:
                for (int j = 1; j <= player_str->boat[ship_num].length; j++) {
                    player_str->field[player_str->boat[ship_num].start_y]
                        [player_str->boat[ship_num].start_x] = '*';
                    player_str->boat[ship_num].start_y++;
                }
                    
                place_ships(player_str->field);   
                break;
        }

        ship_num++;
    }

    clear_label();
}

int is_my_move(enum move_state state) {
    if (state == MY_MOVE) return 1;
    else if (state == ENEMY_MOVE) return 0;
}

void change_move_state(struct player_message msg, enum move_state *state) {
    switch (msg.reply) {
        case DAMAGE:
            *state = MY_MOVE;
            break;
        case KILL:
            *state = MY_MOVE;
            break;
        case MISS:
            *state = ENEMY_MOVE;
            break;
    }
}

void handle_reply(struct player_message msg, enum move_state *state) {
    switch (*state) {
        case MY_MOVE:
            if (msg.reply == LOOSE) return;
            mark_enemy_field(msg.h_coord, msg.v_coord, msg.reply);
            print_reply_label(msg.reply);
            change_move_state(msg, state);
            break;
        case ENEMY_MOVE:
            mark_my_field(msg.h_coord, msg.v_coord, msg.reply);
            break;
    }
}

enum check_state check_ship_param(struct player player_str, int ship_num) {
    switch (player_str.boat[ship_num].orientation) {
        case HORIZONTAL:
            if ((player_str.boat[ship_num].length +
                player_str.boat[ship_num].start_x) > SIZE_FIELD_Y) {
                return FAILED;
            }
            else {
                for (int j = 1; j <= player_str.boat[ship_num].length; j++) {
                    if (player_str.field[player_str.boat[ship_num].start_y]
                        [player_str.boat[ship_num].start_x] == '*') {
                        return FAILED;
                    }
                    if (check_ships_cross(player_str, ship_num, j) == FAILED) return FAILED;
                    player_str.boat[ship_num].start_x++;
                }
            }
            break;
        case VERTICAL:
            if ((player_str.boat[ship_num].length +
                player_str.boat[ship_num].start_y) > SIZE_FIELD_Y) {
                return FAILED;
            }
            else {
                for (int j = 1; j <= player_str.boat[ship_num].length; j++) {
                    if (player_str.field[player_str.boat[ship_num].start_y]
                        [player_str.boat[ship_num].start_x] == '*') {
                        return FAILED;
                    }
                    if (check_ships_cross(player_str, ship_num, j) == FAILED) return FAILED;
                    player_str.boat[ship_num].start_y++;
                }
            }
            break;
    }

    return SUCCESS;
}

enum check_state check_ships_cross(struct player player_str,
    int ship_num, int part_ship_num) {
    switch (player_str.boat[ship_num].orientation) {
        case HORIZONTAL:
            if (part_ship_num == 1) {
                if (player_str.field[player_str.boat[ship_num].start_y - 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y + 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x - 1] == '*'
                    )
                {
                    return FAILED;
                }
            }
            else if (part_ship_num == player_str.boat[ship_num].length) {
                if (player_str.field[player_str.boat[ship_num].start_y - 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y + 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x + 1] == '*'
                    )
                {
                    return FAILED;
                }
            }
            else {
                if (player_str.field[player_str.boat[ship_num].start_y - 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y + 1]
                    [player_str.boat[ship_num].start_x] == '*'
                    )
                {
                    return FAILED;
                }
            }
            break;
        case VERTICAL:
           if (part_ship_num == 1) {
                if (player_str.field[player_str.boat[ship_num].start_y - 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x - 1] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x + 1] == '*'
                    )
                {
                    return FAILED;
                }
            }
            else if (part_ship_num == player_str.boat[ship_num].length) {
                if (player_str.field[player_str.boat[ship_num].start_y + 1]
                    [player_str.boat[ship_num].start_x] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x - 1] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x + 1] == '*'
                    )
                {
                    return FAILED;
                }
            }
            else {
                if (player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x - 1] == '*' ||
                    player_str.field[player_str.boat[ship_num].start_y]
                    [player_str.boat[ship_num].start_x + 1] == '*'
                    )
                {
                    return FAILED;
                }
            }
            break;
    }

    return SUCCESS;
}