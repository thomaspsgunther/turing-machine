#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turing.h"

int main(int argc, char **argv)
{
	char *program_input = nullptr;
	char *tape_input = nullptr;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 ||
		    strcmp(argv[i], "--help") == 0) {
			printf("help");
			exit(EXIT_SUCCESS);
		}
		if (program_input == nullptr) {
			program_input = argv[i];
		} else if (tape_input == nullptr) {
			tape_input = argv[i];
		}
	}

	if (program_input == nullptr) {
		fprintf(stderr, "ERROR: Please provide a program file\n");
		exit(EXIT_FAILURE);
	}
	// if (tape_input == nullptr) {
	// 	fprintf(stderr, "ERROR: Please provide a tape file\n");
	// 	exit(EXIT_FAILURE);
	// }

	FILE *program_file = fopen(program_input, "r");
	FILE *tape_file = fopen(tape_input, "r");

	if (program_file == nullptr) {
		fprintf(stderr, "ERROR: Please provide a valid program file\n");
		exit(EXIT_FAILURE);
	}
	// if (tape_file == nullptr) {
	// 	fprintf(stderr, "ERROR: Please provide a valid tape file\n");
	// 	exit(EXIT_FAILURE);
	// }

	const int count = count_lines(program_file);
	TuringInstruction program[count];
	if (!parse_program(program, program_file)) {
		fprintf(stderr,
			"ERROR: Could not parse program, make sure it is in expected format\n");
		exit(EXIT_FAILURE);
	};

	printf("%s", program[0].next_state);

	free_program(program, count);

	fclose(program_file);
	fclose(tape_file);

	return 0;
}
