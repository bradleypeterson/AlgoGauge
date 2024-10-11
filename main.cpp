/**
 * @brief Starting file of the program. Does nothing except call the starter method
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#include "CLI_Parsing/CLI_Parser.hpp"
#include "CLI_Parsing/sorting_algorithm_caller.cpp"

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


    auto x = AlgorithmSettings{};
    x.empty = false;
    x.Language = "js";
    x.ArrayStrategy = "random";
    x.Algorithm = "default";

    // CreateChildProcess(x);
    runProgram(argc, argv);
    return 0;  // returning 0 regardless of error
    // as for some reason, node treats a non-0 exit code as a server
    // error instead of capturing data from STDERR.
}
// #include <unistd.h>
// #include <stdio.h>

// int main() {
//     const char *command[] = {"/bin/echo", "Hello, world!", NULL};
//     if (fork() == 0) { // child process
//         execvp(command[0], (char *const *)command);
//         perror("execvp failed");
//         return 1;
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <spawn.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/wait.h>

// extern char **environ;

// int main() {
//     // posix_spawn_file_actions_t actions;
//     // posix_spawn_file_actions_init(&actions);  // initialize actions

//     // pid_t child;
//     // const char *commandLine[] = {"/bin/echo", "Hello, world!", NULL};
//     // char *used_environment[] = {NULL};  // or environ if you want to inherit the environment

//     // if (0 != posix_spawn(&child, commandLine[0], &actions, NULL,
//     //                      (char *const *)commandLine, used_environment)) {
//     //     posix_spawn_file_actions_destroy(&actions);
//     //     perror("posix_spawn failed");
//     //     return -1;
//     // }

//     // posix_spawn_file_actions_destroy(&actions);  // destroy actions after use

//     // int status;
//     // waitpid(child, &status, 0);
//     // printf("Child exited with status %d\n", status);
//     const char *command_line[] = {"/bin/echo", "Hello, world!", NULL};
//     struct subprocess_s subprocess;
//     int result = subprocess_create(command_line, 0, &subprocess);
//     if (0 != result) {
//         perror("Failed to create subprocess");
//         return result;
//     }
//     return 0;
// }
