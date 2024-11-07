#ifndef ALGO_GAUGE_DETAILS_HPP
#define ALGO_GAUGE_DETAILS_HPP

#include <string>
#include <vector>
namespace AlgoGauge {

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

enum PERF{
	perfOFF,
	perfON,
	sample
};


struct HashTableSettings{
	std::string Type = "Closed";
	int Capacity = 10;
	int Number = 100;
	std::string Probe = "linear";
	int Load = 0;
	std::string Name = "";
};

struct SortingAlgorithmSettings {
	std::string Name = "";
	std::string Algorithm = "built_in";
	std::string ArrayStrategyString = "default";
	AlgorithmOptions ArrayStrategy = none;
    int ArrayLength = 100;
	std::string Language = "c++";
};


struct AlgoGaugeDetails{
	bool Verbose = false;
	bool Output = false;
	PERF Perf = perfOFF;
    bool Json = false;
    std::string FileWritePath = "";
	bool Unique = false;
	std::vector<SortingAlgorithmSettings> SelectedSortingAlgorithms;
	std::vector<HashTableSettings> SelectedHashTables;
};


} // namespace ns
#endif // ALGO_GAUGE_DETAILS_HPP