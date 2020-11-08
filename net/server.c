#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#define LENGTH_OF_QUEUE 2
#define MAX_SIZE 32

/*
Передаются аргументы: ip, port
Например:
./server 192.168.1.195 5674

Идея следующая:
Адрес сервера и порт получаем из аргументов.
У нас есть массив из двух адресов клиентов. Мы по очереди отсылаем им ходы.
Сначала первый игрок отсылает второму, потом второй игрок отсылает первому.
Всё это происходит в бесконечном цикле, пока не получим сигнал "stopgame"
(этот сигнал пробный, далее его необходимо заменить на другой, чтобы уменьшить
размер пакета)
После этого сигнал сервер выключается, освобождает всю память и закрывает сокет.

*/

int main(int argc, char *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (argc < 2) {
    printf("Too few arguments!");
    return 0;
  }
  if (argc > 3) {
    printf("Too much arguments!");
    return 0;
  }
  if (sock < 0) {
    perror("Error with initilization socket!");
    return 0;
  }
  int port = atoi(argv[2]);

  struct sockaddr_in str_sock;
  str_sock.sin_family = AF_INET;
  str_sock.sin_port = htons(port);
  str_sock.sin_addr.s_addr = inet_addr(argv[1]);

  if (bind(sock, (struct sockaddr *)&str_sock, sizeof(struct sockaddr_in)) <
      0) {
    perror("Bind error");
    return 0;
  }

  int test_listen;
  if (listen(sock, LENGTH_OF_QUEUE) < 0) {
    perror("Listen Error!");
    return 0;
  }

  int accept_socket[2];

  accept_socket[0] = accept(sock, NULL, NULL);
  accept_socket[1] = accept(sock, NULL, NULL);
  int count = 0;
  int mid_count;

  /*
  Работаем, пока не получим stopgame
  mid_count нужен для просчета получателя,
  если count = 0, то mid_count = 1.
  и наоборот
  Пакет отправляется в виде:
  ЧислоЧисло
  Если нужно добавить еще какую то информацию, то в исходниках клиента есть
  структура struct_to_thread.
  Там написано как это сделать.
  */
  while (1) {
    char *buffer1 = calloc(MAX_SIZE, sizeof(char));

    if (recv(accept_socket[count], buffer1, MAX_SIZE, 0) < 0) {
      perror("Receive error!");
      return 0;
    }
    printf("\n%s", buffer1);
    if (count == 0) {
      mid_count = 1;
    } else {
      mid_count = 0;
    }
    if (send(accept_socket[mid_count], buffer1, MAX_SIZE, 0) < 0) {
      perror("Sending Error!");
      return 0;
    }
    count++;
    if (count > 1) {
      count = 0;
    }

    if (!strncmp(buffer1, "stopgame", 8)) {
      free(buffer1);
      close(sock);
      return 0;
    }

    free(buffer1);
  }
}
