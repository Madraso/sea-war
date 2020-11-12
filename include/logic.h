#ifndef LOGIC
#define LOGIC

#define FIELD_SIZE 10
#define HEALTH 20
enum orientation { gorisont, vertical };
enum move_state { MY_MOVE = 1, ENEMY_MOVE };
enum reply_state { DAMAGE, KILL, MISS };

struct ship {
    int start_x;
    int start_y;
    enum orientation orientation;
    int length;
};

struct player {
	int shoot;
	int player_helth;
	int NUM_OF_SHIP;
	char field[FIELD_SIZE][FIELD_SIZE];
	int current_ship;
	struct ship boat[10];
};

struct player_message {
    int v_coord;
    int h_coord;
    int reply;
    int health;

};

int is_my_move(enum move_state);
void change_move_state(struct player_message, enum move_state *);
int handle_reply(struct player_message, enum move_state *);

#endif