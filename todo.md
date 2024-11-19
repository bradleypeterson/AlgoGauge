# AlgoGauge Functionality TODO List
---

### Hash Tables
---
- [ ] Add psudeo code for hash table create and delete methods for front end. 
- [x] Add Hash Tables.
  - [ ] Open hash tables (linked list hash tables)
  - [x] Closed hash tables (array hash tables)
  - [ ] linear probing vs quadratic probing 
    - [ ] Percentage Limit (capacities that follow a prime / 4 that has a remainder of 3)
    - [ ] if linear probing vs quadratic probing then add 
    - [x] (options) how full do you want the hash table to be? 10%, 20% 50%, 80%, 90% 91%
    - [x] insert, delete, look-ups
      - [x] add, remove, look-up items that only exist                      *at 90%
      - [x] add, remove, look-up items that could exist or couldn't exist   *at 90%
    - [ ] worst case senario for linear, quadriadic probing at diferent levels of hashtable fullness. 
    - [ ] worst probe count or time to complete 
    - [x] how much values you want to input into hash table (100 - 1 million)?

### Linked Lists
---
- [x] Add Linked Lists 
- [x] Add arrays to test against 



<!-- ROADMAP -->
## Roadmap

### P1

- [x] Add array, linked list options CRUD operation resize array
- [x] Add Hashing comparisons
- [ ] Site cleanup go over site and fix bugs and pages BIG theta notation on all
  - [x] clean card heavy layout (Experiment, History)
  - [x] footer padding
  - [x] global button / links for experiment operations
  - [x] change mode dropdown
  - [x] remove repeat text  
  - [ ] Implement experiment popup timer
  - [x] Grid layout for multiple cards (Four cards per row max)
  - [ ] Toggle Dark Mode and Light Mode support
  - [x] Better history layout 
  - [ ] GEt rid of user input size (brad wants this for some reason)
  - [ ] When a close education popup I want to go back to the experiment
- [ ] Instructor video and education overhaul
- [ ] Validate Perf, ways to measure l1 cache. *

### P2

- [ ] GPU algorithms, CUDA
- [ ] Variations of current sort algorithms (quick sort, merge sort 1...k, selection)
- [x] JAVA vs C++ vs RUST vs Python ???
- [ ] CPU vectorization algorithms
- [ ] Clear experiment queue
- [ ] Trees AVL vs B trees

### P3

- [ ] Unit/Perf testing for cpu bound, memory bound, and latency bound.
- [ ] Add Tim sort Algorithm and other sort
- [ ] Optimization mode, O0, O1, O2, O3
- [ ] graph storage mechs
- [ ] Over under prediction betting, make the site fun
- [ ] repeated and chunk slider
- [X] Finish documenting code
- [X] Add Perf options
- [X] Fix memory SEGFAULT errors
- [ ] Parallel processing of algorithms vs Serial.