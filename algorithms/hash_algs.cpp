 /**
 * @authors Cooper Maitoza, Brad Peterson Ph.D., et al.
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
// #include "../dependencies/Perf.hpp"

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


    sort algo example: 
      else if (algorithmName == "heap") return new Sorting::Heap<unsigned int>(length, canonicalName, verbose, includeValues, includePerf);

*/

namespace AlgoGauge {

  template <typename T, typename U>
  class ClosedHashTable {
  public:

    ClosedHashTable(
      const int     capacity, 
      const string  probing_Type, 
      const int     hash_table_fullness, 
      const int     CRUDTestAmount, 
      const bool    verbose = false, 
      const string  includePerf = "sample"
    );

    ~ClosedHashTable();

    // Public Funtions
    void    	crudOperation(const int);

    // Getters
    int     	getHashTableFullness();
    int     	getCapacity();
    string  	getProbingType();
    float   	getAmountFilled();
    int     	getCRUDTestAmount();
    string  	getPerfOption();
    string  	getDummyPerfData(bool JSON);


  private:
    // Hash Table methods
    void    	create(const T& key, const U& value);
    U       	retrieve(const T& key) const;
    void    	destroy(const T& key);

    // Testing methods
    void    	fillHashTable();
    void    	loadValues(const int amount);
    void    	destroyValues(const int amount, const bool onlyExist);
    void    	lookupValues(const int amount, const bool onlyExist);
    void    	loadPerf();

    // Private data members
    int*        statusArray = nullptr;
    pair<T, U>* kvArray = nullptr;	

    bool        verbose;
    string      includePerf;
    int         capacity = 10; // array size
    string      probing_type = "linear";
    int         hash_table_fullness = 0; // default hash table fullness.
    int         amountFilled = 0;
    int         CRUDTestAmount;
  };


  // second Closed Hash Table Constructor Method
  template <typename T, typename U>
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity, const string probing_Type, const int hash_table_fullness, const int CRUDTestAmount, const bool verbose, const string includePerf) {
    this->probing_type = probing_Type;
    this->hash_table_fullness = hash_table_fullness;
    this->amountFilled = hash_table_fullness;
    this->capacity = capacity;
    this->CRUDTestAmount = CRUDTestAmount;
    this->verbose = verbose;
    this->includePerf = includePerf;
    this->loadPerf(); // initilize perf setup
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


  // Returns the privite perf datamember.
  template <typename T, typename U>
  string ClosedHashTable<T, U>::getPerfOption() {
    return this->includePerf;
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


  //FIXME: Initializes perf data. Sets which hardware and software events to collect from. (Remove some of these if your system has too little performance CPU registers)
  template<typename T, typename U>
  void ClosedHashTable<T, U>::loadPerf() {
  #ifdef linux
    //CPU Hardware Events
   
  #endif
  }


  template<typename T, typename U>
  string ClosedHashTable<T, U>::getDummyPerfData(bool JSON) {
    string allEvents[17][2] = { //create the static dummy data
      {"PERF NOTE", "\"INCLUDED DATA IS DUMMY DATA!\""},
      {"cpu cycles", "5432316545"},
      {"bus cycles", "1561896"},
      {"cpu instructions", "5151651"},
      {"cache references", "198456156"},
      {"cache misses", "198415652"},
      {"branch predictions", "51894156489"},
      {"retired branch instructions", "98528445"},
      {"branch misses", "7415437"},
      {"total page faults", "574"},
      {"minor page faults", "242"},
      {"major page faults", "473"},
      {"context switches", "4"},
      {"L1 data cache read accesses", "369545"},
      {"L1 instruction cache read accesses", "841616"},
      {"L1 data cache prefetch accesses", "261485"},
      {"L1 instruction cache prefetch accesses", "2117485"}
    };

    string returnString; //create the string that will be returned
    int size = sizeof(allEvents)/sizeof(*allEvents); //get the size of the array
    //loop through the dummy data and format it according to if it's JSON or not
    if (JSON) {
      returnString += "{";
      for (int i = 0; i < size; i++) {
        returnString += "\"" + allEvents[i][0];
        returnString += "\": " + allEvents[i][1];
        if (i + 1 != size) returnString += ", ";
      }
      returnString += "}";
    } else {
      for (int i = 0; i < size; i++) {
        returnString += allEvents[i][0];
        returnString += ": " + allEvents[i][1];
        if (i + 1 != size) returnString += "; ";
      }
    }
    return returnString;
  }


  // Testing function to run hash tables with parameters.
  template <typename T, typename U>
  string runHash(ClosedHashTable<T, U> &&hashObj) {
    auto t1 = std::chrono::high_resolution_clock::now();

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

    string output;
    output += "{ \"algorithmName\": \"Closed Hash Tables\", ";
    output += "\"algorithmCapacity\": " + std::to_string(hashObj.getCapacity()) + ", ";
    output += "\"language\": \"c++\", "; 
    output += "\"probingType\": \"" + hashObj.getProbingType() + "\", ";
    output += "\"algorithmRunTime_ms\": " + std::to_string(fp_ms.count()) + ", ";
    output += "\"density\": " + std::to_string(hashObj.getAmountFilled()) + ", ";
    output += "\"number\": " + std::to_string(hashObj.getCRUDTestAmount()) + ", ";
    output += "\"perfData\": "; //always return the perf data object regardless. If no perf data, perf object will just be empty

    if (hashObj.getPerfOption() == "sample") {
        output += hashObj.getDummyPerfData(true);
    }
  #ifdef linux
    else if (hashObj.getPerfOption() == "true") {
        output += hashObj.perf.getBufferJSON();
    }
  #endif
    else {
      output += "{}";
    }

    return output + "}";
  }

}

# endif

// int main() {
  // cout << runHash(AlgoGauge::ClosedHashTable<string, string> (100000, "linear", 50, 1000, true, "true")) << endl;


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
