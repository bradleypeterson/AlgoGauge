 /**
 * @authors Brad Peterson Ph.D., Brody G, et al.
 * @copyright Weber State University
 */

#ifndef ALGOGAUGE_ARRAYS_CPP
#define ALGOGAUGE_ARRAYS_CPP

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory>
#include <fstream>
#include <ostream>
#include <string>
#include <chrono>
#include <utility>
#include "RandomNum.hpp"
#include "../dependencies/PerfEvent.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;
using std::vector;

struct Array {

    int size;
    int capacity;
    int* data;

    Array(int initialCapacity = 10) : size(initialCapacity), capacity(initialCapacity) {
        data = new int[capacity];

        for (int i = 0; i < size; i++) {
            data[i] = 42;
        }
    }

    ~Array() {
        delete[] data;
    }

    //push new element to back of array 
    //array capacity doubles if size > capacity 
    void push_back(int num) {
        if (size >= capacity) {
            int newCapacity = capacity * 2;
            int* newData = new int[newCapacity];

            //copy old array to new one
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }

            //delete old 
            delete[] data;

            //point to new
            data = newData;
            capacity = newCapacity;
        }

        data[size] = num;
        size++;
    }

    //push new element to front of array 
    //array will double in size if size > capacity 
    void push_front(int num) {
        if (size >= capacity) {
            // Double the capacity and create new array
            int newCapacity = capacity * 2;
            int* newData = new int[newCapacity];

            // Insert the new element at the front
            newData[0] = num;

            // Copy existing elements to the new array, shifted by 1
            for (int i = 0; i < size; i++) {
                newData[i + 1] = data[i];
            }

            // Clean up the old array
            delete[] data;

            // Update 
            data = newData;
            capacity = newCapacity;
        } 
        else {
            // Shift elements for new element
            for (int i = size; i > 0; i--) {
                data[i] = data[i - 1];
            }

            // Insert the new element
            data[0] = num;
        }

        // Increment the size
        size++;
    }

    //remove front element in array 
    void pop_front() {
        if (size > 0) {
            for (int i = 0; i < size - 1; i++) {
                data[i] = data[i + 1];
            }
            size--;
        } 
        else {
            std::cerr << "Array is empty!" << endl;
        }
    }

    //remove last element in array. 
    void pop_back() {
        if (size > 0) {
            size--;
            
        } 
        else {
            std::cerr << "Array is empty!" << endl;
        }
    }

    //print array function
    void print() const {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};


// Performance functions
void ArrayPushBackTime(Array& array, int number) {
    for (int i = 0; i < number; i++) {
        array.push_back(42);
    }
}

void ArrayPopBackTime(Array& array, int number) {
    for (int i = 0; i < number; i++) {
        if (array.size > 0) {
            array.pop_back();
        } else {
            break; 
        }
    }
}


void ArrayPopFrontTime(Array& array, int number) {
    for (int i = 0; i < number; i++) {
        if (array.size > 0) {
            array.pop_front();
        } else {
            break; 
        }
    }
}

void ArrayPushFrontTime(Array& array, int number) {
    for (int i = 0; i < number; i++) {
        array.push_front(42);
    }
}

//Main Performance test function
//capacity, elements, function name, perf, verbose, include values 
//fuctions push_front/back,  pop_front/back,  pushpop_front/back
string ArrayPerformanceTest(int capacity, int number, string function, string perf, bool verbose, bool includeValues) {
    string output = "";
    PerfEvent perfObject;
    string perfObjectString = "{}";

    // Create array with given capacity
    Array array(capacity);

    if (verbose) {
        cout << "Array has been created, Beginning timer and Perf" << endl;
    }

    auto sTime = std::chrono::high_resolution_clock::now();
    perfObject.startCounters();

    if (function == "pop_back") {
        ArrayPopBackTime(array, number);
    } 
    else if (function == "pop_front") {
        ArrayPopFrontTime(array, number);
    } 
    else if (function == "push_front") {
        ArrayPushFrontTime(array, number);
    } 
    else if (function == "push_back") {
        ArrayPushBackTime(array, number);
    } 
    else if (function == "push_pop_front") {
        ArrayPushFrontTime(array, number);
        ArrayPopFrontTime(array, number);
    } 
    else if (function == "push_pop_back") {
        ArrayPushBackTime(array, number);
        ArrayPopBackTime(array, number);
    } 
    else {
        std::cerr << "Invalid function name given" << endl;
    }

    auto eTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;
    perfObject.stopCounters();

    if (includeValues) {
        array.print(); 
    }

    output = "{";
    output += R"("algorithmName": ")" + function + "\",";
    output += R"("nodeCount": )" + std::to_string(capacity) + ",";
    output += "\"type\": \"array\",";
    output += R"("number": )" + std::to_string(number) + ",";
    output += R"("algorithmRunTime_ms": )" + std::to_string(fp_ms.count());

    std::transform(perf.begin(), perf.end(), perf.begin(), ::tolower);

    if (perf == "true") {
        perfObjectString = perfObject.getPerfJSONString();
    }

    if (perf =="sample"){
        perfObjectString = perfObject.getPerfJSONStringDummy();
    }

    output += ", \"perfData\": ";
    output += perfObjectString + "}";

    if (verbose) {
        cout << output << "\n" << endl;
    }

    return output;
}

// int main() {
//     Array a(2);
//     int aSize = a.size;
//     cout << "Size should be 2 here: " << aSize << endl;
//     a.push_back(21); 
//     a.print(); // 42 42 21
//     aSize = a.size;
//     cout << "Size should be 3 here: " << aSize << endl;
//     a.push_back(42);
//     a.print(); //42 42 21 42
//     aSize = a.size;
//     cout << "Size should be 4 here: " << aSize << endl;
//     a.push_front(10);
//     a.print(); //10 42 42 21 42
//     aSize = a.size;
//     cout << "Size should be 5 here: " << aSize << endl;
//     a.pop_back();
//     a.print(); //10 42 42 21
//     aSize = a.size; 
//     cout << "Size should be 4 here: " << aSize << endl;
//     a.pop_front();
//     a.print(); // 42 42 21
//     aSize = a.size;
//     cout << "Size should be 3 here: " << aSize << endl;
//     for (int i = 4; i < 20; i ++) {
//         a.push_back(42);
//         aSize = a.size;
//         cout << "Size should be " << i <<" here: " << aSize << endl;
//     }
    
    //ArrayPerformanceTest(100000, 4002, "push_back", "true", true, false);
    //ArrayPerformanceTest(100000, 4002, "push_front", "true", true, false);

    //ArrayPerformanceTest(2, 5, "push_back", "true", true, true);
    //ArrayPerformanceTest(2, 5, "push_front", "true", true, true);

    //ArrayPerformanceTest(100000, 4002, "pop_back", "true", true, false);
    //ArrayPerformanceTest(100000, 90002, "pop_front", "true", true, false);

    //ArrayPerformanceTest(5, 3, "pop_back", "true", true, true);
    //ArrayPerformanceTest(5, 3, "pop_front", "true", true, true);

    //ArrayPerformanceTest(100000, 4002, "pushpop_front", "true", true, false);
    //ArrayPerformanceTest(100000, 4002, "pushpop_back", "true", true, false);

    //ArrayPerformanceTest(2, 5, "pushpop_front", "true", true, true);
    //ArrayPerformanceTest(10, 5, "pushpop_back", "true", true, true);

    //ArrayPerformanceTest(100000, 4002, "invalid", "true", true, false);

//     return 0;
// }

#endif