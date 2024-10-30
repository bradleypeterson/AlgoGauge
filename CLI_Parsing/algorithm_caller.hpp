#ifndef ALGOGAUGE_SORTING_ALGORITHM_CALLER_HPP
#define ALGOGAUGE_SORTING_ALGORITHM_CALLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>  // For std::unique_ptr
#include <assert.h>
#include <algorithm>
#include <cctype>

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
std::string runCPlusPlusProgram(
    string algorithmName, // Opting for this to be a string and NOT an enum as we can just pass what the user passes as the --algo arg directly and return an error if it doesn't match any algorithms
    const unsigned int& length,
	const AlgoGauge::AlgorithmOptions& ArrayStrategy,

    const string& canonicalName = "",
    const bool& verbose = false,
    const bool& includeValues = false,
    const string& includePerf = "false"

) {
	std::unique_ptr<Sorting::BaseSort<unsigned int>> SortingAlgorithm;
	std::cout << includePerf << "perf";
    //Essentially this is a switch case block that determines which algorithm to create and return
	if (algorithmName == "bubble" || algorithmName == "default") SortingAlgorithm = std::make_unique<Sorting::Bubble<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "selection") SortingAlgorithm = std::make_unique<Sorting::Selection<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "insertion") SortingAlgorithm = std::make_unique<Sorting::Insertion<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "quick") SortingAlgorithm = std::make_unique<Sorting::Quick<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "merge") SortingAlgorithm = std::make_unique<Sorting::Merge<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "heap") SortingAlgorithm = std::make_unique<Sorting::Heap<unsigned int>>(length, canonicalName, verbose, includeValues, includePerf);
    //raise an error if passed algorithmName doesn't match any already existing classes
    else throw std::invalid_argument("Algorithm name \"" + algorithmName + "\" is not listed as a valid algorithm!");
    // if (algorithmName == "bubble" || algorithmName == "default") SortingAlgorithm = new Sorting::Bubble<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // else if (algorithmName == "selection") SortingAlgorithm = new Sorting::Selection<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // else if (algorithmName == "insertion") SortingAlgorithm = new Sorting::Insertion<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // else if (algorithmName == "quick") SortingAlgorithm = new Sorting::Quick<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // else if (algorithmName == "merge") SortingAlgorithm = new Sorting::Merge<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // else if (algorithmName == "heap") SortingAlgorithm = new Sorting::Heap<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    // //raise an error if passed algorithmName doesn't match any already existing classes
    // else throw std::invalid_argument("Algorithm name \"" + algorithmName + "\" is not listed as a valid algorithm!");

	switch (ArrayStrategy) {
		case AlgoGauge::AlgorithmOptions::repeatedSet:
			SortingAlgorithm->loadRepeatedValues();
			break;
		case AlgoGauge::AlgorithmOptions::chunkSet:
			SortingAlgorithm->loadChunkValues();
			break;
		case AlgoGauge::AlgorithmOptions::reversedSet:
			SortingAlgorithm->loadReversedValues();
			break;
		case AlgoGauge::AlgorithmOptions::orderedSet:
			SortingAlgorithm->loadOrderedValues();
			break;
		case AlgoGauge::AlgorithmOptions::randomSet:
			SortingAlgorithm->loadRandomValues();
			break;
		default:
			throw std::invalid_argument("Need to provide a valid Algorithm Option!");
	}

	SortingAlgorithm->runAndCaptureSort();	

	if (verbose) cout << SortingAlgorithm->getStringResult() << endl;
	const std::string jsonDetails = SortingAlgorithm->getJSONResult() + ",";

	// delete SortingAlgorithm;
	// SortingAlgorithm.reset();

	return jsonDetails;
}

std::string printChildProcessSTDOUT(struct subprocess_s &process){
	std::string jsonString;
	std::string stdOUT;
	char buffer[1024];
	 while (true) {
        unsigned bytes_read = subprocess_read_stdout(&process, buffer, sizeof(buffer));
        if (bytes_read == 0) {
            break;  // Exit loop when no more bytes are read
        }
		stdOUT.append(buffer, bytes_read);
    }
	std::vector<std::string> result;
    std::stringstream ss (stdOUT);
    std::string item;

    while (getline (ss, item, '\n')) {
        result.push_back (item);
    }

	for (std::string i : result) {
		if(i.empty()){
			continue;
		}

		if(i.front() == '{' ){
			jsonString += i + ",";
			continue;
		}

		std::cout << i << endl;
	}


	return jsonString;
}


