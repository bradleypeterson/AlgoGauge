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
#include "../dependencies/PerfEvent.hpp"

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
        static string LinkedListPushBackTime(int listSize, int numAdded, bool verbose, bool includeValues);
        static string LinkedListPushFrontTime(int listSize, int numAdded, bool verbose, bool includeValues);
        static string LinkedListPopBackTime(int listSize, int numDeleted, bool verbose, bool includeValues);
        static string LinkedListPopFrontTime(int listSize, int numDeleted, bool verbose, bool includeValues);
        static string LinkedListPushPopFrontTime(int listSize, int numDeleted, bool verbose, bool includeValues);
        static string LinkedListPushPopBackTime(int listSize, int numDeleted, bool verbose, bool includeValues);

        //Linked List Functions
        void push_back(const T& data);
        void push_front(const T& data);
        void pop_back();
        void pop_front();
        void test_push_back();

        string print_List(){
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

    //Timed functions 
    
    //POP BACK
    //amount of time to pop_back "numDeleted" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output )  
    template<typename T>
    string LinkedList<T>::LinkedListPopBackTime(int listSize, int numDeleted, bool verbose, bool includeValues) {
        if (verbose){
            cout << "Checking number removed is less than list size." << endl;
        }

        if (numDeleted > listSize) {
            if (verbose){
                std::cerr << "POP BACK: Cannot delete (" << numDeleted << ") from a list size of (" << listSize << ")." << endl;
            }
            return "{\"ERROR\":\"Call to Pop back rejected, number poped was greater than list size\"}";
        }

        if (verbose){
            cout << "Creating Linked list of length " << listSize << endl;
        }

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(T());  
        }

        if (includeValues){
            list.print_List();
        }        

        if (verbose){
            cout << "Starting Timer to pop back " << numDeleted << " nodes" << endl;
        }

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numDeleted; i++) {
            list.pop_back();
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped" << endl;
        }
                
        if (includeValues){
            list.print_List();
        } 

        string output = "{";
        output += R"("algorithmName": ")" + string("Pop Back") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ",";
        output += R"("number": )" + std::to_string(numDeleted) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";

        if (verbose){
            cout << output << endl;
        }

        return output;
    }

    //POP FRONT 
    //amount of time to pop_back "numDeleted" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output ) 
    template<typename T>
    string LinkedList<T>::LinkedListPopFrontTime(int listSize, int numDeleted, bool verbose, bool includeValues) {
        if (verbose){
            cout << "Checking number deleted is less than list size" << endl;
        }

        if (numDeleted > listSize) {
            if (verbose){
                std::cerr << "POP FRONT: Cannot delete (" << numDeleted << ") from a list size of (" << listSize << ")." << endl;
            }
            return "{\"ERROR\":\"Call to Pop Front rejected, number poped was greater than list size\"}";
        }
        
        if (verbose){
            cout << "Creating Linked list of length "<< listSize << endl;
        }

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(T());  
        }

        if (includeValues){
            list.print_List();
        } 

        if (verbose){
            cout << "Starting Timer to pop front " << numDeleted  << " nodes"<< endl;
        }

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numDeleted; i++) {
            list.pop_front();
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped" << endl;
        }

        if (includeValues){
            list.print_List();
        } 

        string output = "{";
        output += R"("algorithmName": ")" + string("Pop Front") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ","; 
        output += R"("number": )" + std::to_string(numDeleted) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

        if (verbose){
            cout << output << "\n" << endl;
        }

        return output;
    }


    //PUSH BACK
    //amount of time to push_back "numAdded" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output ) 
    template<typename T>
    string LinkedList<T>::LinkedListPushBackTime(int listSize, int numAdded, bool verbose, bool includeValues){
        
        if (verbose){
            cout << "Creating Linked list of length " << listSize << endl;
        }
        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
                list.push_back(42);
        }

        if (includeValues){
            list.print_List();
        } 

        if (verbose){
            cout << "Starting timer to push back "<< numAdded << " nodes" << endl;
        }

        auto sTime= std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
                list.push_back(21);
        }

        auto eTime= std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped." << endl;
        }

        if (includeValues){
            list.print_List();
        }

        string output = "{";
        output += R"("algorithmName": ")" + string("Push Back") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ","; 
        output += R"("number": )" + std::to_string(numAdded) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

        if (verbose){
            cout << output << "\n" << endl;
        }

        return output;
    }

    //PUSH FRONT
    //amount of time to push_front "numAdded" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output ) 
    template<typename T>
    string LinkedList<T>::LinkedListPushFrontTime(int listSize, int numAdded, bool verbose, bool includeValues) {

        if (verbose){
            cout << "Creating Linked list of length " << listSize << endl;
        }

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(42); 
        }
        
        if (includeValues){
            list.print_List();
        }
         
        if (verbose){
            cout << "Starting timer to push front " << numAdded << " nodes" << endl;
        }    

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
            list.push_front(21); 
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped" << endl;
        }  

        if (includeValues){
            list.print_List();
        }
         
        string output = "{";
        output += R"("algorithmName": ")" + string("Push Front") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ","; 
        output += R"("number": )" + std::to_string(numAdded) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

        if (verbose){
            cout << output << "\n" << endl;
        }

        return output;
    }

    //PUSH POP FRONT
    //amount of time to push_front and pop_front "numAdded" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output ) 
    template<typename T>
    string LinkedList<T>::LinkedListPushPopFrontTime(int listSize, int numAdded, bool verbose, bool includeValues) {

        if (verbose){
            cout << "Creating Linked list of length " << listSize << endl;
        }

        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(42); 
        }

        if (verbose){
            cout << "Starting timer to push and pop front " << numAdded << " nodes" << endl;
        }  

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
            list.push_front(42); 
        }

        for (int i = 0; i < numAdded; i++) {
            list.pop_front(); 
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped" << endl;
        }  

        if (includeValues){
            list.print_List();
        }
         
        string output = "{";
        output += R"("algorithmName": ")" + string("Push Pop Front") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ","; 
        output += R"("number": )" + std::to_string(numAdded) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

        if (verbose){
            cout << output << "\n" << endl;
        }

        return output;
    }


    //PUSH POP BACK
    //amount of time to push_back and pop_back "numAdded" to a linked list of "listSize"
    //verbose - print result to STDOUT, error to STDERR
    //include values - prints old and new linked list ( Recommend not doing large linked list for output ) 
    template<typename T>
    string LinkedList<T>::LinkedListPushPopBackTime(int listSize, int numAdded, bool verbose, bool includeValues) {
        if (verbose){
            cout << "Creating Linked list of length " << listSize << endl;
        }
        LinkedList<T> list;
        for (int i = 0; i < listSize; i++) {
            list.push_back(42); 
        }

        if (verbose){
            cout << "Starting timer to push and pop back " << numAdded << " nodes" << endl;
        } 

        auto sTime = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numAdded; i++) {
            list.push_back(42); 
        }

        for (int i = 0; i < numAdded; i++) {
            list.pop_back(); 
        }
        auto eTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = eTime - sTime;

        if (verbose){
            cout << "Timer stopped" << endl;
        } 

        if (includeValues){
            list.print_List();
        }
         
        string output = "{";
        output += R"("algorithmName": ")" + string("Push Pop Back") + "\",";
        output += R"("listSize": )" + std::to_string(listSize) + ","; 
        output += R"("number": )" + std::to_string(numAdded) + ",";
        output += R"("timeTaken": )" + std::to_string(fp_ms.count()) + "}";  

        if (verbose){
            cout << output << "\n" << endl;
        }

        return output;
    }
}

