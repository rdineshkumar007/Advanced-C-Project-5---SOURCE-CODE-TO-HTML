#include "headers.h"

// C keywords
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    "_Bool", "_Complex", "_Imaginary", "inline", "restrict", "bool", "true", "false"
};
const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Standard C library functions (will be colored as functions)
const char *std_functions[] = {
    "printf", "scanf", "fprintf", "fscanf", "sprintf", "sscanf",
    "fopen", "fclose", "fread", "fwrite", "fgets", "fputs",
    "malloc", "calloc", "realloc", "free",
    "strlen", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp",
    "strchr", "strstr", "strtok",
    "memcpy", "memmove", "memset", "memcmp",
    "isdigit", "isalpha", "isalnum", "islower", "isupper", "toupper", "tolower",
    "atoi", "atof", "atol", "itoa",
    "sqrt", "pow", "exp", "log", "log10", "sin", "cos", "tan",
    "abs", "fabs", "ceil", "floor",
    "exit", "system", "getchar", "putchar", "gets", "puts",
    "time", "clock", "rand", "srand",
    "fseek", "ftell", "rewind",
    "perror", "strerror", "feof", "ferror",
    "qsort", "bsearch",
    "setbuf", "setvbuf",
    "remove", "rename",
    "fgetc", "fputc", "fgetpos", "fsetpos",
    "clearerr", "ungetc",
    "vfprintf", "vprintf", "vsprintf",
    "snprintf", "vsnprintf",
    "isprint", "ispunct", "isspace", "iscntrl",
    "isgraph", "isxdigit",
    "strcoll", "strxfrm",
    "memchr",
    "strspn", "strcspn", "strpbrk", "strrchr",
    "strtod", "strtol", "strtoul",
    "calloc", "realloc",
    "abort", "atexit", "exit", "getenv", "system",
    "acos", "asin", "atan", "atan2", "cosh", "sinh", "tanh",
    "frexp", "ldexp", "modf", "fmod",
    "offsetof", "va_start", "va_arg", "va_end",
    "setjmp", "longjmp",
    "signal", "raise",
    "localeconv", "setlocale"
};
const int num_std_functions = sizeof(std_functions) / sizeof(std_functions[0]);

bool is_keyword(const char *str)
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false;
}

bool is_std_function(const char *str)
{
    for (int i = 0; i < num_std_functions; i++)
    {
        if (strcmp(str, std_functions[i]) == 0) return true;
    }
    return false;
}

bool is_identifier_char(char c)
{
    return isalnum(c) || c == '_';
}

bool has_html_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot) return false;
    return (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0);
}

bool file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

void write_html_header(FILE *output_file)
{
    fprintf(output_file, "<!DOCTYPE html>\n");
    fprintf(output_file, "<html>\n");
    fprintf(output_file, "<head>\n");
    fprintf(output_file, "<title>C Source Code</title>\n");
    fprintf(output_file, "<meta charset=\"UTF-8\">\n");
    fprintf(output_file, "<style>\n");
    fprintf(output_file, "body { font-family: 'Consolas', 'Courier New', monospace; background-color: #1E1E1E; margin: 20px; line-height: 1.4; color: #D4D4D4; }\n");
    fprintf(output_file, ".code-container { background-color: #1E1E1E; padding: 20px; border: 1px solid #444; border-radius: 8px; overflow-x: auto; box-shadow: 0 4px 8px rgba(0,0,0,0.3); }\n");
    fprintf(output_file, "pre { margin: 0; font-family: 'Consolas', 'Courier New', monospace; font-size: 14px; white-space: pre-wrap; }\n");
    fprintf(output_file, ".preprocessor { color: %s; font-weight: bold; }\n", COLOR_PREPROCESSOR);
    fprintf(output_file, ".keyword { color: %s; font-weight: bold; }\n", COLOR_KEYWORD);
    fprintf(output_file, ".function { color: %s; }\n", COLOR_FUNCTION);
    fprintf(output_file, ".identifier { color: %s; }\n", COLOR_IDENTIFIER);
    fprintf(output_file, ".number { color: %s; }\n", COLOR_NUMBER);
    fprintf(output_file, ".string { color: %s; }\n", COLOR_STRING);
    fprintf(output_file, ".char { color: %s; }\n", COLOR_CHAR);
    fprintf(output_file, ".comment { color: %s; font-style: italic; }\n", COLOR_COMMENT);
    fprintf(output_file, ".operator { color: %s; }\n", COLOR_OPERATOR);
    fprintf(output_file, "</style>\n");
    fprintf(output_file, "</head>\n");
    fprintf(output_file, "<body>\n");
    fprintf(output_file, "<h2 style=\"color: #569CD6;\">C Source Code</h2>\n");
    fprintf(output_file, "<div class=\"code-container\">\n");
    fprintf(output_file, "<pre>\n");
}

void write_html_footer(FILE *output_file)
{
    fprintf(output_file, "</pre>\n");
    fprintf(output_file, "</div>\n");
    fprintf(output_file, "</body>\n");
    fprintf(output_file, "</html>\n");
}

void escape_html_char(FILE *output_file, char c)
{
    switch(c) {
        case '<': fprintf(output_file, "&lt;"); break;
        case '>': fprintf(output_file, "&gt;"); break;
        case '&': fprintf(output_file, "&amp;"); break;
        default: fputc(c, output_file); break;
    }
}

