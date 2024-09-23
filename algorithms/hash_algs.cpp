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
    string getName() { return hashName; }
    string getCanonicalName() { return this->canonicalName.empty() ? string("") : (string("<") + this->canonicalName + string("> ")); }
    string getAlgorithmOption() { return HashTableOptionsStrings[hashTableOptions]; }
    // T *getValuesPriorToSort() const { return this->valuesPriorToSort; }
    // T *returnValues() const { return this->arr; }
    string getStringResult();
    bool isVerbose() { return this->verbose; }
    bool valuesIncluded() { return this->verbose; }
    // FIXME: REMOVE? 
    // string getValuesRange();
    string getJSONResult();

    //setters
    void setCanonicalName(const string &c) { this->canonicalName = c; }
    void setVerbose(const bool &v = true) { this->verbose = v; }
    void setValuesIncluded(const bool &i = true) { this->verbose = i; }

    //other class members
    void loadRandomValues();
    void loadRepeatedValues();
    void loadChunkValues();
    void loadReversedValues();
    void loadOrderedValues();
    void printValues() const;
    // void verifySort() const;
    void printSortToFile(const string &filePath, const bool &append = true) const;
    void runAndCaptureSort();
    void runAndPrintSort();
    void runAndPrintFileSort(const string &filePath, const bool &append = true);
    string runAndGetJSONSort();
    string getDummyPerfData(bool JSON = false);

    virtual void runSort() = 0; // Pure virtual function.
    // It makes the class **abstract**.  In other words,
    // nothing can instantiate an object of this class.

    protected:
      string hashName;
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







}

# endif

// int main() {
//   cout << "RUNNING HASH TABLES..." << endl;
//   return 0;
// }