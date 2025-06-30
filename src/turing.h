#ifndef TURING_H
#define TURING_H

#define MAX_LINE_LENGTH 256
#define MAX_INPUT_LEN 256
#define MAX_TOKENS 5

// This enum defines the directions the head of the machine can move in, which
// are left and right
typedef enum { L, R } Direction;

// An instruction for the machine, consisting of 5 tokens, indicating current
// state, symbol to read, symbol to write, which direction to move the head in,
// and the next state the machine will be in after the operation
typedef struct {
	char *current_state;
	char symbol_read;
	char symbol_write;
	Direction direction;
	char *next_state;
} TuringInstruction;

// The turing machine, consisting of an array of symbols, our "tape", a head
// with which to read/write a symbol on the current "cell" of the "tape", and
// its current state
typedef struct {
	char *symbols;
	unsigned long head;
	char *state;
} TuringMachine;

int count_lines(FILE *file);

// Parses the specified program file and stores its instructions in an array of
// TuringInstructions
bool parse_program(TuringInstruction *program, FILE *program_file);

// Frees the memory we allocated for our "program" from memory
void free_program(TuringInstruction *program, int count);

// Parses the specified tape file and stores its symbols in a string
char *parse_tape(FILE *tape_file);

// We use this function, which receives an array of instructions, our "program",
// to go from one instruction to the next, performing the machine head movements
// and read/writes as required
bool next_instruction(TuringMachine *machine, TuringInstruction *program,
		      int count, bool debug, int step_count);

bool startsWith(const char *str, const char *prefix);

bool is_line_empty(const char *line);

void trim_whitespace(char *str);

void remove_whitespace(char *str);

char *to_lowercase(char *str);

#endif //TURING_H
