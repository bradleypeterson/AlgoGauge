 /**
 * @authors Brad Peterson Ph.D., Cooper Maitoza, et al.
 * @copyright Weber State University
 */
#ifndef ALGOGAUGE_HASH_TABLE_CPP
#define ALGOGAUGE_HASH_TABLE_CPP

#include <functional>
#include <iostream>
#include <memory>
#include <fstream>
#include <ostream>
#include <string>
#include <chrono>
#include <utility>
#include "RandomNum.hpp"
#include "../dependencies/Perf.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;


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
    ClosedHashTable(const int capacity, const string probing_Type, const int hash_table_fullness, const int CRUDTestAmount);
    ~ClosedHashTable();

    void crudOperation(const int);

    // Getters
    int getHashTableFullness();
    int getCapacity();
    string getProbingType();
    float getAmountFilled();
    int getCRUDTestAmount();

  private:
    // Hash Table methods
    void create(const T& key, const U& value);
    U retrieve(const T& key) const;
    void destroy(const T& key);

    // Testing methods
    void fillHashTable();
    void loadValues(const int amount);
    void destroyValues(const int amount, const bool onlyExist);
    void lookupValues(const int amount, const bool onlyExist);

    // Private 
    int* statusArray = nullptr;
    pair<T, U>* kvArray = nullptr;	
    int capacity = 10; // array size
    string probing_type = "linear";
    int hash_table_fullness = 0; // default hash table fullness.
    int amountFilled = 0;
    int CRUDTestAmount;
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
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity, const string probing_Type, const int hash_table_fullness, const int CRUDTestAmount) {
    this->probing_type = probing_Type;
    this->hash_table_fullness = hash_table_fullness;
    this->amountFilled = hash_table_fullness;
    this->capacity = capacity;
    this->CRUDTestAmount = CRUDTestAmount;
    this->statusArray = new int[capacity];
    for (int i = 0; i < capacity; i++) {
      statusArray[i] = 0;
    }
    this->kvArray = new pair<T, U>[capacity];

    this->fillHashTable();
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
    // cout << "NO VALUE " << key << " FOUND." << endl;
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
        U temp;
        return temp;
        // throw std::logic_error("that key wasn't found");
      }
    }
    U temp;
    return temp;
    // throw std::logic_error("that key wasn't found");
  }

  // filling up hash table
  template <typename T, typename U>
  void ClosedHashTable<T, U>::fillHashTable(){
    if (this->hash_table_fullness == 0) { // stay empty
      return;
    }
    int numToFill = capacity * ((1.0 * this->hash_table_fullness) / (100.0)); // 10 * (45/100) = 5
    for (int i = 0; i < numToFill; i++) {
      this->create(std::to_string(i), "value " + std::to_string(i));
    }
  }

  // load values method
  template <typename T, typename U>
  void ClosedHashTable<T, U>::loadValues(const int amount){
    // if (amount > this->capacity) {
    //   throw std::logic_error(std::to_string(amount) + " will go over " + std::to_string(this->capacity) + " capacity.");
    // }
    for (int i = this->amountFilled; i < (this->amountFilled + amount); i++) {
      this->create(std::to_string(i), "value " + std::to_string(i));
    }
    this->amountFilled += amount;
  }

  // destroy values for a given amount, has an option for only existing values in hash
  // table.
  template <typename T, typename U>
  void ClosedHashTable<T, U>::destroyValues(const int amount, const bool onlyExist) {
    // if (amount > this->capacity) {
    //   throw std::logic_error(std::to_string(amount) + " will go over " + std::to_string(this->capacity) + " capacity.");
    // }
    for (int i = (this->amountFilled - amount); i < this->amountFilled; i++) {
      this->destroy(std::to_string(i));
    }
    this->amountFilled -= amount;
  }

  // looks up values in hash table for a certin amount of times. Has the option to only
  // look up existing values.
  template <typename T, typename U>
  void ClosedHashTable<T, U>::lookupValues(const int amount, const bool onlyExist) {
    // if (amount > this->capacity) {
    //   throw std::logic_error(std::to_string(amount) + " will go over " + std::to_string(this->capacity) + " capacity.");
    // }
    for (int i = 0; i < this->amountFilled; i++) {
      this->retrieve(std::to_string(i));
    }
  }

  // This method creates a bunch of test CRUD operations for testing.
  // get's a random CRUD operation a 100 times (loadValues, lookupValues, destroyValues)
  // runs the CRUD operation for an amount of testOperationAmount (loadValues(testOperationAmount))
  template <typename T, typename U>
  void ClosedHashTable<T, U>::crudOperation(const int testOperationAmount) {
    this->loadValues(testOperationAmount);
    this->lookupValues(testOperationAmount, false);
    this->destroyValues(testOperationAmount, false);
  }

  // returns a float of how full the hash table is.
  template <typename T, typename U>
  float ClosedHashTable<T, U>::getAmountFilled() {
    float count = 0.0;
    for (int i = 0; i < this->capacity; i++) {
      if (this->statusArray[i] == 1) 
        count++;
    }
    return (count / (capacity * 1.0)) * 100;
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

  template <typename T, typename U>
  int ClosedHashTable<T , U>::getCRUDTestAmount(){
    return this->CRUDTestAmount;
  }

  // Testing function to run hash tables with parameters.
  template <typename T, typename U>
  void runHash(ClosedHashTable<T, U> &&hashObj) {
    cout << "Running closed hash table:" << endl;
    cout << "Capacity: " << hashObj.getCapacity() << endl;
    cout << "Probing Type: " << hashObj.getProbingType() << endl;
    cout << "Hash Table Fullness: " << hashObj.getHashTableFullness() << endl;
    cout << "Filling up hash table with values to " << hashObj.getHashTableFullness() << " percent." << endl;
    cout << "Testing CRUD operations with " << hashObj.getCRUDTestAmount() << " values." << endl;
    // Filling up hash table is outside of the timmer.
    auto t1 = std::chrono::high_resolution_clock::now();

    //runing adding and deleting stuff
    hashObj.crudOperation(hashObj.getCRUDTestAmount());

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    cout << "Hash Table CRUD operations completed in " << fp_ms.count() << " milliseconds" << endl;
    cout << endl;
  }
}


