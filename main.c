#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct line {
    struct ch *ch_root;
    struct line *next;
    struct line *prev;
} typedef LineNode;

struct ch {
    char ch_value;
    struct ch *next;
    struct ch *prev;
} typedef CharNode;

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
void print_by_rows(int a, int b);

void cursor();

void find(char *text);

void stats();

int str_to_uint(char *str);

// File Operations
void load_document(char *file_name);

void save_document(char *file_name);

void init();

LineNode *line_root, *cursor_line_p;
CharNode *cursor_ch_p;

void move(int r, int c) {
    LineNode *line_iter = line_root;
    for (int i = 0; i < r - 1; i++) {
        if (line_iter->next == NULL) {
            printf("\n(%d, %d) gecersiz konum", r, c);
            return;
        }
        line_iter = line_iter->next;
    }

    CharNode *ch_iter = c > 0 ? line_iter->ch_root : NULL;
    for (int i = 0; i < c - 1; i++) {
        if (ch_iter->next == NULL) {
            printf("\n(%d, %d) gecersiz konum", r, c);
            return;
        }
        ch_iter = ch_iter->next;
    }
    cursor_line_p = line_iter;
    cursor_ch_p = ch_iter;
}
void test_move() {
    move(1, 12);
    printf("\n%c", cursor_ch_p -> ch_value); //r
    move(3, 12);
    printf("\n%c", cursor_ch_p -> ch_value); //k
}

void left(int n) {
    for (int i = 0; i < n; i++) {
        if (cursor_ch_p == NULL) {
            if (cursor_line_p->prev == NULL) return;
            cursor_line_p = cursor_line_p->prev;
            CharNode *ch_iter = cursor_line_p->ch_root;
            while (ch_iter != NULL && ch_iter->next != NULL)
                ch_iter = ch_iter->next;
            cursor_ch_p = ch_iter;
            continue;
        }
        cursor_ch_p = cursor_ch_p->prev;
    }
}

void right(int n) {
    for (int i = 0; i < n; i++) {
        if (cursor_ch_p != NULL && cursor_ch_p->next == NULL) {
            if (cursor_line_p->next == NULL) return;
            cursor_line_p = cursor_line_p->next;
            cursor_ch_p = NULL;
            continue;
        }
        cursor_ch_p = cursor_ch_p == NULL ? cursor_line_p->ch_root : cursor_ch_p->next;
    }
}

void up(int n) {
    for (int i = 0; i < n; i++) {
        if (cursor_line_p->prev == NULL) return;
        cursor_line_p = cursor_line_p->prev;
    }
    cursor_ch_p = NULL;
}

void down(int n) {
    for (int i = 0; i < n; i++) {
        if (cursor_line_p->next == NULL) return;
        cursor_line_p = cursor_line_p->next;
    }
    cursor_ch_p = NULL;
}

void insert(char *text) {
    if (cursor_ch_p == NULL) {
        cursor_ch_p = (CharNode *) malloc(sizeof(CharNode));
        cursor_ch_p->ch_value = *text++;
        cursor_ch_p->next = cursor_line_p->ch_root;
        cursor_ch_p->prev = NULL;
        cursor_line_p->ch_root = cursor_ch_p;
    }

    char ch;
    while ((ch = *text++) != '\0') {
        CharNode *temp = (CharNode *) malloc(sizeof(CharNode));
        temp->ch_value = ch;
        temp->next = cursor_ch_p->next;
        temp->prev = cursor_ch_p;
        cursor_ch_p->next = temp;

        cursor_ch_p = temp;
    }
}

void del(int n) {
    CharNode *ch_iter = cursor_ch_p == NULL ? cursor_line_p->ch_root : cursor_ch_p->next;
    LineNode *line_iter = cursor_line_p;
    for (int i = 0; i < n; i++) {
        if (ch_iter == NULL) {
            if (line_iter->next == NULL) return;
            LineNode *line_temp = line_iter->next;
            line_iter->next = line_temp->next;
            if (line_temp -> next != NULL)
                line_temp->next->prev = line_iter;

            ch_iter = line_temp->ch_root;
            free(line_temp);
            continue;
        }

        CharNode *temp = ch_iter;
        ch_iter = ch_iter->next;
        free(temp);
    }

    if (cursor_ch_p == NULL) {
        cursor_line_p->ch_root = ch_iter;
        if (ch_iter != NULL)
            ch_iter->prev = cursor_line_p->ch_root;
    } else {
        cursor_ch_p->next = ch_iter;
        if (ch_iter != NULL)
            ch_iter->prev = cursor_ch_p;
    }
}

