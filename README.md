<a id="readme-top"></a>
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
    This program will compare one or many different algorithms and print out the results of the Algorithm. The purpose of this program is to help show case the differences between the many different sorting algorithms and to help
    visualize use cases for each (e.g. quick sort is great for random data, but not so great if the data is already sorted). In the future, this program will also provide more result data via Perf as well as have the ability to test hashing algorithm complexities.
  </p>
</div>

> [!Important]
> Windows is not officially support and is not easy to work with it is recommend to use WSL or a Linux VM instead
> If you would like to just run it please download the latest version from the releases tab

<!-- BREAKING CHANGES -->
## Breaking Changes

There is no output if `-j --json` is not passed

<!-- GETTING STARTED -->
## Getting Started

Before getting started, there are some prerequisites and information you need to know first.

### Repository Information

Use the main branch to link your node project to. If you need to make any changes to this for whatever reason, please
 check the <a href="https://github.com/jzdegrey/AlgoGauge/issues">Repo Issues tab</a> to see if your issue has already
 been reported. If not, then please create a 
<a href="https://github.com/jzdegrey/AlgoGauge/issues/new/choose">New issue</a>. If you do need to make changes to this
code, then you must check out and create a new branch from either main or Development Branch,
make your edits, commit and push them to your branch and then create a pull request. Only owners of this repo may authorize pull requests.

