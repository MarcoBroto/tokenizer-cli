Text Tokenizer
==============
Created by Marco Soto

Prompts the user for a string and outputs the text tokens found in the given string. Supports shell style history access of previously entered strings (ex: '!3' returns the tokens of history item #3).
***************************

### Tokenizer.c
1. `string_length(char* str) ===> int`
> Calculates and returns the length of a string (neglects the null terminator).

2. `is_valid_character(char c) ===> int(boolean)`
> Determines if the character resides in the ascii ranges () and ().

3. `find_word_start(char* str, int offset) ===> int`
> Receives a string and an offset index and finds the starting index of the next valid string (consisting of valid characters) found in the given string; searching begins at the offset. Returns '-1' if there is no valid character found or the string is invalid.

4. `find_word_end(char* str, int offset) ===> int`
> Receives a string and an offset index and finds the ending index of the next valid string (consisting of valid characters) found in the given string; searching begins at the offset.

5. `count_words(char* str) ===> int`
> Returns the total number of valid strings found in a string.

6. `extractWord(char* str, int start, int end) ===> char*`
> Returns a substring beginning and ending at specified indices of a given string.

7. `tokenize(char* str) ===> char**`
> Tokenizer function that returns an array of tokens (strings) that consists of individual tokens found in a given string.

8. `print_tokens(char** words)`
> Prints the tokens inside a given token array.

9. `free_tokens(char** words`)
> Frees memory addresses associated with a given token array.

10. `prompt(char* buffer)`
> Prompts the user for a string input that will later be used to decompose into tokens; Buffer limit = 100

11. `stringToPosInt(char* str) ===> int`
> Helper method that converts a str consisting of numbers and returns the integer value of the number. Returns -1 if the string is invalid.


### History.c
1. `struct List(int id, char* str, List* next)`
> String history node. Consists of the string saved, the position or number of the string saved, and a link to the next history node if one exists, otherwise, 'next' is equal to NULL.

2. `struct Item(List* root)`
> Used to store the first element (node) of a history list.

3. `init_history() ===> List*`
> Initializes memory for a linked list that will store string history.

4. `add_history(List* list, char* str)`
> Adds a new string to a given history list.

5. `print_history(List* list)`
> Prints a formatted history given a history linked list.

6. `free_history(List* list)`
> Frees memory addresses associated with a given history and its respective linked list.