void backspace(int n) {
    CharNode *temp_end = cursor_ch_p == NULL ? cursor_line_p -> ch_root : cursor_ch_p->next;
    for (int i = 0; i < n; i++) {
        if (cursor_ch_p == NULL) {
            if (cursor_line_p->prev == NULL) return;
            LineNode *temp_line = cursor_line_p;
            cursor_line_p = cursor_line_p->prev;
            cursor_line_p->next = temp_line->next;
            if (temp_line -> next != NULL)
                temp_line->next->prev = cursor_line_p;
            free(temp_line);

            CharNode *ch_iter = cursor_line_p->ch_root;
            while (ch_iter->next != NULL)
                ch_iter = ch_iter->next;

            cursor_ch_p = ch_iter;
            continue;
        }

        CharNode *temp_ch = cursor_ch_p;
        cursor_ch_p = cursor_ch_p->prev;
        free(temp_ch);
    }

    if (cursor_ch_p == NULL) {
        cursor_line_p->ch_root = temp_end;
        if (temp_end != NULL)
            temp_end->prev = cursor_line_p->ch_root;
    } else {
        cursor_ch_p->next = temp_end;
        if (temp_end != NULL)
            temp_end->prev = cursor_ch_p;
    }
}

void newline() {
    LineNode *temp_line = (LineNode *) malloc(sizeof(LineNode));
    temp_line->next = cursor_line_p->next;
    temp_line->prev = cursor_line_p;
    if (temp_line->next != NULL)
        temp_line->next->prev = temp_line;

    cursor_line_p->next = temp_line;
    temp_line->ch_root = cursor_ch_p == NULL ? cursor_line_p->ch_root : cursor_ch_p->next;
    if (temp_line->ch_root != NULL)
        temp_line->ch_root->prev = NULL;
    if (cursor_ch_p == NULL)
        cursor_line_p->ch_root = NULL;
    else
        cursor_ch_p->next = NULL;

    cursor_line_p = temp_line;
    cursor_ch_p = NULL;
}

void test_newline() {
    move(2, 0);
    newline();
    move(1, 13);
    newline();
    move(1, 5);
    newline();
    save_document("output.txt");
}

void delline(int r) {
    LineNode *line_iter = line_root;
    for (int i = 0; i < r - 1; i++) {
        if (line_iter->next == NULL) {
            printf("\n%d numarali satir bulunamadi", r);
            return;
        }
        line_iter = line_iter->next;
    }

    CharNode *ch_iter = line_iter->ch_root;
    while (ch_iter != NULL) {
        CharNode *temp_ch = ch_iter;
        ch_iter = ch_iter->next;
        free(temp_ch);
    }
    if (line_iter->prev == NULL) {
        if (line_iter->next == NULL) return;
        cursor_line_p = line_iter->next;
        cursor_line_p->prev = NULL;
        line_root = cursor_line_p;
    } else
        line_iter->prev->next = line_iter->next;

    if (line_iter->next != NULL)
        line_iter->next->prev = line_iter->prev;
    free(line_iter);
}

void test_delline() {
    delline(2);
    delline(1);
    save_document("output.txt");
}

void replace(char *searched, char *replaced) {
    LineNode *line_iter = line_root;
    while (line_iter != NULL) {
        CharNode *ch_iter = line_iter -> ch_root;
        CharNode *ch_begin = NULL;
        int ind = 0;
        while (ch_iter != NULL) {
            char ch = *(searched + ind);
            if (ch_iter -> ch_value == ch) {
                if (ind++ == 0) ch_begin = ch_iter;
                ch_iter = ch_iter -> next;

                if (*(searched + ind) == '\0' && ch_begin != NULL) {
                    CharNode *rm_iter = ch_begin;
                    ch_begin = ch_begin -> prev;
                    while (rm_iter != ch_iter) {
                        CharNode *temp = rm_iter;
                        rm_iter = rm_iter -> next;
                        free(temp);
                    }

                    int r_ind = 0;
                    char r_ch;
                    while ((r_ch = *(replaced + r_ind++)) != '\0') {
                        CharNode *temp = (CharNode*) malloc(sizeof(CharNode));
                        temp -> ch_value = r_ch;
                        temp -> prev = ch_begin;
                        temp -> next = NULL;

                        if (ch_begin == NULL)
                            line_iter -> ch_root = temp;
                        else
                            ch_begin -> next = temp;
                        ch_begin = temp;
                    }
                    ch_begin -> next = ch_iter;
                    if (ch_iter != NULL)
                        ch_iter -> prev = ch_begin;
                }
            }
            else if (ind == 0) {
                ch_iter =  ch_iter -> next;
            }
            else ind = 0;
        }
        line_iter = line_iter -> next;
    }
}

