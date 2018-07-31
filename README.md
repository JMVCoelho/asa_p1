# ASA Project 1

School project for Analysis and Synthesis of Algorithms.

The objective was to identify the strongly connected components (SCC) in a graph as well as the edges that connect them.
More info regarding the project and its objectives can be found in the file p1-enunciado.pdf but it's in Portuguese.
There's also a report that was part of the project, al058-proj1ASA-relatorio.pdf, also in Portuguese.

## Getting Started

There's already a compiled version of the code (file a.out).
The source code is in the file finalV.c, including some useful comments. The code does not have exhaustive comments explaining every step.

## Running the tests

The folder publicTests includes the public tests (.in files) as well as their expected output (.out files).

To run the tests and confirm the results one should do the following:
Run the program a.out with a .in file as input and redirect the output to an empty file of your choice. Example:
$ ./a.out < publicTests/st01.in > results.txt
Compare the output of that file with the respective expected output file. Example:
$ diff results.txt publicTests/st01.out
This command should have no output.

## Authors

* **Rui Alves** - *Initial work* - [ruialves284](https://github.com/ruialves284)

* **João Coelho**
