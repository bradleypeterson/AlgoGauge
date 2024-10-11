



#include <iostream>
#include "../dependencies/subprocess.h"
#include "CLI_Parser.hpp"

#ifndef ALGOGAUGE_SORTING_ALGORITHM_CALLER_CPP
#define ALGOGAUGE_SORTING_ALGORITHM_CALLER_CPP


void printChildProcessSTDOUT(struct subprocess_s &process){
	unsigned bytes_read;

	do {
		char buffer[1024] = {0};
		bytes_read = subprocess_read_stdout(&process, buffer, sizeof(buffer));
		printf("Read %u bytes - '%s'\n", bytes_read, buffer);
	 } while (bytes_read != 0);

	return;
}


int runChildProcess(const char* commandLineArguments[], const char* environment[]){
 	struct subprocess_s process;
	int exit_code;

    int result = subprocess_create_ex(commandLineArguments, subprocess_option_search_user_path | subprocess_option_combined_stdout_stderr, environment, &process);


	// cout << process.child;
    if (result != 0) {
        std::cerr << "Failed to start program!" << std::endl;
        return result;
    }

	subprocess_join(&process, &exit_code);

	printChildProcessSTDOUT(process);


    if (exit_code == 0) {
        std::cout << "Program executed successfully!" << std::endl;
    } else {
        std::cerr << "Program exited with code " << exit_code << std::endl;
    }



    // Clean up
	int cleanUpResult = subprocess_destroy(&process);
	if (cleanUpResult != 0) {
		std::cerr << "Process failed to get destroyed and still might control memory" << std::endl;
	}

    return 0;
}

int CreateChildProcess(const AlgorithmSettings Algo){
	if(Algo.empty){
		std::cout << "No algorithm setting passed in";
		return -1;
	}

	std::string sorting_algorithm = std::format("run {}", Algo.Algorithm);
	std::string array_strategy = std::format("--strategy={}", Algo.ArrayStrategy);
	std::string array_count = std::format("--count={}", Algo.ArrayCount);

	const char *environment[] = {NULL};

	if (Algo.Language == "node" || Algo.Language == "js" || Algo.Language == "javascript"){
		const char* program_arguments[] = {"node", "../MultiLanguage/Javascript/Algogauge.mjs", "run", Algo.Algorithm.c_str(), array_strategy.c_str(), array_count.c_str(), "--output=true", nullptr};
		
		runChildProcess(program_arguments, environment);
		return 0;

	}

	if (Algo.Language == "python" || Algo.Language == "python3"){
		const char* program_arguments[] = {"python3", "../dependencies/Algogauge.mjs", sorting_algorithm.c_str(), nullptr};

		runChildProcess(program_arguments, environment);

		return 0;
	}

	std::string program_path;

	if(Algo.Language == "rust"){
		program_path = "../MultiLanguage/testing/rust_binary";
	}else if (Algo.Language == "c++"){
		program_path = "../MultiLanguage/c++_binary";
	}
	

	const char* program_arguments[] = {program_path.c_str(), sorting_algorithm.c_str(), nullptr};
	std::cout << program_path;
	runChildProcess(program_arguments, environment);


	return 0;
}

#endif //ALGOGAUGE_SAC_CPP