void test_replace() {
    replace("Huseyin\n", "Birol\n");
    save_document("output.txt");
}

void print() {
    LineNode *line_iter = line_root;
    int i = 1;
    while (line_iter != NULL) {
        printf("\n%d| ", i++);
        CharNode *ch_iter = line_iter->ch_root;
        while (ch_iter != NULL) {
            printf("%c", ch_iter->ch_value);
            ch_iter = ch_iter->next;
        }
        line_iter = line_iter->next;
    }
    printf("\n---");
}

void print_by_rows(int a, int b) {
    LineNode *line_iter = line_root;
    int i = 1;
    while (line_iter != NULL) {
        if (i < a) {
            line_iter = line_iter -> next;
            i++;
            continue;
        }
        if (i > b) break;
        printf("\n%d| ", i++);
        CharNode *ch_iter = line_iter->ch_root;
        while (ch_iter != NULL) {
            printf("%c", ch_iter->ch_value);
            ch_iter = ch_iter->next;
        }
        line_iter = line_iter->next;
    }
    printf("\n---");
}

void cursor() {
    int r = 1;
    LineNode *line_iter = line_root;
    while (line_iter != cursor_line_p) {
        line_iter = line_iter->next;
        r++;
    }

    int c;
    if (cursor_ch_p == NULL) c = 0;
    else {
        c = 1;
        CharNode *ch_iter = line_iter->ch_root;
        while (ch_iter != cursor_ch_p) {
            ch_iter = ch_iter->next;
            c++;
        }
    }
    printf("\nCURSOR %d %d", r, c);
}

void find(char *searched) {
    int r = 1;
    LineNode *line_iter = line_root;
    while (line_iter != NULL) {
        CharNode *ch_iter = line_iter -> ch_root;
        int ind = 0, c = 0, c_begin = -1;
        while (ch_iter != NULL) {
            char ch = *(searched + ind);
            if (ch_iter -> ch_value == ch) {
                if (ind++ == 0) c_begin = c;
                ch_iter = ch_iter -> next;
                c++;

                if (*(searched + ind) == '\0') {
                    printf("\n%d:%d", r, c_begin);
                }
            }
            else if (ind == 0) {
                ch_iter =  ch_iter -> next;
                c++;
            }
            else ind = 0;
        }
        line_iter = line_iter -> next;
        r++;
    }
}

void stats() {
    int rows = 0, chars = 0;
    LineNode *line_iter = line_root;
    while (line_iter != NULL) {
        rows++;
        CharNode *ch_iter = line_iter -> ch_root;
        while (ch_iter != NULL) {
            chars++;
            ch_iter = ch_iter -> next;
        }
        line_iter = line_iter -> next;
    }
    printf("\nLINES %d CHARS %d", rows, chars);
}

void load_document(char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Dosya okunamadi");
        return;
    }

    LineNode *line_iter = line_root;
    CharNode *ch_iter = line_root->ch_root; // Definitely NULL
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            LineNode *temp = (LineNode *) malloc(sizeof(LineNode));
            temp->ch_root = NULL;
            temp->next = NULL;
            temp->prev = line_iter;

            line_iter->next = temp;
            line_iter = temp;
            ch_iter = NULL;
            continue;
        }

        if (ch_iter == NULL) {
            ch_iter = (CharNode *) malloc(sizeof(CharNode));
            ch_iter->ch_value = (char) ch;
            ch_iter->next = NULL;
            ch_iter->prev = NULL;

            line_iter->ch_root = ch_iter;
            continue;
        }

        CharNode *temp = (CharNode *) malloc(sizeof(CharNode));
        temp->ch_value = (char) ch;
        temp->next = NULL;
        temp->prev = ch_iter;
        ch_iter->next = temp;

        ch_iter = temp;
    }
    fclose(file);
}

