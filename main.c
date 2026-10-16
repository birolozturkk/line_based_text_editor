#include <stdio.h>
#include <stdlib.h>



struct line {
    int n;
    struct ch *ch_root;
    struct l *next;
    struct l *prev;
} typedef LineNode;

struct ch {
    char ch_value;
    struct ch *next;
    struct ch *prev;
} typedef CharNode;

// Doubly LinkedList Functions
// void add_line_node(LineNode *root, int n) {
//     while (root -> next != NULL)
//         root = root -> next;
//
//     LineNode *temp = (LineNode*) malloc(sizeof(LineNode));
//     temp -> n = n;
//     temp -> next = NULL;
//     temp -> prev = root;
//     root -> next = temp;
// }

// Cursor Commands
void move(int r, int c);
void left(int n);
void right(int n);
void up(int n);
void down(int n);

// Edit Commands
void insert(char *text);
void del(int n);
void backspace(int n);
void newline();
void delline(int r);
void replace(char *searched, char *replaced);

// Display Commands
void print();
void cursor();
void find(char *text);
void stats();

// File Operations
void load_document(char *file_name);
void save_document(char *file_name);

void init();

bool debug = true;

LineNode *line_root, *cursor_line_p;
CharNode *cursor_ch_p;

void move(int r, int c) {
    LineNode *line_iter = line_root;
    for (int i = 0; i < r - 1; i++) {
        if (line_iter -> next == NULL) {
            printf("(%d, %d) gecersiz konum", r, c);
            return;
        }
        line_iter = line_iter -> next;
    }

    CharNode *ch_iter = c > 0 ? line_iter -> ch_root : NULL;
    for (int i = 0; i < c - 1; i++) {
        if (ch_iter -> next == NULL) {
            printf("(%d, %d) gecersiz konum", r, c);
            return;
        }
        ch_iter = ch_iter -> next;
    }
    cursor_line_p = line_iter;
    cursor_ch_p = ch_iter;

    if (debug)
        printf("Cursor char value: %c", cursor_ch_p -> ch_value);
}

void insert(char *text) {
    if (cursor_ch_p == NULL) {
        cursor_ch_p = (CharNode*) malloc(sizeof(CharNode));
        cursor_ch_p -> ch_value = *text++;
        cursor_ch_p -> next = cursor_line_p -> ch_root;
        cursor_ch_p -> prev = NULL;
        cursor_line_p -> ch_root = cursor_ch_p;
    }

    char ch;
    while ((ch = *text++) != '\n') {
        CharNode *temp = (CharNode*) malloc(sizeof(CharNode));
        temp -> ch_value = ch;
        temp -> next = NULL;
        temp -> prev = cursor_ch_p;
        cursor_ch_p -> next = temp;

        cursor_ch_p = temp;
    }
}

void del(int n) {

    CharNode *ch_iter = cursor_ch_p == NULL ? cursor_line_p -> ch_root : cursor_ch_p -> next;
    LineNode *line_iter = cursor_line_p;
    for (int i = 0; i < n - 1; i++) {
        if (ch_iter == NULL) {
            if (line_iter -> next == NULL) return;
            line_iter = line_iter -> next;
            ch_iter = line_iter -> ch_root;
        }

        CharNode *temp = ch_iter;
        ch_iter = ch_iter -> next;
        free(temp);
    }

    if (cursor_ch_p == NULL) {
        cursor_line_p -> ch_root = ch_iter;
        ch_iter -> prev = cursor_line_p -> ch_root;
    } else {
        cursor_ch_p -> next = ch_iter;
        ch_iter -> prev = cursor_ch_p;
    }
}

void backspace(int n) {
    CharNode *temp_end = cursor_ch_p -> next;
    for (int i = 0; i < n; i++) {
        if (cursor_ch_p == NULL) {
            if (cursor_line_p -> prev == NULL) return;
            LineNode *temp_line = cursor_line_p;
            cursor_line_p = cursor_line_p -> prev;
            cursor_line_p -> next = temp_line -> next;
            temp_line -> next -> prev = cursor_line_p;
            free(temp_line);

            CharNode *ch_iter = cursor_line_p -> ch_root;
            while (ch_iter -> next != NULL)
                ch_iter = ch_iter -> next;

            cursor_ch_p = ch_iter;
            continue;
        }

        CharNode *temp_ch = cursor_ch_p;
        cursor_ch_p = cursor_ch_p -> prev;
        free(temp_ch);
    }

    if (cursor_ch_p == NULL) {
        cursor_line_p -> ch_root = temp_end;
        temp_end -> prev = cursor_line_p -> ch_root;
    } else {
        cursor_ch_p -> next = temp_end;
        temp_end -> prev = cursor_ch_p;
    }
}

void load_document(char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Dosya okunamadi");
        return;
    }

    LineNode *line_iter = line_root;
    CharNode *ch_iter = line_root -> ch_root; // Definitely NULL
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            LineNode *temp = (LineNode*) malloc(sizeof(LineNode));
            temp -> n = line_iter -> n + 1;
            temp -> ch_root = NULL;
            temp -> next = NULL;
            temp -> prev = line_iter;

            line_iter -> next = temp;
            line_iter = temp;
            ch_iter = NULL;
            continue;
        }

        if (ch_iter == NULL) {
            ch_iter = (CharNode*) malloc(sizeof(CharNode));
            ch_iter -> ch_value = (char) ch;
            ch_iter -> next = NULL;
            ch_iter -> prev = NULL;

            line_iter -> ch_root = ch_iter;
            continue;
        }

        CharNode *temp = (CharNode*) malloc(sizeof(CharNode));
        temp -> ch_value = (char) ch;
        temp -> next = NULL;
        temp -> prev = ch_iter;
        ch_iter -> next = temp;

        ch_iter = temp;
    }
    fclose(file);
}

void save_document(char *file_name) {
    FILE *file = fopen(file_name, "w");

    LineNode *line_iter = line_root;
    CharNode *ch_iter = line_root -> ch_root;

    while (ch_iter != NULL || line_iter -> next != NULL) {
        if (ch_iter == NULL) {
            fputc('\n', file);
            line_iter = line_iter -> next;
            ch_iter = line_iter -> ch_root;
            continue;
        }

        char ch = ch_iter -> ch_value;
        fputc(ch, file);
        ch_iter = ch_iter -> next;
    }
    fclose(file);
}

void init() {
    line_root = (LineNode*) malloc(sizeof(LineNode));
    line_root -> n = 1;
    line_root -> ch_root = NULL;
    line_root -> next = NULL;
    line_root -> prev = NULL;

    cursor_line_p = line_root;
    cursor_ch_p = NULL;
}

int main(void) {
    init();
    load_document("input.txt");
    save_document("output.txt");
    move(1, 12);
    return 0;
}