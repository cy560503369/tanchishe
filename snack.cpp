//
// Created by chi on 2020/1/14.
//

#include <random>
#include <ctime>
#include <iostream>
#include "snack.h"

GameBoard::GameBoard() :GameBoard(20){

}

GameBoard::GameBoard(int game_size)
	:GameBoard(game_size, single_tile(game_size * game_size)){

}

GameBoard::GameBoard(int game_size, GameBoard::single_tile preboard)
	: tcs_board{game_size, preboard}{

}

bool GameBoard::has_game_win() {
	return win;
}

//0:无结果，1、胜利，2、失败
int GameBoard::check_game_win(){
	if(score >= 5) {
		win = true;
		return 1;
	}

	int gamesize = std::get<0>(tcs_board);
	if(std::get<1>(tcs_board)[snake[0].x * gamesize + snake[0].y] == WALL) {
		win = false;
		return 2;
	}

	for(int i = 3; i < snake.size(); i++){
		if((snake[0].x == snake[i].x) && (snake[0].y == snake[i].y))
		{
			win = false;
			return 2;
		}
	}
	return 0;
}

void GameBoard::move() {
	post tmp = snake[snake.size() - 1];

	for(int i = snake.size() - 1; i >= 1; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}

	switch(direction){
		case UP:
			snake[0].x -= 1;
			break;
		case DOWN:
			snake[0].x += 1;
			break;
		case LEFT:
			snake[0].y -= 1;
			break;
		case RIGHT:
			snake[0].y += 1;
		default:
			break;
	}

	if((snake[0].x == food.x) && (snake[0].y == food.y)) {
		score++;
		snake.push_back(tmp);
		generate_food();
	}
}

void GameBoard::update() {
	int gamesize = std::get<0>(tcs_board);
	// 清除上次的结果
	for(int i = 0; i < gamesize; i++){
		for(int j = 0; j < gamesize; j++){
			if(std::get<1>(tcs_board)[i*gamesize + j] != WALL){
				std::get<1>(tcs_board)[i*gamesize + j] = NOTHING;
			}
		}
	}

	//更新最新结果到棋盘
	for(int i = 0; i < snake.size(); i++) {
		if(i == 0) {
			if(std::get<1>(tcs_board)[snake[i].x * gamesize + snake[i].y] != WALL) {
				std::get<1>(tcs_board)[snake[i].x * gamesize + snake[i].y] = SNAKE_HEAD;
			}
		}
		else {
			std::get<1>(tcs_board)[snake[i].x * gamesize + snake[i].y] = SNAKE_BODY;
		}
	}

	std::get<1>(tcs_board)[food.x * gamesize + food.y] = GIFT;
}

