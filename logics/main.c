#include<stdio.h>
#include "libfull.h"

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
