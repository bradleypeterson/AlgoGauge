#include <iostream>
#include "../dependencies/subprocess.h"
#include "../dependencies/PerfEvent.hpp"
#include <cstdio>
#include <cstdlib>

int runPIDSomething(){
	std::cout << "Another option" << std::endl << std::endl;
	pid_t pid = fork(); // Create a new process

    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    } else if (pid == 0) {
        // In the child process
 	execlp(
            "node",
            "node",  // argv[0] should be the name of the executable
            "../MultiLanguage/Javascript/AlgoGauge.mjs",
            "--algorithm", "bubble",
            "--length", "100",
            "--strategy", "random",
            "-v",
            "-j",
            nullptr
        ); 
        // If execlp returns, there was an error
        std::cerr << "Exec failed!" << std::endl;
        return 1;
    } else {
        // In the parent process
		
        std::cout << "Child process PID: " << pid << std::endl;

        // Optionally, wait for the child to finish
        int status;
		PerfEvent e(pid);
		e.startCounters();

        waitpid(pid, &status, 0);
		e.stopCounters();
		
        if (WIFEXITED(status)) {
            std::cout << "Child exited with status: " << WEXITSTATUS(status) << std::endl;
        }
		e.printReportVertical(std::cout, 1);
    }

    return 0;
}



int main(int argc, char *argv[])
{

	const char *command_line[] = {"node", 
	"../MultiLanguage/Javascript/AlgoGauge.mjs", 
	"--algorithm=bubble",
	"--length=100",
	"--strategy=random",
	"-v",
	"-j",
	NULL};
	//perf stat node AlgoGauge.mjs -a bubble --length=100 --strategy=random -j -v
	struct subprocess_s process;
	std::cout << process.child << std::endl;
	int result = subprocess_create(command_line, subprocess_option_search_user_path, &process);
	if (0 != result) {
		std::cout << "ERRROR!";
	}

	std::cout << "Hello" << std::endl;
	int process_return;
	std::cout << process.child << std::endl;
	PerfEvent e(process.child);
	e.startCounters();
	result = subprocess_join(&process, &process_return);
	if (0 != result) {
	// an error occurred!
			std::cout << "ERRROR2";
	}
	e.stopCounters();

	e.printReportVertical(std::cout, 1);

	std::string stdOUT;
	char buffer[1024];
	 while (true) {
        unsigned bytes_read = subprocess_read_stdout(&process, buffer, sizeof(buffer));
        if (bytes_read == 0) {
            break;  // Exit loop when no more bytes are read
        }
		stdOUT.append(buffer, bytes_read);
    }

	std::cout << stdOUT;
	std::cout << "New Test:" << std::endl << std::endl;
	int result2 = std::system("perf stat node ../MultiLanguage/Javascript/AlgoGauge.mjs -a bubble --length=100 --strategy=random -j -v"); // Replace "ls -l" with your command
    if (result2 != 0) {
        std::cerr << "Command failed with error code: " << result2 << std::endl;
    }else{
		std::cout << "something";
	}

	
    return runPIDSomething();
}	