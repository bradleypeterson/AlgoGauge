<a name="readme-top"></a>
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/jzdegrey/AlgoGauge">
    <img src="https://catalog.weber.edu/mime/media/22/2687/east_horiz.png" alt="Weber State University Logo">
  </a>

<h2 align="center">AlgoGauge CLI</h3>


  <h6 align="center">
    The CLI version of Dr. Brad Peterson's AlgoGauge Tenure project.
    
  </h6>
  <p align="left">
    This program will compare one or many different algorithms and print out the results of the Algorithm. The purpose 
    of this program is to help show case the differences between the many different sorting algorithms and to help 
    visualize use cases for each (e.g. quick sort is great for random data, but not so great if the data is already
    sorted). In the future, this program will also provide more result data via Perf as well as have the ability to 
    test hashing algorithm complexities.
  </p>
</div>

<!-- BREAKING CHANGES -->
## Breaking Changes

There are currently no breaking changes.

<!-- GETTING STARTED -->
## Getting Started

Before getting started, there are some prerequisites and information you need to know first. 

### Repository Information

Use the main branch to link your node project to. If you need to make any changes to this for whatever reason, please
 check the <a href="https://github.com/jzdegrey/AlgoGauge/issues">Repo Issues tab</a> to see if your issue has already
 been reported. If not, then please create a 
<a href="https://github.com/jzdegrey/AlgoGauge/issues/new/choose">New issue</a>. If you do need to make changes to this
code, then you must check out and create a new branch from either main or the <a href="">Development Branch</a>, 
make your edits, commit and push them to your branch and then create a pull request. Only owners of this repo may 
authorize pull requests.

If you do want to add to this repo, please also read the <a href="./DEV-README.md">DEV-README.md</a>

### Prerequisites

This project is built on cmake. Please make sure to have cmake installed before continuing.

### Installation
```shell
cd [source code root directory]
mkdir cmake-build-debug
cd cmake-build-debug
cmake --build ..
```

_next steps vary slightly depending on your OS_
#### Windows OS:
To run on Windows OS (Calls help option `-h`)

```shell
.\AlgoGauge.exe -h
```

#### Unix/Mac OS:
To run on Unix/Mac OS (Calls help option `-h`)

```shell
./AlgoGauge -h
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Please use ```./AlgoGauge -h``` to see the help message along with options.

### Usage Help
---

#### Basic Usage

```txt
AlgoGauge -a (algorithm name) -s (algorthim strategy) -n (number of items to process) [-v, -j, -h]
```
<br>

#### Algorithm Name, Strategy, and Number [REQUIRED] options:

##### -a, --algorithm 
Name of the first algorithm to run. 
- Sorting: 
  - `default`
  - `bubble`
  - `selection`
  - `insertion`
  - `quick`
  - `merge`
  - `heap`
- Hash Table: 
  - `linear_probe`
- Linked List:
  - `pop`
  - `push`
  - `push_pop`

##### -s, --strategy
Determines the strategy of loading values for sorting algorithms, the type of hash table, or the method of action for linked lists.
- Sorting: 
  - `ran/random`
  - `rep/repeated` 
  - `chunks`
  - `rev/sorted-reverse`
  - `sorted`
- Hash Table: 
  - `closed`
- Linked List:
  - `front`
  - `back`
  - `front_back`
  - `back_front`

##### -n, --number
The number of items the algorithm will process. Provide an int value between `0` and `4294967295`
- Sorting: Size of Array to Sort
- Hash Table: Number of testing operations
- Linked List: Number of operations
<br>

#### Sorting Algorithm Specific [Required] options:

##### -x, --language 
What programming language will process the sorting algorithm
- Options: 
  - `C++`
  - `Py/Python3`
  - `JS/NodeJS`
<br>

#### Sorting Algorithm [Optional] options:

##### -i, --iterator 
How much the number per iteration (default: 1)

##### -e, --additional 
The ending number for step operations (default: 0)
<br>

#### Linked List Specific [Required] options:

##### -l, --size 
How many elements can be stored in the Linked List.Provide a number (int > 0) that determines linked list size.
<br>

#### Hash Table Specific [Required] options:

##### -c, --capacity 
How many elements can be stored in the Hash Table. Provide a number (int > 0) that determines hash table size.

##### -d, --density
How full or the density of the starting hash table as a percentage. Provide an int value between 0 (0%) and 100 (100%) to load into hash table.
<br>

#### Program Output [OPTIONAL] options:

##### -f, --file
File path must include the file name (extension not required). Regardless of file extension, file content's
structure will always be JSON. The file path and name to output the file to (default: "")

##### -j, --json
Prints the output as a json formatted object.

#####   -y, --name 
A canonical name that will be returned in output if provided.
<br>

#### Program Settings [OPTIONAL] options:

##### -v, --verbose
Runs the program in Verbose mode

##### -0, --output
Will output the arrays in the output. It is highly recommended to use a small length (less than 100).

##### -p, --perf
Includes Perf data in the output. Actual Perf data only works on Linux.
If you are not on Linux and want to use this anyways, you can set this to "sample". e.g. `--sample`
##### -h, --help
Prints this help page.
<br>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Examples

**All examples will be shown for a Unix Shell. For Windows, use ```.\AlgoGauge.exe``` instead of ```./AlgoGauge```**

### Running a single Algorithm using *Quick* sort with *100,000* *Random* values

```shell
./AlgoGauge -a quick -s random -n 100000 -x c++ -j
```
#### Results:

```{"sorting_algorithm": [{"algorithmName": "Quick","algorithmOption": "Random","algorithmLength": 100000,"language": "C++","algorithmCanonicalName": "","algorithmRunTime_ms": 18.326541, "perfData": {}}]}```

### Passing multiple Algorithms at the same time

```shell
./AlgoGauge -a quick -s random -n 100000 -x c++ -j -a quick -s random -n 100000 -x c++ -j
```

#### Results:

```
Sort 'Quick' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Bubble' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Quick' with Algorithm Option 'Ordered' of length 25000, completed in x.xxxxxx milliseconds
```


### More examples of passing multiple Algorithms at the same time.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -j -a merge -s random -n 1000 -x c++ -j
```