void save_document(char *file_name) {
    FILE *file = fopen(file_name, "w");

    LineNode *line_iter = line_root;
    CharNode *ch_iter = line_root->ch_root;

    while (ch_iter != NULL || line_iter->next != NULL) {
        if (ch_iter == NULL) {
            fputc('\n', file);
            line_iter = line_iter->next;
            ch_iter = line_iter->ch_root;
            continue;
        }

        char ch = ch_iter->ch_value;
        fputc(ch, file);
        ch_iter = ch_iter->next;
    }
    fclose(file);
}

void run_commands(char *command_file) {
    FILE *file = fopen(command_file, "r");
    if (file == NULL) {
        printf("\nKomut dosyasi okunamadi");
        return;
    }

    int cmd_row = 1;
    char command_line[30];
    while (fgets(command_line, sizeof(command_line), file) != NULL) {
        char *command = strtok(command_line, " \n");
        if (command == NULL || command[0] == '\n') continue;

        if (strcmp(command, "MOVE") == 0) {
            char *arg1 = strtok(NULL, " \n");
            char *arg2 = strtok(NULL, " \n");

            if (arg1 == NULL || arg2 == NULL) {
                printf("\nERROR %d: Eksik bagimsiz degisken", cmd_row);
                continue;
            }

            int r = str_to_uint(arg1);
            int c = str_to_uint(arg2);

            if (r < 0 || c < 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            move(r, c);
        }
        else if (strcmp(command, "LEFT") == 0) {
            char *arg1 = strtok(NULL, " \n");

            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            left(n);
        }
        else if (strcmp(command, "RIGHT") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            right(n);
        }
        else if (strcmp(command, "UP") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            up(n);
        }
        else if (strcmp(command, "DOWN") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            down(n);
        }
        else if (strcmp(command, "INSERT") == 0) {
            char *arg1 = strtok(NULL, "\"\n");
            insert(arg1);
        }
        else if (strcmp(command, "DELETE") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            del(n);
        }
        else if (strcmp(command, "BACKSPACE") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int n = str_to_uint(arg1);
            if (n <= 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            backspace(n);
        }
        else if (strcmp(command, "NEWLINE") == 0)
            newline();
        else if (strcmp(command, "DELLINE") == 0) {
            char *arg1 = strtok(NULL, " \n");
            int r = str_to_uint(arg1);
            if (r < 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }
            delline(r);
        }
        else if (strcmp(command, "REPLACE") == 0) {
            char *arg1 = strtok(NULL, " \"\n");
            char *arg2 = strtok(NULL, " \"\n");
            replace(arg1, arg2);
        }
        else if (strcmp(command, "PRINT") == 0) {
            char *arg1 = strtok(NULL, " []\n");
            char *arg2 = strtok(NULL, " []\n");
            if (arg1 == NULL) {
                print();
                continue;
            }
            int a = str_to_uint(arg1);
            int b = str_to_uint(arg2);

            if (a < 0 || b < 0) {
                printf("\nERROR %d: Gecersiz sayi", cmd_row);
                continue;
            }

            int rows = 0;
            LineNode *line_iter = line_root;
            while (line_iter != NULL) {
                rows++;
                line_iter = line_iter -> next;
            }
            if (b > rows || a > rows) {
                printf("\nERROR %d: Sinirlar uygun aralikta degil", cmd_row);
                continue;
            }
            print_by_rows(a, b);
        }
        else if (strcmp(command, "CURSOR") == 0) cursor();
        else if (strcmp(command, "FIND") == 0) {
            char *arg1 = strtok(NULL, " \"\n");
            find(arg1);
        }
        else if (strcmp(command, "STATS") == 0) stats();
        else if (strcmp(command, "EXIT") == 0) return;
        else printf("\nERROR %d: Tanimsiz komut");
        cmd_row++;
    }
}

int str_to_uint(char *str) {
    char *endptr;
    int decimal = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return -1;
    return decimal;
}

void init() {
    line_root = (LineNode *) malloc(sizeof(LineNode));
    line_root->ch_root = NULL;
    line_root->next = NULL;
    line_root->prev = NULL;

    cursor_line_p = line_root;
    cursor_ch_p = NULL;
}

int main(void) {
    init();
    load_document("metin.txt");
    run_commands("komutlar.txt");
    save_document("metin2.txt");
    return 0;
}
