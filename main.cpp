/**
 * @brief Starting file of the program. Does nothing except call the starter method
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#include "CLI_Parsing/CLI_Parser.hpp"

int main(int argc, char *argv[]) {
    /*
     * Getting Started:
     * Recommended CMake commands:
     *      cd [source code root directory]
     *      mkdir cmake-build-debug
     *      cd cmake-build-debug
     *      cmake ..
     *      cd ..
     *      cmake --build .
     *      ./AlgoGauge -h [to see how program is run or use example below]
     * CLI command examples:
     *      ./AlgoGauge --algo quick --len 100000 -r --algo bubble --len 100000 -r
     *      ./AlgoGauge --algo quick --len 10000 --algo bubble --len 10000 --algo selection --len 10000 --algo insertion --len 10000 --algo merge --len 10000 --algo heap --len 10000 --algo quick --len 10000 --algo bubble --len 10000 --algo selection --len 10000 --algo insertion --len 10000 --algo merge --len 10000 --algo heap --len 10000 --algo quick --len 10000 --algo bubble --len 10000 --algo selection --len 10000 --algo insertion --len 10000 --algo merge --len 10000 --algo heap --len 10000 --algo quick --len 10000 --algo bubble --len 10000 --algo selection --len 10000 --algo insertion --len 10000 --algo merge --len 10000 --algo heap --len 10000 --algo quick --len 10000 --algo bubble --len 10000 --algo selection --len 10000 --algo insertion --len 10000 --algo merge --len 10000 --algo heap --len 10000 -rrrrrrssssssooooooeeeeeecccccc
     *      ./AlgoGauge --algo quick --len 1000 --algo bubble --len 1000 --algo selection --len 1000 --algo insertion --len 1000 --algo merge --len 1000 --algo heap --len 1000 -rrrrrr
     */
    runProgram(argc, argv);
    return 0;  // returning 0 regardless of error
    // as for some reason, node treats a non-0 exit code as a server
    // error instead of capturing data from STDERR.
}