#### Results:

```
{"sorting_algorithm": 
  [
    {"algorithmName": "Quick"
    ,"algorithmOption": "Random"
    ,"algorithmLength": 1000
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 0.129291
    , "perfData": {}}
  ,
    {"algorithmName": "Merge"
    ,"algorithmOption": "Random"
    ,"algorithmLength": 1000
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 0.321083
    , "perfData": {}}
  ]
}
```


### JSON Results ```-j``` or ```--json```

You can specify whether the results should be printed out in JSON format instead of the human-readable versions like 
the previous examples. Note: the JSON flag ***only*** pertains to STDOUT. 

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -json
```

#### Results:

```
{"sorting_algorithm": 
  [
    {"algorithmName": "Quick"
    ,"algorithmOption": "Random"
    ,"algorithmLength": 1000
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 0.151958
    , "perfData": {}}
  ]
}
```

### Perf Values ```-p``` or ```--perf```

To include Perf values, you ***MUST*** be on Linux machine and provide the Perf argument.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -json -p
```

#### Results:
```
{"sorting_algorithm": 
  [
    {"algorithmName": "Quick"
    ,"algorithmOption": "Random"
    ,"algorithmLength": 1000
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 0.130833
    ,"perfData": 
      {"task-clock": 364444262.000000
      , "context switches": 214.000000
      , "cycles": 595348804.490000
      , "kcycles": 170739620.860000
      , "instructions": 1009631347.960000
      , "L1-misses": 5583046.540000
      , "LLC-misses": 897165.960000
      , "branch predictions": 182104579.700000
      , "branch-misses": 3082415.940000
      , "cache references": 5131660.310000
      , "retired branch instructions": 161551834.080000
      , "total page faults": 10020.000000
      , "minor page faults": 10020.000000
      , "major page faults": 0.000000
      , "context switches": 214.000000
      , "scale": 1.000000
      , "IPC": 1.700000
      , "CPUs": 0.370000
      , "GHz": 1.630000}
    }
  ]
}
```

### Perf Values for *Non-Linux* Machines ```--perf=sample```

