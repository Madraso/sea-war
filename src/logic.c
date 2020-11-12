#include "../include/logic.h"
#include "../include/graphic.h"

/* Нужно еще добавить:
    1) Генерацию кораблей
    2) Завершение работы клиента и сервера при окончании игры (когда один из игроков потопил все корабли)
    3) (По возможности) Проверка корабля, поврежден он или уже полностью убит
*/

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

int handle_reply(struct player_message msg, enum move_state *state) {
    switch (*state) {
        case MY_MOVE:
            mark_enemy_field(msg.h_coord, msg.v_coord, msg.reply);
            print_reply_label(msg.reply);
            change_move_state(msg, state);
            break;
        case ENEMY_MOVE:
            mark_my_field(msg.h_coord, msg.v_coord, msg.reply);
            break;
    }
}