#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

typedef struct WINDOW_OBJECT {
	WINDOW* window;
	int height;
	int width;
} WINDOW_OBJECT;

#define K_UP 65
#define K_DOWN 66
#define K_RIGHT 67
#define K_LEFT 68

bool movement(int input, int* y, int* x, WINDOW_OBJECT window_text, int* height,
			  int lines, int* chars);
int count_char(char* file_name, int ref);
int* count_chars(char* file_name, int lines);
char** read_from_file(char* file_name, int count_of_lines);
void add_to_string(char* str, char ch, int pos);
void delete_from_string(char* str, int pos);
void delete_line(char** text, int* chars, int y, int lines);
int max(int a, int b);
int min(int a, int b);

#endif