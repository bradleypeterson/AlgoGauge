/**
 * @brief This file handles all the command line parsing and translates the options
 * into algorithm objects that are then called to be run. It is also handles all the
 * errors that may be thrown.
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#ifndef ALGOGAUGE_CLI_PARSER_HPP
#define ALGOGAUGE_CLI_PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <map>
#include <charconv>
#include <deque>


#include "algorithm_caller.hpp"
#include "../algorithms/hash_algs.cpp"

#include "../algorithms/sort_7algs.cpp"
#include "../dependencies/cxxopts.hpp"



using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::cerr;

using namespace cxxopts;
using namespace Sorting;




/**
 * Creates the initial Options object and houses all the available options a user can pass into
 * @return Options object with all the available options available
 */
Options getOptions(string type) {
    //Note: With these different groups, regardless of when they are specified in code, the Options object will
    //overwrite this ordering by always displaying them in alphabetical order according to the Group name. That's why
    //the group names are also named in alphabetical order according to the ordering in this code.

    // Sets up the initial Options object. Specifies the program name and description
    Options options("AlgoGauge", "This is the command line version of the AlgoGauge Program.");
    // Adds the final group for setting any additional program wide settings
    options.add_options("Program Settings [OPTIONAL]")
        ("v,verbose", "Runs the program in Verbose mode", cxxopts::value<bool>()->implicit_value("true"))
        ("o,output", "Will output the arrays in the output. It is highly recommended to use a small length (less than 100).", cxxopts::value<bool>()->implicit_value("true"))
        ("h,help", "Prints this help page.")
#ifdef __linux
        ("p,perf", "Includes Perf data in the output.", cxxopts::value<bool>()->implicit_value("true"))
#else
        ("p perf", "Includes PERF data in the output. Actual PERF only works on linux all perf data returned will be sample (dummy) data", cxxopts::value<bool>()->implicit_value("true"))
#endif
        ("sample", "Return PERF sample (dummy) data data in the output.", cxxopts::value<bool>()->implicit_value("true"))
    ;
    // Adds the output settings group to allow the user to change where the output of the program should go
    options.add_options("Program Output [OPTIONAL]")

        ("f,file","The file path and name to output the file to",value<string>()->default_value(""),
        "File path must include the file name (extension not required). Regardless of file extension, file content's structure will always be JSON.")

        ("j,json", "Prints the output as a json formatted object", cxxopts::value<bool>()->implicit_value("true"), "Pass this flag if you want the STDOUT to be JSON formatted.")

    ;

    options.add_options("Algorithm Name and Length [REQUIRED]")
        ("a,algo,probe,algorithm", "Sorting: [default, bubble, selection, insertion, quick, merge, heap]\nHash Table: [linear]", cxxopts::value<vector<string>>(), "Name of the algorithm to run.")
        ("s, strat, type, strategy", "Sorting: [ran/random, rep/repeated, chunks, rev/sorted-reverse, sorted]\nHash Table: [closed]", cxxopts::value<vector<std::string>>(), "Determines what type or strategy used in generating")
        ("n, num, number", "Provide an int value between 0 and " + std::to_string(UINT32_MAX) + "\nSorting: Size of Array to Sort\nHash Table: Number of testing operations", cxxopts::value<vector<int>>(), "Number of items the algorithm will process")
    ;

    options.add_options("Required Sorting Algotithm")
        ("x,lang,language", "Options: C++, Py/Python3, JS/NodeJS", cxxopts::value<vector<string>>()->default_value("c++"), "What programming langauge will process the sorting algorithm");
    ;
    options.add_options("Required Hash Table")
        ("c, capacity", "Provide a number (int > 0) that determines hash table size.", cxxopts::value<vector<int>>(), "How many elements can be stored in the Hash Table")
        ("d, density", "Provide an int value between 0 (0%) and 100 (100%) to load into hash table.", cxxopts::value<vector<double>>(), "How full or the density of the starting hash table as a percentage")
    ;
    options.add_options("Optional Options")
        ("y, name", "A canonical name that will be returned in output if provided.", cxxopts::value<vector<string>>()->default_value(""))
    ;

    return options;
}


/**
 * Conducts additional parsing in addition to what the cxxopts library has already parsed. Makes sure that all passed
 * command line arguments/flags are valid and consistent. It then takes all args/flags passed and converts them into a
 * list of sorting algorithm objects. This is useful in that a user can specify n number of algorithms in a single
 * command line command.
 * @param result The resulting ParseResult object created by cxxopts from auto-parsing the CLI args.
 * @param options The cxxopts Options object. This is needed in the case that the "help" arg is passed.
 * @return A list of sorting algorithm objects in the form of a vector object where each element type is a pointer to
 * a BaseSort ABC object. Essentially, this pointer will be filled with the actual sorting algorithm that inherits from
 * the BaseSort ABC.
 */
