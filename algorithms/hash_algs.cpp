 /**
 * @authors Brad Peterson Ph.D., Cooper Maitoza, et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <memory>
#include <fstream>
#include <ostream>
#include <string>
#include <chrono>
#include "RandomNum.hpp"
#include "../dependencies/Perf.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

#ifndef ALGOGAUGE_HASH_TABLE_CPP
#define ALGOGAUGE_HASH_TABLE_CPP

//FIXME: Edit these options
enum HashTableOptions {
  none = 0,
  randomSet,
  repeatedSet,
  chunkSet,
  reversedSet,
  orderedSet
};

//FIXME: Edit these options
static const std::string HashTableOptionsStrings[] = {
  "None Set",
  "Random",
  "Repeated",
  "Chunks",
  "Reversed",
  "Ordered"
};

namespace AlgoGauge {
  template<typename T>
  class BaseHashTable {

  public:
    //constructors and destructors
    BaseHashTable(
      const string &hashTableName,
      const unsigned int capacity,
      const string &canonicalName = "",
      const bool &verbose = false,
      const bool &includeValues = false,
      const string &includePerf = "false"
    );
    virtual ~BaseHashTable();

    //getters
    string getName() { return hashTableName; }
    string getCanonicalName() { return this->canonicalName.empty() ? string("") : (string("<") + this->canonicalName + string("> ")); }
    string getHashTableOptions() { return HashTableOptionsStrings[hashTableOptions]; }
    string getStringResult(); // TODO: ADD METHOD
    bool isVerbose() { return this->verbose; }
    bool valuesIncluded() { return this->verbose; }
    string getJSONResult(); // TODO: ADD METHOD

    //setters
    void setCanonicalName(const string &c) { this->canonicalName = c; }
    void setVerbose(const bool &v = true) { this->verbose = v; }
    void setValuesIncluded(const bool &i = true) { this->verbose = i; }

    //other class members
    void loadRandomHashValues(); // TODO: ADD METHOD
    void printValues() const; // TODO: ADD METHOD
    void printSortToFile(const string &filePath, const bool &append = true) const;
    void runAndCaptureSort(); // TODO: ADD METHOD
    void runAndPrintSort(); // TODO: ADD METHOD
    void runAndPrintFileSort(const string &filePath, const bool &append = true);
    string runAndGetJSONSort(); // TODO: ADD METHOD
    string getDummyPerfData(bool JSON = false); // TODO: ADD METHOD

    virtual void runHashTable() = 0; // Pure virtual function.
    // It makes the class **abstract**.  In other words,
    // nothing can instantiate an object of this class.

    protected:
      string hashTableName;
      HashTableOptions hashTableOptions;
      T *arr{nullptr};
      unsigned int capacity{0};
      string canonicalName; // Used to store a canonical name to the instantiated object.
      // This is different from sortName as this can be whatever the user defines.
      bool verbose;
      bool includeValues;
      string includePerf;
#ifdef linux
      Perf::Perf perf;
#endif

    private:
      // unsigned int *valuesPriorToSort; //Stores the values prior to sorting
      std::chrono::duration<double, std::milli> executionTime;
      void loadPerf();
  };


  template<typename T>
  BaseHashTable<T>::BaseHashTable(
    const string &hashTableName,
    const unsigned int capacity,
    const string &canonicalName,
    const bool &verbose,
    const bool &includeValues,
    const string &includePerf
  ) {
    this->hashTableName = hashTableName;
    if (capacity > 0 && capacity < UINT32_MAX) this->capacity = capacity;
    else throw std::invalid_argument("Length must be greater than 0 and less than " + std::to_string(UINT32_MAX));
    this->arr = new T[capacity];
    // this->valuesPriorToSort = new T[capacity];
    this->canonicalName = canonicalName;
    this->hashTableOptions = HashTableOptions::none;
    this->executionTime = (std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now());
    this->verbose = verbose;
    this->includeValues = includeValues;
    this->includePerf = includePerf;
    this->loadPerf();
  }

  template<typename T>
  BaseHashTable<T>::~BaseHashTable() {
    delete[] arr;
  }






  template<typename T>
  void BaseHashTable<T>::loadPerf() {
#ifdef linux
    //CPU Hardware Events
    perf.addNewPerfEvent("cpu cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    perf.addNewPerfEvent("bus cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES);
    perf.addNewPerfEvent("cpu instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    perf.addNewPerfEvent("cache references", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES);
    perf.addNewPerfEvent("cache misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);
    perf.addNewPerfEvent("branch predictions", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_BPU);
    perf.addNewPerfEvent("retired branch instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS);
    perf.addNewPerfEvent("branch misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES);

    //CPU Software (OS) Events
    perf.addNewPerfEvent("total page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS);
    perf.addNewPerfEvent("minor page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MIN);
    perf.addNewPerfEvent("major page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MAJ);
    perf.addNewPerfEvent("context switches", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES);

    //CPU Cache Events
    perf.addNewPerfEvent(
        "L1 data cache read accesses",
        PERF_TYPE_HW_CACHE,
        (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
    );
    perf.addNewPerfEvent(
        "L1 instruction cache read accesses",
        PERF_TYPE_HW_CACHE,
        (PERF_COUNT_HW_CACHE_L1I) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
    );
    perf.addNewPerfEvent(
        "L1 data cache prefetch accesses",
        PERF_TYPE_HW_CACHE,
        (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
    );
    perf.addNewPerfEvent(
        "L1 instruction cache prefetch accesses",
        PERF_TYPE_HW_CACHE,
        (PERF_COUNT_HW_CACHE_L1I) | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
    );
#endif
  }

}

# endif

int main() {
  cout << "RUNNING HASH TABLES..." << endl;
  return 0;
}