#include "../include/server.h"
#include "../include/logic.h"

void *handle_player(void *args) {
    int *fd = (int *) args;
    struct player_message msg;

    while (1) {
        if (recv(fd[0], (void *) &msg, sizeof(msg), 0) < 0) {
            perror("receive");
            pthread_exit(NULL);
        }

        if (send(fd[1], (void *) &msg, sizeof(msg), 0) < 0) {
            perror("send");
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char *argv[]) {
    system("clear");

    if (argc != 3) {
        printf("Запускать как ./server ip_address port");
        exit(EXIT_FAILURE);
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

    if (bind(sock_fd, (struct sockaddr *) &server_str, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int players_fd[2], players_fd_copy[2];

    printf("Ожидание подключения первого игрока...\n");
    players_fd[0] = accept(sock_fd, NULL, NULL);

    printf("Ожидание подключения второго игрока...\n");
    players_fd[1] = accept(sock_fd, NULL, NULL);

    printf("Оба игрока подключились!\n");

    int send_status = send(players_fd[0], "1", 1, 0);
    if (send_status < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    send_status = send(players_fd[1], "2", 1, 0);
    if (send_status < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    pthread_t tid_1, tid_2;

    pthread_create(&tid_1, NULL, handle_player, (void *) &players_fd);

    players_fd_copy[0] = players_fd[1];
    players_fd_copy[1] = players_fd[0];

    pthread_create(&tid_2, NULL, handle_player, (void *) &players_fd_copy);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
}
