#include "../include/client.h"
#include "../include/logic.h"
#include "../include/graphic.h"

int main(int argc, char *argv[]) {
    system("clear");

    if (argc != 3) {
        printf("Запускать как ./client ip_address port");
        return 0;
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_str;
    server_str.sin_family = AF_INET;
    server_str.sin_port = htons(atoi(argv[2]));
    server_str.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock_fd, (struct sockaddr *) &server_str, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char player_num;

    printf("Ожидание подключения второго игрока...\n");

    int receieve_status = recv(sock_fd, &player_num, 1, 0);
    if (receieve_status < 0) {
        perror("receive");
        exit(EXIT_FAILURE);
    }

    struct player_message msg;
    struct player player_str;

    init_screen();
    draw_fields();

    init_player_str(&player_str);
    generate_ships(&player_str);

    enum move_state state;

    if ((player_num - '0') == 1) state = MY_MOVE;
    else if ((player_num - '0') == 2) state = ENEMY_MOVE;

    while (1) {
        if (is_my_move(state)) {
            set_move_state_label(MY_MOVE);
            enter_coords(&msg.v_coord, &msg.h_coord);

            int send_status = send(sock_fd, (void *) &msg, sizeof(msg), 0);
            if (send_status < 0) {
                perror("send");
                exit(EXIT_FAILURE);
            }

            int receieve_status = recv(sock_fd, (void *) &msg, sizeof(msg), 0);
            if (receieve_status < 0) {
                perror("receive");
                exit(EXIT_FAILURE);
            }

            handle_reply(msg, &state);

            if (msg.reply == LOOSE) {
                print_win_label();
                sleep(2);
                deinit_screen();
                close(sock_fd);
                exit(EXIT_SUCCESS);
            }

            sleep(2);
            clear_label();
        }
        else {
            set_move_state_label(ENEMY_MOVE);

            int receieve_status = recv(sock_fd, (void *) &msg, sizeof(msg), 0);

            if (receieve_status < 0) {
                perror("receive");
                exit(EXIT_FAILURE);
            }

            switch (player_str.field[msg.v_coord][msg.h_coord]) {
                case '*':
                    msg.reply = DAMAGE;
                    player_str.health--;

                    handle_reply(msg, &state);

                    state = ENEMY_MOVE;
                    player_str.field[msg.v_coord][msg.h_coord] = ' ';
                    break;
                case ' ':
                    msg.reply = MISS;

                    handle_reply(msg, &state);

                    state = MY_MOVE;
                    break;
            }

            if (player_str.health == 0) {
                msg.reply = LOOSE;

                int send_status = send(sock_fd, (void *) &msg, sizeof(msg), 0);
                if (send_status < 0) {
                    perror("send");
                    exit(EXIT_FAILURE);
                }

                print_lose_label();
                sleep(2);
                deinit_screen();
                close(sock_fd);
                exit(EXIT_SUCCESS);
            }

            int send_status = send(sock_fd, (void *) &msg, sizeof(msg), 0);
            
            if (send_status < 0) {
                perror("send");
                exit(EXIT_FAILURE);
            }

            sleep(2);
            clear_label();
        }
    }
}