AlgoGauge::AlgoGaugeDetails parseAndGetAlgorithms(const ParseResult& result, const Options& options, string type) {
    std::cout << "what";
    static const std::unordered_map<std::string, AlgoGauge::AlgorithmOptions> strategyMap = {
        {"random", AlgoGauge::AlgorithmOptions::randomSet},
        {"repeated", AlgoGauge::AlgorithmOptions::repeatedSet},
        {"chunks", AlgoGauge::AlgorithmOptions::chunkSet},
        {"reversed", AlgoGauge::AlgorithmOptions::reversedSet},
        {"ordered", AlgoGauge::AlgorithmOptions::orderedSet}
    };

    AlgoGauge::AlgoGaugeDetails algogaugeDetails;

    if (result.count("help")){
        // check if help is passed, if so, ONLY insert the help message into STDOUT
        cout << endl << endl << options.help() << endl;
        //should consider return the algorithms vector here so that the rest of the function isn't wrapped in an
        //else block. The only reason I initially did this is to avoid redundancies. This current way allows only one
        return algogaugeDetails; //line instead of two. However, for readability’s sake, it may be worth changing this.
    }


    algogaugeDetails.Verbose = result["verbose"].as<bool>();
    algogaugeDetails.Output = result["output"].as<bool>();

    if (result["sample"].as<bool>()) {
        algogaugeDetails.Perf = sample;
    } else if (result["perf"].as<bool>()) {
        algogaugeDetails.Perf = perfON;
    } else {
        algogaugeDetails.Perf = perfOFF;
    }

    algogaugeDetails.Json = result["json"].as<bool>();
    algogaugeDetails.FileWritePath = result["file"].as<string>();

    
    auto algorithmVector = result["algorithm"].as<std::vector<std::string>>();
    std::deque<std::string> algorithmDeque(algorithmVector.begin(), algorithmVector.end());
    //    std::deque<int> deq(vec.begin(), vec.end());
    auto strategyVector = result["strategy"].as<vector<std::string>>();
    std::deque<std::string> strategyDeque(strategyVector.begin(), strategyVector.end());

    auto numberVector = result["number"].as<vector<int>>();
    std::deque<int> numberDeque(numberVector.begin(), numberVector.end());

    auto langaugeVector = result["language"].as<vector<std::string>>();
    std::deque<std::string> langaugeDeque(langaugeVector.begin(), langaugeVector.end());

    // auto capacityVector = result["capacity"].as<vector<int>>();
    // std::deque<int> capacityDeque(capacityVector.begin(), capacityVector.end());

    auto densityVector = result["density"].as<vector<int>>();
    std::deque<int> densityDeque(densityVector.begin(), densityVector.end());

    auto namesVector = result["name"].as<vector<std::string>>();
    std::deque<std::string> namesDeque(namesVector.begin(), namesVector.end());

    for (auto& str : namesDeque) {
        // Convert each character in the string to lowercase
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }

    for(auto& algo: algorithmDeque){
        std::transform(algo.begin(), algo.end(), algo.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }

    for(const auto& algo: algorithmDeque){
        // std::transform(algo.begin(), algo.end(), algo.begin(), ::tolower);

        if(algo == "linear"){
            struct AlgoGauge::HashTableSettings newHashTable;
            newHashTable.Probe = algo;
            newHashTable.Number = numberDeque.front();
            // newHashTable.Capacity = capacityDeque.front();
            newHashTable.Load = densityDeque.front();
            newHashTable.Type = strategyDeque.front();

            numberDeque.pop_front();
            // capacityDeque.pop_front();
            densityDeque.pop_front();
            strategyDeque.pop_front();
            
            algogaugeDetails.SelectedHashTables.push_back(newHashTable);
            continue;
        }

        struct AlgoGauge::SortingAlgorithmSettings newSortingAlgorithm;

        newSortingAlgorithm.Language = langaugeDeque.front();
        newSortingAlgorithm.Algorithm = algo;

        auto it = strategyMap.find(algo);
        std::cout << it->first << std::endl;
        newSortingAlgorithm.Algorithm = it->second;

        // if (it != strategyMap.end()) {
        //     newSortingAlgorithm.Algorithm = newSortingAlgorithm.Algorithm->second;
        // } else {
        //     throw std::invalid_argument("There is no array strategy: " + arrayStrategy);
        // }

    }
    
       
    return algogaugeDetails;
    

        // check to make sure data has the same number of required arguments
        // if (algorithmVector.size() != countVector.size() || 
        //     algorithmVector.size() != languageVector.size() || 
        //     algorithmVector.size() != strategyVector.size() || 
        //     algorithmVector.size() == 0) {
            
        //     throw std::invalid_argument("Number of provided algorithm(s), count(s), language(s), and strategy(s) arguments do not match!");
        // }
// 

        // for (int i = 0; i < algorithmVector.size(); i++) {
        //     struct AlgoGauge::SortingAlgorithmSettings newObject;
        //     newObject.Language = languageVector[i];
        //     newObject.Algorithm = algorithmVector[i];

        //     auto arrayStrategy = strategyVector[i];
            

         

        //     // newObject.ArrayLength = countVector[i];
        //     if (i < names.size()) {
        //         newObject.Name = names[i];     // Assign name if valid
        //     } else {
        //         newObject.Name = "";
        //     }

        //     algogaugeDetails.SelectedSortingAlgorithms.push_back(newObject);
        // }


        //convert the value that is passed to the Perf flag to lowercase. This is mainly meant for
        //if "sample" is passed to the Perf flag so that "sample" is case-insensitive.
        // transform(includePerf.begin(), includePerf.end(), includePerf.begin(), ::tolower);

#ifndef linux
    // raise error if tyring to use perf on non-linux system
    if (algogaugeDetails.Perf && !algogaugeDetails.PerfSample) {
        std::cerr << "Warning: PERF is not supported on this system. PERF functionality is only available on Linux distributions. The program will continue to run, but any PERF data will be replaced with sample (dummy) values. To avoid this warning, remove the 'p' or 'perf' option from your arguments, or add --sample to explicitly request dummy data." << std::endl;
    }
        return algogaugeDetails;
#else
    return algogaugeDetails;
#endif
}



/**
 * Runner function that manages the whole program
 * @param argc The number of arguments passed (THIS SHOULD BE DIRECTLY PASSED FROM MAIN() ARGS)!
 * @param argv The actual arguments passed (THIS SHOULD BE DIRECTLY PASSED FROM MAIN() ARGS)!
 * @return The return code for the program
 */
int runProgram(int argc, char *argv[]) {
    int return_code = 0;
    vector<BaseSort<unsigned int>*> algorithms; //create the initial vector to hold all the passed algorithms
    try {
        string type = argv[1]; // gets algorithm type
        Options options = getOptions(type); //get the initial Options object
        auto result = options.parse(argc, argv); //have the cxxopts library parse the CLI args
        auto algo = parseAndGetAlgorithms(result, options, type); //get all valid algorithms
        // TODO: implement hash table functionality to processAlgorithms function. 
        //       may need to pass type.
        processAlgorithms(algo);

    //all possible errors are caught and returned here to STDERR
    } catch (int e_code) { //catch errors that only have an error number
        cerr << "An error has occurred! Error number: " << e_code << endl;
        return_code = e_code;
    } catch (exceptions::specification& e_spec) {
        cerr << "Error defining options! Error: " << e_spec.what() << endl;
        return_code = -1;
    } catch (exceptions::parsing& e_parse) {
        cerr << "Error parsing arguments! Error: " << e_parse.what() << endl;
        return_code = -1;
    } catch (exceptions::exception& e_e) {
        cerr << "An error has occurred! Error: " << e_e.what() << endl;
        return_code = -1;
    } catch (std::invalid_argument& e_ia) {
        cerr << "An error has occurred! Error: " << e_ia.what() << endl;
        return_code = -1;
    } catch (std::exception& e) {
        // Should be a catch-all exception since all exceptions should inherit from std::exception
        cerr << "An error has occurred! Error: " << e.what() << endl;
        return_code = -1;
    }catch (char e_string) { //catch errors that only have an error message
        cerr << "An error has occurred! Error: " << e_string << endl;
        return_code = -1;
    
    } catch (...) {
        // A final catch-all in case the first catch-all doesn't in fact... (wait for it)... catch-all.
        cerr << "An unknown error has occurred!" << endl;
        return_code = -1;
    }
    
    //deallocate memory
    for (auto algorithm : algorithms) {
        delete algorithm;
    }
    
    if (return_code) cerr << R"(To view how to use this program, run "AlgoGauge -h" or "AlgoGauge --help")" << endl;
    return return_code;
}




#endif //ALGOGAUGE_CLI_PARSER_HPP