std::string runChildProcess(const char* commandLineArguments[], const char* environment[], const bool& verbose, const std::string_view programmingLanguage){
 	struct subprocess_s process;
	int exit_code;
	std::string stdJSON = "";


    // int result = subprocess_create_ex(commandLineArguments, subprocess_option_search_user_path | subprocess_option_combined_stdout_stderr, environment, &process);
    int result = subprocess_create_ex(commandLineArguments, subprocess_option_search_user_path | subprocess_option_enable_async | subprocess_option_combined_stdout_stderr, environment, &process);

	// cout << process.child;
    if (result != 0) {
        std::cerr << "Failed to start program!" << std::endl;
        return "";
    }

	subprocess_join(&process, &exit_code);

	std::string stdOUT = printChildProcessSTDOUT(process);

	if (!stdOUT.empty() && stdOUT[0] == '{') {
        stdJSON = stdOUT;
    }else{
		cout << stdOUT << std::endl;
	}

	if(exit_code == 0 && verbose){
		std::cout << programmingLanguage << " Program executed successfully!" << std::endl;
	} 
	
	if(exit_code != 0){
        std::cerr << "Program exited with code " << exit_code << std::endl;
    }



    // Clean up
	int cleanUpResult = subprocess_destroy(&process);
	if (cleanUpResult != 0) {
		std::cerr << "Process failed to get destroyed and still might control memory" << std::endl;
	}

    return stdJSON;
}


void processAlgorithms(const AlgoGauge::AlgoGaugeDetails& algorithmsController){
	// int x = 7;
    // assert (x==5);
	std::string includePerf = algorithmsController.Perf ? "true" : "false";
	if(algorithmsController.PerfSample){
		includePerf = "sample";
	}
	
	std::string jsonResults = "{\"algorithms\": ["; //create the json results object even if not specified

	for(auto algo: algorithmsController.SelectedSortingAlgorithms){

		std::transform(algo.Language.begin(), algo.Language.end(), algo.Language.begin(), ::tolower);
		// std::transform(algo.Name.begin(), algo.Name.end(), algo.Name.begin(), ::tolower); // make input lowercase

		if(algo.Language == "c++"){
			jsonResults += runCPlusPlusProgram(
				algo.Algorithm, 
				algo.ArrayLength, 
				algo.ArrayStrategy,
				algo.Name, 
				algorithmsController.Verbose, 
				algorithmsController.Output, 
				includePerf);
			continue;
		}
		if(algorithmsController.Output){
			std::cerr << "Output is not supported with languages besides C++" << endl;
			continue;
		}

		const std::string selectedSortingAlgorithm = "--algorithm=" + algo.Algorithm;

		const std::string selectedArrayStrategy = "--strategy=" + algo.ArrayStrategyString;
		const std::string selectedArrayLength = "--length=" + std::to_string(algo.ArrayLength);
		
		const std::string includeJSON = algorithmsController.Json ? "--json": "--ignore";

		const std::string output = algorithmsController.Output ? "--output" : "--ignore";
		const std::string verbose = algorithmsController.Verbose ? "--verbose": "--ignore";
		// const std::string output = "--output=false";


		const char *environment[] = {NULL};

		if (algo.Language == "node" || algo.Language == "nodejs"|| algo.Language == "js" || algo.Language == "javascript"){
			//node Algogauge.mjs -vTrue -c10 -aBubble -aMerge -c10 -sOrdered -sreversed -j -oTrue -c20 -aDefault -sordered --file="../temp/javascript.txt"
			const char* program_arguments[] = {"node", "../MultiLanguage/Javascript/Algogauge.mjs", selectedSortingAlgorithm.c_str(), selectedArrayStrategy.c_str(), selectedArrayLength.c_str(), output.c_str(), verbose.c_str(), includeJSON.c_str(), nullptr};
			jsonResults += runChildProcess(program_arguments, environment, algorithmsController.Verbose, algo.Language);
			continue;

		}

		if (algo.Language == "python" || algo.Language == "python3" || algo.Language == "py"){
			const char* program_arguments[] = {"python3", "../MultiLanguage/Python/src/AlgoGauge_bradleypeterson/__main__.py", selectedSortingAlgorithm.c_str(), selectedArrayStrategy.c_str(), selectedArrayLength.c_str(), output.c_str(), verbose.c_str(), includeJSON.c_str(), nullptr};

			jsonResults += runChildProcess(program_arguments, environment, algorithmsController.Verbose, algo.Language);

			continue;
		}

		std::string program_path;

		if(algo.Language == "rust"){
			program_path = "../MultiLanguage/testing/rust_binary";
		}


		const char* program_arguments[] = {program_path.c_str(), selectedSortingAlgorithm.c_str(), nullptr};
		std::cout << program_path;
		jsonResults +=  runChildProcess(program_arguments, environment, algorithmsController.Verbose, algo.Language);

	}
	
	jsonResults.pop_back(); //remove extraneous comma
	jsonResults += "]}"; //finish json string

	if (algorithmsController.Json) std::cout << jsonResults << endl;

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
//     x.ArrayLength = 1000;
//     x.ArrayStrategy = randomSet;
//     x.Name = "John";

//     SortingAlgorithmSettings y;
//     y.Algorithm = "merge";
//     y.ArrayLength = 1000;
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
