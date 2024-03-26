#include "texteditor.h"

int main(int argc, char** argv) {
	if (argc < 2) return 1;

	initscr();
	noecho();
	refresh();

	WINDOW_OBJECT window_main = {stdscr, getmaxy(stdscr), getmaxx(stdscr)};

	WINDOW_OBJECT window_text_border = {
		newwin(window_main.height, window_main.width, 0, 0),
		window_text_border.height = getmaxy(window_text_border.window) - 1,
		window_text_border.width = getmaxx(window_text_border.window) - 1};

	WINDOW_OBJECT window_text = {
		newwin(window_main.height - 4, window_main.width - 4, 2, 2),
		window_text.height = getmaxy(window_text.window) - 1,
		window_text.width = getmaxx(window_text.window) - 1};

	box(window_text_border.window, 0, 0);
	mvwprintw(window_text_border.window, 0, 2, " [ %s ] ", argv[1]);
	wrefresh(window_text_border.window);

	int count_of_lines = count_char(argv[1], '\n');
	char** file_content = read_from_file(argv[1], count_of_lines);

	int input_char = ' ';
	int cur_pos_y = min(count_of_lines - 1, window_text.height - 1);
	int cur_pos_x = strlen(file_content[cur_pos_y]);
	int screen_height = max(count_of_lines - window_text.height, 0);

	for (int i = screen_height, k = 0; i < count_of_lines; ++i, ++k) {
		mvwprintw(window_text.window, k, 0, "%s", file_content[i]);
		wrefresh(window_text.window);
	}

	while (input_char = wgetch(window_text.window)) {
		if (input_char == 27) {
			input_char = wgetch(window_text.window);
			if (input_char == 27) {
				break;
			}
		}

		if (movement(input_char, &cur_pos_y, &cur_pos_x, window_text,
					 &screen_height)) {
			if (screen_height + cur_pos_y > count_of_lines) {
				screen_height -= 1;
				cur_pos_x = strlen(file_content[count_of_lines - 1] - 1);
			}

			wclear(window_text.window);
			for (int i = screen_height, k = 0; i < count_of_lines; ++i, ++k) {
				mvwprintw(window_text.window, k, 0, "%s", file_content[i]);
				wrefresh(window_text.window);
			}
		}

		cur_pos_x =
			min(cur_pos_x, strlen(file_content[cur_pos_y + screen_height]) - 1);

		// FIX TABS IN STRLEN

		// bool outside_line = true;
		// for (int i = cur_pos_x; i < getmaxx(window_text.window); ++i) {
		// 	if (mvwinch(window_text.window, cur_pos_y, i) != 32) {
		// 		outside_line = false;
		// 		break;
		// 	}
		// }

		// if (outside_line) {
		// 	if (input_char == 67 &&
		// 		mvwinch(window_text.window, cur_pos_y, cur_pos_x - 1) == 32) {
		// 		cur_pos_y += 1;
		// 		cur_pos_x = 0;
		// 	} else {
		// 		while (mvwinch(window_text.window, cur_pos_y, cur_pos_x - 1) ==
		// 				   32 &&
		// 			   cur_pos_x > 0) {
		// 			cur_pos_x -= 1;
		// 		}
		// 	}
		// }

		wmove(window_text.window, cur_pos_y, cur_pos_x);
		wrefresh(window_text_border.window);
	}

	delwin(window_text.window);
	delwin(window_text_border.window);
	endwin();

	for (int i = 0; i < count_of_lines; ++i) {
		free(file_content[i]);
	}
	free(file_content);

	printf("%d %d %d %d\n", screen_height, count_of_lines - window_text.height,
		   count_of_lines, window_text.height);

	return 0;
}

// mvwprintw(win_text_border, 0, getmaxx(win_text_border) - 17,
// 				  " [ %02d %02d %03d ] ", cur_pos_x, cur_pos_y,
// 				  mvwinch(win_text, cur_pos_y, cur_pos_x));