/**
 * @brief TODO
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <chrono>
#include "RandomNum.hpp"
#include "../dependencies/Perf.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

#ifndef ALGOGAUGE_SORT_7ALGS_CPP
#define ALGOGAUGE_SORT_7ALGS_CPP

namespace AlgoGauge {

    /**
     * @brief Used for determining valid Algorithm Options.
     * none is default, however, if set, should always throw an error.
     */
    enum AlgorithmOptions {
        none = 0,
        randomSet,
        repeatedSet,
        chunkSet,
        reversedSet,
        orderedSet
    };

    /**
     * @brief This is used to map to the AlgorithmOptions enum
     */
    static const std::string AlgorithmOptionsStrings[] = {
            "None Set",
            "Random",
            "Repeated",
            "Chunks",
            "Reversed",
            "Ordered"
    };


    template<typename T>
    class BaseSort {
    public:
        //constructors and destructors
        BaseSort(
            const string &sortName,
            const unsigned int capacity,
            const string &canonicalName = "",
            const bool &verbose = false,
            const bool &includeValues = false,
            const string &includePerf = "false"
        );
        virtual ~BaseSort();

        //getters
        string getName() { return sortName; }
        string getCanonicalName() {
            return this->canonicalName.empty() ? string("") : (string("<") + this->canonicalName + string("> "));
        }
        string getAlgorithmOption() { return AlgorithmOptionsStrings[algorithmOption]; }
        T *getValuesPriorToSort() const { return this->valuesPriorToSort; }
        T *returnValues() const { return this->arr; }
        string getStringResult();
        bool isVerbose() { return this->verbose; }
        bool valuesIncluded() { return this->verbose; }
        string getValuesRange();
        string getJSONResult();

        //setters
        void setCanonicalName(const string &c) { this->canonicalName = c; }
        void setVerbose(const bool &v = true) { this->verbose = v; }
        void setValuesIncluded(const bool &i = true) { this->verbose = i; }

        //other class members
        void loadRandomValues();
        void loadRepeatedValues();
        void loadChunkValues();
        void loadReversedValues();
        void loadOrderedValues();
        void printValues() const;
        void verifySort() const;
        void printSortToFile(const string &filePath, const bool &append = true) const;
        void runAndCaptureSort();
        void runAndPrintSort();
        void runAndPrintFileSort(const string &filePath, const bool &append = true);
        string runAndGetJSONSort();
        string getDummyPerfData(bool JSON = false);

        virtual void runSort() = 0; // Pure virtual function.
        // It makes the class **abstract**.  In other words,
        // nothing can instantiate an object of this class.

    protected:
        string sortName;
        AlgorithmOptions algorithmOption;
        T *arr{nullptr};
        unsigned int capacity{0};
        string canonicalName; // Used to store a canonical name to the instantiated object.
        // This is different from sortName as this can be whatever the user defines.
        bool verbose;
        bool includeValues;
        string includePerf;
#ifdef linux
        Perf::Perf perf;
#endif

    private:
        unsigned int *valuesPriorToSort; //Stores the values prior to sorting
        std::chrono::duration<double, std::milli> executionTime;
        void loadPerf();
    };

    template<typename T>
    BaseSort<T>::BaseSort(
            const string &sortName,
            const unsigned int capacity,
            const string &canonicalName,
            const bool &verbose,
            const bool &includeValues,
            const string &includePerf
    ) {
        this->sortName = sortName;
        if (capacity > 0 && capacity < UINT32_MAX) this->capacity = capacity;
        else throw std::invalid_argument("Length must be greater than 0 and less than " + std::to_string(UINT32_MAX));
        this->arr = new T[capacity];
        this->valuesPriorToSort = new T[capacity];
        this->canonicalName = canonicalName;
        this->algorithmOption = AlgorithmOptions::none;
        this->executionTime = (std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now());
        this->verbose = verbose;
        this->includeValues = includeValues;
        this->includePerf = includePerf;
        this->loadPerf();
    }

    template<typename T>
    BaseSort<T>::~BaseSort() {
        delete[] arr;
        delete[] valuesPriorToSort;
    }

    template<typename T>
    void BaseSort<T>::loadRandomValues() {
        algorithmOption = AlgorithmOptions::randomSet;
        RandomNum distrib;

        for (unsigned int i = 0; i < capacity; ++i) {
            auto newRand = distrib.getRandomNumber();
            arr[i] = valuesPriorToSort[i] = newRand; //We copy this so that we can have a history before sorting
        }
    }

    template<typename T>
    void BaseSort<T>::loadRepeatedValues() {
        algorithmOption = AlgorithmOptions::repeatedSet;
        RandomNum distrib;
        auto randNum = distrib.getRandomNumber();
        //get random number to load in the repeated set. Honestly, it doesn't
        //really matter which specific number we use, we could have just used 0 or 1,
        //but it feels like this function needs a little extra seasoning to it.

        for (unsigned int i = 0; i < capacity; i++)
            arr[i] = valuesPriorToSort[i] = randNum; //We copy this so that we can have a history before sorting
    }

    template<typename T>
    void BaseSort<T>::loadChunkValues() {
        algorithmOption = AlgorithmOptions::chunkSet;
        RandomNum distrib;
        unsigned int chunkSize = capacity < 10 ? 1 : 5;
        bool flipFlop = RandomNum<>::randomBool(); // this is to prevent always choosing one subset first over the other
        // e.g. if flipFlop was false first always, then the first set will always be repeated numbers

        for (unsigned int i = 0; i < capacity; flipFlop = !flipFlop) { //flip-flop the flipFlop
            auto randNum = distrib.getRandomNumber();
            for (int j = 0; j < chunkSize && i < capacity; j++, i++) {
                if (flipFlop) randNum = distrib.getRandomNumber(); //choose a new random number if subset is on random mode
                arr[i] = valuesPriorToSort[i] = randNum;
            }
        }
    }

    template<typename T>
    void BaseSort<T>::loadPerf() {
#ifdef linux
        //CPU Hardware Events
        perf.addNewPerfEvent("cpu cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
        perf.addNewPerfEvent("bus cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES);
        perf.addNewPerfEvent("cpu instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
        perf.addNewPerfEvent("cache references", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES);
        perf.addNewPerfEvent("cache misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);
        perf.addNewPerfEvent("branch predictions", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_BPU);
        perf.addNewPerfEvent("retired branch instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS);
        perf.addNewPerfEvent("branch misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES);

        //CPU Software (OS) Events
        perf.addNewPerfEvent("total page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS);
        perf.addNewPerfEvent("minor page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MIN);
        perf.addNewPerfEvent("major page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MAJ);
        perf.addNewPerfEvent("context switches", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES);

        //CPU Cache Events
        perf.addNewPerfEvent(
            "L1 data cache read accesses",
            PERF_TYPE_HW_CACHE,
            (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
        );
        perf.addNewPerfEvent(
            "L1 instruction cache read accesses",
            PERF_TYPE_HW_CACHE,
            (PERF_COUNT_HW_CACHE_L1I) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
        );
        perf.addNewPerfEvent(
            "L1 data cache prefetch accesses",
            PERF_TYPE_HW_CACHE,
            (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
        );
        perf.addNewPerfEvent(
            "L1 instruction cache prefetch accesses",
            PERF_TYPE_HW_CACHE,
            (PERF_COUNT_HW_CACHE_L1I) | (PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
        );
#endif
    }

    template<typename T>
    string BaseSort<T>::getDummyPerfData(bool JSON) {
        string allEvents[17][2] = {
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

        string returnString;
        int size = sizeof(allEvents)/sizeof(*allEvents);
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

    template<typename T>
    void BaseSort<T>::loadReversedValues() {
        algorithmOption = AlgorithmOptions::reversedSet;
        for (unsigned int i = capacity; i > 0; i--) arr[i - 1] = valuesPriorToSort[i - 1] = i - 1;
    }

    template<typename T>
    void BaseSort<T>::loadOrderedValues() {
        algorithmOption = AlgorithmOptions::orderedSet;
        for (unsigned int i = 0; i < capacity; i++) arr[i] = valuesPriorToSort[i] = i;
    }

    template<typename T>
    void BaseSort<T>::verifySort() const {
        for (unsigned int i = 0; i < capacity - 1; i++) {
            if (arr[i + 1] < arr[i]) {
                throw std::runtime_error("Not sorted at index: " + std::to_string((i + 1)));
            }
        }
    }

    template<typename T>
    void BaseSort<T>::runAndCaptureSort() {
        if (verbose) cout << "Starting sort: \"" << sortName << "\"" << getCanonicalName() << "..." << endl;
        auto startTime = std::chrono::high_resolution_clock::now();
#ifdef linux
        if (includePerf == "true") {
            ioctl(perf.getFirstFileDescriptor(), PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
            ioctl(perf.getFirstFileDescriptor(), PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
            runSort();
            ioctl(perf.getFirstFileDescriptor(), PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
        } else {
#endif
        runSort();
#ifdef linux
        }
#endif

        auto stopTime = std::chrono::high_resolution_clock::now();
        if (verbose) cout << "Verifying sort: \"" << sortName << "\"" << getCanonicalName() << "..." << endl;
        verifySort();
        if (verbose) cout << "Sort: \"" << sortName << "\"" << getCanonicalName() << "Verified!" << endl;
        executionTime = stopTime - startTime;
#ifdef linux
        perf.readBuffer();
#endif
    }

    template<typename T>
    string BaseSort<T>::getStringResult() {
        string perfString = "; Perf Data: ";
        if (includePerf == "sample") {
            perfString += getDummyPerfData();
        }
#ifdef linux
        else if (includePerf == "true") {
            perfString += perf.getBufferString();
        }
#endif
        else perfString = "";

        return string("Sort '")
               + sortName
               + string("' ")
               + getCanonicalName()
               + string("with Algorithm Option '")
               + getAlgorithmOption()
               + string("' of length ")
               + std::to_string(capacity)
               + string(", completed in ")
               + std::to_string(executionTime.count())
               + string(" milliseconds")
               + perfString;
    }

    template<typename T>
    string BaseSort<T>::getValuesRange() {
        string returnString;
        returnString = std::to_string(arr[0]);
        // store the first 25 values only
        for (unsigned int i = 1; i < (capacity < 25 ? capacity : 25); i++) {
            returnString += ", " + std::to_string(arr[i]);
        }

        //if capacity is more than 25, store up to the last 25
        if (capacity > 25) {
            returnString += " ... ";
            for (unsigned int i = (capacity - 25 > 0 ? capacity - 25 : 25); i < capacity; i++) {
                returnString += ", " + std::to_string(arr[i]);
            }
        }
        return returnString;
    }

    template<typename T>
    void BaseSort<T>::printValues() const {
        cout << "Values for sort: \"" << sortName << "\"" + getCanonicalName() << getValuesRange() << endl;
    }

    template<typename T>
    string BaseSort<T>::getJSONResult() {
        string output = "{";
        output += R"("algorithmName": ")" + sortName + "\",";
        output += R"("algorithmOption": ")" + getAlgorithmOption() + "\",";
        output += R"("algorithmLength": )" + std::to_string(capacity) + ",";
        output += R"("algorithmCanonicalName": ")" + canonicalName + "\",";

        if (includeValues) {
            output += R"("valuesBeforeSort": [)";
            for (unsigned int i = 0; i < capacity; i++) {
                output += std::to_string(valuesPriorToSort[i]) + ",";
            }

            output.pop_back();
            output += R"(],"valuesAfterSort": [)";

            for (unsigned int i = 0; i < capacity; i++) {
                output += std::to_string(arr[i]) + ",";
            }

            output.pop_back();
            output += "],";
        }

        output += "\"algorithmRunTime_ms\": " + std::to_string(executionTime.count());

        output += ", \"perfData\": ";
        if (includePerf == "sample") {
            output += getDummyPerfData(true);
        }
#ifdef linux
        else if (includePerf == "true") {
            output += perf.getBufferJSON();
        }
#endif
        else output += "{}";

        return output + "}";
    }

    template<typename T>
    void BaseSort<T>::printSortToFile(const std::string &filePath, const bool &append) const {
        std::ofstream outFile;
        append ? outFile.open(filePath, std::ios::app) : outFile.open(filePath);
        outFile << getJSONResult();
        outFile.close();
    }

    template<typename T>
    void BaseSort<T>::runAndPrintSort() {
        if (includeValues) cout << "Values before sort:" << endl << getValuesRange() << endl;
        runAndCaptureSort();
        if (includeValues) cout << "Values after sort:" << endl << getValuesRange() << endl;
        cout << getStringResult() << endl;
    }

    template<typename T>
    void BaseSort<T>::runAndPrintFileSort(const std::string &filePath, const bool &append) {
        runAndCaptureSort();
        printSortToFile(filePath, append);
    }

    template<typename T>
    string BaseSort<T>::runAndGetJSONSort() {
        runAndCaptureSort();
        return getJSONResult();
    }


//Algorithms
    template<typename T>
    class Bubble : public BaseSort<T> {
    public:
        explicit Bubble(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("Bubble", capacity, canonicalName, verbose, includedValues, includePerf) {}

        void runSort();

    private:
    };

    template<typename T>
    void Bubble<T>::runSort() {
        for (unsigned int round = 0; round < this->capacity - 1; round++) {
            for (unsigned int i = 0; i < this->capacity - 1 - round; i++) {
                if (this->arr[i + 1] < this->arr[i]) {
                    T temp = this->arr[i];
                    this->arr[i] = this->arr[i + 1];
                    this->arr[i + 1] = temp;
                }
            }
        }
    }

    template<typename T>
    class Selection : public BaseSort<T> {
    public:
        Selection(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("Selection", capacity, canonicalName, verbose, includedValues, includePerf) {};

        void runSort();

    private:
    };

    template<typename T>
    void Selection<T>::runSort() {
        // The outer loop's job is to...
        // Each time the outer loop iterators, it works with the leftmost/unsorted index value
        for (unsigned int i = 0; i < this->capacity - 1; i++) {
            // The inner loop's job is to...
            // Compare the current index to the rest of the unsorted region
            for (unsigned int j = i + 1; j < this->capacity; j++) {
                // We have an i (the index we hope to sort) and a j (a candidate to check)
                if (this->arr[j] < this->arr[i]) {
                    T temp = this->arr[i];
                    this->arr[i] = this->arr[j];
                    this->arr[j] = temp;
                }
            }
        }
    }

    template<typename T>
    class Insertion : public BaseSort<T> {
    public:
        Insertion(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("Insertion", capacity, canonicalName, verbose, includedValues, includePerf) {};

        void runSort();

    private:
    };

    template<typename T>
    void Insertion<T>::runSort() {
        // The outer loop's job is to...
        // Obtain the leftmost unsorted value, iterates one index to the right each round.  Starts at 1
        for (unsigned int i = 1; i < this->capacity; i++) {
            // The inner loop's job is to...
            // Take the leftmost unsorted value, walk to the left (back to index 0), swapping along the way until
            // no swaps are needed, or until we get to index 0.
            unsigned int j = i;
            while (j > 0 && this->arr[j] < this->arr[j - 1]) {
                T temp = this->arr[j];
                this->arr[j] = this->arr[j - 1];
                this->arr[j - 1] = temp;
                j--;
            }
        }
    }

    template<typename T>
    class Quick : public BaseSort<T> {
    public:
        Quick(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("Quick", capacity, canonicalName, verbose, includedValues, includePerf) {};

        void runSort();

    private:
        void runSort(unsigned int first, unsigned int last);

        unsigned int quickSortPartition(unsigned int first, unsigned int last);
    };

    template<typename T>
    void Quick<T>::runSort() {
        runSort(0, this->capacity);
    }

    template<typename T>
    void Quick<T>::runSort(unsigned int first, unsigned int last) {
        if (first < last) {
            // Obtain a pivot, move all values smaller to the left of pivot
            // and all values larger to the right of pivot
            unsigned int pivotLocation = quickSortPartition(first, last);
            runSort(first, pivotLocation);
            runSort(pivotLocation + 1, last);
        }
    }

    template<typename T>
    unsigned int Quick<T>::quickSortPartition(unsigned int first, unsigned int last) {
        T pivot;
        unsigned int index;
        unsigned int smallIndex;
        T temp;
        pivot = this->arr[first];
        smallIndex = first;
        for (index = first + 1; index < last; index++) {
            if (this->arr[index] < pivot) {
                smallIndex++;
                //swap
                temp = this->arr[smallIndex];
                this->arr[smallIndex] = this->arr[index];
                this->arr[index] = temp;
            }
        }
        // swap pivot into its final spot
        temp = this->arr[first];
        this->arr[first] = this->arr[smallIndex];
        this->arr[smallIndex] = temp;

        return smallIndex;
    }


    template<typename T>
    class Heap : public BaseSort<T> {
    public:
        Heap(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("Heap", capacity, canonicalName, verbose, includedValues, includePerf) {};

        void runSort();

    private:
        void heapify(unsigned int lastUnsortedIndex, unsigned int parent);
    };

    template<typename T>
    void Heap<T>::heapify(unsigned int lastUnsortedIndex, unsigned int parent) {
        // Find largest among root, left child and right child
        unsigned int largest = parent;
        unsigned int left = 2 * parent + 1;
        unsigned int right = 2 * parent + 2;

        if (left < lastUnsortedIndex && this->arr[left] > this->arr[largest])
            largest = left;

        if (right < lastUnsortedIndex && this->arr[right] > this->arr[largest])
            largest = right;

        // Swap and continue heapifying if root is not largest
        if (largest != parent) {
            T temp = this->arr[parent];
            this->arr[parent] = this->arr[largest];
            this->arr[largest] = temp;
            heapify(lastUnsortedIndex, largest);
        }
    }

    template<typename T>
    void Heap<T>::runSort() {

        // Put tree in max heap (all parents bigger than their children)
        unsigned int i = this->capacity / 2;
        while (i > 0) {
            i--;
            heapify(this->capacity, i);
        }

        // Swap root to sorted position, re-heap, repeat.
        unsigned int lastUnsortedIndex = this->capacity;
        while (lastUnsortedIndex > 0) {
            lastUnsortedIndex--;
            T temp = this->arr[0];
            this->arr[0] = this->arr[lastUnsortedIndex];
            this->arr[lastUnsortedIndex] = temp;
            heapify(lastUnsortedIndex, 0);
        }
    }

    template<typename T>
    class Merge : public BaseSort<T> {
    public:
        Merge(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false,
                const string &includePerf = "false"
        ) : BaseSort<T>("merge", capacity, canonicalName, verbose, includedValues, includePerf) {}

        void runSort();

    private:
        void runSort(unsigned int firstIndex, unsigned int lastIndex);
    };


    template<typename T>
    void Merge<T>::runSort() {
        runSort(0, this->capacity);
    }

    template<typename T>
    void Merge<T>::runSort(unsigned int firstIndex, unsigned int lastIndex) {
        if (lastIndex - firstIndex < 2) {
            return;
        }

        // Find a middle
        unsigned int middleIndex = (lastIndex - firstIndex) / 2 + firstIndex;

        runSort(firstIndex, middleIndex);
        runSort(middleIndex, lastIndex);

        unsigned int leftHalfSize = middleIndex - firstIndex;
        T *leftArray = new T[leftHalfSize];
        for (unsigned int i = 0; i < leftHalfSize; i++) {
            leftArray[i] = this->arr[firstIndex + i];
        }
        unsigned int rightHalfSize = lastIndex - middleIndex;
        T *rightArray = new T[rightHalfSize];
        for (unsigned int i = 0; i < rightHalfSize; i++) {
            rightArray[i] = this->arr[middleIndex + i];
        }
        unsigned int leftIndex = 0;
        unsigned int rightIndex = 0;
        unsigned int arrIndex = firstIndex;

        while (leftIndex < leftHalfSize && rightIndex < rightHalfSize) {
            if (leftArray[leftIndex] <= rightArray[rightIndex]) {
                this->arr[arrIndex] = leftArray[leftIndex];
                leftIndex++;
            } else {
                this->arr[arrIndex] = rightArray[rightIndex];
                rightIndex++;
            }
            arrIndex++;
        }
        while (leftIndex < leftHalfSize) {
            this->arr[arrIndex] = leftArray[leftIndex];
            leftIndex++;
            arrIndex++;
        }
        while (rightIndex < rightHalfSize) {
            this->arr[arrIndex] = rightArray[rightIndex];
            rightIndex++;
            arrIndex++;
        }
        delete[] leftArray;
        delete[] rightArray;

    }

    template<typename T, unsigned int K>
    class ArrayKMerge : public BaseSort<T> {
    public:
        ArrayKMerge(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false
        ) : BaseSort<T>("Array K-Way Merge", capacity, canonicalName, verbose, includedValues) {}

        void runSort();

    private:
        void runSort(unsigned int firstIndex, unsigned int lastIndex);
    };

    template<typename T, unsigned int K>
    void ArrayKMerge<T, K>::runSort() {
        runSort(0, this->capacity);
    }

    struct KIndexes {
        unsigned int kthStartIndex{0};
        unsigned int kthCurrentIndex{0};
        unsigned int kthEndIndex{0};
    };

    template<typename T, unsigned int K>
    void ArrayKMerge<T, K>::runSort(unsigned int firstIndex, unsigned int lastIndex) {

        if (lastIndex - firstIndex < 2) {
            return;
        }

        unsigned int numValues = lastIndex - firstIndex;
        KIndexes indexes[K];
        T *arrayCopy = new T[numValues];

        for (unsigned int i = 0; i < K; i++) {
            indexes[i].kthStartIndex = numValues * i / K;
            indexes[i].kthEndIndex = numValues * (i + 1) / K;
            runSort(firstIndex + indexes[i].kthStartIndex, firstIndex + indexes[i].kthEndIndex);
        }
        for (unsigned int i = 0; i < numValues; i++) {
            arrayCopy[i] = this->arr[firstIndex + i];
        }
        for (unsigned int i = 0; i < K; i++) {
            indexes[i].kthCurrentIndex = indexes[i].kthStartIndex;
        }

        unsigned int itemsSorted = 0;

        while (itemsSorted < numValues) {
            // Scan across all arrays looking for the smallest
            int arrayWithSmallestValue = -1;
            for (unsigned int i = 0; i < K; i++) {
                if (indexes[i].kthCurrentIndex < indexes[i].kthEndIndex) {
                    if (arrayWithSmallestValue == -1) {
                        arrayWithSmallestValue = i;
                    } else if (arrayCopy[indexes[i].kthCurrentIndex] <
                               arrayCopy[indexes[arrayWithSmallestValue].kthCurrentIndex]) {
                        arrayWithSmallestValue = i;
                    }
                }
            }
            // We've found the smallest, move it in
            this->arr[firstIndex + itemsSorted] = arrayCopy[indexes[arrayWithSmallestValue].kthCurrentIndex];
            indexes[arrayWithSmallestValue].kthCurrentIndex++;
            itemsSorted++;
        }
        delete[] arrayCopy;

    }

//template <typename T>
//struct SortSubArrays {
//	T* rangeStartIndex;
//	T* currentIndex;
//	T* rangeEndIndex;
//};

    template<typename T, unsigned int K>
    class MinHeap {
    public:
        MinHeap(T *sourceArray, unsigned int sourceArraySize);

        void insert(T *item);

        //T getAndReplace(const T& replacement);
        T getSmallest();

    private:
        T *heap[K]; // Make this a stack array and not a heap array for performance.  It's a pointer existing array values and pointer arithmetic
        unsigned int size;
        T *sourceArray{nullptr};
        unsigned int sourceArraySize{0};
        //SortSubArrays subArrayIndexes[k];
    };

    template<typename T, unsigned int K>
    MinHeap<T, K>::MinHeap(T *sourceArray, unsigned int sourceArraySize) {
        this->sourceArray = sourceArray;
        this->sourceArraySize = sourceArraySize;
        for (unsigned int i = 0; i < K; i++) {

            //if ((i * sourceArraySize) / K == 0) {
            this->insert(sourceArray + (i * sourceArraySize) / K);
            //}
            //else {
            //	this->insert(sourceArray + (i * sourceArraySize) / K + 1);
            //}
            //	unsigned int extra = sourceArraySize % K;
            //	this->insert(sourceArray + (i * (sourceArraySize + extra)) / K);
            //}
            //else {
            //	this->insert(sourceArray + (i * sourceArraySize) / K);
            //}
            //subArrayIndexes[i].rangeStartIndex = sourceArraySize * i / K;
            //subArrayIndexes[i].currentIndex = sourceArraySize * i / K;
            //subArrayIndexes[i].rangeEndIndex = sourceArraySize * (i+1) / K;
        }
    }

    template<typename T, unsigned int K>
    void MinHeap<T, K>::insert(T *item) {

        unsigned int currIndex = size;

        heap[size] = item;
        size++;

        // Work upward
        while (currIndex > 0 && *heap[currIndex] < *heap[(currIndex - 1) / 2]) {
            //swap
            T *temp = heap[currIndex];
            heap[currIndex] = heap[(currIndex - 1) / 2];
            heap[(currIndex - 1) / 2] = temp;
            currIndex = (currIndex - 1) / 2;
        }
    }

    template<typename T, unsigned int K>
    T MinHeap<T, K>::getSmallest() {

        // Get top, replace it with the last item to keep the tree balanced
        T retVal = *heap[0];

        // See if this subarray has more values or not.
        if (((int(heap[0] - sourceArray) + 2) * K - 1) % sourceArraySize >= K) {
            // We're still within a subarray, use the next value of the subarray
            heap[0] = heap[0] + 1;
        } else {
            // This sub array is done.  Put the last leaf of the heap at root
            if (size == 0) {
                cout << "break here" << endl;
            }
            heap[0] = heap[size - 1];
            size--;
        }

        // Work downward
        unsigned int parent = 0;
        while (true) {
            unsigned int left = 2 * parent + 1;
            unsigned int right = 2 * parent + 2;
            unsigned int smallest = parent;
            if (left < size && *heap[left] < *heap[smallest])
                smallest = left;

            if (right < size && *heap[right] < *heap[smallest])
                smallest = right;

            if (smallest != parent) {
                T *temp = heap[parent];
                heap[parent] = heap[smallest];
                heap[smallest] = temp;
                // Prepare for next loop
                parent = smallest;
            } else {
                break;
            }
        }

        return retVal;

    }

    template<typename T, unsigned int K>
    class HeapKMerge : public BaseSort<T> {
    public:
        HeapKMerge(
                const unsigned int capacity,
                const string &canonicalName = "",
                const bool &verbose = false,
                const bool &includedValues = false
        ) : BaseSort<T>("Heap - K-Way Merge", capacity, canonicalName, verbose, includedValues) {}

        void runSort();

    private:
        void runSort(unsigned int firstIndex, unsigned int lastIndex);

    };

    template<typename T, unsigned int K>
    void HeapKMerge<T, K>::runSort() {
        runSort(0, this->capacity);
    }

    template<typename T, unsigned int K>
    void HeapKMerge<T, K>::runSort(unsigned int firstIndex, unsigned int lastIndex) {

        if (lastIndex - firstIndex < 2) {
            return;
        }

        unsigned int numValues = lastIndex - firstIndex;
        for (unsigned int i = 0; i < K; i++) {
            runSort(firstIndex + numValues * i / K, firstIndex + numValues * (i + 1) / K);
        }

        T *arrayCopy = new T[numValues];
        for (unsigned int i = 0; i < numValues; i++) {
            arrayCopy[i] = this->arr[firstIndex + i];
        }

        // Build heap.  First put all values in the heap.
        MinHeap<T, K> minHeap(arrayCopy, numValues);

        unsigned int itemsSorted = 0;

        while (itemsSorted < numValues) {

            this->arr[firstIndex + itemsSorted] = minHeap.getSmallest();

            itemsSorted++;
        }
        delete[] arrayCopy;

    }

    template<typename T>
    void runMySort(BaseSort<T> &&sortObj) {
        cout << "Running sort: " << sortObj.getName() << endl;
        sortObj.loadRandomValues();
        sortObj.printValues();
        auto t1 = std::chrono::high_resolution_clock::now();
        sortObj.runSort();
        auto t2 = std::chrono::high_resolution_clock::now();

        sortObj.printValues();
        sortObj.verifySort();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        std::cout << "Sort completed in " << fp_ms.count() << " milliseconds" << endl;

    }
}

# endif

/* Unable to link to this with this other main() function.
   If you uncomment this portion, make sure to only compile
   and run this file. Otherwise, you'll be greeted with a
   lovely compile error.
*/
//int main() {
//    runMySort(Bubble<unsigned int>(20000));
//    runMySort(Selection<unsigned int>(20000));
//    runMySort(Insertion<unsigned int>(20000));
//    runMySort(Quick<unsigned int>(1000000));
//    runMySort(Heap<unsigned int>(1000000));
//    runMySort(Merge<unsigned int>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 2>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 3>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 4>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 5>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 6>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 7>(1000000));
//    runMySort(ArrayKMerge<unsigned int, 8>(1000000));
//    runMySort(HeapKMerge<unsigned int, 2>(1000000));
//    runMySort(HeapKMerge<unsigned int, 3>(1000000));
//    runMySort(HeapKMerge<unsigned int, 4>(1000000));
//    runMySort(HeapKMerge<unsigned int, 5>(1000000));
//    runMySort(HeapKMerge<unsigned int, 6>(1000000));
//    runMySort(HeapKMerge<unsigned int, 7>(1000000));
//    runMySort(HeapKMerge<unsigned int, 8>(1000000));
//    cin.get();
//    return 0;
//}
