#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turing.h"

int count_lines(FILE *file)
{
	int count = 0;
	char buffer[MAX_LINE_LENGTH];
	while (fgets(buffer, sizeof(buffer), file)) {
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
		line[strcspn(line, "\n")] = 0;

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

		if (!instruction.current_state || !instruction.next_state) {
			free(instruction.current_state);
			free(instruction.next_state);
			return false;
		}

		strcpy(instruction.current_state, tokens[0]);
		strcpy(instruction.next_state, tokens[4]);

		instruction.symbol_read = tokens[1][0];
		instruction.symbol_write = tokens[2][0];
		instruction.direction = strcmp(tokens[3], "L") == 0 ? L : R;

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

bool next_instruction(TuringMachine *machine, TuringInstruction *program)
{
	return false;
}
