//
// Created by chi on 2020/1/14.
//

#ifndef TANCHISHE_SNACK_H
#define TANCHISHE_SNACK_H

#include <vector>
#include <tuple>
enum {
	NOTHING,    //没任何东西
	WALL,       //墙
	GIFT,       //食物
	SNAKE_HEAD, //蛇头
	SNAKE_BODY  //蛇的身体
};

enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct post {
	int x;
	int y;
};

struct GameBoard {
	using single_tile = std::vector<int>;
	using board_tile = std::tuple<int, single_tile>;

	board_tile tcs_board;   //游戏区域

	using snake_node = std::vector<post>;

	snake_node snake;   //蛇的位置，蛇头永远在vector头位置

	post food;  //食物的位置
	bool win{false};  //是否胜利
	int score{0};   //记录得分，每吃一个food得1分

	int direction; //蛇前进的方向

	GameBoard();
	GameBoard(int game_size);
	GameBoard(int game_size, single_tile preboard);

	bool has_game_win();
	bool check_input_direct(int direct);

	void move();

	void update();

	int check_game_win();

	void generate_food();
	void show_board();
};

void reset_board(GameBoard &board);
void get_user_input(GameBoard *board);

#endif //TANCHISHE_SNACK_H
