#ifndef ALGO_GAUGE_DETAILS_HPP
#define ALGO_GAUGE_DETAILS_HPP

#include <string>
#include <vector>
/**
 * @brief Used for determining valid Algorithm Options.
 * none is default, however, if set, should always throw an error.
 */
enum AlgorithmOptions {
	none = 0,
	randomSet,
	repeatedSet,
	chunkSet,
	reversedSet,
	orderedSet
};

struct HashTableSettings{

};

struct SortingAlgorithmSettings {
	std::string Name = "None";
	std::string Algorithm = "built_in";
	AlgorithmOptions ArrayStrategy = none;
    int ArrayCount = 100;
	std::string Language = "c++";
};


struct AlgoGaugeDetails{
	bool Verbose = false;
	bool Output = false;
	bool Perf = false;
	bool PerfFileWrite = false;
	bool PerfSample = false;
    bool Json = false;
    std::string FileWritePath = "";
	bool Unique = false;
	std::vector<SortingAlgorithmSettings> SelectedSortingAlgorithms;
	std::vector<HashTableSettings> SelectedHashTables;
};
#endif // ALGO_GAUGE_DETAILS_HPP
