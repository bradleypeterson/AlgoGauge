#ifndef ALGOGAUGE_SORTING_ALGORITHM_CALLER_HPP
#define ALGOGAUGE_SORTING_ALGORITHM_CALLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "../dependencies/subprocess.h"
#include "../algorithms/sort_7algs.cpp"
#include "CLI_Parser.hpp"
#include "../AlgoGaugeDetails.hpp"

/**
 * Method for determining which Sorting Algorithm to return
 * @param algorithmName The name of the algorithm to return (must match exactly from a set of names)
 * @param length The cardinality of the initial set to produce for the algorithm to sort
 * @param canonicalName A human-readable name to help keep track of the algorithm
 * @param verbose Whether or not all output should be included in STD::0UT
 * @param includeValues Whether or not the initial set and final set should be included in the output
 * @param includePerf Whether or not this should include Perf metrics within the output
 * @return A sorting algorithm object
 */
Sorting::BaseSort<unsigned int>* getAlgorithm(
    string algorithmName, // Opting for this to be a string and NOT an enum as we can just pass what the user passes as the --algo arg directly and return an error if it doesn't match any algorithms
    const unsigned int& length,
    const string& canonicalName = "",
    const bool& verbose = false,
    const bool& includeValues = false,
    const string& includePerf = "false"
) {
    std::transform(algorithmName.begin(), algorithmName.end(), algorithmName.begin(), ::tolower); // make input lowercase

    //Essentially this is a switch case block that determines which algorithm to create and return
    if (algorithmName == "bubble") return new Sorting::Bubble<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "selection") return new Sorting::Selection<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "insertion") return new Sorting::Insertion<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "quick") return new Sorting::Quick<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "merge") return new Sorting::Merge<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "heap") return new Sorting::Heap<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    //raise an error if passed algorithmName doesn't match any already existing classes
    else throw std::invalid_argument("Algorithm name '" + algorithmName + "' is not listed as a valid algorithm!");
}

std::string getJsonOutputs(const std::string file){
	std::ifstream ifs(file);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()) );
	return content;
}

void printChildProcessSTDOUT(struct subprocess_s &process){
	// unsigned bytes_read;

	// do {
	// 	char buffer[1024] = {0};
	// 	bytes_read = subprocess_read_stdout(&process, buffer, sizeof(buffer));
	// 	printf("Read %u bytes - '%s'\n", bytes_read, buffer);
	//  } while (bytes_read != 0);

	// return;
}


