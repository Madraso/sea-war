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
    msg.health = HEALTH;
    /* Здесь должна быть генерация кораблей */



    for (int i = 0; i < SIZE_FIELD_Y; i++){
        for (int j = 0; j < SIZE_FIELD_Y; j++){
            player_str.field[i][j] = ' ';
        }
    }
    init_screen();
    draw_fields();
    
    int step_to_ch = 0;
    do{ 

        int v, h, len, distr;
        enter_coords_param(&v, &h, &distr, &len);

    
    int choice = test_coords_param(v,h,distr,len, &player_str);



    switch (choice)
    {
    case 0:
    
            for (int j = 1; j <= len; j++){
                player_str.field[v][h] = '*';
                v++;
                step_to_ch++;
        }
        place_ships(player_str.field);
        break;
    

    case 1:
      
       for (int j = 1; j <= len; j++){
       player_str.field[v][h] = '*';
       h++;
        step_to_ch++;
        }
        place_ships(player_str.field);   
        break;
    
    case 3:

    break;
    
    default:
        break;
    }

    } while (step_to_ch < HEALTH); // Считаем, что максимальная длинна всех кораблей - 20

// Тут генерация заканчивается. Потом доделаю условия для постановки кораблей
    enum move_state state;

    if ((player_num - '0') == 1) state = MY_MOVE;
    else if ((player_num - '0') == 2) state = ENEMY_MOVE;
    int id_of_exit = 0;
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

            id_of_exit = who_win(MY_MOVE, msg.health);
            if (id_of_exit == 1){
                close(sock_fd);
                deinit_screen();
                printf("\nYou win\n");
                return 0;
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
                    msg.health--;

        

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

            int send_status = send(sock_fd, (void *) &msg, sizeof(msg), 0);
            
            if (send_status < 0) {
                perror("send");
                exit(EXIT_FAILURE);
            }
            id_of_exit = who_win(MY_MOVE, msg.health);
            if (id_of_exit == 1){
                close(sock_fd);
                deinit_screen();
                printf("\nYou lose!\n");
                return 0;
            }            
            sleep(2);
            clear_label();
        }
    }
}
