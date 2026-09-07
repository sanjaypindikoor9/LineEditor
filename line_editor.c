#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 500

typedef struct {
    char **lines;
    int count;
    int capacity;
} Document;

/* ---------- Utility ---------- */

static void trim_newline(char *text) {
    text[strcspn(text, "\n")] = '\0';
}

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void init_document(Document *doc) {
    doc->count = 0;
    doc->capacity = INITIAL_CAPACITY;
    doc->lines = malloc(doc->capacity * sizeof(char *));
    if (!doc->lines) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

static void free_document(Document *doc) {
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    free(doc->lines);
}

static void ensure_capacity(Document *doc) {
    if (doc->count < doc->capacity) return;

    int new_capacity = doc->capacity * 2;
    char **temp = realloc(doc->lines, new_capacity * sizeof(char *));
    if (!temp) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    doc->lines = temp;
    doc->capacity = new_capacity;
}

static char *duplicate_string(const char *text) {
    char *copy = malloc(strlen(text) + 1);
    if (!copy) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(copy, text);
    return copy;
}

/* ---------- Core Features ---------- */

static void display_document(const Document *doc) {
    if (doc->count == 0) {
        printf("\n[Document is empty]\n");
        return;
    }

    printf("\n----- DOCUMENT -----\n");
    for (int i = 0; i < doc->count; i++) {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }
    printf("--------------------\n");
}

static void insert_line(Document *doc, int line_number, const char *text) {
    if (line_number < 1 || line_number > doc->count + 1) {
        printf("Error: line number must be between 1 and %d.\n", doc->count + 1);
        return;
    }

    ensure_capacity(doc);

    int index = line_number - 1;

    for (int i = doc->count; i > index; i--) {
        doc->lines[i] = doc->lines[i - 1];
    }

    doc->lines[index] = duplicate_string(text);
    doc->count++;

    printf("Line %d inserted successfully.\n", line_number);
}

static void delete_line(Document *doc, int line_number) {
    if (doc->count == 0) {
        printf("Error: document is empty.\n");
        return;
    }

    if (line_number < 1 || line_number > doc->count) {
        printf("Error: line number must be between 1 and %d.\n", doc->count);
        return;
    }

    int index = line_number - 1;
    free(doc->lines[index]);

    for (int i = index; i < doc->count - 1; i++) {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->count--;

    printf("Line %d deleted successfully.\n", line_number);
}

/* ---------- Save / Load ---------- */

static void save_file(const Document *doc, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < doc->count; i++) {
        fprintf(file, "%s\n", doc->lines[i]);
    }

    fclose(file);
    printf("Document saved to '%s'.\n", filename);
}

static void load_file(Document *doc, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    doc->count = 0;

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), file)) {
        trim_newline(buffer);
        ensure_capacity(doc);
        doc->lines[doc->count++] = duplicate_string(buffer);
    }

    fclose(file);
    printf("Document loaded from '%s'. %d line(s) loaded.\n",
           filename, doc->count);
}

/* ---------- Bonus: Search ---------- */

static void search_document(const Document *doc, const char *term) {
    if (doc->count == 0) {
        printf("Document is empty.\n");
        return;
    }

    if (strlen(term) == 0) {
        printf("Error: search term cannot be empty.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < doc->count; i++) {
        if (strstr(doc->lines[i], term) != NULL) {
            printf("Found at line %d: %s\n", i + 1, doc->lines[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("'%s' was not found.\n", term);
    }
}

/* ---------- Bonus: Statistics ---------- */

static void show_count(const Document *doc) {
    int words = 0;

    for (int i = 0; i < doc->count; i++) {
        int in_word = 0;

        for (size_t j = 0; j < strlen(doc->lines[i]); j++) {
            if (!isspace((unsigned char)doc->lines[i][j])) {
                if (!in_word) {
                    words++;
                    in_word = 1;
                }
            } else {
                in_word = 0;
            }
        }
    }

    printf("Lines: %d\n", doc->count);
    printf("Words: %d\n", words);
}

/* ---------- Help ---------- */

static void show_help(void) {
    printf("\n========== LINE EDITOR HELP ==========\n");
    printf("insert <line> <text>  Insert text at a line number\n");
    printf("delete <line>         Delete a line\n");
    printf("display               Display the document\n");
    printf("save <file>           Save document to a .txt file\n");
    printf("load <file>           Load a .txt file\n");
    printf("search <text>         Search for text in the document\n");
    printf("count                 Show line and word count\n");
    printf("help                  Show this help\n");
    printf("exit                  Exit the editor\n");
    printf("\nExamples:\n");
    printf("  insert 1 Hello World\n");
    printf("  insert 2 This is line two\n");
    printf("  delete 1\n");
    printf("  save document.txt\n");
    printf("  search World\n");
    printf("======================================\n");
}

/* ---------- Command Processing ---------- */

static void process_command(Document *doc, char *input) {
    char command[20];

    if (sscanf(input, "%19s", command) != 1) {
        return;
    }

    if (strcmp(command, "insert") == 0) {
        int line_number;
        char text[MAX_LINE_LENGTH];

        char *p = input + strlen(command);
        while (isspace((unsigned char)*p)) p++;

        if (sscanf(p, "%d", &line_number) != 1) {
            printf("Usage: insert <line> <text>\n");
            return;
        }

        while (*p && !isspace((unsigned char)*p)) p++;
        while (isspace((unsigned char)*p)) p++;

        if (*p == '\0') {
            printf("Error: text cannot be empty.\n");
            return;
        }

        strncpy(text, p, MAX_LINE_LENGTH - 1);
        text[MAX_LINE_LENGTH - 1] = '\0';

        insert_line(doc, line_number, text);
    }
    else if (strcmp(command, "delete") == 0) {
        int line_number;

        if (sscanf(input, "%*s %d", &line_number) != 1) {
            printf("Usage: delete <line>\n");
            return;
        }

        delete_line(doc, line_number);
    }
    else if (strcmp(command, "display") == 0) {
        display_document(doc);
    }
    else if (strcmp(command, "save") == 0) {
        char filename[260];

        if (sscanf(input, "%*s %259s", filename) != 1) {
            printf("Usage: save <file>\n");
            return;
        }

        save_file(doc, filename);
    }
    else if (strcmp(command, "load") == 0) {
        char filename[260];

        if (sscanf(input, "%*s %259s", filename) != 1) {
            printf("Usage: load <file>\n");
            return;
        }

        load_file(doc, filename);
    }
    else if (strcmp(command, "search") == 0) {
        char *term = input + strlen(command);

        while (isspace((unsigned char)*term)) term++;

        search_document(doc, term);
    }
    else if (strcmp(command, "count") == 0) {
        show_count(doc);
    }
    else if (strcmp(command, "help") == 0) {
        show_help();
    }
    else if (strcmp(command, "exit") == 0) {
        printf("Exiting Line Editor. Goodbye!\n");
        exit(EXIT_SUCCESS);
    }
    else {
        printf("Unknown command: '%s'. Type 'help' for commands.\n", command);
    }
}

int main(void) {
    Document doc;
    char input[MAX_LINE_LENGTH + 50];

    init_document(&doc);

    printf("=====================================\n");
    printf("       SIMPLE LINE EDITOR IN C      \n");
    printf("=====================================\n");
    printf("Type 'help' to see available commands.\n");

    while (1) {
        printf("\n> ");

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        trim_newline(input);
        process_command(&doc, input);
    }

    free_document(&doc);
    return 0;
}
