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
struct ship{

int start_x;// начальное положение коробля по x
int start_y;// начальное положение коробля по y
enum orientation orientation;
int length; // длина коробля
//int deck[][];
};
struct player{

	int shoot;
	int player_helth;
	int NUM_OF_SHIP;
	char field[FIELD_SIZE][FIELD_SIZE];
	int current_ship;
	struct ship boat[10];
};


void drow_ship(struct player *player){
	if(player->boat[player->current_ship].orientation==gorisont){
		
	for(int i=player->boat[player->current_ship].start_x;i<player->boat[player->current_ship].start_x+player->boat[player->current_ship].length;i++){
	player->field[i][player->boat[player->current_ship].start_x]='0';	
	
	}
}
else{

	for(int i=player->boat[player->current_ship].start_y+player->boat[player->current_ship].length;i>player->boat[player->current_ship].start_y;i--){
	player->field[player->boat[player->current_ship].start_y][i]='0';	
	
	}


}

}
void init_map(void* param){
	struct player *player=(struct player*) param;
	for(int i =0;i<FIELD_SIZE;i++){
		for(int j=0;j<FIELD_SIZE;j++){
		if(player->field[i][j]=='1')
		player->field[i][j]=' ';	
		if((i==player->boat[player->current_ship].start_x)&&(j==player->boat[player->current_ship].start_y)){
			player->field[i][j]='*';
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
		printf("%c",player->field[i][j]);	
		}
	
			printf("\n");
	}


}

//проверки можем ли мы расположить корабль
void check_place(struct player *player){
	for(int i=player->current_ship-1;i>0;i--){
		
	int l=player->boat[i].length;
		for(int j=0;j<l;j++){
			if(player->boat[player->current_ship].orientation==gorisont){
				if(((player->boat[player->current_ship].start_x!=player->boat[i].start_x+l)&&
				(player->boat[player->current_ship].start_y!=player->boat[i].start_y))||
				((player->boat[player->current_ship].start_x!=player->boat[i].start_x+l)||	
			  	(player->boat[player->current_ship].start_x!=player->boat[i].start_x-1))&&
			 	((player->boat[player->current_ship].start_y!=player->boat[i].start_y+1)||
			  	(player->boat[player->current_ship].start_y!=player->boat[i].start_y-1))){
			
			drow_ship(player);
			
			}
	else{
	perror("place is error ");
	}
			}
			else{
			
				if(((player->boat[player->current_ship].start_x!=player->boat[i].start_x)&&
				(player->boat[player->current_ship].start_y!=player->boat[i].start_y+l))||
				((player->boat[player->current_ship].start_x!=player->boat[i].start_x+1)||	
			  	(player->boat[player->current_ship].start_x!=player->boat[i].start_x-1))&&
			 	((player->boat[player->current_ship].start_y!=player->boat[i].start_y+l)||
			  	(player->boat[player->current_ship].start_y!=player->boat[i].start_y-1))){
			
			drow_ship(player);
			
			}
	else{
	perror("place is error ");
	}
			
			
			}
		
		}
}
}
/*
void check_final_place(struct player *player){
	if(player->)	
	

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

void placement(void *param){
struct player *p = (struct player*) param;
int x_size=FIELD_SIZE;//размеры поля по х
int y_size=FIELD_SIZE;//размеры поля по у
p->current_ship=0;
bool still_works=true;
p->boat[p->current_ship].orientation=gorisont;
p->boat[p->current_ship].start_x=FIELD_SIZE/2;
p->boat[p->current_ship].start_y=FIELD_SIZE/2;
init_map((struct player*)p);
while(still_works){
switch(getc(stdin)){//задание начальных точек
	case 'a':
		p->field[p->boat[p->current_ship].start_x][p->boat[p->current_ship].start_y]=' ';
		p->boat[p->current_ship].start_y--;
		if(p->boat[p->current_ship].start_y-1<0){
		p->boat[p->current_ship].start_y=FIELD_SIZE;
		}
		init_map((struct player*)p);
		break;
	case 'd':
		p->field[p->boat[p->current_ship].start_x][p->boat[p->current_ship].start_y]=' ';
		p->boat[p->current_ship].start_y++;
		if(p->boat[p->current_ship].start_y+1>FIELD_SIZE){
		p->boat[p->current_ship].start_y=0;
		}
		init_map((struct player*)p);
		break;
	case 'w':
		p->field[p->boat[p->current_ship].start_x][p->boat[p->current_ship].start_y]=' ';
		p->boat[p->current_ship].start_x--;
		if(p->boat[p->current_ship].start_x-1<0){
			p->boat[p->current_ship].start_x=FIELD_SIZE;
		}
		init_map((struct player*)p);
		break;

	case 's':
		p->field[p->boat[p->current_ship].start_x][p->boat[p->current_ship].start_y]=' ';
		p->boat[p->current_ship].start_x++;
		if(p->boat[p->current_ship].start_x+1>FIELD_SIZE){
		p->boat[p->current_ship].start_x=0;
		}
		init_map((struct player*)p);
		break;

	case 'e':
		p->current_ship++;
		if(p->current_ship==10)
			p->current_ship=0;
		init_map((struct player*)p);
		break;
	
	case 'q':
		p->current_ship--;
		if(p->current_ship==-1)
			p->current_ship=9;
		init_map((struct player*)p);
		break;

	case 'g':
		p->boat[p->current_ship].orientation=gorisont;
		init_map((struct player*)p);
		break;

	case 'v':
		p->boat[p->current_ship].orientation=vertical;
		init_map((struct player*)p);
		break;
	case 'x':		
		check_place(p);
		//drow_ship(p);
		//printf("\n %d \t %d ",p->boat[p->current_ship].start_x,p->boat[p->current_ship].start_y);
		p->current_ship++;
		init_map((struct player*)p);
		break;
		default: break;
		
     		}
	}	
}

int main(int argc,char *argv[]){
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
return 0;
       }
