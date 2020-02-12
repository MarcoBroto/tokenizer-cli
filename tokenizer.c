#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "history.h"

#define BUFFER_SIZE 100 // Size of input buffer
static const wchar_t BOMBA = 0x1F4A3; // Unicode of 💣 [bomb]

int string_length(char* str) {
	if (str == NULL) return -1;
	int strLen = 0;
	while (str[strLen] != '\0')
		strLen++;
	return strLen;
}


int is_valid_character(char c) {
	int charVal = (int)c;
	/* 
	 * Letters -> uppercase 65-90, lowercase 97-122
	 * Numbers -> 48-57
	 */
	return (charVal >= 48 && charVal <= 57) || // Numbers
		(charVal >= 65 && charVal <= 90) || // Uppercase letters
		(charVal >= 97 && charVal <= 122);  // Lowercase letters
}


int find_word_start(char* str, int offset) {
	if (str == NULL) return -1;
	int ind = offset;
	while (ind > 0 && is_valid_character(str[ind]))
		ind--;
	while (str[ind] != '\0' && !is_valid_character(str[ind]))
		ind++;
	return ind;
}


/*
 * Returns exact zero-based index of last character in a found word.
 */
int find_word_end(char* str, int offset) {
	if (str == NULL) return -1;
	int ind = offset;
	while (str[ind] != '\0' && !is_valid_character(str[ind]))
		ind++;
	while (is_valid_character(str[ind]))
		ind++;
	return ind-1;
}


int count_words(char* str) {
	int numWords = 0, i = 0;
	int strlen = string_length(str);
	while (str[i] != '\0') {
		i = find_word_end(str, i);
		if (is_valid_character(str[i])) numWords++; // Ignore invalid characters at end
		i++;
	}
	return numWords;
}


char* extractWord(char* str, int start, int end) {
	if (str == NULL || start < 0 || end > BUFFER_SIZE) return NULL; // Invalid string and/or offsets, end program
	else if (start > end) return NULL; // Invalid offsets, end program]
	int size = end-start+2; // Adding 2 to account for null character and zero based index
	if (size < 2) return NULL;

	char* word = (char*)malloc(sizeof(char)*size);
	word[size-1] = '\0';
	for (int i = start; i <= end && str[i] != '\0'; i++) {
		if (!is_valid_character(str[i])) {
			printf("Null found in \'%s\' at index [%d].\n", str, i);
			return NULL;
		}
		int word_index = i-start; // Subtract position in word by offset 'start' in buffer string
		word[word_index] = str[i]; // Save character to word string
	}

	return word;
}


char** tokenize(char* str) {
	if (str == NULL) return NULL; // No string passed, end program
	int word_count = count_words(str);
	if (word_count < 1) return NULL; // Non valid parsing pattern (no words), end program

	char** words = (char**)malloc(sizeof(char*)*word_count+1);
	words[word_count] = NULL;
	int count = 0, i = 0; // 'count' keeps track of index of token, 'i' is the offset used to find starts of words
	while (count < word_count && i < string_length(str)) {
		int start = find_word_start(str, i);
		int end = find_word_end(str, start);
		words[count] = (char*)malloc(sizeof(char)*(end-start+2));
		words[count] = extractWord(str, start, end);
		count++;
		i = end+1;
	}

	return words;
}


void print_tokens(char** words) {
	if (words == NULL) return;
	for (int ind = 0; words[ind] != NULL; ind++)
		printf("[%d] => \'%s\'\n", ind, words[ind]);
	printf("\n");
}


void free_tokens(char** words) {
	if (words == NULL) return;
	for (int i = 0; words[i] != NULL; i++) {
		char* word = words[i];
		if (word == NULL) continue;
		for (int j = 0; word[i] != '\0'; j++)
			word[j] = '\0';
		free(words[i]);
	}
	words = NULL;
	free(words);
}


void prompt(char* buffer) {
	if (buffer == NULL) return;
	// printf("$ "); // Standard prompt char
	wprintf(L"%lc ", BOMBA); // Bomba prompt char
	fgets(buffer, BUFFER_SIZE, stdin);
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (buffer[i] == '\n')
			buffer[i] = '\0';
	}
}


/*
 * Used to parse buffer string to read history accessor string in the form of '!23' where the number is the index of the string in the history
 */
int stringToPosInt(char* str) {
	if (str == NULL) return -1; // No string attached, end program
	else if ( str[0] == '\0' || str[0] == '0') return -1; // Invalid number, end program
	
	#define numOffset 48 // Offset for zero (digit) character ascii value
	
	int strlen = string_length(str);
	int val = 0;
	for (int i = 0; i < strlen; i++) {
		int adjCharVal = (int)str[i] - numOffset;
		if (adjCharVal < 0 || adjCharVal > 9) return -1; // Invalid character (Nan) input, end program
		int exp = strlen-i-1;
		int digit_place = 1;
		for (int j = 0; j < exp; j++) // Necessary without <math.h> to perform pow
			digit_place *= 10;
		val += adjCharVal*digit_place;
	}
	return val;
}


int main() {
	setlocale(LC_CTYPE, ""); // Unicode setting used for printing bomb char
	List* history = init_history(); // Stores every string tokenized.

	while (1) { // Run until exit command (Ctrl-C)
		char buffer[BUFFER_SIZE]; // Holds input string inputted by the user
		print_history(history); // Print previous tokenized string history
		prompt(buffer); // Prompt and receive user input, store input in 'buffer'
		char* evalStr = buffer; // Wraps the buffer string into a pointer for the tokenizer to use
		
		int isInHistory = 0; // Stand-in boolean flag that determines whether to add the prompt to history if it doesn't exist.
		int word_count = count_words(buffer); // Counts number of words found
		if (word_count < 1) goto Error; // Invalid input string
		else if (word_count == 1 && buffer[0] == '!') { // History operator accessed
			char* ind_str = extractWord(buffer, 1, find_word_end(buffer, 1)); // Get history number string
			int ind_val = stringToPosInt(ind_str); // Convert string to number
			evalStr = get_history(history, ind_val); // Get history index at 'ind_val'
			if (evalStr == NULL) {
				printf("Not in History!\n\n");
				continue;
			}
			word_count = count_words(evalStr);
			isInHistory = 1; // Set flag to prevent storing of history string
		}
		
		/*	Error block  */
		if (evalStr == NULL || evalStr[0] == '\0') {
			Error: // Handles invalid input errors
				printf("Error - Invalid Input, Try Again.\n\n");
				continue;
		}

		/*	Buffer string info	*/
		printf("\'%s\'\n", evalStr);
		printf("\n%d Tokens Found\n", word_count);

		char** tokens = tokenize(evalStr); // Stores each token in that was parsed from the buffer.
		if (!isInHistory) add_history(history, buffer); // Add to history if not already present (flag is not set).
		print_tokens(tokens);
		free_tokens(tokens); // Recalim token memory
	}
}
