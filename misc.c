#include "texteditor.h"

bool movement(int input, int* y, int* x, WINDOW_OBJECT window_text, int* height,
			  int lines, int* chars) {
	if (*x > chars[*y + *height] && (input == K_RIGHT || input == K_LEFT)) {
		*x = chars[*y + *height];
	}

	if (input == K_UP) *y -= 1;
	if (input == K_DOWN) *y += 1;
	if (input == K_RIGHT) *x += 1;
	if (input == K_LEFT) *x -= 1;

	if (*x < 0) {
		*y -= 1;
		*x = chars[*y + *height];
	}

	if (*x > chars[*y + *height] && input == K_RIGHT) {
		if (*y + *height <= lines) {
			*x = 0;
			*y += 1;
		} else {
			*x -= 1;
		}
	}

	if (*y < 0) {
		*height -= 1;
		*y = 0;

		if (*height < 0) {
			*height = 0;
			*x = 0;
			return false;
		}

		return true;
	}

	if (*y > lines) {
		*y = lines;

		return false;
	}

	if (*y > window_text.height) {
		*y = window_text.height;
		*height += 1;
		return true;
	}

	return false;
}

int count_char(char* file_name, int ref) {
	int count_of_lines = 1;

	FILE* file = fopen(file_name, "r");
	if (file != NULL) {
		int ch = ' ';

		while ((ch = fgetc(file)) != EOF) {
			count_of_lines += (ch == ref);
		}

		fclose(file);
	}

	return count_of_lines;
}

int* count_chars(char* file_name, int lines) {
	int* count_of_chars;
	FILE* file = fopen(file_name, "r");

	if (file != NULL) {
		count_of_chars = (int*)calloc(lines, sizeof(int));
		int ch = ' ';
		int i = 0;

		while ((ch = fgetc(file)) != EOF) {
			if (ch != '\n') {
				count_of_chars[i] += 1;

				if (ch == 9) {
					count_of_chars[i] += 3;
				}
			} else {
				i += 1;
			}
		}

		fclose(file);
	} else {
		count_of_chars = (int*)calloc(1, sizeof(int));
		count_of_chars[0] = 0;
	}

	return count_of_chars;
}

char** read_from_file(char* file_name, int count_of_lines) {
	char** file_content = (char**)malloc(count_of_lines * sizeof(char*));
	for (int i = 0; i < count_of_lines; ++i) {
		file_content[i] = (char*)malloc(1024 * sizeof(char));
	}

	FILE* file = fopen(file_name, "r");
	if (file != NULL) {
		char buffer[1024];

		int i = 0;
		int k = 0;
		int input = ' ';
		while (input = fgetc(file)) {
			if (input == '\n') {
				file_content[i][k] = '\0';
				++i;
				k = 0;
			} else if (input == '\t') {
				for (int j = 0; j < 4; ++j) {
					file_content[i][k] = ' ';
					++k;
				}
			} else if (input == EOF) {
				file_content[i][k] = '\0';
				break;
			} else {
				file_content[i][k] = input;
				++k;
			}
		}

		fclose(file);
	} else {
		file_content[0][0] = '\0';
	}

	return file_content;
}

int max(int a, int b) {
	if (a > b) return a;
	return b;
}

int min(int a, int b) {
	if (a < b) return a;
	return b;
}

void add_to_string(char* str, char ch, int pos) {
	size_t size = strlen(str) + 2;

	for (int i = size - 1; i >= pos; --i) {
		str[i] = str[i - 1];
	}

	str[pos] = ch;
}

void delete_from_string(char* str, int pos) {
	size_t size = strlen(str) + 1;

	for (int i = pos; i < size; ++i) {
		str[i] = str[i + 1];
	}
}

void delete_line(char** text, int* chars, int y, int lines) {
	strcat(text[y - 1], text[y]);

	free(text[y]);

	for (int i = y; i < lines - 1; ++i) {
		text[i] = text[i + 1];
		chars[i] = chars[i + 1];
	}

	// text = (char**)realloc(text, (lines - 1) * sizeof(char*));
}