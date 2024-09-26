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

namespace AlgoGauge {
  template <typename T, typename U>
  class ClosedHashTable {
  public:
    ClosedHashTable(const int capacity);
    ~ClosedHashTable();
    void create(const T& key, const U& value);
    U retrieve(const T& key) const;
    void distroy(const T& key);
    bool exists(const T& key) const;
  private:
    int* statusArray = nullptr;
    pair<T, U>* kvArray = nullptr;	
    int capacity = 10;
  };

  template <typename T, typename U>
  ClosedHashTable<T, U>::ClosedHashTable(const int capacity) {
    this->capacity = capacity;
    this->statusArray = new int[capacity];
    for (int i = 0; i < capacity; i++) {
      statusArray[i] = 0;
    }
    this->kvArray = new pair<T, U>[capacity];
}
}

# endif

int main() {
  cout << "RUNNING HASH TABLES..." << endl;
  return 0;
}