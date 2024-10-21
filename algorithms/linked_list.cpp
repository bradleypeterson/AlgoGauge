 /**
 * @authors Brad Peterson Ph.D., Brody G, et al.
 * @copyright Weber State University
 */

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

#ifndef ALGOGAUGE_LINKED_LIST_CPP
#define ALGOGAUGE_LINKED_LIST_CPP

namespace AlgoGauge {
     
    template <typename T>
    struct Node {
        T data{};
        Node<T>* link{nullptr};
        Node(const T& data, Node<T>* next = nullptr): data(data), link(next) {}
    };

    template <typename T>
    class LinkedList {
    public:
        LinkedList(int size, const T& value = T()) : head(nullptr), tail(nullptr), count(0) {
            for (int i = 0; i < size; i++) {
                push_back(value);
            }
        }
        LinkedList() : head(nullptr), tail(nullptr), count(0) {}
        //testers
        static void LinkedListPushBackTime(int listSize, int numAdded, const T& value = T());
        static void LinkedListPopBackTime(int listSize, int numDeleted);
        static void LinkedListPushFrontTime(int listSize, int numAdded, const T& value);
        static void LinkedListPopFrontTime(int listSize, int numDeleted);

        //
        void push_back(const T& data);
        void push_front(const T& data);
        void pop_back();
        void pop_front();
        void test_push_back();
        void print_List() const {
            Node<T>* current = head;
            while (current != nullptr) {
                cout << current->data << " ";
                current = current->link;
            }
            cout << "\n" << endl;
        }
    private:
        Node<T>* head;
        Node<T>* tail;
        int count;
    }; 

    //push back 
    template <typename T>
    void LinkedList<T>::push_back(const T& data) {
        Node<T>* temp = new Node<T>(data);
        // 1. error scenarios
            // None
        // 2. specific scenarios
        if ( !head ) { 
            // The list is empty
            temp->data = data;
            this->head = temp;
            this->tail = temp;
            this->count++;
            return;
        }
        // 3. General scenarios
        temp->data = data;
        this->tail->link = temp;
        this->tail = temp;
        count++;

    }

    //push_front
    template <typename T>
    void LinkedList<T>::push_front(const T& data){
        Node<T>* temp = new Node<T>(data);
        //1.error
        //2. specific scenarios 
        if(!head){      
            temp->data = data;
            this->head = temp;
            this->tail = temp;
            this->count++;
            return;
        }
        //3. General scenarios 
        temp->data = data;
        temp->link = this->head;
        this->head = temp;
        this->count++;
    }

    //pop_back
    template <typename T> 
    void LinkedList<T>::pop_back(){
        //1. error scenarios
        if(!this->head){
            cout << "The list is empty!" << endl;
            return;
        }
        //2. Specific scenarios
        //Get pointer to second to last node 
        if(head == tail){
            delete this->head;
            this->head = nullptr;
            this->tail = nullptr;
            this->count--;
            return;
        }

        //General scenarios 
        //get pointer to second to last node 
        auto temp = this->head;
        while (temp->link != this->tail){
            temp = temp->link;
        }
        //temp is at the second to last node now 
        delete this ->tail;
        this->tail = temp;
        this->tail->link = nullptr;
        this->count--;
    }
    
    //pop_front 
    template <typename T>
    void LinkedList<T>::pop_front(){
        //1. error scenarios 
        if (!this->head){
            cout << "The list is empty!" << endl;
            return;
        }
        //2. specific scenarios 
        if (head == tail) {
            //one node scenario 
            delete this->head;
            this->head = nullptr;
            this->tail = nullptr;
            this->count--;
            return;
        }
        //3. General Scenario
        Node<T>* temp = this->head;
        this->head = this->head->link;
        delete temp;
        count--;
    }

