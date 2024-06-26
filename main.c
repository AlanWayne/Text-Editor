#include "texteditor.h"

int main(int argc, char** argv) {
	if (argc < 2) return 1;

	// ------------------------[ INIT ]------------------------

	initscr();
	noecho();
	refresh();

	// --- [ windows ] ---

	WINDOW_OBJECT window_main = {stdscr, getmaxy(stdscr), getmaxx(stdscr)};

	WINDOW_OBJECT window_text_border = {
		newwin(window_main.height, window_main.width, 0, 0),
		window_text_border.height = getmaxy(window_text_border.window) - 1,
		window_text_border.width = getmaxx(window_text_border.window) - 1};

	WINDOW_OBJECT window_text = {
		newwin(window_main.height - 4, window_main.width - 4, 2, 2),
		window_text.height = getmaxy(window_text.window) - 1,
		window_text.width = getmaxx(window_text.window) - 1};

	// --- [ read file ] ---

	int count_of_lines = count_char(argv[1], '\n');
	int* count_of_chars = count_chars(argv[1], count_of_lines);
	char** file_content = read_from_file(argv[1], count_of_lines);

	int input_char = ' ';
	int cur_pos_y = min(count_of_lines - 1, window_text.height - 1);
	int cur_pos_x = strlen(file_content[cur_pos_y]);
	int screen_height = max(count_of_lines - window_text.height - 1, 0);

	// --- [ first draw ] ---

	box(window_text_border.window, 0, 0);
	mvwprintw(window_text_border.window, 0, 2, " [ %s ] ", argv[1]);
	wrefresh(window_text_border.window);

	for (int i = screen_height, k = 0; i < count_of_lines; ++i, ++k) {
		mvwprintw(window_text.window, k, 0, "%s", file_content[i]);
		wrefresh(window_text.window);
	}

	// ------------------------[ loop ]------------------------

	while (input_char = wgetch(window_text.window)) {
		// --- [ read system keys ] ---

		bool redraw = false;

		if (input_char == 27) {
			input_char = wgetch(window_text.window);

			if (input_char == 91) {
				// --- [ arrow movement ] ---

				input_char = wgetch(window_text.window);

				redraw =
					movement(input_char, &cur_pos_y, &cur_pos_x, window_text,
							 &screen_height, count_of_lines, count_of_chars);

				if (redraw) {
					if (screen_height + cur_pos_y >= count_of_lines) {
						screen_height -= 1;
						cur_pos_x = count_of_chars[cur_pos_y + screen_height];
					}
				}
			} else {
				// --- [ escape key ] ---

				break;
			}
		} else if (input_char == 127) {
			// ---- [ backspace ] ----

			int cursor = cur_pos_y + screen_height;
			delete_from_string(file_content[cursor], cur_pos_x - 1);

			bool delete_raw = (cur_pos_x == 0);

			movement(K_LEFT, &cur_pos_y, &cur_pos_x, window_text,
					 &screen_height, count_of_lines, count_of_chars);
			count_of_chars[cursor] -= 1;
			if (count_of_chars < 0) count_of_chars = 0;

			if (delete_raw) {
				delete_line(file_content, count_of_chars, cursor,
							count_of_lines);
				count_of_lines -= 1;
			}

			redraw = true;
		} else {
			// --- [ read regular symbols ] ---

			int cursor = cur_pos_y + screen_height;
			add_to_string(file_content[cursor], input_char, cur_pos_x);
			count_of_chars[cursor] += 1;

			movement(K_RIGHT, &cur_pos_y, &cur_pos_x, window_text,
					 &screen_height, count_of_lines, count_of_chars);
			redraw = true;
		}
		// --- [ loop draw ] ---

		if (redraw) {
			wclear(window_text.window);
			for (int i = screen_height, k = 0; i < count_of_lines; ++i, ++k) {
				mvwprintw(window_text.window, k, 0, "%s", file_content[i]);
			}
		}

		// --- [ move cursor ] --

		wmove(window_text.window, cur_pos_y,
			  min(cur_pos_x, count_of_chars[cur_pos_y + screen_height]));

		wrefresh(window_text.window);
	}

	// ------------------------[ exit ]------------------------

	// --- [ delete windows ] --

	delwin(window_text.window);
	delwin(window_text_border.window);
	endwin();

	// --- [ free memory ] --

	for (int i = 0; i < count_of_lines; ++i) {
		printf("%s\n", file_content[i]);
		free(file_content[i]);
	}
	free(file_content);
	free(count_of_chars);

	return 0;
}