If you do want to add to this repo, please also read the [DEV-README](./docs/DEV-README.md#algogauge-cli)

### Prerequisites

The following prerequisites are required.

- [CMAKE](https://cmake.org/) for compiling C++
- [Deno](https://deno.com/) for compiling Javascript
- [PipX](https://pipx.pypa.io/) for making Python CLI tools easy

### Installation

#### Windows OS

You can ask a Linux user to build it for you if they run. Or you can attempt to use CMake and download make

```bash
make build_windows
```

You will also need to directly use pipx with python

They will need the mingw32 toolchain in there path

#### Linux and Mac OS

```shell
make 
make python_install
cd build
```

Pipx should install packages globally in there own isolated environment to check you can run.

 ```shell
 AlgogaugePY -h
 ```

AlgoGauge and AlgoGaugeJS can be found in build bin. AlgoGaugeJS will need to be moved to your path to be able to be ran from AlgoGauge (C++). The easiest way to do this is to run

```bash
sudo cp ./AlgoGaugeJS /usr/local/bin
```

To run on Unix/Mac OS (Calls help option `-h`)

```shell
./AlgoGauge -h
./AlgoGaugeJS -h
```

> [!IMPORTANT]
> LINUX Users
> In order for perf to work correctly you either need to run Algogauge with sudo or update perf_event_paranoid [PERF.md#Security](./docs/PERF.md#security)

##### Installation Production

To use AlgoGauge CLI in production both AlgogaugeJS and Algogauge need to be in your global path (by default pipx should have added AlgogaugePY to path already). There is a lot of different ways to this but for Linux OS you can just copy the file
to /usr/local/bin.

```shell
sudo cp ./AlgoGauge /usr/local/bin
sudo cp ./AlgoGaugeJS /usr/local/bin
```

To ensure it worked run

```shell
Algogauge -h
AlgogaugeJS -h
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Please use ```./AlgoGauge -h``` to see the help message along with options.

### Usage Help
---

#### Basic Usage

```txt
AlgoGauge -a (algorithm name) -s (algorithm strategy) -n (number of items to process) [-v, -j, -h]
```

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
  - `push_front`
  - `push_back`
  - `pop_front`
  - `pop_back`
  - `pushpop_back`
  - `pushpop_front`

##### -s, --strategy

Determines the strategy of loading values for sorting algorithms, the type of hash table, or the method of action for linked lists.

- Sorting:
  - `random`
  - `repeated`
  - `chunks`
  - `sorted`
  - `sorted_reversed`
- Hash Table:
  - `closed`
- Linked List:
  - `array`
  - `linked_list`

##### -n, --number

The number of items the algorithm will process. Provide an int value between `0` and `4294967295`

- Sorting: Size of Array to Sort
- Hash Table: Number of testing operations
- Linked List: Number of operations

#### Sorting Algorithm Specific [Required] options

##### -x, --language

What programming language will process the sorting algorithm

- Options:
  - `C++`
  - `Py/Python`
  - `JS/Javascript`

#### Sorting Algorithm [Optional] options

##### -i, --iterator

How much the number per iteration (default: 1) this needs to be used in combination with `--additional`

##### -e, --additional

The ending number for step operations (default: 0) this needs to be used in combination with `--iterator`

#### Linked List Specific [Required] options

##### -c, --capacity

How many elements can be stored in the Linked List. Provide a number (int > 0) that determines linked list size.

#### Hash Table Specific [Required] options

##### -c, --capacity

How many elements can be stored in the Hash Table. Provide a number (int > 0) that determines hash table size.

##### -d, --density

How full or the density of the starting hash table as a percentage. Provide an int value between 0 (0%) and 100 (100%) to load into hash table.

#### Program Output [OPTIONAL] options

##### -f, --file

File path must include the file name (extension not required). Regardless of file extension, file content's
structure will always be JSON. The file path and name to output the file to (default: "")

##### -j, --json

Prints the output as a json formatted object.

##### -y, --name

A canonical name that will be returned in output if provided.

#### Program Settings [OPTIONAL] options

##### -v, --verbose

Runs the program in Verbose mode

##### -o, --output

Will output the arrays in the output. It is highly recommended to use a small length (less than 100).

##### -p, --perf

Includes Perf data in the output. Actual Perf data only works on Linux.
If you are not on Linux and want to use this anyways, you can set this to "sample". e.g. `--sample`

> [!IMPORTANT]
> Perf may not work without sudo however this can be easily be fixed by following does not need sudo look at [PERF.md](./docs/PERF.md) to fix this

##### -h, --help

Prints this help page.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Examples

**All examples will be shown for a Unix Shell. For Windows, use ```.\AlgoGauge.exe``` instead of ```./AlgoGauge```**

### Running a single Algorithm using *Quick* sort with *100,000* *Random* values

```shell
./AlgoGauge --algorithm quick --strategy random --number 10000 --language c++ --json
```

#### Results:

```{"sorting_algorithm": [{"algorithmName": "Quick","algorithmOption": "Random","algorithmLength": 100000,"language": "C++","algorithmCanonicalName": "","algorithmRunTime_ms": 18.326541, "perfData": {}}]}```

### Passing multiple Algorithms at the same time

```shell
./Algogauge -a quick -s random -n 1000 -x c++ -j -a bubble -s random -n 1200 -x js -j
```

#### Results:

```
Sort 'Quick' with Algorithm Option 'Random' of length 1000, completed in x.xxxxxx milliseconds
Sort 'Bubble' with Algorithm Option 'Random' of length 1200, completed in x.xxxxxx milliseconds
```

### More examples of passing multiple Algorithms at the same time

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ -j -a merge -s random -n 1200 -x c++ -j
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
    ,"algorithmOption": "Bubble"
    ,"algorithmLength": 1200
    ,"language": "C++"
    ,"algorithmCanonicalName": ""
    ,"algorithmRunTime_ms": 1.910916
    , "perfData": {}}
  ]
}
```

### Using iterator and additional

```shell
./AlgoGauge --algorithm bubble --strategy random --number 1000 --language js -v --iterator 2 --additional 20 --name bubbleTesting
```

#### Results:

```txt
Sorting Algorithm algorithm: bubble array_length: 1000 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1002 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1004 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1006 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1008 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1010 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1012 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1014 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1016 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1018 strategy: random language: js name: bubbleTesting
Sorting Algorithm algorithm: bubble array_length: 1020 strategy: random language: js name: bubbleTesting
```

### JSON Results ```-j``` or ```--json```

You can specify whether the results should be printed out in JSON format instead of the human-readable versions like
the previous examples. Note: the JSON flag ***only*** pertains to STDOUT.

```shell
./AlgoGauge -a quick -s random -n 1000 -x c++ --json
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

To include Perf values, you ***MUST*** be on Linux machine to get back really PERF values. Otherwise you will get dummy data.

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