    void testLinkedList() {

        LinkedList<int> list;

        cout << "Testing push_back function\n" << endl;
        list.push_back(10);
        list.push_back(20);
        list.push_back(30);
        list.print_List();
        cout << "Now testing push_front\n list should display 10 20 30 40 50\n";
        list.push_front(40);
        list.push_front(50);
        list.push_front(60);
        list.print_List();
        cout << "Now testing pop_front\n list should display 10 20 30\n";
        list.pop_front();
        list.pop_front();
        list.pop_front();
        list.print_List();
        cout << "Now testing pop_back\n list should display 10\n";
        list.pop_back();
        list.pop_back();
        list.print_List();
    }

    //testing functions 

    //POP BACK
    //amount of time to pop_back "numDeleted" to a linked list of "listSize" 
    template<typename T>
    void LinkedList<T>::LinkedListPopBackTime(int listSize, int numDeleted) {
    
        if (numDeleted > listSize) {
            cout << "Cannot delete (" << numDeleted << ") from a list size of (" << listSize << ")." << endl;
            return;
        }

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(T());  
        }

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numDeleted; i++) {
            list.pop_back();
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        cout << "POP_BACK: Removing " << numDeleted << " nodes from a Linked List of size " << listSize << " took " << fp_ms.count() << " milliseconds." << endl;
    }

    //POP FRONT 
    //amount of time to pop_back "numDeleted" to a linked list of "listSize" 
    template<typename T>
    void LinkedList<T>::LinkedListPopFrontTime(int listSize, int numDeleted) {
    
        if (numDeleted > listSize) {
            cout << "POP FRONT: Cannot delete (" << numDeleted << ") from a list size of (" << listSize << ")." << endl;
            return;
        }
        
        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(T());  
        }

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numDeleted; i++) {
            list.pop_front();
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;
        
        cout << "POP_FRONT: Removing " << numDeleted << " nodes from a Linked List of size " << listSize << " took " << fp_ms.count() << " milliseconds." << endl;
    }


    //PUSH BACK
    //amount of time to push_back "numAdded" to a linked list of "listSize"
    template<typename T>
    void LinkedList<T>::LinkedListPushBackTime(int listSize, int numAdded, const T& value){
        
        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
                list.push_back(value);
        }

        auto sTime= std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
                list.push_back(value);
        }
        auto eTime= std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;
        cout << "PUSH_BACK: Adding in " << numAdded << " nodes to a Linked List of size " << listSize << " took " <<  fp_ms.count() << " milliseconds" << endl;
    }

    //PUSH FRONT
    //amount of time to push_front "numAdded" to a linked list of "listSize"
    template<typename T>
    void LinkedList<T>::LinkedListPushFrontTime(int listSize, int numAdded, const T& value) {

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(value); 
        }

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
            list.push_front(value); 
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        
        // Calculate and display the time taken for the push_front operations
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;
        cout << "PUSH_FRONT: Adding " << numAdded << " nodes to the front of a Linked List of size " << listSize << " took " << fp_ms.count() << " milliseconds" << endl;
    }

}

#endif

using namespace AlgoGauge;
template<typename T>
void LinkedListPerformanceTest(int listSize, int numAdded, int numDeleted, const T& value) {
    cout << "Running Linked List performance tests...\n";
    // Measure time to push_back 
    LinkedList<T>::LinkedListPushBackTime(listSize, numAdded, value);
    // Measure time to push_front 
    LinkedList<T>::LinkedListPushFrontTime(listSize, numAdded, value);
    // Measure time to pop_back 
    LinkedList<T>::LinkedListPopBackTime(listSize, numDeleted);
    // Measure time to pop_front 
    LinkedList<T>::LinkedListPopFrontTime(listSize, numDeleted);
}


int main() {
    cout << "YEP";
    // LinkedList<int>::LinkedListPushBackTime(100000, 50000, 42);
    // LinkedList<int>::LinkedListPushFrontTime(100000, 50000, 42);
    // LinkedList<int>::LinkedListPopBackTime(1000, 500);
    // LinkedList<int>::LinkedListPopBackTime(100, 500);
    // LinkedList<int>::LinkedListPopFrontTime(100000, 50000);
    // LinkedList<int>::LinkedListPopFrontTime(100, 500);

    cout << "Running LinkedList performance tests with \n";
    LinkedListPerformanceTest(100000, 10000, 10000, 42);
    return 0;
}