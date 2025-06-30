#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "turing.h"

int count_lines(FILE *file)
{
	int count = 0;
	char buffer[MAX_LINE_LENGTH];
	while (fgets(buffer, sizeof(buffer), file)) {
		trim_whitespace(buffer);

		// Ignore comments, which are indicated by lines starting with
		// "#", and also empty lines
		if (startsWith(buffer, "#") || is_line_empty(buffer)) {
			continue;
		}

		count++;
	}
	rewind(file);

	return count;
}

bool parse_program(TuringInstruction *program, FILE *program_file)
{
	int i = 0;
	char line[MAX_LINE_LENGTH];

	while (fgets(line, sizeof(line), program_file)) {
		trim_whitespace(line);

		// Ignore comments, which are indicated by lines starting with
		// "#", and also empty lines
		if (startsWith(line, "#") || is_line_empty(line)) {
			continue;
		}

		int token_count = 0;
		char *tokens[MAX_TOKENS];
		char *token = strtok(line, " ");
		while (token != nullptr && token_count < MAX_TOKENS) {
			tokens[token_count++] = token;
			token = strtok(nullptr, " ");
		}

		if (token != nullptr || token_count < MAX_TOKENS) {
			return false;
		}

		TuringInstruction instruction;
		instruction.current_state = malloc(strlen(tokens[0]) + 1);
		instruction.next_state = malloc(strlen(tokens[4]) + 1);

		if (instruction.current_state == nullptr ||
		    instruction.next_state == nullptr) {
			free(instruction.current_state);
			free(instruction.next_state);
			return false;
		}

		strcpy(instruction.current_state, tokens[0]);
		strcpy(instruction.next_state, tokens[4]);

		instruction.symbol_read = tokens[1][0];
		instruction.symbol_write = tokens[2][0];

		if (strcmp(tokens[3], "L") == 0) {
			instruction.direction = L;
		} else if (strcmp(tokens[3], "R") == 0) {
			instruction.direction = R;
		} else {
			return false;
		}

		program[i++] = instruction;
	}

	return true;
}

void free_program(TuringInstruction *program, int count)
{
	for (int i = 0; i < count; i++) {
		free(program[i].current_state);
		free(program[i].next_state);
	}
}

char *parse_tape(FILE *tape_file)
{
	fseek(tape_file, 0, SEEK_END);
	const long filesize = ftell(tape_file);
	rewind(tape_file);

	char *buffer = malloc(filesize + 1);
	if (!buffer) {
		perror("ERROR: Memory allocation for tape failed\n");
		return nullptr;
	}

	const size_t read_size = fread(buffer, 1, filesize, tape_file);
	buffer[read_size] = '\0';

	remove_whitespace(buffer);

	return buffer;
}

bool next_instruction(TuringMachine *machine, TuringInstruction *program,
		      int count, bool debug, int step_count)
{
	for (int i = 0; i < count; i++) {
		if ((strcmp(machine->state, program[i].current_state) == 0) &&
		    (machine->symbols[machine->head] ==
		     program[i].symbol_read)) {
			machine->symbols[machine->head] =
				program[i].symbol_write;
			if (program[i].direction == L) {
				if (machine->head == 0) {
					machine->head =
						strlen(machine->symbols) - 1;
				} else {
					machine->head = --machine->head;
				}
			} else if (program[i].direction == R) {
				machine->head = (++machine->head) %
						strlen(machine->symbols);
			}
			free(machine->state);
			machine->state = strdup(program[i].next_state);

			if (debug) {
				printf("STEP %d INFO:\n", step_count);
				printf("   MACHINE STATE: %s\n",
				       machine->state);
				printf("   MACHINE HEAD POSITION: %d\n",
				       machine->head);
				printf("   TAPE: %s\n\n", machine->symbols);
			}

			return true;
		}
	}

	return false;
}

bool startsWith(const char *str, const char *prefix)
{
	while (*prefix && *str == *prefix)
		++str, ++prefix;

	return *prefix == 0;
}

bool is_line_empty(const char *line)
{
	for (int i = 0; line[i] != '\0'; i++) {
		if (!isspace((unsigned char)line[i])) {
			return false;
		}
	}

	return true;
}

void trim_whitespace(char *str)
{
	char *start = str;

	while (isspace((unsigned char)*start)) {
		start++;
	}

	if (*start == '\0') {
		str[0] = '\0';
		return;
	}

	char *end = start + strlen(start) - 1;
	while (end > start && isspace((unsigned char)*end)) {
		end--;
	}

	*(end + 1) = '\0';

	if (start != str) {
		memmove(str, start, end - start + 2);
	}
}

void remove_whitespace(char *str)
{
	char *src = str, *dst = str;
	while (*src) {
		if (!isspace((unsigned char)*src)) {
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';
}

char *to_lowercase(char *str)
{
	char *buffer = malloc(strlen(str) + 1);
	if (buffer == nullptr) {
		return nullptr;
	}
	strcpy(buffer, str);
	for (int i = 0; str[i] != '\0'; i++) {
		buffer[i] = tolower((unsigned char)str[i]);
	}

	return buffer;
}
