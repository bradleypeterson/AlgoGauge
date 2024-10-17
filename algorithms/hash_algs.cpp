 /**
 * @authors Brad Peterson Ph.D., Cooper Maitoza, et al.
 * @copyright Weber State University
 */

#include <functional>
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

    // Hash Table methods
    void create(const T& key, const U& value);
    U retrieve(const T& key) const;
    void destroy(const T& key);

    // Testing methods
    void fillHashTable();

    // Getters
    int getHashTableFullness();
    int getCapacity();
    string getProbingType();
  private:
    int* statusArray = nullptr;
    pair<T, U>* kvArray = nullptr;	
    int capacity = 10; // array size
    string probing_type = "linear";
    int hash_table_fullness = 0; // default hash table fullness.
  };

  // first Closed Hash Table Constructor Method
  template <typename T, typename U>
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity) {
    this->capacity = capacity;
    this->statusArray = new int[capacity];
    for (int i = 0; i < capacity; i++) {
      statusArray[i] = 0;
    }
    this->kvArray = new pair<T, U>[capacity];
  }

  // second Closed Hash Table Constructor Method
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
    std::hash<T> hash;
    auto hashedIndex = hash(key) % capacity;
    int counter = 0;
    while (counter < capacity) {
      if (this->statusArray[hashedIndex] == -1 || statusArray[hashedIndex] == 0){
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

  // Closed Hash Table Destroy Method
  template <typename T, typename U>
  void ClosedHashTable<T, U>::destroy(const T& key){
    std::hash<T> hash;
    auto hashedIndex = hash(key) % capacity;
    int counter = 0;
    while (counter < capacity) {
      if (statusArray[hashedIndex] == 1) {
        if (kvArray[hashedIndex].first == key) {
          statusArray[hashedIndex] = -1;
          return;
        } else {
          counter++;
          hashedIndex = (hashedIndex + 1) % capacity;
        }
      } else {
        counter++;
        hashedIndex = (hashedIndex + 1) % capacity;
      }
    }
    cout << "NO VALUE " << key << " FOUND." << endl;
  }

  // Closed Hash Table retrive Method
  template <typename T, typename U>
  U ClosedHashTable<T, U>::retrieve(const T& key) const {
    std::hash<U> hash;
    auto hashedIndex = hash(key) % this->capacity;
    int counter = 0;
    while (counter < this->capacity) {
      if (this->statusArray[hashedIndex] == 1) {
        if (this->kvArray[hashedIndex].first == key) {
          return this->kvArray[hashedIndex].second;
        } 
        else {
          counter++;
          hashedIndex = (hashedIndex + 1) % capacity;
        }
      }
      else if (this->statusArray[hashedIndex] == -1) {
        counter++;
        hashedIndex = (hashedIndex + 1) % capacity;
      } 
      else {
        throw std::logic_error("that key wasn't found");
      }
    }
    throw std::logic_error("that key wasn't found");
  }

  // getter for hash_table_fullness privite data member.
  template <typename T, typename U>
  int ClosedHashTable<T,U>::getHashTableFullness() {
    return this->hash_table_fullness;
  }

  // getter for capacity privite data member.
  template <typename T, typename U>
  int ClosedHashTable<T, U>::getCapacity() {
    return this->capacity;
  }

  // getter for probing_type privite data member.
  template <typename T, typename U>
  string ClosedHashTable<T , U>::getProbingType(){
    return this->probing_type;
  }

  // TODO: filling up hash table
  template <typename T, typename U>
  void ClosedHashTable<T, U>::fillHashTable(){
    if (this->hash_table_fullness == 0) { // stay empty
      return;
    }
    int numToFill = capacity * ((1.0 * this->hash_table_fullness) / (1.0 * 100)); // 10 * (45/100) = 5
    for (int i = 0; i < numToFill; i++) {
      this->create(std::to_string(i), "value " + std::to_string(i));
    }
  }




  // Testing function to run hash tables with parameters.
  template <typename T, typename U>
  void runHash(ClosedHashTable<T, U> &&hashObj) {
    cout << "Running closed hash table:" << endl;
    cout << "Capacity: " << hashObj.getCapacity() << endl;
    cout << "Probing Type: " << hashObj.getProbingType() << endl;
    cout << "Hash Table Fullness: " << hashObj.getHashTableFullness() << endl;
    // sortObj.loadRandomValues();
    cout << "Filling up hash table with values to " << hashObj.getHashTableFullness() << " percent." << endl;
   // Filling up hash table is outside of the timmer.
    hashObj.fillHashTable();
    auto t1 = std::chrono::high_resolution_clock::now();
    // TODO: runing adding and deleting stuff
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    std::cout << "Sort completed in " << fp_ms.count() << " milliseconds" << endl;
  }
// END OF ALGOGUAGE
}


# endif

int main() {
  cout << "RUNNING HASH TABLES..." << endl;

  cout << "testing empty fill hash table" << endl;
  runHash(AlgoGauge::ClosedHashTable<string, string>(100));

  cout << "testing 50 percent fill hash table" << endl;
  runHash(AlgoGauge::ClosedHashTable<string, string>(100, "linear", 50));

  cout << "testing 70 percent fill hash table" << endl;
  runHash(AlgoGauge::ClosedHashTable<string, string>(100, "linear", 70));
  // hashTable.create("first", "cooper");
  // hashTable.create("second", "kade");
  // hashTable.create("third", "brody");
  // hashTable.create("fourth", "coleton");
  // cout << "Retrieve: " << hashTable.retrieve("second") << endl;
  // hashTable.destroy("second");
  // hashTable.create("second", "new value");
  // cout << "Retrive after Destroy: " << hashTable.retrieve("second") << endl;

  // runHash(AlgoGauge::ClosedHashTable<string, string>(10));
  // runHash(AlgoGauge::ClosedHashTable<string, string>(100, "linear", 50));

  return 0;
}
