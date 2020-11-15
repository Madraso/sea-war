#ifndef LOGIC
#define LOGIC

#define FIELD_SIZE 10
#define HEALTH 20

enum orientation { HORIZONTAL, VERTICAL };
enum move_state { MY_MOVE = 1, ENEMY_MOVE };
enum reply_state { DAMAGE, KILL, MISS, LOOSE, SHIPS_GEN };
enum check_state { FAILED, SUCCESS };

struct ship {
    enum orientation orientation;
    int start_x;
    int start_y;
    int length;
    int health;
};

struct field {
    int cell_number;
    char cell;
};

struct player {
    struct ship boat[10];
    struct field field[FIELD_SIZE][FIELD_SIZE];
	int health;
};

struct player_message {
    int v_coord;
    int h_coord;
    int reply;
};

void init_player_str(struct player *);
void generate_ships(struct player *);
int is_my_move(enum move_state);
void change_move_state(struct player_message, enum move_state *);
void handle_reply(struct player_message, enum move_state *);
enum check_state check_ship_param(struct player, int);
enum check_state check_ships_cross(struct player, int, int);

#endif