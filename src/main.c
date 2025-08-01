#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "turing.h"

int main(int argc, char **argv)
{
	bool interactive = false;
	bool debug = false;
	unsigned long step_count = 1;
	char *initial_state = strdup("BEGIN");
	unsigned long initial_head = 0;
	char *program_input = nullptr;
	char *tape_input = nullptr;

	for (int i = 1; i < argc; i++) {
		if (starts_with(argv[i], "-")) {
			if (strcmp(argv[i], "-h") == 0 ||
			    strcmp(argv[i], "--help") == 0) {
				printf("Usage: turing_machine [OPTIONS] <input.turing> <input.tape>\n");
				printf("OPTIONS:\n");
				printf("  --help|-h             print this help to stdout and exit with 0 exit code\n");
				printf("  --state|-s=[STATE]    start from a specific initial state (default: BEGIN)\n");
				printf("  --head|-p=[POSITION]  start from a specific head position (default: 0)\n");
				printf("  --interactive|-i      execute the program interactively\n");
				printf("  --debug|-d            print extra info during program execution\n");
				free(initial_state);
				exit(EXIT_SUCCESS);
			}
			if (strcmp(argv[i], "-i") == 0 ||
			    strcmp(argv[i], "--interactive") == 0) {
				interactive = true;
			} else if (starts_with(argv[i], "-s") ||
				   starts_with(argv[i], "--state")) {
				char *token = strtok(argv[i], "=");
				while (token != nullptr) {
					if (!starts_with(token, "-")) {
						free(initial_state);
						initial_state = strdup(token);
					}
					token = strtok(nullptr, "=");
				}
			} else if (starts_with(argv[i], "-p") ||
				   starts_with(argv[i], "--head")) {
				char *token = strtok(argv[i], "=");
				while (token != nullptr) {
					if (!starts_with(token, "-")) {
						char *endptr;
						errno = 0;

						initial_head = strtol(
							token, &endptr, 10);

						if (errno == ERANGE) {
							perror("ERROR: Overflow or underflow occurred with given initial head position\n");
							free(initial_state);
							exit(EXIT_FAILURE);
						}
						if (*endptr != '\0') {
							fprintf(stderr,
								"ERROR: Invalid character after initial head position number: %s\n",
								endptr);
							free(initial_state);
							exit(EXIT_FAILURE);
						}
					}
					token = strtok(nullptr, "=");
				}
			} else if (starts_with(argv[i], "-d") ||
				   starts_with(argv[i], "--debug")) {
				debug = true;
			} else {
				fprintf(stderr,
					"ERROR: Unknown argument provided\n");
				free(initial_state);
				exit(EXIT_FAILURE);
			}
		} else {
			if (program_input == nullptr) {
				program_input = argv[i];
			} else if (tape_input == nullptr) {
				tape_input = argv[i];
			}
		}
	}

	if (program_input == nullptr) {
		fprintf(stderr, "ERROR: Please provide a program file\n");
		free(initial_state);
		exit(EXIT_FAILURE);
	}
	if (tape_input == nullptr && !interactive) {
		fprintf(stderr, "ERROR: Please provide a tape file\n");
		free(initial_state);
		exit(EXIT_FAILURE);
	}

	FILE *program_file = fopen(program_input, "r");
	FILE *tape_file = fopen(tape_input, "r");

	if (program_file == nullptr && tape_file == nullptr && !interactive) {
		fprintf(stderr, "ERROR: Please provide a valid program file\n");
		fprintf(stderr, "ERROR: Please provide a valid tape file\n");
		free(initial_state);
		exit(EXIT_FAILURE);
	}
	if (program_file == nullptr) {
		fprintf(stderr, "ERROR: Please provide a valid program file\n");
		free(initial_state);
		if (tape_file != nullptr) {
			fclose(tape_file);
		}
		exit(EXIT_FAILURE);
	}
	if (tape_file == nullptr && !interactive) {
		fprintf(stderr, "ERROR: Please provide a valid tape file\n");
		free(initial_state);
		fclose(program_file);
		exit(EXIT_FAILURE);
	}

	const unsigned long count = count_lines(program_file);
	TuringInstruction program[count];
	if (!parse_program(program, program_file)) {
		fprintf(stderr,
			"ERROR: Could not parse program, make sure it is in expected format\n");
		free(initial_state);
		fclose(program_file);
		fclose(tape_file);
		exit(EXIT_FAILURE);
	};

	char *symbols = nullptr;
	if (tape_file != nullptr) {
		symbols = parse_tape(tape_file);
		if (symbols == nullptr) {
			free(initial_state);
			free_program(program, count);
			free(symbols);
			fclose(program_file);
			fclose(tape_file);
			exit(EXIT_FAILURE);
		}

		printf("TURING MACHINE\n\n");
	} else {
		printf("TURING MACHINE\n\n");
		printf("Type an input for the machine or type EXIT to exit the program:\n");

		symbols = malloc(MAX_INPUT_LEN);
		if (symbols == nullptr) {
			perror("ERROR: Memory allocation for input failed.\n");
			free(initial_state);
			free_program(program, count);
			free(symbols);
			fclose(program_file);
			exit(EXIT_FAILURE);
		}

		if (fgets(symbols, MAX_INPUT_LEN, stdin) == nullptr) {
			fprintf(stderr, "ERROR: Failed to read input.\n");
			free(initial_state);
			free_program(program, count);
			free(symbols);
			fclose(program_file);
			exit(EXIT_FAILURE);
		}

		remove_whitespace(symbols);

		char *lowercase_symbols = to_lowercase(symbols);
		if (lowercase_symbols != nullptr) {
			if (strcmp(lowercase_symbols, "exit") == 0) {
				free(initial_state);
				free_program(program, count);
				free(symbols);
				free(lowercase_symbols);
				fclose(program_file);
				exit(EXIT_SUCCESS);
			}
		}
		free(lowercase_symbols);
	}

	fclose(program_file);
	if (tape_file != nullptr) {
		fclose(tape_file);
	}

	TuringMachine machine = { .symbols = strdup(symbols),
				  .head = initial_head,
				  .state = strdup(initial_state) };

	free(symbols);

	printf("ORIGINAL TAPE:\n%s\n\n", machine.symbols);

	while (next_instruction(&machine, program, count, debug, step_count)) {
		++step_count;
	}

	printf("POST COMPUTATION TAPE:\n%s\n\n", machine.symbols);

	if (interactive) {
		while (1) {
			printf("Type an input for the machine or type EXIT to exit the program:\n");

			symbols = malloc(MAX_INPUT_LEN);
			if (symbols == nullptr) {
				perror("ERROR: Memory allocation for input failed.\n");
				free(initial_state);
				free_program(program, count);
				free(machine.symbols);
				free(machine.state);
				free(symbols);
				exit(EXIT_FAILURE);
			}

			if (fgets(symbols, MAX_INPUT_LEN, stdin) == nullptr) {
				fprintf(stderr,
					"ERROR: Failed to read input.\n");
				free(initial_state);
				free_program(program, count);
				free(machine.symbols);
				free(machine.state);
				free(symbols);
				exit(EXIT_FAILURE);
			}

			remove_whitespace(symbols);

			char *lowercase_symbols = to_lowercase(symbols);
			if (lowercase_symbols != nullptr) {
				if (strcmp(lowercase_symbols, "exit") == 0) {
					free(initial_state);
					free_program(program, count);
					free(machine.symbols);
					free(machine.state);
					free(symbols);
					free(lowercase_symbols);
					exit(EXIT_SUCCESS);
				}
			}
			free(lowercase_symbols);

			free(machine.symbols);
			free(machine.state);

			machine.symbols = strdup(symbols);
			free(symbols);
			machine.head = initial_head;
			machine.state = strdup(initial_state);

			printf("ORIGINAL TAPE:\n%s\n\n", machine.symbols);

			step_count = 1;
			while (next_instruction(&machine, program, count, debug,
						step_count)) {
				++step_count;
			}

			printf("POST COMPUTATION TAPE:\n%s\n\n",
			       machine.symbols);
		}
	}

	free(initial_state);
	free_program(program, count);
	free(machine.symbols);
	free(machine.state);

	return 0;
}
