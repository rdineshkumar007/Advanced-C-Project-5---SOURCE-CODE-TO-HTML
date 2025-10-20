#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 2048
#define MAX_TOKEN_LENGTH 256

// Color scheme (exactly like chat terminal colors)
#define COLOR_KEYWORD "#569CD6"        // Blue for keywords
#define COLOR_FUNCTION "#DCDCAA"       // Light yellow for functions
#define COLOR_IDENTIFIER "#9CDCFE"     // Light blue for identifiers
#define COLOR_NUMBER "#B5CEA8"         // Green for numbers
#define COLOR_STRING "#CE9178"         // Orange for strings
#define COLOR_CHAR "#CE9178"           // Orange for characters
#define COLOR_COMMENT "#6A9955"        // Green for comments
#define COLOR_PREPROCESSOR "#C586C0"   // Pink for preprocessor
#define COLOR_OPERATOR "#D4D4D4"       // Light gray for operators

// C keywords
extern const char *keywords[];
extern const int num_keywords;

// Standard C library functions (will be colored as functions)
extern const char *std_functions[];
extern const int num_std_functions;

// Function prototypes
bool is_keyword(const char *str);
bool is_std_function(const char *str);
bool is_identifier_char(char c);
void write_html_header(FILE *output_file);
void write_html_footer(FILE *output_file);
void escape_html_char(FILE *output_file, char c);
void process_file(const char *input_filename, const char *output_filename);
bool has_html_extension(const char *filename);
bool file_exists(const char *filename);

#endif
