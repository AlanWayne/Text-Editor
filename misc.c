#include "texteditor.h"

bool movement(int input, int* y, int* x, WINDOW_OBJECT window_text, int* height,
			  int lines) {
	if (input == 65) *y -= 1;
	if (input == 66) *y += 1;
	if (input == 67) *x += 1;
	if (input == 68) *x -= 1;

	if (*x < 0) {
		*x = window_text.width;
		*y -= 1;
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
	FILE* file = fopen(file_name, "r");
	int count_of_lines = 1;
	int ch = ' ';

	while ((ch = fgetc(file)) != EOF) {
		count_of_lines += (ch == ref);
	}

	fclose(file);

	return count_of_lines;
}

char** read_from_file(char* file_name, int count_of_lines) {
	FILE* file = fopen(file_name, "r");
	char buffer[1024];

	char** file_content = (char**)malloc(count_of_lines * sizeof(char*));
	for (int i = 0; i < count_of_lines; ++i) {
		file_content[i] = (char*)malloc(1024 * sizeof(char));
	}

	int i = 0;
	while (fgets(buffer, 1024, file) != NULL) {
		strcpy(file_content[i], buffer);
		++i;
	}

	fclose(file);

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