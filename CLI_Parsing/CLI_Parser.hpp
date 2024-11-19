/**
 * @brief This file handles all the command line parsing and translates the options
 * into algorithm objects that are then called to be run. It is also handles all the
 * errors that may be thrown.
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

//./AlgoGauge -j -v --algo bubble --number=100  --strat=random --lang=c++  --end=200  --step_count = 500

#ifndef ALGOGAUGE_CLI_PARSER_HPP
#define ALGOGAUGE_CLI_PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <map>
#include <charconv>
#include <deque>
#include <queue>
#include <sstream>



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
        ("p,perf", "Includes Perf data in the output.", cxxopts::value<bool>()->implicit_value("true"))
// #ifdef __linux__
//         ("p,perf", "Includes Perf data in the output.", cxxopts::value<bool>()->implicit_value("true"))
// #else
//         ("p perf", "Includes PERF data in the output. Actual PERF only works on linux all perf data returned will be sample (dummy) data", cxxopts::value<bool>()->implicit_value("true"))
// #endif
        ("sample", "Return PERF sample (dummy) data data in the output.", cxxopts::value<bool>()->implicit_value("true"))
    ;
    // Adds the output settings group to allow the user to change where the output of the program should go
    options.add_options("Program Output [OPTIONAL]")

        ("f,file","The file path and name to output the file to",value<string>()->default_value(""),
        "File path must include the file name (extension not required). Regardless of file extension, file content's structure will always be JSON.")

        ("j,json", "Prints the output as a json formatted object", cxxopts::value<bool>()->implicit_value("true"), "Pass this flag if you want the STDOUT to be JSON formatted.")

    ;

    options.add_options("Algorithm Name and Length [REQUIRED]")
        ("a,algo,probe,algorithm", "Sorting: [default, bubble, selection, insertion, quick, merge, heap]\nHash Table: [linear_probe]\nCRUD Operation: [push_front, push_back, pop_front, pop_back, pushpop_back, pushpop_front]", cxxopts::value<vector<string>>(), "Name of the algorithm to run.")
        ("s, strat, type, strategy", "Sorting: [random, repeated, chunks, sorted, sorted_reversed]\nHash Table: [closed]\nCRUD Operation:[array, linked_list]", cxxopts::value<vector<std::string>>(), "Determines what type or strategy used in generating")
        ("n, num, length, start, number", "Provide an int value between 0 and " + std::to_string(UINT32_MAX) + "\nSorting: Size of Array to Sort\nHash Table: Number of testing operations\nCRUD Operation: Number of operations", cxxopts::value<vector<int>>(), "Number of items the algorithm will process")
    ;

    options.add_options("Required Sorting Algorithm")
        ("x,lang,language", "Options: C++, Py/Python3, JS/NodeJS", cxxopts::value<vector<string>>(), "What programming language will process the sorting algorithm")
    ;

    options.add_options("Optional Sorting Algorithm")
        ("i, step,step_count, iterator", "How much the number per iteration", cxxopts::value<vector<int>>()->default_value("1"))
        ("e, end, additional", "The ending number for step operations",  cxxopts::value<vector<int>>()->default_value("0")) 
    ;
    options.add_options("Required CRUD Operation and Hash Table")
        ("c, capacity", "Provide a number (int > 0) that determines hash table, array, or linked list size .", cxxopts::value<vector<int>>(), "How many elements can be stored given strategy")
    ;



    options.add_options("Required Hash Table")
        ("d, load, density", "Provide an int value between 0 (0%) and 100 (100%) to load into hash table.", cxxopts::value<vector<double>>(), "How full or the density of the starting hash table as a percentage")
    ;

    options.add_options("Optional Options")
        ("y, name", "A canonical name that will be returned in output if provided.", cxxopts::value<vector<string>>())
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



    auto languageVector = result["language"].as_optional<vector<string>>();      
    std::deque<std::string> languageDeque;
    if (languageVector.has_value()) {
        languageDeque.assign(languageVector->begin(), languageVector->end());
    }

    auto iteratorVector = result["iterator"].as_optional<vector<int>>();
    std::deque<int> iteratorDeque;
    if(iteratorVector.has_value()){
        iteratorDeque.assign(iteratorVector->begin(), iteratorVector->end());
    }

    auto additionalVector = result["additional"].as_optional<vector<int>>();
    std::deque<int> additionalDeque;
    if(additionalVector.has_value()){
        additionalDeque.assign(additionalVector->begin(), additionalVector->end());
    }
    
    auto capacityVector = result["capacity"].as_optional<vector<int>>();
    std::deque<int> capacityDeque;
    if (capacityVector.has_value()) {
        capacityDeque.assign(capacityVector->begin(), capacityVector->end());
    }

    auto densityVector = result["density"].as_optional<vector<double>>();
    std::deque<double> densityDeque;
    if(densityVector.has_value()){
        densityDeque.assign(densityVector->begin(), densityVector->end());
    }

    auto namesVector = result["name"].as_optional<vector<std::string>>();
    std::deque<std::string> namesDeque;
    if(namesVector.has_value()){
        namesDeque.assign(namesVector->begin(), namesVector->end());
    }


    // cout << "lowered?"<< endl;

    for (auto& str : strategyDeque) {
        // Convert each character in the string to lowercase
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }

    for(auto& algo: algorithmDeque){
        std::transform(algo.begin(), algo.end(), algo.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }
    for(auto& lang: languageDeque){
        std::transform(lang.begin(), lang.end(), lang.begin(),
                       [](unsigned char c){ return std::tolower(c); });
    }
    // cout << "lowered" << endl;
    
    for(const auto& algo: algorithmDeque){

         if(strategyDeque.empty() || numberDeque.empty()){
            std::cerr 
                << "The number of " 
                << (strategyDeque.empty() ? "STRATEGIES " : "")
                << (strategyDeque.empty() && numberDeque.empty() ? "and ": "")
                << (numberDeque.empty() ? "NUMBERS ": "")
                << "passed did not match the number of algorithms"
                << std::endl
            ;

            throw std::invalid_argument("Missing required options for algorithm: STRATEGY or NUMBER per algorithm.");
        }

     

        if(algo == "linear" || algo == "linear_probe"){
            struct AlgoGauge::HashTableSettings newHashTable;
            newHashTable.Probe = algo;
            newHashTable.Number = numberDeque.front();

            if(capacityDeque.empty() || densityDeque.empty()){
                std::cerr
                    << "The number of " 
                    << (capacityDeque.empty()? "CAPACITY ": "")
                    << (densityDeque.empty()? "DENSITY  ": "")
                    << "did not match the number of hash table algorithms" 
                    << std::endl
                ;
                throw std::invalid_argument("Missing required options for algorithm: CAPACITY or DENSITY per hash algorithm.");
            }


            if(densityDeque.front() >= 100 || densityDeque.front() < 0){
                throw std::invalid_argument("The DENSITY or load of the hash table can not be bigger then 100\% or 0\%");
            }else if(densityDeque.front() < 1){
                newHashTable.Load = densityDeque.front() * 100;
            }else if(densityDeque.front() >= 100){
                throw std::invalid_argument("The DENSITY or load of the hash table can not be bigger then 100%");
            }
            else{
                newHashTable.Load = densityDeque.front();
            }

            newHashTable.Capacity = capacityDeque.front();
            newHashTable.Type = strategyDeque.front();
            if(!namesDeque.empty()){
                newHashTable.Name = namesDeque.front();
            }

            numberDeque.pop_front();
            capacityDeque.pop_front();
            densityDeque.pop_front();
            strategyDeque.pop_front();

            if(!namesDeque.empty()){
                namesDeque.pop_front();
            }         

            algogaugeDetails.SelectedHashTables.push_back(newHashTable);
            continue;
        }
       
        if(AlgoGauge::crudOperations.find(algo) != AlgoGauge::crudOperations.end()){
            if(capacityDeque.empty()){
                std::cerr
                    << "The number of " 
                    << (capacityDeque.empty()? "CAPACITY ": "")
                    << "did not match the number of CRUD operations" 
                    << std::endl
                ;
                throw std::invalid_argument("Missing required options for algorithm: CAPACITY per CRUD operation.");
            }

            if(!(strategyDeque.front() == "array" || strategyDeque.front() == "linked_list")){
                throw std::invalid_argument("Strategy isn't a type of array or linked_list: " + strategyDeque.front());
            }

            struct AlgoGauge::CRUDOperationSettings newCRUDOperation;
            newCRUDOperation.Number = numberDeque.front();
            newCRUDOperation.Operation = algo;
            newCRUDOperation.Type = strategyDeque.front();
            newCRUDOperation.Size = capacityDeque.front();


            if(!namesDeque.empty()){
                newCRUDOperation.Name = namesDeque.front();
            }

            strategyDeque.pop_front();
            capacityDeque.pop_front();

            numberDeque.pop_front();

            if(!namesDeque.empty()){
                namesDeque.pop_front();
            }

            algogaugeDetails.SelectedCRUDOperations.push_back(newCRUDOperation);
            continue;
        }




        if(languageDeque.empty()){
            throw std::invalid_argument("The number of programming languages passed do not match the number of sorting algorithms passed");
        }

      
        if(!iteratorDeque.empty() && additionalDeque.empty() || iteratorDeque.empty() && !additionalDeque.empty()){
            std::cerr
                << "The number of "
                << (iteratorDeque.empty() ? "ITERATOR ": "")
                << (additionalDeque.empty() ? "END ": "")
                << "passed did not math the number of iterators"
                << std::endl;

            throw std::invalid_argument("Missing optional requirements for algorithm: ITERATOR and END per algorithm.");
        } 

        for(int i = 0; i <= (!additionalDeque.empty() ? additionalDeque.front(): 0); i+=(!iteratorDeque.empty()? iteratorDeque.front(): 1)){
            struct AlgoGauge::SortingAlgorithmSettings newSortingAlgorithm;
            newSortingAlgorithm.Algorithm = algo;
            newSortingAlgorithm.ArrayLength = numberDeque.front() + i;
            newSortingAlgorithm.ArrayStrategyString = strategyDeque.front();
            newSortingAlgorithm.Language = languageDeque.front();

            if(!namesDeque.empty()){
                newSortingAlgorithm.Name = namesDeque.front();
            }

            auto it = strategyMap.find(strategyDeque.front());

            if (it != strategyMap.end()) {
                newSortingAlgorithm.ArrayStrategy = it->second;
            } else {
                throw std::invalid_argument("There is no array strategy: " + strategyDeque.front());
            }
            
            algogaugeDetails.SelectedSortingAlgorithms.push_back(newSortingAlgorithm);
            
        }
        strategyDeque.pop_front();
        languageDeque.pop_front();
        numberDeque.pop_front();
        if(!iteratorDeque.empty()){
            iteratorDeque.pop_front();
        }
        if(!additionalDeque.empty()){
            additionalDeque.pop_front();
        }
        

        if(!namesDeque.empty()){
            namesDeque.pop_front();
        }
        continue;   
    }

    if(algogaugeDetails.Verbose){
        std::string verboseOutput;
        for(const auto& sorting: algogaugeDetails.SelectedSortingAlgorithms){
            std::ostringstream oss;
            oss << "Sorting Algorithm"
                << " algorithm: " << sorting.Algorithm
                << " array_length: " << sorting.ArrayLength
                << " strategy: " << sorting.ArrayStrategy
                << " language: " << sorting.Language
                << " name: " << sorting.Name
                << "\n"
            ;
            verboseOutput += oss.str();
        }
        for(const auto& hashtable: algogaugeDetails.SelectedHashTables){
            std::ostringstream oss;
            oss << "Hash Tables"
                << " probe: " << hashtable.Probe
                << " type: " << hashtable.Type
                << " capacity: " << hashtable.Capacity
                << " load: " << hashtable.Load
                << " number: " << hashtable.Number
                << " name: " << hashtable.Name
                << "\n";

            verboseOutput += oss.str();

        }
        for(const auto& crudOP: algogaugeDetails.SelectedCRUDOperations){
            std::ostringstream oss;
            oss << "CRUD Operation"
                << " type: " << crudOP.Type
                << " operation: " << crudOP.Operation
                << " size: " << crudOP.Size
                << " number: " << crudOP.Number
                << " name: " << crudOP.Name
                << "\n";

            verboseOutput += oss.str();

        }

        std::cout << verboseOutput;
    }
       
#ifndef linux
    // raise warning if trying to use perf on non-linux system
    if (algogaugeDetails.Perf == perfON) {
        std::cerr << "Warning: PERF is not supported on this system. PERF functionality is only available on Linux distributions. The program will continue to run, but any PERF data will be replaced with sample (dummy) values. To avoid this warning, remove the 'p' or 'perf' option from your arguments, or add --sample to explicitly request dummy data." << std::endl;
    }
#endif
    return algogaugeDetails;
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