void process_file(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        printf("Error: Cannot open input file '%s'\n", input_filename);
        exit(1);
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file)
    {
        printf("Error: Cannot create output file '%s'\n", output_filename);
        fclose(input_file);
        exit(1);
    }

    write_html_header(output_file);

    char line[MAX_LINE_LENGTH];
    bool in_multiline_comment = false;
    bool in_string = false;
    bool in_char = false;
    bool escape_next = false;

    while (fgets(line, sizeof(line), input_file))
    {
        int i = 0;
        bool in_preprocessor = false;

        // Check for preprocessor at line start
        if (line[0] == '#')
        {
            in_preprocessor = true;
        }
        else
        {
            // Skip leading whitespace to check for preprocessor
            int j = 0;
            while (isspace(line[j])) j++;
            if (line[j] == '#') in_preprocessor = true;
        }

        while (line[i] != '\0')
        {
            // Handle preprocessor lines
            if (in_preprocessor)
            {
                fprintf(output_file, "<span class=\"preprocessor\">");
                while (line[i] != '\0' && line[i] != '\n')
                {
                    escape_html_char(output_file, line[i]);
                    i++;
                }
                fprintf(output_file, "</span>");
                break;
            }

            // Handle escape sequences in strings and chars
            if ((in_string || in_char) && line[i] == '\\' && !escape_next)
            {
                escape_next = true;
                fputc(line[i], output_file);
                i++;
                continue;
            }

            // Handle multi-line comments
            if (!in_string && !in_char && !escape_next)
            {
                if (!in_multiline_comment && line[i] == '/' && line[i+1] == '*')
                {
                    fprintf(output_file, "<span class=\"comment\">/*");
                    in_multiline_comment = true;
                    i += 2;
                    continue;
                }

                if (in_multiline_comment && line[i] == '*' && line[i+1] == '/')
                {
                    fprintf(output_file, "*/</span>");
                    in_multiline_comment = false;
                    i += 2;
                    continue;
                }
            }

            if (in_multiline_comment)
            {
                escape_html_char(output_file, line[i]);
                i++;
                continue;
            }

            // Handle single-line comments
            if (!in_string && !in_char && !escape_next && line[i] == '/' && line[i+1] == '/')
            {
                fprintf(output_file, "<span class=\"comment\">");
                while (line[i] != '\0' && line[i] != '\n')
                {
                    escape_html_char(output_file, line[i]);
                    i++;
                }
                fprintf(output_file, "</span>");
                continue;
            }

            // Handle strings
            if (!in_char && !in_multiline_comment && line[i] == '"' && !escape_next)
            {
                if (!in_string)
                {
                    fprintf(output_file, "<span class=\"string\">\"");
                    in_string = true;
                }
                else
                {
                    fprintf(output_file, "\"</span>");
                    in_string = false;
                }
                i++;
                continue;
            }

            // Handle characters
            if (!in_string && !in_multiline_comment && line[i] == '\'' && !escape_next)
            {
                if (!in_char)
                {
                    fprintf(output_file, "<span class=\"char\">'");
                    in_char = true;
                }
                else
                {
                    fprintf(output_file, "'</span>");
                    in_char = false;
                }
                i++;
                continue;
            }

            // Handle content inside strings and characters
            if (in_string || in_char)
            {
                escape_html_char(output_file, line[i]);
                i++;
                escape_next = false;
                continue;
            }

            // Handle numbers
            if (isdigit(line[i]) && (i == 0 || !is_identifier_char(line[i-1])))
            {
                fprintf(output_file, "<span class=\"number\">");
                while (isdigit(line[i]) || line[i] == '.' || line[i] == 'e' ||
                       line[i] == 'E' || line[i] == '+' || line[i] == '-' ||
                       line[i] == 'x' || line[i] == 'X' ||
                       (tolower(line[i]) >= 'a' && tolower(line[i]) <= 'f'))
                {
                    escape_html_char(output_file, line[i]);
                    i++;
                }
                fprintf(output_file, "</span>");
                continue;
            }

            // Handle operators (FIXED - now they will have color)
            if (strchr("+-*/%=&|<>!~^?:(){}[];,." , line[i]))
            {
                fprintf(output_file, "<span class=\"operator\">");
                escape_html_char(output_file, line[i]);
                // Handle compound operators
                if (strchr("=+&-|<>", line[i+1]))
                {
                    i++;
                    escape_html_char(output_file, line[i]);
                }
                fprintf(output_file, "</span>");
                i++;
                continue;
            }

            // Handle identifiers, keywords and functions
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
                else if (is_std_function(token))
                {
                    fprintf(output_file, "<span class=\"function\">%s</span>", token);
                }
                else
                {
                    fprintf(output_file, "<span class=\"identifier\">%s</span>", token);
                }
                continue;
            }

            // Default case - preserve all other characters
            escape_html_char(output_file, line[i]);
            i++;
            escape_next = false;
        }

        // End of line - reset escape sequence flag
        escape_next = false;

        // Add newline character
        fprintf(output_file, "\n");
    }

    write_html_footer(output_file);

    fclose(input_file);
    fclose(output_file);
}
