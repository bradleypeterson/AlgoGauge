/**
 * @brief This file handles all the command line parsing and translates the options
 * into algorithm objects that are then called to be run. It is also handles all the
 * errors that may be thrown.
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "../algorithms/hash_algs.cpp"
#include "../algorithms/sort_7algs.cpp"
#include "../dependencies/cxxopts.hpp"

#ifndef ALGOGAUGE_CLI_PARSER_HPP
#define ALGOGAUGE_CLI_PARSER_HPP

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::cerr;

using namespace cxxopts;
using namespace AlgoGauge;

/**
 * Creates the initial Options object and houses all the available options a user can pass into
 * @return Options object with all the available options available
 */
Options getOptions() {
    //Note: With these different groups, regardless of when they are specified in code, the Options object will
    //overwrite this ordering by always displaying them in alphabetical order according to the Group name. That's why
    //the group names are also named in alphabetical order according to the ordering in this code.

    // Sets up the initial Options object. Specifies the program name and description
    Options options("AlgoGauge", "This is the command line version of the AlgoGauge Program.");

    // Adds the first group of options that are specific to the Algorithm
    options.add_options("Algorithm Name and Length [REQUIRED]")
        ("a,algo,algorithm", /*TODO: ADD HASH ARRAY AND HASH LINKED LIST*/"Supported algorithms include: bubble, selection, insertion, quick, merge, heap.", value<vector<string>>(), "Name of the first algorithm to run.")
        ("l,len,length", "Provide an int value between 0 and " + std::to_string(UINT32_MAX), value<vector<int>>(), "Number of items to process")
        // name is not required. May consider moving this to Program Output group instead to avoid confusion
        ("n,name", "A canonical name that will be returned in output if provided.", value<vector<string>>()->default_value(""))
    ;

    // Adds the Options group where one and only one option may be specified per Algorithm.
    options.add_options("Algorithm Options [ONE REQUIRED]")
        ("r,ran,rand,random", "Generated data will be a random set", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("e,rep,repeated", "Generated data set will have repeated values", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("c,chunks", "Generated data set will have various subsets that will contain both random and in order values", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("s,rev,reversed", "Generated data set will be in reverse order e.g. (9-0)", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("o,ord,ordered", "Generated data set will be ordered e.g. (0-9)", value<vector<bool>>()->implicit_value("true")->default_value("false"))
    ;

    // Adds the output settings group to allow the user to change where the output of the program should go
    options.add_options("Program Output [OPTIONAL]")
        (
        "f,file",
        "The file path and name to output the file to",
        value<string>()->default_value(""),
        "File path must include the file name (extension not required). Regardless of file extension, file content's structure will always be JSON."
        )
        ("j,json", "Prints the output as a json formatted object", value<bool>()->implicit_value("true"), "Pass this flag if you want the STDOUT to be JSON formatted.")
    ;

    // Adds the final group for setting any additional program wide settings
    options.add_options("Program Settings [OPTIONAL]")
        ("v,verbose", "Runs the program in Verbose mode", value<bool>()->implicit_value("true"), "Results will be passed to STDOUT and errors to STDERR regardless of flag.")
        ("i,include-values", "Will include values in the output. It is highly recommended to use a small length (less than 100).", value<bool>()->implicit_value("true"))
        ("h,help", "Prints this help page.")
        ("p,perf", "Includes Perf data in the output. Actual Perf data only works on Linux.", value<string>()->implicit_value("true")->default_value("false"),
            "If you are not on Linux and want to use this anyways, you can set this to \"sample\". e.g. --perf=sample"
        )
    ;

    // Custom help message that displays when -h or --help is passed instead of the typical USAGE: ...
    options.custom_help(R"(--algo (some algorithm) --len [some int > 0][OPTIONALS: -r | -e | -c | -s | -o] [-f "some file", -v, -j, -i, -p])" );

    return options;
}

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
BaseSort<unsigned int>* getAlgorithm(
    string algorithmName, // Opting for this to be a string and NOT an enum as we can just pass what the user passes as the --algo arg directly and return an error if it doesn't match any algorithms
    const unsigned int& length,
    const string& canonicalName = "",
    const bool& verbose = false,
    const bool& includeValues = false,
    const string& includePerf = "false"
) {
    transform(algorithmName.begin(), algorithmName.end(), algorithmName.begin(), ::tolower); // make input lowercase

    //Essentially this is a switch case block that determines which algorithm to create and return
    if (algorithmName == "bubble") return new Bubble<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "selection") return new Selection<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "insertion") return new Insertion<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "quick") return new Quick<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "merge") return new Merge<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else if (algorithmName == "heap") return new Heap<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);
    else throw std::invalid_argument("Algorithm name '" + algorithmName + "' is not listed as a valid algorithm!");
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
vector<BaseSort<unsigned int>*> parseAndGetAlgorithms(const ParseResult& result, const Options& options) {
    vector<BaseSort<unsigned int>*> algorithms; //create the initial vector object that will contain all the sorting algorithm objects to return
    if (result.count("help"))
    // check if help is passed, if so, ONLY insert the help message into STDOUT
    {
        cout << endl << endl << options.help() << endl;
        //should consider return the algorithms vector here so that the rest of the function isn't wrapped in an
        //else block. The only reason I initially did this is to avoid redundancies. This current way allows only one
        //return algorithms; line instead of two. However, for readability’s sake, it may be worth changing this.
    } else {
        // get all data
        vector<string> algo = result["a"].as<vector<string>>();
        vector<int> len = result["l"].as<vector<int>>();

        /* The following lines are currently commented out as they were initially needed, but no longer needed since
         * we are manually pulling these out later on. We are manually pulling these out because we need to make the
         * number of algorithms passed matches the number of lengths passed, and number of algorithm options passed.
         * Originally, we were using the commented out block to easily do this since the vector object already has a
         * builtin length property. The problem we ran into however, is that we would then have no way of knowing
         * which algorithm option was tied to which algorithm.
         *
         * E.g.
         * If the following args were "--algo quick --len 5 -r --algo selection --len 10 -e --algo merge --len 15 -r"
         * The original way would store the args as follows:
         *      vector<string> algo = {"quick", "selection", "merge"}
         *      vector<string> len = {5, 10, 15}
         *      vector<bool> ran = {true, true}
         *      vector<bool> rep = {true}
         * The algo and len vectors are fine since each algorithm is stored in the order it was listed in the CLI,
         * however, since the algorithm options are all separate bool vectors, there is no way to know if the two
         * true values in the ran vector were for the first 2 algorithms, last 2 algorithms, or ends.
         *
         * One solution to this is to get rid of the separate algorithm option vector<bool> objects and replace it with
         * a single vector<string> object like with the algo vector object. However, I opted against this as it would
         * make the code side more simple, but the CLI side more complicated and more prone to user errors. Since I
         * valued prioritizing simplicity on the CLI side, this is the route I went for.
         *
         * There is no harm in uncommenting out the following lines. The only reason they were commented out to begin
         * with is it's best practice to remove or comment out unused objects.
         */

//        vector<bool> ran = result["r"].as<vector<bool>>();
//        vector<bool> rep = result["e"].as<vector<bool>>();
//        vector<bool> chunks = result["c"].as<vector<bool>>();
//        vector<bool> rev = result["s"].as<vector<bool>>();
//        vector<bool> ord = result["o"].as<vector<bool>>();
//        vector<string> cns = result["n"].as<vector<string>>(); //get the canonical name strings

        bool verbose = result["v"].as<bool>(); //set the Verbose flag
        bool includeValues = result["i"].as<bool>(); //set the Include Values flag
        string includePerf = result["p"].as<string>(); //set the Perf flag

        //convert the value that is passed to the Perf flag to lowercase. This is mainly meant for
        //if "sample" is passed to the Perf flag so that "sample" is case-insensitive.
        transform(includePerf.begin(), includePerf.end(), includePerf.begin(), ::tolower);

#ifndef linux
        // raise error if tyring to use perf on non-linux system
        if (includePerf == "true") {
            throw std::invalid_argument("Option 'p' or 'perf' is not supported on this system! Perf only works on Linux distros! Please remove option 'p' or 'perf' from your args and try again. If you want to test Perf on a non-Linux system, then set --perf=sample instead. Note: this only returns dummy values!");
        }
#endif

        // check to make sure data has the same number of required arguments
        vector<AlgorithmOptions> algorithmOptions; //store all the algorithm options in the order they were listed in the CLI
        vector<string> cNames; // This is to store all the canonical names that we pull from the results
        int counter = 0; // This is to keep track of how many algorithms are passed compared to the ones with canonical names
        for (const auto& r : result) { // loop through all results
            if (r.key() == "algorithm") counter++; // increment the counter for every time an algorithm argument is passed
            else if (r.key() == "name") {
                // collect the canonical name and decrement the counter for every canonical name that is passed
                cNames.push_back(r.value());
                counter--;
            }
            for (string s : AlgorithmOptionsStrings) { // loop through all the valid algorithm options to map the string to
                transform(s.begin(), s.end(), s.begin(), ::tolower); // convert the string to lowercase to make the input case-insensitive
                if (r.key() == s && r.value() == "true") {
                    //if the key equals one of the valid algorithm options and the value is true, then append the algorithmOptions vector with the appropriate algorithm option
                    if (r.key() == "random")  algorithmOptions.push_back(AlgorithmOptions::randomSet);
                    if (r.key() == "repeated")  algorithmOptions.push_back(AlgorithmOptions::repeatedSet);
                    if (r.key() == "chunks")  algorithmOptions.push_back(AlgorithmOptions::chunkSet);
                    if (r.key() == "reversed")  algorithmOptions.push_back(AlgorithmOptions::reversedSet);
                    if (r.key() == "ordered")  algorithmOptions.push_back(AlgorithmOptions::orderedSet);
                }
            }
        }

        //fill in the rest of the cNames with empty strings
        while (counter-- > 0) cNames.push_back("");

        //make sure the cardinality of the algorithms, lengths, and algorithm options are the same
        if (!(result.count("a") == result.count("l") && result.count("a") == algorithmOptions.size())) {
            throw std::invalid_argument("Number of provided algorithm(s), length(s) and algorithm option(s) arguments do not match!");
        }

        //loop through all passed algorithms args, get the algorithm object, load it with the values, and append it to the returning object
        for (int i = 0; i < algo.size(); i++) {
            // create the new algorithm object
            auto newAlgorithm = getAlgorithm(
                algo[i],
                len[i],
                cNames[i],
                verbose,
                includeValues,
                includePerf
            );
            // load it with the appropriate algorithm option values
            switch (algorithmOptions[i]) {
                case AlgorithmOptions::randomSet:
                    newAlgorithm->loadRandomValues();
                    break;
                case AlgorithmOptions::repeatedSet:
                    newAlgorithm->loadRepeatedValues();
                    break;
                case AlgorithmOptions::chunkSet:
                    newAlgorithm->loadChunkValues();
                    break;
                case AlgorithmOptions::reversedSet:
                    newAlgorithm->loadReversedValues();
                    break;
                case AlgorithmOptions::orderedSet:
                    newAlgorithm->loadOrderedValues();
                    break;
                default:
                    throw std::invalid_argument("Need to provide a valid Algorithm Option!");
            }
            //append it to the returning object
            algorithms.push_back(newAlgorithm);
        }
    }

    return algorithms;
}