If you are not on a Linux machine, but will be deploying this to a Linux machine eventually and will be needing Perf data, 
but you need to test and debug on a non-Linux machine, you can include the ```--sample``` argument which mimics the Perf output by providing 
dummy Perf data. This flag works with every other flag.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -json --sample
```

#### Results:
```
{"sorting_algorithm": 
  [
    {"algorithmName": "Quick"
    ,"algorithmOption": "Random"
    ,"algorithmLength": 1000
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 0.130833
    ,"perfData": 
      {"task-clock": 364444262.000000
      , "context switches": 214.000000
      , "cycles": 595348804.490000
      , "kcycles": 170739620.860000
      , "instructions": 1009631347.960000
      , "L1-misses": 5583046.540000
      , "LLC-misses": 897165.960000
      , "branch predictions": 182104579.700000
      , "branch-misses": 3082415.940000
      , "cache references": 5131660.310000
      , "retired branch instructions": 161551834.080000
      , "total page faults": 10020.000000
      , "minor page faults": 10020.000000
      , "major page faults": 0.000000
      , "context switches": 214.000000
      , "scale": 1.000000
      , "IPC": 1.700000
      , "CPUs": 0.370000
      , "GHz": 1.630000}
    }
  ]
}
```

Notice that in the JSON response, a "PERF NOTE" is included to remind that the output does not contain actual Perf data, but instead only dummy data.

### Printing to a file ```-f [path_to_file]``` or ```--file [path_to_file]```

You can have the program print the results in a JSON formatted file. You just need to provide the path to the file 
including the file name itself. It is highly recommended to include the file extension as ```.json```, however, this 
isn't required and you can have the file ext be whatever you want or nonexistent. Regardless, it will always create 
the file as a JSON formatted text file. When you include this flag, nothing will be written to STDOUT.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -json -p -f ./test
```

#### Results:

**No STDOUT**

### Printing to a file *and* printing to STDOUT via Verbose mode ```-v``` or ```--verbose```

If you want the program to print the results to a JSON formatted file and have it output the results to STDOUT, then 
you should include the Verbose flag. However, the Verbose flag will provide you with more than what the first few 
examples have shown.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -json -p -f ./test -v
```

#### Results:

```
Sorting Algorithm algorithm: quick array_length: 1000 strategy: 1 language: c++ name: 
Warning: PERF is not supported on this system. PERF functionality is only available on Linux distributions. The program will continue to run, but any PERF data will be replaced with sample (dummy) values. To avoid this warning, remove the 'p' or 'perf' option from your arguments, or add --sample to explicitly request dummy data.
C++ Starting sort: "Quick"...
Starting PERF
Stopping PERF it ran for: 0
C++ Verifying sort: "Quick"...
C++ Sort: "Quick" Verified!
C++ Sort "Quick"with Algorithm Option "Random" of length 1000, completed in 0.163458 milliseconds{"task-clock": 364444262.000000, "context switches": 214.000000, "cycles": 595348804.490000, "kcycles": 170739620.860000, "instructions": 1009631347.960000, "L1-misses": 5583046.540000, "LLC-misses": 897165.960000, "branch predictions": 182104579.700000, "branch-misses": 3082415.940000, "cache references": 5131660.310000, "retired branch instructions": 161551834.080000, "total page faults": 10020.000000, "minor page faults": 10020.000000, "major page faults": 0.000000, "context switches": 214.000000, "scale": 1.000000, "IPC": 1.700000, "CPUs": 0.370000, "GHz": 1.630000}
{"sorting_algorithm": [{"algorithmName": "Quick","algorithmOption": "Random","algorithmLength": 1000,"language": "C++","algorithmCanonicalName": "","algorithmRunTime_ms": 0.163458, "perfData": {"task-clock": 364444262.000000, "context switches": 214.000000, "cycles": 595348804.490000, "kcycles": 170739620.860000, "instructions": 1009631347.960000, "L1-misses": 5583046.540000, "LLC-misses": 897165.960000, "branch predictions": 182104579.700000, "branch-misses": 3082415.940000, "cache references": 5131660.310000, "retired branch instructions": 161551834.080000, "total page faults": 10020.000000, "minor page faults": 10020.000000, "major page faults": 0.000000, "context switches": 214.000000, "scale": 1.000000, "IPC": 1.700000, "CPUs": 0.370000, "GHz": 1.630000}}]}
Results written in JSON at: './test'
```

Note: The Verbose flag also works with or without the File flag and also works on the JSON flag as well.

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

See the [open issues](https://github.com/jzdegrey/AlgoGauge/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

If you would like to contribute to this project, please create a new branch from either main or development and 
create a pull request. ***NEVER*** commit and push directly to main!!!

1. Create your Feature Branch from Development or Main
2. Commit and push your Changes
3. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- AUTHORS -->
## Authors

#### Weber State University - School of Computing

- Dr. Brad Peterson
- Coleton Watt 
- Cooper Maitoza
- Brody Gardner 
- John Z. DeGrey

<p align="right">(<a href="#readme-top">back to top</a>)</p>
