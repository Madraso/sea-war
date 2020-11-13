#include<stdio.h>
#include<stdlib.h>
//#include<curses.h>
#include<dirent.h>
#include<string.h>
#include<pthread.h>
#include<stdbool.h>

#define FIELD_SIZE_ALL 12
#define FIELD_SIZE 10
#define NUM_1_BOAT 4
#define NUM_2_BOAT 3
#define NUM_3_BOAT 2
#define NUM_4_BOAT 1
#define NUM_5_BOAT 0


enum orientation{ 
	gorisont,
	vertical
};
struct aim{
int x;
int y;

};
struct ship{

int start_x;// начальное положение коробля по x
int start_y;// начальное положение коробля по y
enum orientation orientation;
int length; // длина коробля
//int deck[][];
};
struct player{
	
	int health;
	int NUM_OF_SHIP;
	int current_ship;
	char field[FIELD_SIZE][FIELD_SIZE];
	char *path;	
	struct ship boat[10];
};


void save_field(struct player *player){//сохранение поля игрока
FILE *fd;
fd=fopen(player->path,"r+");
for(int i=0;i<FIELD_SIZE;i++){
	printf("\n");
	for(int j=0;j<FIELD_SIZE;j++){
fprintf(fd,"%c",player->field[i][j]);
	}
}
pclose(fd);
}
void open_field(struct player *player){//открытие поля игрока
FILE *fd;
fd=fopen(player->path,"r");
for(int i=0;i<FIELD_SIZE;i++){
	for(int j=0;j<FIELD_SIZE;j++){
fscanf(fd,"%c",&player->field[i][j]);
	}
}
}
void drow_ship(struct player *player){//отрисовка кораблей
	for(int k=9;k>=player->current_ship;k--){
	if(player->boat[k].orientation==vertical){
	for(int i=player->boat[k].start_y;i<player->boat[k].start_y+player->boat[k].length;i++){
	player->field[i][player->boat[k].start_x]='0';	
	
	}
}
	if(player->boat[k].orientation==gorisont){

	for(int i=player->boat[k].start_x;i<player->boat[k].start_x+player->boat[k].length;i++){
	player->field[player->boat[k].start_y][i]='0';	
	
	}


}
}

}
void init_map(void* param){// создание карты и отрисовка кораблей
	struct player *player=(struct player*) param;
		open_field(player);	
	
	for(int i =0;i<FIELD_SIZE;i++){
		for(int j=0;j<FIELD_SIZE;j++){
	//	if(player->field[i][j]!='0'){
		player->field[i][j]=' ';
	//	}
		/*else{
		player->field[i][j]='0';
		}*/	
		if(((i==player->boat[player->current_ship].start_y)&&(j==player->boat[player->current_ship].start_x))&&(player->field[i][j]!='0')){
			player->field[i][j]='+';
			}
		
		else if(i==0){
			player->field[i][j]='#';	
		}
		
		else if(i==FIELD_SIZE-1){
			player->field[i][j]='#';	
		}
	
		
		else if(j==0){
			player->field[i][j]='#';	
		}
		
		else if(j==FIELD_SIZE-1){
			player->field[i][j]='#';	
			}
		drow_ship(player);
		printf("%c",player->field[i][j]);	
		}
			printf("\n");
	}	
save_field(player);
}
void init_map_other_player(struct player *player,struct aim *aim){//игровое поле, где игрок стреляет
	open_field(player);
	char field1[FIELD_SIZE][FIELD_SIZE];
	for(int i =0;i<FIELD_SIZE;i++){
		for(int j=0;j<FIELD_SIZE;j++){
		field1[i][j]=' ';
		if(player->field[aim->y][aim->x]=='0'){
		field1[aim->y][aim->x]='*';
		}
		if((i==aim->y)&&(j==aim->x)){
		field1[i][j]='+';
			}
		
		else if(i==0){
		field1[i][j]='#';	
		}
		
		else if(i==FIELD_SIZE-1){
		field1[i][j]='#';	
		}
	
		
		else if(j==0){
		field1[i][j]='#';	
		}
		
		else if(j==FIELD_SIZE-1){
		field1[i][j]='#';	
			}
		
		printf("%c",player->field[i][j]);	
		}
			printf("\n");
	}	

}
//	
//
//     xxxxxx
//     x0000x
//     xxxxxx
//     xxx
//     x0x
//     x0x
//     x0x
//     xxx
//проверки можем ли мы расположить корабль
bool check_place(struct player *player){									
	int i=player->current_ship;
	int b=player->boat[player->current_ship].length;	
	bool still[1][b+1];
		for(int k=0;k<2;k++){
		for(int j=0;j<=b+1;j++){
			if(player->boat[i].orientation==gorisont){
			if((player->field[player->boat[i].start_y+1][player->boat[i].start_x+(j-1)]==' ')&&
				(player->field[player->boat[i].start_y-1][player->boat[i].start_x+(j-1)]==' ')){
			
			
						
			still[k][j]=true;						
			}
	else{
	still[k][j]=false;
	
	}
			}
			if(player->boat[i].orientation==vertical){
			if((player->field[player->boat[i].start_y+(j-1)][player->boat[i].start_x-1]==' ')&&
			(player->field[player->boat[i].start_y+(j-1)][player->boat[i].start_x+1]==' ')){
			still[k][j]=true;						
			}
	else{
	still[k][j]=false;
	
	}
		
}}		
}
bool l=true;
for(int k=0;k<2;k++)	{
		for(int j=0;j<=b+1;j++){
	l= l && still[k][j];	
	
	
	}
		}
	
return l;
}
/*
void check_final_place(struct player *player){
	if(player->)	
	
				((player->boat[i].orientation==vertical)&&(player->field[player->boat[i].start_y+j][player->boat[i].start_x]!='0'))){

}*/