bool GameBoard::check_input_direct(int direct) {
	int gamesize = std::get<0>(tcs_board);
	if(direct == UP) {
		if((snake[0].x == 0) || ((snake[0].x == snake[1].x + 1) && (snake[0].y == snake[1].y))){
			return false;
		}
	}
	else if(direct == DOWN) {
		if((snake[0].x == gamesize - 1) || ((snake[0].x == snake[1].x - 1) && (snake[0].y == snake[1].y))){
			return false;
		}
	}
	else if(direct == LEFT) {
		if((snake[0].y == 0) || ((snake[0].x == snake[1].x) && (snake[0].y == snake[1].y + 1))){
			return false;
		}
	}
	else if(direct == RIGHT) {
		if((snake[0].y == gamesize - 1) || ((snake[0].x == snake[1].x) && (snake[0].y == snake[1].y - 1))){
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

void GameBoard::generate_food() {
	int game_size = std::get<0>(tcs_board); //获取游戏区域的宽度

	post food_post;
	bool sucess;

	do {
		sucess = true;

		srand(time(NULL));
		food_post.x = rand() % (game_size - 2) + 1;
		food_post.y = rand() % (game_size - 2) + 1;

		for(int i=0; i < snake.size(); i++) {
			if((food_post.x == snake[i].x) && (food_post.y == snake[i].y)) {
				sucess = false;
				break;
			}
		}
	} while(!sucess);

	std::get<1>(tcs_board)[food_post.x * game_size + food_post.y] = GIFT;
	food = food_post;
}

void GameBoard::show_board() {
	int game_size = std::get<0>(tcs_board); //获取游戏区域的宽度

	system("clear");

	for(int i = 0; i < game_size; i++) {
		for(int j = 0; j < game_size; j++){
			if(std::get<1>(tcs_board)[i * game_size + j] == 1) {
				std::cout << "■ ";
			}
			else if(std::get<1>(tcs_board)[i * game_size + j] == 2) {
				std::cout << "★ ";
			}
			else if(std::get<1>(tcs_board)[i * game_size + j] == 3) {
				std::cout << "● ";
			}
			else if(std::get<1>(tcs_board)[i * game_size + j] == 4) {
				std::cout << "◎ ";
			}
			else {
				std::cout << "  ";
			}
//			std::wcout << std::get<1>(tcs_board)[i * game_size + j];
		}
		std::wcout << std::endl;
	}
	std::flush(std::wcout);
}

void get_user_input(GameBoard *board) {
	char c;
	std::cin >> c;

	switch(c) {
		case 'a':
			if(board->check_input_direct(LEFT)) {
				board->direction = LEFT;
			}
			break;
		case 's':
			if(board->check_input_direct(DOWN)) {
				board->direction = DOWN;
			}
			break;
		case 'd':
			if(board->check_input_direct(RIGHT)) {
				board->direction = RIGHT;
			}
			break;
		case 'w':
			if(board->check_input_direct(UP)) {
				board->direction = UP;
			}
			break;
		default:
			break;
	}
}

void reset_board(GameBoard &board) {
	int game_size = std::get<0>(board.tcs_board); //获取游戏区域的宽度

	post head;

	//产生随机数
	srand(time(NULL));

	head.x = rand() % (game_size - 2) + 1;
	head.y = rand() % (game_size - 2) + 1;

	board.snake.push_back(head); //放置蛇头

	std::get<1>(board.tcs_board)[head.x * game_size + head.y] = SNAKE_HEAD;

	//随机生成方向
	board.direction = rand() % 4;

	//根据方向放置蛇尾
	switch(board.direction) {
		case UP:
			if(board.check_input_direct(UP)){
				board.snake.push_back(post{head.x-1, head.y});
				std::get<1>(board.tcs_board)[(head.x-1) * game_size + head.y] = SNAKE_BODY;
				board.direction = UP;
				break;
			}
		case DOWN:
			if(board.check_input_direct(DOWN)){
				board.snake.push_back(post{head.x+1, head.y});
				std::get<1>(board.tcs_board)[(head.x+1) * game_size + head.y] = SNAKE_BODY;
				board.direction = DOWN;
				break;
			}
		case LEFT:
			if(board.check_input_direct(LEFT)){
				board.snake.push_back(post{head.x, head.y - 1});
				std::get<1>(board.tcs_board)[head.x * game_size + head.y - 1] = SNAKE_BODY;
				board.direction = LEFT;
				break;
			}
		case RIGHT:
			if(board.check_input_direct(RIGHT)){
				board.snake.push_back(post{head.x, head.y + 1});
				std::get<1>(board.tcs_board)[head.x * game_size + head.y + 1] = SNAKE_BODY;
				board.direction = RIGHT;
				break;
			}
		default:
			break;
	}

	//生成food
	board.generate_food();


	for(int i = 0; i < game_size; i++) {
		std::get<1>(board.tcs_board)[i] = WALL;
		std::get<1>(board.tcs_board)[i * game_size] = WALL;
		std::get<1>(board.tcs_board)[(i + 1) * game_size -1] = WALL;
		std::get<1>(board.tcs_board)[(game_size - 1) * game_size + i] = WALL;
	}
}
