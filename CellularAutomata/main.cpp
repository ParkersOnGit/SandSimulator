#include <iostream>
#include <array>
#include <thread>
#include <chrono>
#include <conio.h> // Windows specific.
#include <random>

const int WIDTH = 25, HEIGHT = 25;

std::string buffer;
std::array<std::array<int, WIDTH>, HEIGHT> board;
std::array<int, 2> cursorPosition = {(int)(WIDTH / 2), 1};

void printBoard() {
	buffer = "";

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x == cursorPosition[0] - 1 && y == cursorPosition[1] - 1) {
				buffer += "\033[48;2;150;10;10m";
			} 
			else if (board[y][x] == 0) {
				buffer += x % 2 - y % 2 == 0 ? "\033[48;2;100;100;100m" : "\033[48;2;95;95;95m";
			}
			else {
				std::mt19937 gen(x + 1 << y);
				std::uniform_int_distribution<int> distr(0, 15);
				buffer += "\033[48;2;" + std::to_string(200 - distr(gen)) + ";" + std::to_string(180 - distr(gen)) + ";130m";
			}
			buffer += "  ";
		}
		buffer += "\n";
	}

	// Return home and print buffer.
	std::cout << "\033[H"; 
	std::cout << buffer << std::flush;
}

void updateBoard() {
	for (int y = HEIGHT - 2; y > -1; y--) {
		for (int x = 0; x < WIDTH; x++) {
			if (board[y][x] != 1) continue;

			if (board[y + 1][x] != 1) {
				board[y][x] = 0;
				board[y + 1][x] = 1;
				continue;
			}

			if (x != WIDTH - 1 && board[y + 1][x + 1] == 0) {
				// Move to the right.
				board[y][x] = 0;
				board[y + 1][x + 1] = 1;
			}

			else if (x != 0 && board[y + 1][x - 1] == 0) {
				// Move to the left.
				board[y][x] = 0;
				board[y + 1][x - 1] = 1;
			}
		}
	}
}

void getInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if (cursorPosition[1] > 1) cursorPosition[1]--;
			break;
		case 'a':
			if (cursorPosition[0] > 1) cursorPosition[0]--;
			break;
		case 's':
			if (cursorPosition[1] < HEIGHT) cursorPosition[1]++;
			break;
		case 'd':
			if (cursorPosition[0] < WIDTH) cursorPosition[0]++;
			break;
		case ' ':
			board[cursorPosition[1] - 1][cursorPosition[0] - 1] = 1;
			break;
		}
	}
}

int main() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// Initial board print.
	printBoard();
	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	while (true) {
		getInput();
		updateBoard();
		printBoard();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}