#endif

using namespace AlgoGauge;

//function name, listsize, numAdded, numDeleted, value
//funct option: push_back, push_front, pop_back, pop_front, push_pop_back, push_pop_front
//verbose - print result to STDOUT, error to STDERR
//include values - prints old and new linked list ( Recommend not doing large linked list for output )  
template<typename T>
string LinkedListPerformanceTest(string funct, int listSize, int num, bool verbose, bool includeValues) {

    if (verbose){
        cout << "Running Linked List test for function: " << funct << "\n" <<endl;
    }     

    string result;

    if (funct == "push_back") {
        result = LinkedList<T>::LinkedListPushBackTime(listSize, num, verbose, includeValues);
    }

    else if (funct == "push_front") {
        result = LinkedList<T>::LinkedListPushFrontTime(listSize, num, verbose, includeValues);
    }

    else if (funct == "pop_back") {
        result = LinkedList<T>::LinkedListPopBackTime(listSize, num, verbose, includeValues);
    }

    else if (funct == "pop_front") {
        result = LinkedList<T>::LinkedListPopFrontTime(listSize, num, verbose, includeValues);
    }

    else if (funct == "push_pop_front") {
        result = LinkedList<T>::LinkedListPushPopFrontTime(listSize, num, verbose, includeValues);
    } 
    
    else if (funct == "push_pop_back") {
        result = LinkedList<T>::LinkedListPushPopBackTime(listSize, num, verbose, includeValues);
    } 

    else {
        result = "{\"ERROR\":\"Invalid function name. Please use one of the following: push_back, push_front, pop_back, pop_front.\"}";
        if (verbose){
            cout << result << "\n" << endl;
        }
    }
    
    //check for verbose and include values 
    return result;
}


//using namespace AlgoGauge;
int main() {

    //performance tests with verbose
    LinkedListPerformanceTest<int>("push_back", 100000, 4002, true, false);
    LinkedListPerformanceTest<int>("push_front", 100000, 4002, true, false);
    LinkedListPerformanceTest<int>("pop_front", 1000000, 40002, true, false);
    LinkedListPerformanceTest<int>("pop_back", 10000, 4002, true, false);
    LinkedListPerformanceTest<int>("push_pop_front", 1000000, 4002, true, false);
    LinkedListPerformanceTest<int>("push_pop_back", 10000, 4002, true, false);

    // Invalid case
    LinkedListPerformanceTest<int>("invalid", 1000, 0, true, false);

    // Include values 
    LinkedListPerformanceTest<int>("push_back", 100, 200, true, true);
    return 0;
}