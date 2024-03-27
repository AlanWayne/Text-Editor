#include <ncurses.h>

int main() {
	initscr();
	noecho();
	raw();
	
	while (1) {
		int ch = getch();
		printw("%d %c\n", ch, ch);
	}
	
	endwin();
	return 0;
	
}
