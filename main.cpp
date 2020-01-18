#include <iostream>
#include <ctime>
#include <pthread.h>
#include "snack.h"

using namespace std;

//接收按键的线程
void *func_th(void *game) {
	while(1){
		get_user_input((GameBoard*)game);
	}
}

int main() {
	GameBoard snake_board = GameBoard();

	reset_board(snake_board);

	pthread_t pid;

	int rc = pthread_create(&pid, NULL, func_th, (void*)&snake_board);

	if(rc) {
		exit(-1);
	}

	while(1)
	{
		snake_board.move();
		snake_board.update();

		snake_board.show_board();
		int ret = snake_board.check_game_win();

		if(ret == 1){
			cout << "you win!!!" << endl;
			break;
		}
		else if(ret == 2) {
			cout << "you lost!!!" << endl;
			break;
		}

		int start = time(NULL);
		while(time(NULL) - start < 2) {;}
	}

	pthread_exit(NULL);
	return 0;
}