/**
 * Processes all passed algorithms by running them one at a time and handling their output accordingly
 * @param algorithms The list of algorithms to run and output. Stored as a vector of algorithms that inherit from the BaseSort ABC
 * @param json Boolean value of whether the output should be in JSON format
 * @param verbose Boolean value of whether everything should be outputted or not
 * @param file File path of where the file of the output should be created. Set as an empty string to bypass this
 */
void processAlgorithms(const vector<BaseSort<unsigned int>*>& algorithms, const bool& json, const bool& verbose, const string& file) {
    string jsonResults = "{\"algorithms\": ["; //create the json results object even if not specified
    //loop through all algorithms and run them according to the last 3 arguments passed to this function
    for (auto algorithm : algorithms) {
        if (json || !file.empty()) jsonResults += algorithm->runAndGetJSONSort() + ",";
        if (!(json || !file.empty())) algorithm->runAndPrintSort();
        if (verbose) cout << algorithm->getStringResult() << endl;
    }

    //print out json to STDOUT or file if specified or print to file if specified
    if (json || !file.empty()) {
        jsonResults.pop_back(); //remove extraneous comma
        jsonResults += "]}"; //finish json string
        if (json) cout << jsonResults << endl; //print out json results if specified
        if (!file.empty()) { //print output to file
            std::ofstream outFile;
            outFile.open(file);
            outFile << jsonResults;
            outFile.close();
        }
        if (verbose && !file.empty()) cout << "Results written in JSON at: '" << file << "'" << endl;
    }
    cout << endl;
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
        Options options = getOptions(); //get the initial Options object
        auto result = options.parse(argc, argv); //have the cxxopts library parse the CLI args
        algorithms = parseAndGetAlgorithms(result, options); //get all valid algorithms
        if (!algorithms.empty()) //process all valid algorithms if any found
        processAlgorithms(
            algorithms,
            result["json"].as<bool>(),
            result["verbose"].as<bool>(),
            result["file"].as<string>()
        );
    //all possible errors are caught and returned here to STDERR
    } catch (int e_code) { //catch errors that only have an error number
        cerr << "An error has occurred! Error number: " << e_code << endl;
        return_code = e_code;
    } catch (char e_string) { //catch errors that only have an error message
        cerr << "An error has occurred! Error: " << e_string << endl;
        return_code = -1;
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