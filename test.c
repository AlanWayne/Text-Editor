#include <ncurses.h>

int main() {
	initscr();
	noecho();
	raw();

	int count = 0;

	while (count < 5) {
		int ch = getch();
		printw("%d %c\n", ch, ch);
		++count;
	}

	endwin();
	return 0;
}
