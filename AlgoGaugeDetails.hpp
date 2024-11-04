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


struct HashTableSettings{
	std::string Type = "Closed";
	int Capacity = 10;
	int Number = 100;
	std::string Probe = "linear";
	int Load = 0;
};

struct SortingAlgorithmSettings {
	std::string Name = "None";
	std::string Algorithm = "built_in";
	std::string ArrayStrategyString = "default";
	AlgorithmOptions ArrayStrategy = none;
    int ArrayLength = 100;
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
	HashTableSettings SelectedHashTables;
};


} // namespace ns
#endif // ALGO_GAUGE_DETAILS_HPP