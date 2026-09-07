# Line Editor - Help

## Description

This is a command-line line editor written in C. It stores document lines in memory using a dynamically allocated array of strings.

## Commands

### 1. Insert

Inserts a new line at the specified line number. Existing lines are shifted down.

```text
insert <line> <text>
```

Example:

```text
insert 1 Hello World
insert 2 This is my second line
```

### 2. Delete

Deletes the specified line. Lines below it are shifted up.

```text
delete <line>
```

Example:

```text
delete 1
```

### 3. Display

Displays all current lines with line numbers.

```text
display
```

### 4. Save

Writes the document to a text file.

```text
save <file>
```

Example:

```text
save document.txt
```

### 5. Load

Loads lines from an existing text file.

```text
load <file>
```

Example:

```text
load document.txt
```

### 6. Search

Searches for a word or phrase and reports matching line numbers.

```text
search <text>
```

Example:

```text
search World
```

### 7. Count

Shows the total number of lines and words.

```text
count
```

### 8. Help

Displays all available commands.

```text
help
```

### 9. Exit

Closes the editor.

```text
exit
```

## Example Session

```text
> insert 1 Hello World
Line 1 inserted successfully.

> insert 2 Welcome to Line Editor
Line 2 inserted successfully.

> display

----- DOCUMENT -----
1: Hello World
2: Welcome to Line Editor
--------------------

> search World
Found at line 1: Hello World

> count
Lines: 2
Words: 5

> save document.txt
Document saved to 'document.txt'.

> delete 1
Line 1 deleted successfully.

> display

----- DOCUMENT -----
1: Welcome to Line Editor
--------------------

> exit
```
