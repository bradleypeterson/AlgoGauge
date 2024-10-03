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
#include <utility>
// #include "RandomNum.hpp"
#include "../dependencies/Perf.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;

#ifndef ALGOGAUGE_HASH_TABLE_CPP
#define ALGOGAUGE_HASH_TABLE_CPP

/*
  == layout == 
  two classes:
    closed hash tables:
      
    open hash tables
  
  testing functions:
    run hash table:
       input's size (int 100 to 1000000)
       probing type (string = "linear", "quadratic")
       hash table fullness (int = 30 to 99)
       CRUD operations test (existing data = true, either or = false)
       load test (bool)
       worst probe count or time to complete (string)
    
    ex. runHashTable(ClosedHashTable<unsigned int, >(<capacity>, 1000000, "linear", 30));
    ex. runHashTable(ClosedHashTable(10, 1000, "quadratic", 99));
*/

namespace AlgoGauge {

  template <typename T, typename U>
  class ClosedHashTable {
  public:
    ClosedHashTable(const int capacity);
    ClosedHashTable(const int capacity, const string probing_Type, const int hash_table_fullness);
    ~ClosedHashTable();
    void create(const T& key, const U& value);
    U retrieve(const T& key) const;
    void destroy(const T& key);
    int getHashTableFullness();
  private:
    int* statusArray = nullptr;
    pair<T, U>* kvArray = nullptr;	
    int capacity = 10; // array size
    string probing_type = "";
    int hash_table_fullness = 0; // default hash table fullness.
  };

  // Closed Hash Table Constructor Method
  template <typename T, typename U>
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity) {
    this->capacity = capacity;
    this->statusArray = new int[capacity];
    for (int i = 0; i < capacity; i++) {
      statusArray[i] = 0;
    }
    this->kvArray = new pair<T, U>[capacity];
  }

  template <typename T, typename U>
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity, const string probing_Type, const int hash_table_fullness) {
    this->probing_type = probing_Type;
    this->hash_table_fullness = hash_table_fullness;
    this->capacity = capacity;
    this->statusArray = new int[capacity];
    for (int i = 0; i < capacity; i++) {
      statusArray[i] = 0;
    }
    this->kvArray = new pair<T, U>[capacity];
  }

  // Closed Hash Table destructor Method
  template <typename T, typename U>
  ClosedHashTable<T, U>::~ClosedHashTable() {
    delete[] this->statusArray;
    delete[] this->kvArray;
  }

  // Closed Hash Table create Method
  template <typename T, typename U>
  void ClosedHashTable<T, U>::create(const T& key, const U& value) {
    // Step 1, Hash the key, mod by capacity. Obtain an index
    // While loop as long as you haven't iterated capacity times
      // Step 2, Go to the status array at that index.
      // if the status array is 0 or -1, write the pair at this index
      // Else if the status array is 1, linear probe to the next index
    auto hashedIndex = std::hash<T>(key) % capacity;
    int counter = 0;
    while (counter < capacity) {
    // Assume the while loop was coded...
      if (this->satusArray[hashedIndex] == -1 || statusArray[hashedIndex] == 0){
        // Use this slot
        kvArray[hashedIndex] = pair<T, U>(key, value);
        statusArray[hashedIndex] = 1;
        return;
      } 
      else {
        counter++;
        hashedIndex = (hashedIndex + 1) % capacity; // wrap around
      }
    }
  }

  // Closed Hash Table retrive Method
  template <typename T, typename U>
  U ClosedHashTable<T, U>::retrieve(const T& key) const {
    auto hashedIndex = std::hash<T>(key) % capacity;
    int counter = 0;
    while (counter < capacity) {
      if (statusArray[hashedIndex] == 1) {
        if (kvArray[hashedIndex].first == key) {
          return kvArray[hashedIndex].second;
        } else {
          counter++;
          hashedIndex = (hashedIndex + 1) % capacity;
        }
      } else if (statusArray[hashedIndex] == -1) {
        counter++;
        hashedIndex = (hashedIndex + 1) % capacity;
      } else {
        throw 1;
      }
    }
    throw 1;
  }

  // getter for hash_table_fullness privite data member.
  template <typename T, typename U>
  int ClosedHashTable<T,U>::getHashTableFullness() {
    return this->hash_table_fullness;
  }



  template <typename T, typename U>
  void runHash(ClosedHashTable<T, U> &&hashObj) {
    cout << "Running closed hash table" << endl;
    // sortObj.loadRandomValues();
    cout << "Filling up hash table with values to " << hashObj.getHashTableFullness() << " percent." << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    // TODO: add function to create, look up, and delete values.
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    std::cout << "Sort completed in " << fp_ms.count() << " milliseconds" << endl;
  }
// END OF ALGOGUAGE
}


# endif

int main() {
  cout << "RUNNING HASH TABLES..." << endl;
  runHash(AlgoGauge::ClosedHashTable<string, string>(10));
  runHash(AlgoGauge::ClosedHashTable<string, string>(10, "linear", 50));
  return 0;
}