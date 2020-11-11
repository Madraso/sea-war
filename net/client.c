#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../logics/libfull.h"
#define LENGTH_OF_QUEUE 2
#define MAX_SIZE 32

/*
Это тестовый код. 
Идея такая:
Собираем весь код воедино, смотрим работает или нет.
Если тесты прошли, все работает, то потом разнесем по разным файлам и тд.
Сейчас будет просто сеть + логика (без атаки и проверки).

По идее, сейчас все выглядит так:
Игрок сначала создает свое поле, ставит корабли.
Потом создается дополнительный поток (который принимает сообщения).
Этот же поток анализирует.
Так как он имеет доступ к терминалу, то предлагаю оставить за ним и отображение всего.

Структура связи:
Игрок 1 посылает свое сообщение на сервер.
Вид сообщения:


/__________________________________/______________/______________/
/Дефолтное значение результата (2) / Координата X / Координата Y /
/----------------------------------/--------------/--------------/

Сервер перенаправляет это сообщение игроку 2.
Он анализирует принятое сообщение.
Если игрок 1 попал в корабль, то меняет дефолтное значение на "1".
Если игрок 1 попал в корабль, то меняет дефолтное значение на "0".

Далее он отправляет это сообщение обратно на сервер, который отправляет его игроку 1.

После отправки этого сообщения, игрок 2 начинает делать свой ход.


*/

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
ЗначениеРезультатаЧислоЧисло

*/
/*
Эта структура нужна для связи основного потока со вторым потоком.
Если нужно, чтобы клиент работал не только с двумя цифрами, то нужно добавить
сюда нужные поля. Потом перевести нужные байты в принятом сообщении в формат
этих полей и
приравнять их со структурой strc во втором потоке.
Как то так :)
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
  int result;
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


/* Отображение результата хода вот тут:*/
    // Здесь принятие и обработка сообщения
    strc->result = strc->str[0] - '0';
    strc->height = strc->str[1] - '0';
    strc->width = strc->str[2] - '0';

    printf("\n%d %d %d\n", strc->result,strc->height, strc->width);

    // Если результат пришел дефолтный, то сообщение нужно отправить на проверку своего поля, далее на отправку другому игроку и отображению на своем поле
    // (отправку сделать в виде фуннкции)
    // Если результат пришел отличный от дефолтного, тогда отображаем на экране

switch (strc->result)
{
case 2:
  // функция проверки
  //strc->result = ?
  //send(strc->sk, strc->str, MAX_SIZE, 0);
  //printf();
  break;
case 1:
  // функция отображения результата
  //printf();
  break;
case 0:
  // функция отображения результата
  //printf();
  break;

default:
  break;
}

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


/*                        Создание поля                                  */
/*_______________________________________________________________________*/

struct player *player1=(struct player*)calloc(1,sizeof(struct player));
for(int i=1;i<9;i++){
	for(int j=1;j<9;j++){
	player1->field[i][j]='1';
	
	}
}

player1->boat[0].length=1;
player1->boat[1].length=1;
player1->boat[2].length=1;
player1->boat[3].length=1;
player1->boat[4].length=2;
player1->boat[5].length=2;
player1->boat[6].length=2;
player1->boat[7].length=3;
player1->boat[8].length=3;
player1->boat[9].length=4;
placement((struct player* )player1);

/*                            Конец создания поля                        */
/*_______________________________________________________________________*/







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

  // Тут создается ход игрока. Нужно получить ХУ, потом поставить дефолтный результат
  // потом отправить и ждать ответа на в другом потоке.
  struct struct_to_thread ptr;
  ptr.sk = sock;
  ptr.str = str;
  ptr.sig = 0;
  ptr.result = 2;

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