/*struct winsize size;
void sig_winch(int signo){
ioctl(fileno(stdout),TIOCGWINSZ,(char *)&size);
resizeterm(size.ws_row,size.ws_col);
}*/
	
/*void *usage(void *param){
printf("\n_________________\n");
printf("for start game enter  -s");
printf("");
printf("");
printf("");



}*/
void print_numship(struct player *player){
int p=player->current_ship;
	if(p<4)
		printf("odnopalubn\n");
		if((p>=4)&&(p<7))
			printf("2palub\n");
		if((p>=7)&&(p<9))
			printf("3palub\n");
		if(p==9)
			printf("4palub\n");

}
void placement(void *param){//расстановка короблей
struct player *p = (struct player*) param;
int x_size=FIELD_SIZE;//размеры поля по х
int y_size=FIELD_SIZE;//размеры поля по у
p->current_ship=9;
bool still_works=true;
init_map((struct player*)p);
for(int k=0;k<10;k++){
p->boat[k].orientation=gorisont;
p->boat[k].start_x=FIELD_SIZE/2;
p->boat[k].start_y=FIELD_SIZE/2;
}
while(still_works){
	print_numship(p);
	switch(getc(stdin)){//задание начальных точек
	case 'w':
		p->field[p->boat[p->current_ship].start_y][p->boat[p->current_ship].start_x]=' ';
		p->boat[p->current_ship].start_y--;
		if(p->boat[p->current_ship].start_y-1<0){
		p->boat[p->current_ship].start_y=FIELD_SIZE;
		}
		init_map((struct player*)p);
		break;
	case 's':
		p->field[p->boat[p->current_ship].start_y][p->boat[p->current_ship].start_x]=' ';
		p->boat[p->current_ship].start_y++;
		if(p->boat[p->current_ship].start_y+1>FIELD_SIZE){
		p->boat[p->current_ship].start_y=0;
		}
		init_map((struct player*)p);
		break;
	case 'a':
		p->field[p->boat[p->current_ship].start_y][p->boat[p->current_ship].start_x]=' ';
		p->boat[p->current_ship].start_x--;
		if(p->boat[p->current_ship].start_x-1<0){
			p->boat[p->current_ship].start_x=FIELD_SIZE;
		}
		init_map((struct player*)p);
		break;

	case 'd':
		p->field[p->boat[p->current_ship].start_y][p->boat[p->current_ship].start_x]=' ';
		p->boat[p->current_ship].start_x++;
		if(p->boat[p->current_ship].start_x+1>FIELD_SIZE){
		p->boat[p->current_ship].start_x=0;
		}
		init_map((struct player*)p);
		break;

/*	case 'e'://выбор коробля
		p->current_ship++;
		print_numship(p);
		if(p->current_ship==10)
			p->current_ship=0;
		drow_ship(p);
		init_map((struct player*)p);
		break;
	
	case 'q':
		p->current_ship--;
		print_numship(p);
		if(p->current_ship==-1)
			p->current_ship=9;
		drow_ship(p);
		init_map((struct player*)p);
		break;
*/
	case 'g'://выбор ориентцаии
		printf("gorisont");
		p->boat[p->current_ship].orientation=gorisont;
		init_map((struct player*)p);
		break;

	case 'v':
		printf("vertical");
		p->boat[p->current_ship].orientation=vertical;
		init_map((struct player*)p);
		break;
	case 'x':		
		if(check_place(p)){
			if(p->current_ship>=0){
			p->current_ship--;
			save_field(p);
			init_map((struct player*)p);
			}
			else{
			still_works=false;
			}
		}
		else{
		printf("anather place\n");
		}
		break;
		default: break;
		
     		}
	}	
}
/*
void start_game(void *param){
init_map();
placement();

while(hp!=0){
switch(){

}
}


}
*/

bool check_finish(struct player *player,struct player *player1){
if(player->health==0){

printf("2 player win");
return false;
}
if(player1->health==0){

printf("1 player win");
return false;
}

}
void shoot(struct aim *aim){

printf("Enter place\n");

scanf("%d",&aim->y);
scanf("%d",&aim->x);
}

void popal_or_nepopal(struct player *player,struct aim *aim){
shoot(aim);
init_map_other_player(player,aim);
if(player->field[aim->y][aim->x]!='0'){
printf("nepopal\n");
//player->health--;
}
else{
printf("popal\n");
player->health--;
}

}

int main(int argc,char *argv[]){
struct player *player1=(struct player*)calloc(1,sizeof(struct player));
struct player *player2=(struct player*)calloc(1,sizeof(struct player));
struct aim *aim1=(struct aim*)calloc(1,sizeof(struct aim));
struct aim *aim2=(struct aim*)calloc(1,sizeof(struct aim));
player1->path="player1_field.txt";
player2->path="player2_field.txt";
player1->health=20;
player2->health=20;
for(int i=1;i<9;i++){
	for(int j=1;j<9;j++){
	player1->field[i][j]='1';
	player2->field[i][j]='1';	
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
while(check_finish(player1,player2)){
popal_or_nepopal(player1,aim2);
}
free(player1);
free(player2);
free(aim1);
free(aim2);
return 0;
       }
