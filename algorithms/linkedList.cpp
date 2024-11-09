 /**
 * @authors Brad Peterson Ph.D., Brody G, et al.
 * @copyright Weber State University
 */

#ifndef ALGOGAUGE_LINKED_LIST_CPP
#define ALGOGAUGE_LINKED_LIST_CPP

#include <iostream>
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

//node
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
            push_back(size);
        }
        this->number = number;
    }

    //print list 
    string print_List(){
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->link;
        }
        cout << "\n" << endl;
    }

    //push_back 
    void LinkedList::push_back(int data) {
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
    void LinkedList::push_front(int data) {
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
    void LinkedList::pop_back() {
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
    void LinkedList::pop_front() {
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

std::string LinkedListUltimateCoolfunction(int size, int number, string function, bool verbose, bool includeValues){
    string output = "";
    if(size < number){

        if(verbose) {
            std::cerr << "Error: Cannot delete (" << size << ") from a list size of (" << number << ")." << endl;
        }

        output = "{\"ERROR\":\"List size must be larger than number removed or added\"}";
    }
    LinkedList list(size, number);
    
    if(verbose){
        cout << "List has been created, Beginging timer and Perf";
    }

    //start time
    auto sTime = std::chrono::high_resolution_clock::now();

    //start perf
    //look at sort7  

    //if function == "push___x"
    if(function == "pop_back"){
        LinkedListPopBackTime(list);
    }
    else if(function == "pop_front"){
        LinkedListPopFrontTime(list);
    }
    else if(function == "push_back"){
        LinkedListPushBackTime(list);
    }
    else if(function == "push_back"){
        LinkedListPushFrontTime(list);
    }
    else if(function == "push_pop_front"){
        LinkedListPushFrontTime(list);
        LinkedListPopFrontTime(list);
    }
    else if(function == "push_pop_back"){
        LinkedListPushBackTime(list);
        LinkedListPopBackTime(list);
    }

    //end time
    auto eTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;
    //end perf

    //return json string 
    if (verbose){
            cout << "Timer and Perf stopped." << endl;
    } 

    if (includeValues){
        list.print_List();
    }
         
    string output = "{";
    output += R"("algorithmName": ")" + string("Push Pop Back") + "\",";
    output += R"("listSize": )" + std::to_string(size) + "\","; 
    output += R"("number": )" + std::to_string(number) + "\",";
    output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

    //add to perft  
    if (verbose){
        cout << output << "\n" << endl;
    }

    return output;

}
#endif