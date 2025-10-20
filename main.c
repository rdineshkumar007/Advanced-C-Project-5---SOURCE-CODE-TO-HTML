/*
Name  : Dineshkumar R
Date  : 18/10/2025
Title : C program to convert C source code to HTML with syntax highlighting
*/
#include "headers.h"

int main(int argc, char *argv[])
{
    char output_filename[256];

    if (argc < 2)
    {
        printf("Error: No input file specified!\n");
        printf("Usage: %s input.c [output.html]\n", argv[0]);
        printf("Example: %s program.c\n", argv[0]);
        printf("Example: %s program.c result.html\n", argv[0]);
        return 1;
    }

    // Check if input file exists
    if (!file_exists(argv[1]))
    {
        printf("Error: Input file '%s' not found!\n", argv[1]);
        return 1;
    }

    // Determine output filename
    if (argc >= 3)
    {
        if (!has_html_extension(argv[2]))
        {
            printf("Error: Output file must have .html extension!\n");
            return 1;
        }
        strcpy(output_filename, argv[2]);
    }
    else
    {
        // Default output filename
        strcpy(output_filename, "result.html");
    }

    printf("Converting %s to %s...\n", argv[1], output_filename);
    process_file(argv[1], output_filename);
    printf("HTML file successfully generated: %s\n", output_filename);

    return 0;
}