std::string runChildProcess(const char* commandLineArguments[], const char* environment[], const std::string jsonReadFilePath){
 	struct subprocess_s process;
	int exit_code;

    int result = subprocess_create_ex(commandLineArguments, subprocess_option_search_user_path | subprocess_option_combined_stdout_stderr, environment, &process);


	// cout << process.child;
    if (result != 0) {
        std::cerr << "Failed to start program!" << std::endl;
        return "";
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

    return getJsonOutputs(jsonReadFilePath);
}


void processAlgorithms(const AlgoGauge::AlgoGaugeDetails& algorithmsController){
	std::string includePerf = std::to_string(algorithmsController.Perf);
	if(algorithmsController.PerfSample){
		includePerf = "sample";
	}
	
	std::string jsonResults = "{\"algorithms\": ["; //create the json results object even if not specified

	for(auto algo: algorithmsController.SelectedSortingAlgorithms){

		std::transform(algo.Language.begin(), algo.Language.end(), algo.Language.begin(), ::tolower);

		if(algo.Language == "c++"){
			auto newAlgorithm = getAlgorithm(
					algo.Algorithm,
					algo.ArrayCount,
					algo.Name,
					algorithmsController.Verbose,
					algorithmsController.Output,
					includePerf
					);

			switch (algo.ArrayStrategy) {
				case AlgoGauge::AlgorithmOptions::repeatedSet:
					newAlgorithm->loadRepeatedValues();
					break;
				case AlgoGauge::AlgorithmOptions::chunkSet:
					newAlgorithm->loadChunkValues();
					break;
				case AlgoGauge::AlgorithmOptions::reversedSet:
					newAlgorithm->loadReversedValues();
					break;
				case AlgoGauge::AlgorithmOptions::orderedSet:
					newAlgorithm->loadOrderedValues();
					break;
				case AlgoGauge::AlgorithmOptions::randomSet:
					newAlgorithm->loadRandomValues();
					break;
				default:
					throw std::invalid_argument("Need to provide a valid Algorithm Option!");
			}

			jsonResults+= newAlgorithm->runAndGetJSONSort() + ",";

			if (algorithmsController.Verbose) cout << newAlgorithm->getStringResult() << endl;

			continue;
		}

		const std::string sorting_algorithm = std::format("--algorithm={}", algo.Algorithm);

		const std::string array_strategy = "--strategy=" + algo.ArrayStrategyString;
		const std::string array_count = std::format("--count={}", algo.ArrayCount);
		const std::string output = std::format("--output={}", algorithmsController.Output);
		const std::string verbose = std::format("--verbose={}", algorithmsController.Verbose);
		const std::string include_json = std::format("--json={}", algorithmsController.Json);
		const std::string read_json_path = std::format("../MultiLanguage/temp/{}.txt", algo.Language);
		const std::string temp_path = std::format("--file=../temp/{}.txt",algo.Language);


		const char *environment[] = {NULL};

		if (algo.Language == "node" || algo.Language == "js" || algo.Language == "javascript"){
			//node Algogauge.mjs -vTrue -c10 -aBubble -aMerge -c10 -sOrdered -sreversed -j -oTrue -c20 -aDefault -sordered --file="../temp/javascript.txt"
			const char* program_arguments[] = {"node", "../MultiLanguage/Javascript/Algogauge.mjs", sorting_algorithm.c_str(), array_strategy.c_str(), array_count.c_str(), output.c_str(), verbose.c_str(), include_json.c_str(), nullptr};
			jsonResults += runChildProcess(program_arguments, environment, read_json_path);
			continue;

		}

		if (algo.Language == "python" || algo.Language == "python3"){
			const char* program_arguments[] = {"python3", "../dependencies/Algogauge.mjs", sorting_algorithm.c_str(), nullptr};

			jsonResults += runChildProcess(program_arguments, environment, read_json_path);

			continue;
		}

			std::string program_path;

		if(algo.Language == "rust"){
			program_path = "../MultiLanguage/testing/rust_binary";
		}


		const char* program_arguments[] = {program_path.c_str(), sorting_algorithm.c_str(), nullptr};
		std::cout << program_path;
		jsonResults +=  runChildProcess(program_arguments, environment, read_json_path);

	}
	
	jsonResults.pop_back(); //remove extraneous comma
	jsonResults += "]}"; //finish json string
	if (algorithmsController.Json) cout << jsonResults << endl;

	if (!algorithmsController.FileWritePath.empty()) { //print output to file
            std::ofstream outFile;
            outFile.open(algorithmsController.FileWritePath);
            outFile << jsonResults;
            outFile.close();
	}
	if (algorithmsController.Verbose && !algorithmsController.FileWritePath.empty()) cout << "Results written in JSON at: '" << algorithmsController.FileWritePath << "'" << endl;

    cout << endl;
}


#endif //ALGOGAUGE_SAC_CPP

// int main(){
// 	// cout << "hello";
//     AlgoGaugeDetails algoObjects;
//     algoObjects.Verbose = true;
// 	algoObjects.Json = true;
	
//     SortingAlgorithmSettings x;
//     x.Algorithm = "bubble";
//     x.ArrayCount = 1000;
//     x.ArrayStrategy = randomSet;
//     x.Name = "John";

//     SortingAlgorithmSettings y;
//     y.Algorithm = "merge";
//     y.ArrayCount = 1000;
//     y.ArrayStrategy = repeatedSet;
//     y.Name = "carlos";

//     algoObjects.SelectedSortingAlgorithms.push_back(x);

//     algoObjects.SelectedSortingAlgorithms.push_back(y);
// 	processAlgorithms(algoObjects);
//     // baseSort(algoObjects);


//     return 0;
// }

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