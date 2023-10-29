/**
 * @brief This file handles all the command line parsing and translates the options
 * into algorithm objects that are then called to be run. It is also handles all the
 * errors that may be thrown.
 * @authors John Z. DeGrey, Brad Peterson Ph.D., et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <fstream>
#include <vector>
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


Options getOptions() {
    Options options("AlgoGauge", "This is the command line version of the AlgoGauge Program.");

    options.add_options("Algorithm Name and Length [REQUIRED]")
        ("a,algo,algorithm", "Supported algorithms include: bubble, selection, insertion, quick, merge, heap.", value<vector<string>>(), "Name of the first algorithm to run.")
        ("l,len,length", "Provide an int value between 0 and " + std::to_string(UINT32_MAX), value<vector<int>>(), "Number of items to process")
        ("n,name", "A canonical name that will be returned in output if provided.", value<vector<string>>()->default_value(""))
    ;

    options.add_options("Algorithm Options [ONE REQUIRED]")
        ("r,ran,rand,random", "Generated data will be a random set", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("e,rep,repeated", "Generated data set will have repeated values", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("c,chunks", "Generated data set will have various subsets that will contain both random and in order values", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("s,rev,reversed", "Generated data set will be in reverse order e.g. (9-0)", value<vector<bool>>()->implicit_value("true")->default_value("false"))
        ("o,ord,ordered", "Generated data set will be ordered e.g. (0-9)", value<vector<bool>>()->implicit_value("true")->default_value("false"))
    ;

    options.add_options("Program Output [OPTIONAL]")
        (
        "f,file",
        "The file path and name to output the file to",
        value<string>()->default_value(""),
        "File path must include the file name (extension not required). Regardless of file extension, file content's structure will always be JSON."
        )
        ("j,json", "Prints the output as a json formatted object", value<bool>(), "Pass this flag if you want the STDOUT to be JSON formatted.")
    ;

    options.add_options("Program Settings [OPTIONAL]")
        ("v,verbose", "Runs the program in Verbose mode", value<bool>(), "Results will be passed to STDOUT and errors to STDERR regardless of flag.")
        ("i,include-values", "Will include values in the output. It is highly recommended to use a small length (less than 100).", value<bool>())
        ("h,help", "Prints this help page.")
    ;

    options.custom_help(R"(--algo (some algorithm) --len [some int > 0][OPTIONALS: -r | -e | -c | -s | -o] [-f "some file", -v, -j])" );

    return options;
}

BaseSort<unsigned int>* getAlgorithm(
    string algorithmName,
    const unsigned int& length,
    const string& canonicalName = "",
    const bool& verbose = false,
    const bool& includeValues = false
) {
    transform(algorithmName.begin(), algorithmName.end(), algorithmName.begin(), ::tolower);
    if (algorithmName == "bubble") return new Bubble<unsigned int>(length, canonicalName, verbose, includeValues);
    else if (algorithmName == "selection") return new Selection<unsigned int>(length, canonicalName, verbose, includeValues);
    else if (algorithmName == "insertion") return new Insertion<unsigned int>(length, canonicalName, verbose, includeValues);
    else if (algorithmName == "quick") return new Quick<unsigned int>(length, canonicalName, verbose, includeValues);
    else if (algorithmName == "merge") return new Merge<unsigned int>(length, canonicalName, verbose, includeValues);
    else if (algorithmName == "heap") return new Heap<unsigned int>(length, canonicalName, verbose, includeValues);
    else throw std::invalid_argument("Algorithm name '" + algorithmName + "' is not listed as a valid algorithm!");
}

vector<BaseSort<unsigned int>*> parseAndGetAlgorithms(const ParseResult& result, const Options& options) {
    vector<BaseSort<unsigned int>*> algorithms;
    if (result.count("help"))
    {
        cout << endl << endl << options.help() << endl;
    } else {
        // get all data
        vector<string> algo = result["a"].as<vector<string>>();
        vector<int> len = result["l"].as<vector<int>>();
        vector<bool> ran = result["r"].as<vector<bool>>();
        vector<bool> rep = result["e"].as<vector<bool>>();
        vector<bool> chunks = result["c"].as<vector<bool>>();
        vector<bool> rev = result["s"].as<vector<bool>>();
        vector<bool> ord = result["o"].as<vector<bool>>();
        vector<string> cns = result["n"].as<vector<string>>();
        bool verbose = result["v"].as<bool>();
        bool includeValues = result["i"].as<bool>();

        // check to make sure data has the same number of required arguments
        vector<AlgorithmOptions> algorithmOptions;
        vector<string> cNames;
        int counter = 0;
        for (const auto& r : result) {
            if (r.key() == "algorithm") counter++;
            else if (r.key() == "name") {
                cNames.push_back(r.value());
                counter--;
            }
            for (string s : AlgorithmOptionsStrings) {
                transform(s.begin(), s.end(), s.begin(), ::tolower);
                if (r.key() == s && r.value() == "true") {
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
        
        if (!(result.count("a") == result.count("l") && result.count("a") == algorithmOptions.size())) {
            throw std::invalid_argument("Number of provided algorithm(s), length(s) and algorithm option(s) arguments do not match!");
        }
        
        for (int i = 0; i < algo.size(); i++) {
            auto newAlgorithm = getAlgorithm(
                algo[i],
                len[i],
                cNames[i],
                verbose,
                includeValues
            );
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
            algorithms.push_back(newAlgorithm);
        }
    }

    return algorithms;
}

void processAlgorithms(const vector<BaseSort<unsigned int>*>& algorithms, const bool& json, const bool& verbose, const string& file) {
    string jsonResults = "{\"algorithms\": [";
    for (auto algorithm : algorithms) {
        if (json || !file.empty()) jsonResults += algorithm->runAndGetJSONSort() + ",";
        if (!(json || !file.empty())) algorithm->runAndPrintSort();
        if (verbose) cout << algorithm->getStringResult() << endl;
    }
    
    if (json || !file.empty()) {
        jsonResults.pop_back(); //remove extraneous comma
        jsonResults += "]}";
        if (json) cout << jsonResults << endl;
        if (!file.empty()) {
            std::ofstream outFile;
            outFile.open(file);
            outFile << jsonResults;
            outFile.close();
        }
        if (verbose && !file.empty()) cout << "Results written in JSON at: '" << file << "'" << endl;
    }
    cout << endl;
}

int runProgram(int argc, char *argv[]) {
    int return_code = 0;
    vector<BaseSort<unsigned int>*> algorithms;
    try {
        Options options = getOptions();
        auto result = options.parse(argc, argv);
        algorithms = parseAndGetAlgorithms(result, options);
        if (!algorithms.empty())
        processAlgorithms(
            algorithms,
            result["json"].as<bool>(),
            result["verbose"].as<bool>(),
            result["file"].as<string>()
        );
    } catch (int e_code) {
        cerr << "An error has occurred! Error number: " << e_code << endl;
        return_code = e_code;
    } catch (char e_string) {
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