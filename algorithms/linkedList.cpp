 /**
 * @authors Brad Peterson Ph.D., Brody G, et al.
 * @copyright Weber State University
 */

#ifndef ALGOGAUGE_LINKED_LIST_CPP
#define ALGOGAUGE_LINKED_LIST_CPP

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

//Node
struct Node {
    int data =42;
    Node* link;
    Node(int data): data(data) {}
};

struct LinkedList {

    Node* head;
    Node* tail;
    int count;
    int number;
    

    LinkedList(): head(nullptr), tail(nullptr),count(0){}
    //create linked list of given size
    LinkedList(int size, int number) : head(nullptr), tail(nullptr), count(0) {
        for (int i = 0; i < size; i++) {
            push_back(number);
        }
        this->number = number;
    }

    //print list function 
    string print_List(){
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->link;
        }
        cout << "\n" << endl;
    }

    //push_back 
    //push node to back of linked list
    void push_back(int data) {
        Node* temp = new Node(data);

        if (!head) { 
            // The list is empty
            head = temp;
            tail = temp;
        } 
        else {
            //ink the new node to the end of the list
            tail->link = temp;
            tail = temp;
        }
        count++;
    }

    //push_front
    //pushed node to front of list
    void push_front(int data) {
        Node* temp = new Node(data);
        if (!head) {      
            // The list is empty
            head = temp;
            tail = temp;
        } else {
            // General case: insert at the front
            temp->link = head;
            head = temp;
        }
        count++;
    }

    //pop_back
    //removes last node in linked list 
    void pop_back() {
        // Error scenario: List is empty
        if (!head) {
            std::cerr << "The list is empty!" << std::endl;
            return;
        }
        // Specific scenario: Only one node in the list
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            count--;
            return;
        }
        // General case: More than one node in the list
        Node* temp = head;
        while (temp->link != tail) {
            temp = temp->link;
        }

        // Delete the last node and update tail
        delete tail;
        tail = temp;
        tail->link = nullptr;
        count--;
    }

    //pop_front
    //removes first node in linked list 
    void pop_front() {
        //List is empty
        if (!head) {
            std::cout << "The list is empty!" << std::endl;
            return;
        }
        //Only one node in the list
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            count--;
            return;
        }
        //More than one node in the list
        Node* temp = head;
        head = head->link;
        delete temp;
        count--;
    }


};

void LinkedListPushBackTime(LinkedList &list){
    for(int i =0; i < list.number; i++){
        list.push_back(42);
    }
}

void LinkedListPopBackTime(LinkedList &list) {
    for (int i = 0; i < list.number; i++) {
        list.pop_back();
    }
}

void LinkedListPopFrontTime(LinkedList &list) {
    for (int i = 0; i < list.number; i++) {
        list.pop_front();
    }
}

void LinkedListPushFrontTime(LinkedList &list){
    for(int i=0; i < list.number; i++){
        list.push_front(42);
    }
}

//takes linked list size, number of nodes added/deleted, function name, perf, verbose, includeValues
//function names: push_front/back , pop_front/back, pushpop_front/back
std::string LinkedListPerformanceTest(int size, int number, string function, string perf, bool verbose, bool includeValues){
    string output = "";
    PerfEvent perfObject;
    string perfObjectString = "{}";

    if(size < number){

        if(verbose) {
            std::cerr << "Error: Cannot delete (" << size << ") from a list size of (" << number << ")." << endl;
        }

        output = "{\"ERROR\":\"List size must be larger than number removed or added\"}";
    }
    LinkedList list(size, number);
    
    if(verbose){
        cout << "List has been created.\nBegining timer and Perf" << endl;
    }
    
    if(includeValues){
        list.print_List();
    }

    //start time
    auto sTime = std::chrono::high_resolution_clock::now();

    //start perf
    perfObject.startCounters();

    //function check

    if(function == "pop_back"){
        LinkedListPopBackTime(list);
    }
    else if(function == "pop_front"){
        LinkedListPopFrontTime(list);
    }
    else if(function == "push_front"){
        LinkedListPushFrontTime(list);
    }
    else if(function == "push_back"){
        LinkedListPushBackTime(list);
    }
    else if(function == "pushpop_front"){
        LinkedListPushFrontTime(list);
        LinkedListPopFrontTime(list);
    }
    else if(function == "pushpop_back"){
        LinkedListPushBackTime(list);
        LinkedListPopBackTime(list);
    }

    //end time
    auto eTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

    //end perf
    perfObject.stopCounters();

 
    if (verbose){
        cout << "Timer and Perf stopped." << endl;
    } 

    if (includeValues){
        list.print_List();
    }

    //create return     
    output = "{";
    output += R"("algorithmName": ")" + function + "\",";
    output += R"("listSize": )" + std::to_string(size) + ","; 
    output += R"("number": )" + std::to_string(number) + ",";
    output += R"("algorithmRunTime_ms": )" + std::to_string(fp_ms.count());  

    std::transform(perf.begin(), perf.end(), perf.begin(), ::tolower);

    if(perf == "true"){
        perfObjectString = perfObject.getPerfJSONString();
    }  

    if (perf =="sample"){
        perfObjectString = perfObject.getPerfJSONStringDummy();
    }
    
    output += ", \"perfData\": ";  
    output += perfObjectString + "}";

    if (verbose){
        cout << output << "\n" << endl;
    }

    return output;

}
#endif

//int main() {

    //performance tests with verbose
    //LinkedListPerformanceTest(100000, 4002, "push_back", "true", true, false);
    //LinkedListPerformanceTest(100000, 4002, "pushpop_back", "true", true, false);

    // Invalid case
    //LinkedListPerformanceTest(100000, 4002, "invalid", "true", true, false);

    // Include values 
    //LinkedListPerformanceTest(100, 200, "push_back", "true", true, true);

//    return 0;
//}