# endif

// int main() {
  // cout << "RUNNING HASH TABLES..." << endl << endl;

  // runHash(AlgoGauge::ClosedHashTable<string, string> (100000, "linear", 50, 1000));
  // runHash(AlgoGauge::ClosedHashTable<string, string> (10000, "linear", 0, 5000));
  // runHash(AlgoGauge::ClosedHashTable<string, string> (10000, "linear", 0, 5000));

  // cout << "Testing CRUD Operation Method..." << endl;
  // hashtable1.crudOperation();

  // cout << "the hash table is " << hashtable1.getAmountFilled() << "\% filled" << endl; 
  // hashtable1.loadValues(20);
  // cout << "the hash table is " << hashtable1.getAmountFilled() << "\% filled" << endl; 
  // hashtable1.destroyValues(60, false); 
  // cout << "the hash table is " << hashtable1.getAmountFilled() << "\% filled" << endl; 
  // cout << "looking up 60 values..." << endl; 
  // hashtable1.lookupValues(60, false); 
  // cout << "destroying past values..." << endl;
  // hashtable1.destroyValues(60, false); 
  // cout << "the hash table is " << hashtable1.getAmountFilled() << "\% filled" << endl; 
  // cout << endl;

  // cout << "testing size method." << endl;
  // AlgoGauge::ClosedHashTable<string, string> hashtable2(100, "linear", 50);
  // cout << "the hash table is " << hashtable2.getAmountFilled() << "\% filled" << endl; 
  // cout << endl;

  // cout << "testing empty fill hash table" << endl;
  // runHash(AlgoGauge::ClosedHashTable<string, string>(100));

  // cout << "testing 50 percent fill hash table" << endl;
  // runHash(AlgoGauge::ClosedHashTable<string, string>(100, "linear", 50));

  // cout << "testing 70 percent fill hash table" << endl;
  // runHash(AlgoGauge::ClosedHashTable<string, string>(100, "linear", 70));

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

  // return 0;
// }
