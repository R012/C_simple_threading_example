# Basic threading example in C
This repository contains a basic example of threading in C. It is designed for learning purposes specifically, with an UNIX environment in mind.

## Contents
The respository is comprised of the following files:

- `simple_threads.c`: Source code of the example. It has been documented extensively to facilitate understanding. Note that this amount of commenting is not standard in coding projects, and can in fact be detrimental in professional environments where highly skilled programmers interact by hindering the ability to directly read the code.
- `Makefile`: standard Makefile, with `all`, `debug` and `clean` rules. See `Compiling` for more information.

## Compiling
Reliance on the `make` command is advised. The following rules are included:

- `all`/`simple_threads`: Standard default compilation rule. The program is built without debugging symbols. Simply calling `make` will invoke this rule.
- `debug`: Compilation with debug symbols. Calling `make debug` will invoke this rule.
- `clean`: Standard phony clean-up rule. Simply removes the generated executable.

Note that `all` and `debug` use the same rule as target ouput. Feel free to change the name of the target if you wish to keep both executables handy for any reason. You might need to update clean accordingly if you choose to do so.

# Running
Once compiled, simply run `./simple_threads` to get the output. This output is wholly uninteresting on its own, but should provide brief demonstration of the programs functionality to facilitate understanding the attached code.
