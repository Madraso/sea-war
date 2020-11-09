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
./client 192.168.1.195 5674

Здесь поступаем следующим образом:
Создаем принимающий поток, который получает сообщения о ходах другого игрока,
далее он передает сообщение, чтобы им мог воспользоваться основной поток.
В основном потоке отслылает данные на сервер.
"stopgame" завершает работу сервера и клиента (основного и второго потока).
Есть баг или фича, что если первый игрок завершит игру, то отключается он и
сервер, второй же
игрок продолжает висеть в игре пока не введет любой символ.

*/
/*
Структура пакет:
ЧислоЧисло

*/
/*
Эта структура нужна для связи основного потока со вторым потоком.
Если нужно, чтобы клиент работал не только с двумя цифрами, то нужно добавить
сюда нужные поля. Потом перевести нужные байты в принятом сообщении в формат
этих полей и
приравнять их со структурой strc во втором потоке.
Как то таr :)
*/
struct struct_to_thread {
  int sk;
  char *str;
  int sig;

  /*
  В эти данные записываются ходы игроков
  так как поле 10*10, то значения, принмаемые этими полями
  равны 0-9.
  */
  int height;
  int width;
};

void *recieve_stream(void *ptr) {
  struct struct_to_thread *strc = (struct struct_to_thread *)ptr;
  int *a;
  while (1) {
    strc->str = calloc(MAX_SIZE, sizeof(char));

    int recieve_status = recv(strc->sk, strc->str, MAX_SIZE, MSG_WAITALL);

    if (recieve_status < 0) {
      perror("Receive Error!");
      return 0;
    }

    if (!strncmp(strc->str, "stopgame", 8)) {
      strc->sig = 1;
      free(strc->str);
      return 0;
    }

    int height, width;
    strc->height = strc->str[0] - '0';
    strc->width = strc->str[1] - '0';

    printf("\n%d %d\n", strc->height, strc->width);

    free(strc->str);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Too few arguments!");
    return 0;
  }
  if (argc > 3) {
    printf("Too much arguments!");
    return 0;
  }
  pthread_t pthread_ID;
  char *str;
  int port = atoi(argv[2]);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Error with initilization socket!");
    return 0;
  }

  struct sockaddr_in str_sock;
  str_sock.sin_family = AF_INET;
  str_sock.sin_port = htons(port);
  str_sock.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr *)&str_sock, sizeof(struct sockaddr)) <
      0) {
    perror("Connect Error!");
    return 0;
  }

  struct struct_to_thread ptr;
  ptr.sk = sock;
  ptr.str = str;
  ptr.sig = 0;

  pthread_create(&pthread_ID, NULL, recieve_stream, &ptr);

  while (1) {
    str = calloc(MAX_SIZE, sizeof(char));
    sleep(0.1);
    if (ptr.sig == 1) {
      pthread_cancel(pthread_ID);
      free(str);
      close(sock);
      return 0;
    }
    fgets(str, MAX_SIZE, stdin);

    int send_status = send(sock, str, MAX_SIZE, 0);
    if (send_status < 0) {
      perror("Sending Error!");
      return 0;
    }
    if (!strncmp(str, "stopgame", 8)) {
      pthread_cancel(pthread_ID);
      free(str);
      close(sock);
      return 0;
    }

    free(str);
  }
}
