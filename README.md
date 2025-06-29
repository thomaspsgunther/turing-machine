# Turing Machine in C (Máquina de Turing em C)

## English

Simple deterministic Turing Machine interpreter or simulator written in C. \
Inspiration, examples and parts of this README taken from
Tsoding's [Turing Machine Interpreter in D](https://github.com/tsoding/turd).

### Quick Start

```console
$ mkdir build
$ cd build
$ cmake -G Ninja ..
$ ninja
$ ./turing_machine ../examples/turing-programs/add.turing ../examples/tapes/input-05.tape
```

To view options:

```console
$ ./turing_machine --help
```

### Turing File Format

Turing files (examples are located in the [./examples/turing-programs/](./examples/turing-programs/) folder) are the
files that contain instructions for the Turing Machine to interpret:

- Each instruction is located on a separate line.
- Correct instruction has the format: `<CURRENT-STATE> <READ-SYMBOL> <WRITE-SYMBOL> <DIRECTION> <NEXT-STATE>`:
    - `<CURRENT-STATE>` is a sequence of non-space characters that represents the state in which this instruction is
      activated.
    - `<READ-SYMBOL>` is a non-space character that represents the symbol that is read by the head of the
      machine which along with a specific `<CURRENT-STATE>` activates the instruction.
    - `<WRITE-SYMBOL>` is a non-space character that represents the symbol that is written to the current
      cell on the tape when the instruction is activated.
    - `<DIRECTION>` is either symbol `L` or `R` which indicates the direction in which the head of the Turing Machine
      should step after executing the instruction.
    - `<NEXT-STATE>` is a sequence of non-space characters that represents the state to which the Machine should switch
      after executing the instruction.
- Any line may have any amount of leading or trailing whitespaces. All of them are stripped off before processing any
  instructions.
- Any empty line after stripping whitespace is ignored.
- Any line that starts with `#` after stripping whitespaces is ignored.

### Tape File Format

Tape files (examples are located in the [./examples/tapes/](./examples/tapes/) folder) are the files that contain
initial state of the Turing Machine tape.

- A tape file consists of sequence of symbols separated by any amount of whitespace characters.
- Each symbol is a sequence of non-space characters that represents the symbol stored in the corresponding cell of the
  Machine's tape.

### Execution Process

Execution process starts with

- loading the provided tape file into the tape of the Virtual Turing Machine,
- setting the head position to 0 (can be changed with flag `-p`),
- switch to the state `BEGIN` (can be changed with flag `-s`).

Then on each iteration of execution the machine finds the first instruction with the matching `<CURRENT-STATE>` and
`<READ-SYMBOL>` and executes that instruction. If the machine cannot find an instruction with the matching
`<CURRENT-STATE>` and `<READ-SYMBOL>` it halts.

---

## Português

Interpretador ou simulador simples de Máquina de Turing determinística escrito em C.
Inspiração, exemplos e partes deste README foram retirados do
[Interpretador de Máquina de Turing em D](https://github.com/tsoding/turd) do Tsoding.

### Início Rápido

```console
$ mkdir build
$ cd build
$ cmake -G Ninja ..
$ ninja
$ ./turing_machine ../examples/turing-programs/add.turing ../examples/tapes/input-05.tape
```

Para visualizar opções:

```console
$ ./turing_machine --help
```

### Formato de Arquivo Turing

Arquivos Turing (exemplos estão localizados na pasta [./examples/turing-programs/](./examples/turing-programs/)) são os
arquivos que contêm instruções para a Máquina de Turing interpretar:

* Cada instrução está localizada em uma linha separada.
* Instruções corretas têm o formato: `<ESTADO-ATUAL> <SÍMBOLO-LIDO> <SÍMBOLO-ESCRITO> <DIREÇÃO> <PRÓXIMO-ESTADO>`:

    * `<ESTADO-ATUAL>` é uma sequência de caracteres sem espaço que representa o estado no qual esta instrução é
      ativada.
    * `<SÍMBOLO-LIDO>` é um caractere diferente de espaço que representa o símbolo lido pela cabeça da
      máquina que, com um `<ESTADO-ATUAL>` específico, ativa a instrução.
    * `<SÍMBOLO-ESCRITO>` é um caractere diferente de espaço que representa o símbolo escrito na célula
      atual da fita quando a instrução é ativada.
    * `<DIREÇÃO>` é ou o símbolo `L` ou `R`, o qual indica a direção na qual a cabeça da Máquina de Turing
      deve se mover após executar a instrução.
    * `<PRÓXIMO-ESTADO>` é uma sequência de caracteres sem espaço que representa o estado para o qual a Máquina
      deve mudar após executar a instrução.
* Qualquer linha pode ter qualquer quantidade de espaços em branco no início ou no fim. Todos eles são removidos
  antes de processar qualquer instrução.
* Qualquer linha vazia após a remoção dos espaços em branco é ignorada.
* Qualquer linha que começa com `#` após a remoção dos espaços em branco é ignorada.

### Formato de Arquivo de Fita

Arquivos de fita (exemplos estão localizados na pasta [./examples/tapes/](./examples/tapes/)) são os arquivos que contêm
o estado inicial da fita da Máquina de Turing.

* Um arquivo de fita consiste em uma sequência de símbolos separados por qualquer quantidade de caracteres de espaço em
  branco.
* Cada símbolo é uma sequência de caracteres sem espaço que representa o símbolo armazenado na célula correspondente
  da fita da Máquina.

### Processo de Execução

O processo de execução começa com

* carregando o arquivo de fita fornecido na fita da Máquina de Turing Virtual,
* definindo a posição da cabeça para 0 (pode ser alterado com a flag `-p`),
* mudando para o estado `BEGIN` (pode ser alterado com a flag `-s`).

Então, a cada iteração da execução, a máquina encontra a primeira instrução com `<ESTADO-ATUAL>` e
`<SÍMBOLO-LIDO>` correspondentes e executa essa instrução. Se a máquina não puder encontrar uma instrução com
`<ESTADO-ATUAL>` e `<SÍMBOLO-LIDO>` correspondentes, ela para.
