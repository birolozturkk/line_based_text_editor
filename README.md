# Line-Based Text Editor

A command-driven text editor written in C as a university **Data Structures course assignment**. The project uses doubly linked lists to manage document lines and characters dynamically.

## Features

- Cursor navigation between lines and characters
- Text insertion, deletion, and replacement
- Line manipulation
- Text search
- Document statistics
- File loading and saving
- Command execution from a file

## Commands

### Cursor Commands

| Command | Description |
|---|---|
| `MOVE r c` | Moves the cursor to line `r` and column `c`. |
| `LEFT n` | Moves the cursor `n` characters to the left, crossing to the previous line when necessary. |
| `RIGHT n` | Moves the cursor `n` characters to the right, crossing to the next line when necessary. |
| `UP n` | Moves the cursor `n` lines up and places it at the beginning of the destination line. |
| `DOWN n` | Moves the cursor `n` lines down and places it at the beginning of the destination line. |

### Editing Commands

| Command | Description |
|---|---|
| `INSERT "text"` | Inserts text at the cursor position. |
| `DELETE n` | Deletes `n` characters after the cursor. |
| `BACKSPACE n` | Deletes `n` characters before the cursor. |
| `NEWLINE` | Splits the current line at the cursor position. |
| `DELLINE r` | Deletes line `r`. |
| `REPLACE "old" "new"` | Replaces occurrences of the specified text with new text. |

### Display and Search Commands

| Command | Description |
|---|---|
| `PRINT` | Displays the entire document. |
| `PRINT [a b]` | Displays lines from `a` to `b`, inclusive. |
| `CURSOR` | Displays the current cursor position. |
| `FIND "text"` | Finds occurrences of the specified text and displays their line and column positions. |
| `STATS` | Displays the total number of lines and characters. |

### General Commands

| Command | Description |
|---|---|
| `EXIT` | Stops processing the current command. |

## Usage

The program loads the initial document from `metin.txt`, executes commands from `komutlar.txt`, and saves the modified document to `metin2.txt`.

Compile and run:

```bash
gcc main.c -o text-editor
./text-editor
```

Replace `main.c` with the name of your C source file if necessary.

## Technologies

- C
- Doubly linked lists
- Pointers and dynamic memory allocation
- File I/O
- String processing

