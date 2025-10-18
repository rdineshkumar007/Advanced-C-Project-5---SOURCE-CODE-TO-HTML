/*
Nmae  : Dineshkumar R
Date  : 18/10/2025
Title : C program to convert C source code to HTML with syntax highlighting
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 256

// Standard compiler colors
#define COLOR_KEYWORD "#0000FF"        // Blue for keywords
#define COLOR_IDENTIFIER "#000000"     // Black for identifiers
#define COLOR_NUMBER "#FF00FF"         // Magenta for numbers
#define COLOR_STRING "#C41E3A"         // Red for strings
#define COLOR_CHAR "#C41E3A"           // Red for characters
#define COLOR_COMMENT "#008000"        // Green for comments
#define COLOR_PREPROCESSOR "#8B008B"   // Dark magenta for preprocessor
#define COLOR_OPERATOR "#000000"       // Black for operators

// C keywords
const char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

bool is_keyword(const char *str);
bool is_identifier_char(char c);

void process_file(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (!input_file || !output_file)
    {
        printf("Error opening files!\n");
        exit(1);
    }

    // Write HTML header
    fprintf(output_file, "<!DOCTYPE html>\n");
    fprintf(output_file, "<html>\n");
    fprintf(output_file, "<head>\n");
    fprintf(output_file, "<title>C Source Code</title>\n");
    fprintf(output_file, "<style>\n");
    fprintf(output_file, "body { font-family: Consolas, 'Courier New', monospace; background-color: #f8f8f8; margin: 20px; }\n");
    fprintf(output_file, ".code-container { background-color: white; padding: 20px; border: 1px solid #ddd; border-radius: 5px; }\n");
    fprintf(output_file, ".preprocessor { color: %s; font-weight: bold; }\n", COLOR_PREPROCESSOR);
    fprintf(output_file, ".keyword { color: %s; font-weight: bold; }\n", COLOR_KEYWORD);
    fprintf(output_file, ".number { color: %s; }\n", COLOR_NUMBER);
    fprintf(output_file, ".string { color: %s; }\n", COLOR_STRING);
    fprintf(output_file, ".char { color: %s; }\n", COLOR_CHAR);
    fprintf(output_file, ".comment { color: %s; }\n", COLOR_COMMENT);
    fprintf(output_file, ".operator { color: %s; }\n", COLOR_OPERATOR);
    fprintf(output_file, "</style>\n");
    fprintf(output_file, "</head>\n");
    fprintf(output_file, "<body>\n");
    fprintf(output_file, "<div class=\"code-container\">\n");
    fprintf(output_file, "<pre>\n");

    char line[MAX_LINE_LENGTH];
    bool in_comment = false;
    bool in_string = false;
    bool in_char = false;

    while (fgets(line, sizeof(line), input_file))
    {
        int i = 0;
        bool in_preprocessor = false;

        while (line[i] != '\0')
        {
            // Start of preprocessor line
            if (line[i] == '#' && !in_comment && !in_string && !in_char && !in_preprocessor)
            {
                fprintf(output_file, "<span class=\"preprocessor\">");
                in_preprocessor = true;
            }

            // Handle preprocessor line completely
            if (in_preprocessor)
            {
                if (line[i] == '<') fprintf(output_file, "&lt;");
                else if (line[i] == '>') fprintf(output_file, "&gt;");
                else if (line[i] == '&') fprintf(output_file, "&amp;");
                else fputc(line[i], output_file);

                if (line[i] == '\n' || line[i] == '\0')
                {
                    fprintf(output_file, "</span>");
                    in_preprocessor = false;
                }
                i++;
                continue;
            }

            // Handle comments
            if (!in_string && !in_char)
            {
                if (!in_comment && line[i] == '/' && line[i+1] == '/')
                {
                    fprintf(output_file, "<span class=\"comment\">");
                    while (line[i] != '\0' && line[i] != '\n')
                    {
                        if (line[i] == '<') fprintf(output_file, "&lt;");
                        else if (line[i] == '>') fprintf(output_file, "&gt;");
                        else if (line[i] == '&') fprintf(output_file, "&amp;");
                        else fputc(line[i], output_file);
                        i++;
                    }
                    fprintf(output_file, "</span>");
                    continue;
                }

                if (!in_comment && line[i] == '/' && line[i+1] == '*')
                {
                    fprintf(output_file, "<span class=\"comment\">");
                    in_comment = true;
                }

                if (in_comment && line[i] == '*' && line[i+1] == '/')
                {
                    fputc(line[i], output_file);
                    fputc(line[i+1], output_file);
                    fprintf(output_file, "</span>");
                    in_comment = false;
                    i += 2;
                    continue;
                }
            }

            if (in_comment)
            {
                if (line[i] == '<') fprintf(output_file, "&lt;");
                else if (line[i] == '>') fprintf(output_file, "&gt;");
                else if (line[i] == '&') fprintf(output_file, "&amp;");
                else fputc(line[i], output_file);
                i++;
                continue;
            }

            // Handle strings
            if (!in_char && line[i] == '"' && !in_comment)
            {
                if (!in_string)
                {
                    fprintf(output_file, "<span class=\"string\">");
                    in_string = true;
                }
                else
                {
                    fputc(line[i], output_file);
                    fprintf(output_file, "</span>");
                    in_string = false;
                    i++;
                    continue;
                }
            }

            // Handle characters
            if (!in_string && line[i] == '\'' && !in_comment && !in_char)
            {
                fprintf(output_file, "<span class=\"char\">");

                // Output the entire character literal
                fputc(line[i], output_file);  // Output the opening quote
                i++;

                // Output until closing quote (handle escaped quotes)
                while (line[i] != '\0' && line[i] != '\n')
                {
                    fputc(line[i], output_file);

                    // If we find a quote that's not escaped, end the character literal
                    if (line[i] == '\'' && (i == 0 || line[i-1] != '\\'))
                    {
                        i++;
                        break;
                    }
                    i++;
                }

                fprintf(output_file, "</span>");
                continue;
            }

            if (in_string || in_char)
            {
                if (line[i] == '<') fprintf(output_file, "&lt;");
                else if (line[i] == '>') fprintf(output_file, "&gt;");
                else if (line[i] == '&') fprintf(output_file, "&amp;");
                else fputc(line[i], output_file);
                i++;
                continue;
            }

            // Handle numbers
            if (isdigit(line[i]))
            {
                fprintf(output_file, "<span class=\"number\">");
                while (isdigit(line[i]) || line[i] == '.' || line[i] == 'e' || line[i] == 'E' ||
                       line[i] == '+' || line[i] == '-')
                {
                    fputc(line[i], output_file);
                    i++;
                }
                fprintf(output_file, "</span>");
                continue;
            }

            // Handle operators
            if (strchr("+-*/%=&|<>!~^(){}[];,:.", line[i]))
            {
                fprintf(output_file, "<span class=\"operator\">");
                fputc(line[i], output_file);
                i++;
                if (strchr("=+&-|<>", line[i]))
                {
                    fputc(line[i], output_file);
                    i++;
                }
                fprintf(output_file, "</span>");
                continue;
            }

            // Handle identifiers and keywords
            if (isalpha(line[i]) || line[i] == '_')
            {
                char token[MAX_TOKEN_LENGTH];
                int j = 0;

                while (is_identifier_char(line[i]) && j < MAX_TOKEN_LENGTH - 1)
                {
                    token[j++] = line[i++];
                }
                token[j] = '\0';

                if (is_keyword(token))
                {
                    fprintf(output_file, "<span class=\"keyword\">%s</span>", token);
                }
                else
                {
                    fprintf(output_file, "%s", token);
                }
                continue;
            }

            // Default case - preserve all whitespace and formatting
            if (line[i] == '<') fprintf(output_file, "&lt;");
            else if (line[i] == '>') fprintf(output_file, "&gt;");
            else if (line[i] == '&') fprintf(output_file, "&amp;");
            else fputc(line[i], output_file);
            i++;
        }

        // Close preprocessor if still open
        if (in_preprocessor)
        {
            fprintf(output_file, "</span>");
        }
    }

    fprintf(output_file, "</pre>\n");
    fprintf(output_file, "</div>\n");
    fprintf(output_file, "</body>\n");
    fprintf(output_file, "</html>\n");

    fclose(input_file);
    fclose(output_file);
}

bool is_keyword(const char *str)
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false;
}

bool is_identifier_char(char c)
{
    return isalnum(c) || c == '_';
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input.c output.html\n", argv[0]);
        return 1;
    }

    process_file(argv[1], argv[2]);
    printf("HTML file generated: %s\n", argv[2]);
    return 0;
}
