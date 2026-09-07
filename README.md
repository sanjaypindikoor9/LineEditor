# Simple Line Editor in C

## Portfolio Building - Studio Course 3rd Semester Coding Competition

A command-line line editor written in C.

## Data Structure

The project uses a **dynamic array of strings**.

Each document line is stored as a dynamically allocated C string, while the array of pointers grows when more space is required.

### Why a dynamic array?

- Simple to implement within the competition time.
- Direct access to a line using its line number.
- Insertion and deletion are straightforward using shifting.
- Memory grows as needed instead of using a fixed maximum number of lines.

### Trade-off

Insertion and deletion can require shifting multiple pointers, so they are O(n) in the worst case. Direct access to a line is O(1).

## Implemented Features

### Core
- Insert a line
- Delete a line
- Display the document
- Save / load a `.txt` file

### Bonus
- Search
- Line count / word count

## Compile

Using GCC:

```bash
gcc line_editor.c -o line_editor
```

On Windows with MinGW:

```powershell
gcc line_editor.c -o line_editor.exe
```

## Run

Linux/macOS:

```bash
./line_editor
```

Windows PowerShell:

```powershell
.\line_editor.exe
```

## Example

```text
> insert 1 Hello World
> insert 2 This is a line editor
> display

1: Hello World
2: This is a line editor

> search World
> count
> save document.txt
> exit
```

## Team Members

- Member 1: P Sanjay
- Member 2: Priyas RS
- Member 3: 

## Competition Notes

The competition requires the team to design the data structure, commands and core logic on paper before coding. Keep the handwritten design and submit it as required.
