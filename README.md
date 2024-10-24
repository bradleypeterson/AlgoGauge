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
cmake ..
```

_next steps vary slightly depending on your OS_
#### Windows OS:

```shell
cmake --build .
.\AlgoGauge.exe -h
```

#### Unix OS:

```shell
cmake --build .
./AlgoGauge -h
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Please use ```./AlgoGauge -h``` to see the help message along with options.

### Usage Help

#### Basic Usage

```shell
AlgoGauge --algo (some algorithm) --len [some int > 0][OPTIONALS: -r | -e | -c | -s | -o] [-f "some file", -v, -j, -i]
```

#### Algorithm Name and Length [REQUIRED] options:

##### -a, --algorithm 
Name of the first algorithm to run. Supported algorithms include: _bubble, selection, insertion, quick, merge, heap._

##### -l, --length 
Number of items to process. Provide an int value between 0 and 4,294,967,295 (UNSIGNED INT)

##### -n, --name 
A canonical name that will be returned in output if provided. (default: "")

#### Algorithm Options [ONE REQUIRED] options:

##### -r, --random [=arg(=true)]
Generated data will be a random set (default: false)

##### -e, --repeated [=arg(=true)]
Generated data set will have repeated values (default: false)

##### -c, --chunks [=arg(=true)]
Generated data set will have various subsets that will contain both random and in order values (default: false)

##### -s, --reversed [=arg(=true)]
Generated data set will be in reverse order e.g. (9-0) (default: false)

##### -o, --ordered [=arg(=true)]
Generated data set will be ordered e.g.(0-9) (default: false)

#### Program Output [OPTIONAL] options:

##### -f, --file
File path must include the file name (extension not required). Regardless of file extension, file content's
structure will always be JSON. The file path and name to output the file to (default: "")

##### -j, --json
Prints the output as a json formatted object.

#### Program Settings [OPTIONAL] options:

##### -v, --verbose
Runs the program in Verbose mode

##### -i, --include-values
Will include values in the output. It is highly recommended to use a small length (less than 100).

##### -p, --perf
Includes Perf data in the output. Actual Perf data only works on Linux.
If you are not on Linux and want to use this anyways, you can set this to "sample". e.g. --perf=sample

##### -h, --help
Prints this help page.

### Current Available Algorithms

This is a list of Algorithms you can pass as values to the Algo flag:

- bubble
- selection
- insertion
- quick
- merge
- heap

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Examples

**All examples will be shown for a Unix Shell. For Windows, use ```.\AlgoGauge.exe``` instead of ```./AlgoGauge```**

### Running a single Algorithm using *Quick* sort with *100,000* *Random* values

```shell
./AlgoGauge --algo quick --len 100000 -r
```
#### Results:

```Sort 'Quick' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds```

#### Explanation:
We choose the 'Quick' sorting Algorithm by first including the flag ```--algo``` and then the string 'quick' afterwords.
We then indicate the number of values by including the flag ```--len``` and then the value '100000'. Finally, we
indicate that the values should be random by including the flag ```-r```. Variants for the Algorithm include ```-a```, 
```--algo``` or ```--algorithm```. Variants for the Length flag include ```-l```, ```--len``` or ```--length```.

### Passing multiple Algorithms at the same time

```shell
./AlgoGauge --algo quick --len 100000 -r --algo bubble --len 100000 -r --algo quick --len 25000 -o
```

#### Results:

```
Sort 'Quick' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Bubble' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Quick' with Algorithm Option 'Ordered' of length 25000, completed in x.xxxxxx milliseconds
```

#### Explanation:
We can pass in **n** number of algorithms to the program by specifying the different flags multiple times. You can also
specify the flags grouped together. All that matters is the order in which they occur (see next example). When you pass 
in multiple algorithms to run, they do ***NOT*** run in parallel. They run sequentially in order to produce the most 
accurate results possible.

### Passing multiple Algorithms at the same time, flags grouped together

```shell
./AlgoGauge --algo quick --algo bubble --algo quick --len 100000 --len 100000 --len 25000 -rro
```

#### Results:

```
Sort 'Quick' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Bubble' with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Quick' with Algorithm Option 'Ordered' of length 25000, completed in x.xxxxxx milliseconds
```

#### Explanation:

This outputs the same as the previous example. You can place the flags anywhere you want, the only thing that matters
is that you specify the correct number of flags and the order they are placed in. E.g. if we want to run Random Quick
 with length of 1000 and Random Bubble with length of 2500, specifying the flags as: 
```--algo quick --algo bubble -rr --len 2500 --len 1000``` would be incorrect and tell the program to run quick 
with 2500 random values instead of 1000 and bubble to run 1000 instead of 2500.

### Naming an Algorithm ```-n``` or ```--name```

You can pass in any name to an Algorithm that will be mapped to it and included in the results. This is useful when 
you have many algorithms you pass in to run and want to an easy way to keep track of each passed algorithm.

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name second -r --algo quick --len 25000 --name third -o
```

#### Results:

```
Sort 'Quick' <first> with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Bubble' <second> with Algorithm Option 'Random' of length 100000, completed in x.xxxxxx milliseconds
Sort 'Quick' <third> with Algorithm Option 'Ordered' of length 25000, completed in x.xxxxxx milliseconds
```

### JSON Results ```-j``` or ```--json```

You can specify whether the results should be printed out in JSON format instead of the human-readable versions like 
the previous examples. Note: the JSON flag ***only*** pertains to STDOUT. 

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name final -rj
```

#### Results:

```
{"algorithms": [{"algorithmName": "Quick","algorithmOption": "Random","algorithmLength": 100000,"algorithmCanonicalName": "first","algorithmRunTime_ms": x.xxxxxx},{"algorithmName": "Bubble","algorithmOption": "Random","algorithmLength": 100000,"algorithmCanonicalName": "final","algorithmRunTime_ms": x.xxxxxx}]}
```

### Perf Values ```-p``` or ```--perf```

To include Perf values, you ***MUST*** be on Linux machine and provide the Perf argument.

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name final -rjp
```

#### Results:
```
{"algorithms": [{"algorithmName": "Quick","algorithmOption": "Ordered","algorithmLength": 1000,"algorithmCanonicalName": "first","algorithmRunTime_ms": x.xxxxxx, "perfData": {"cpu cycles": xxxx, "bus cycles": xxxx, "cpu instructions": xxxx, "cache references": xxxx, "cache misses": xxxx, "branch predictions": xxxx, "retired branch instructions": xxxx, "branch misses": xxxx, "total page faults": xxxx, "minor page faults": xxxx, "major page faults": xxxx, "context switches": xxxx, "L1 data cache read accesses": xxxx, "L1 instruction cache read accesses": xxxx, "L1 data cache prefetch accesses": xxxx, "L1 instruction cache prefetch accesses": xxxx}},{"algorithmName": "Bubble","algorithmOption": "Random","algorithmLength": 1000,"algorithmCanonicalName": "final","algorithmRunTime_ms": x.xxxxxx, "perfData": {"cpu cycles": xxxx, "bus cycles": xxxx, "cpu instructions": xxxx, "cache references": xxxx, "cache misses": xxxx, "branch predictions": xxxx, "retired branch instructions": xxxx, "branch misses": xxxx, "total page faults": xxxx, "minor page faults": xxxx, "major page faults": xxxx, "context switches": xxxx, "L1 data cache read accesses": xxxx, "L1 instruction cache read accesses": xxxx, "L1 data cache prefetch accesses": xxxx, "L1 instruction cache prefetch accesses": xxxx}}]}
```

### Perf Values for *Non-Linux* Machines ```--perf=sample```

If you are not on a Linux machine, but will be deploying this to a Linux machine eventually and will be needing Perf data, 
but you need to test and debug on a non-Linux machine, you can include the ```--perf=sample``` argument which mimics the Perf output by providing 
dummy Perf data. This flag works with every other flag.

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name final -rj --perf=sample
```

#### Results:
```
{"algorithms": [{"algorithmName": "Quick","algorithmOption": "Ordered","algorithmLength": 1000,"algorithmCanonicalName": "first","algorithmRunTime_ms": x.xxxxxx, "perfData": {"PERF NOTE": "INCLUDED DATA IS DUMMY DATA!", "cpu cycles": xxxx, "bus cycles": xxxx, "cpu instructions": xxxx, "cache references": xxxx, "cache misses": xxxx, "branch predictions": xxxx, "retired branch instructions": xxxx, "branch misses": xxxx, "total page faults": xxxx, "minor page faults": xxxx, "major page faults": xxxx, "context switches": xxxx, "L1 data cache read accesses": xxxx, "L1 instruction cache read accesses": xxxx, "L1 data cache prefetch accesses": xxxx, "L1 instruction cache prefetch accesses": xxxx}},{"algorithmName": "Bubble","algorithmOption": "Random","algorithmLength": 1000,"algorithmCanonicalName": "final","algorithmRunTime_ms": x.xxxxxx, "perfData": {"PERF NOTE": "INCLUDED DATA IS DUMMY DATA!", "cpu cycles": xxxx, "bus cycles": xxxx, "cpu instructions": xxxx, "cache references": xxxx, "cache misses": xxxx, "branch predictions": xxxx, "retired branch instructions": xxxx, "branch misses": xxxx, "total page faults": xxxx, "minor page faults": xxxx, "major page faults": xxxx, "context switches": xxxx, "L1 data cache read accesses": xxxx, "L1 instruction cache read accesses": xxxx, "L1 data cache prefetch accesses": xxxx, "L1 instruction cache prefetch accesses": xxxx}}]}
```

Notice that in the JSON response, a "PERF NOTE" is included to remind that the output does not contain actual Perf data, but instead only dummy data.

### Printing to a file ```-f [path_to_file]``` or ```--file [path_to_file]```

You can have the program print the results in a JSON formatted file. You just need to provide the path to the file 
including the file name itself. It is highly recommended to include the file extension as ```.json```, however, this 
isn't required and you can have the file ext be whatever you want or nonexistent. Regardless, it will always create 
the file as a JSON formatted text file. When you include this flag, nothing will be written to STDOUT.

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name final -r --file ./results.json
```

#### Results:

**No STDOUT**

### Printing to a file *and* printing to STDOUT via Verbose mode ```-v``` or ```--verbose```

If you want the program to print the results to a JSON formatted file and have it output the results to STDOUT, then 
you should include the Verbose flag. However, the Verbose flag will provide you with more than what the first few 
examples have shown.

```shell
./AlgoGauge --algo quick --len 100000 --name first -r --algo bubble --len 100000 --name final -r --file ./results.json -v
```

#### Results:

```
Starting sort: "Quick"<first> ...
Verifying sort: "Quick"<first> ...
Sort: "Quick"<first> Verified!
Sort 'Quick' <first> with Algorithm Option 'Random' of length 10000, completed in x.xxxxxx milliseconds
Starting sort: "Bubble"<final> ...
Verifying sort: "Bubble"<final> ...
Sort: "Bubble"<final> Verified!
Sort 'Bubble' <final> with Algorithm Option 'Random' of length 10000, completed in x.xxxxxx milliseconds
Results written in JSON at: './results.json'
```

Note: The Verbose flag also works with or without the File flag and also works on the JSON flag as well.

### Include values ```-i``` or ```--include-values```

If you want to see the values before and after running the algorithm, pass in the Include-Values flag. It will only 
include the first and last values if the list of values is too long to output.

***IMPORTANT: IF PASSING IN THE JSON OR FILE FLAG, THE INCLUDE-VALUES FLAG WILL INCLUDE ALL VALUES. IT IS EXTREMELY 
NOT RECOMMENDED TO PASS IN THIS FLAG IF PASSING IN A LENGTH MORE THAN 100!*** 

```shell
./AlgoGauge --algo quick --len 50 -ri
```

#### Results

```Values before sort:
2412496532, 3119216746, 1495923606, 3931352601, 26313293, 2552602825, 3745457912, 2213446826, 4119067789, 4188234190, 728322180, 2841381042, 1361173914, 2270545070, 3012213090, 3386551741, 1096517083, 318260028, 2467059067, 1416695198, 1451902384, 2507160589, 1023210396, 133024446, 738742523 ... , 3988278624, 4149187129, 84836057, 2807460976, 3347377057, 2052069331, 1443301765, 283741067, 1471137817, 59622745, 1781636238, 3614324154, 3896880493, 1215130632, 2798086528, 3952394013, 2450837735, 3642544339, 33621616, 2406532855, 1232451556, 3820123330, 3133462892, 4610458, 3016786978
Values after sort:
4610458, 26313293, 33621616, 59622745, 84836057, 133024446, 283741067, 318260028, 728322180, 738742523, 1023210396, 1096517083, 1215130632, 1232451556, 1361173914, 1416695198, 1443301765, 1451902384, 1471137817, 1495923606, 1781636238, 2052069331, 2213446826, 2270545070, 2406532855 ... , 2412496532, 2450837735, 2467059067, 2507160589, 2552602825, 2798086528, 2807460976, 2841381042, 3012213090, 3016786978, 3119216746, 3133462892, 3347377057, 3386551741, 3614324154, 3642544339, 3745457912, 3820123330, 3896880493, 3931352601, 3952394013, 3988278624, 4119067789, 4149187129, 4188234190
Sort 'Quick' with Algorithm Option 'Random' of length 50, completed in x.xxxxxx milliseconds
```

### Print everything!!! ```-j -v -f -i```
You can provide all program option flags at once (excluding the help flag). Perf flag is not included here as it is only 
available on Linux machines. However, you can add it here too if needed.

```shell
./AlgoGauge --algo quick --len 50 --name first -r --algo bubble --len 50 --name final -r --file ./results.json -vji
```

#### Results:

```
Starting sort: "Quick"<first> ...
Verifying sort: "Quick"<first> ...
Sort: "Quick"<first> Verified!
Sort 'Quick' <first> with Algorithm Option 'Random' of length 50, completed in x.xxxxxx milliseconds
Starting sort: "Bubble"<final> ...
Verifying sort: "Bubble"<final> ...
Sort: "Bubble"<final> Verified!
Sort 'Bubble' <final> with Algorithm Option 'Random' of length 50, completed in x.xxxxxx milliseconds
{"algorithms": [{"algorithmName": "Quick","algorithmOption": "Random","algorithmLength": 50,"algorithmCanonicalName": "first","valuesBeforeSort": [2412496532,3119216746,1495923606,3931352601,26313293,2552602825,3745457912,2213446826,4119067789,4188234190,728322180,2841381042,1361173914,2270545070,3012213090,3386551741,1096517083,318260028,2467059067,1416695198,1451902384,2507160589,1023210396,133024446,738742523,3988278624,4149187129,84836057,2807460976,3347377057,2052069331,1443301765,283741067,1471137817,59622745,1781636238,3614324154,3896880493,1215130632,2798086528,3952394013,2450837735,3642544339,33621616,2406532855,1232451556,3820123330,3133462892,4610458,3016786978],"valuesAfterSort": [4610458,26313293,33621616,59622745,84836057,133024446,283741067,318260028,728322180,738742523,1023210396,1096517083,1215130632,1232451556,1361173914,1416695198,1443301765,1451902384,1471137817,1495923606,1781636238,2052069331,2213446826,2270545070,2406532855,2412496532,2450837735,2467059067,2507160589,2552602825,2798086528,2807460976,2841381042,3012213090,3016786978,3119216746,3133462892,3347377057,3386551741,3614324154,3642544339,3745457912,3820123330,3896880493,3931352601,3952394013,3988278624,4119067789,4149187129,4188234190],"algorithmRunTime_ms": x.xxxxxx},{"algorithmName": "Bubble","algorithmOption": "Random","algorithmLength": 50,"algorithmCanonicalName": "final","valuesBeforeSort": [2412496532,3119216746,1495923606,3931352601,26313293,2552602825,3745457912,2213446826,4119067789,4188234190,728322180,2841381042,1361173914,2270545070,3012213090,3386551741,1096517083,318260028,2467059067,1416695198,1451902384,2507160589,1023210396,133024446,738742523,3988278624,4149187129,84836057,2807460976,3347377057,2052069331,1443301765,283741067,1471137817,59622745,1781636238,3614324154,3896880493,1215130632,2798086528,3952394013,2450837735,3642544339,33621616,2406532855,1232451556,3820123330,3133462892,4610458,3016786978],"valuesAfterSort": [4610458,26313293,33621616,59622745,84836057,133024446,283741067,318260028,728322180,738742523,1023210396,1096517083,1215130632,1232451556,1361173914,1416695198,1443301765,1451902384,1471137817,1495923606,1781636238,2052069331,2213446826,2270545070,2406532855,2412496532,2450837735,2467059067,2507160589,2552602825,2798086528,2807460976,2841381042,3012213090,3016786978,3119216746,3133462892,3347377057,3386551741,3614324154,3642544339,3745457912,3820123330,3896880493,3931352601,3952394013,3988278624,4119067789,4149187129,4188234190],"algorithmRunTime_ms": x.xxxxxx}]}
Results written in JSON at: './results.json'
```

<!-- ROADMAP -->
## Roadmap

### P1

- [ ] Add array, linked list options CRUD operation resize array
- [ ] Add Hashing comparisons
- [ ] Site cleanup go over site and fix bugs and pages BIG theta notation on all
  - [ ] clean card heavy layout (Experiment, History)
  - [ ] footer padding
  - [ ] global button / links for experiment operations
  - [ ] change mode dropdown
  - [ ] remove repeat text  
  - [ ] Implement experiment popup timer
  - [ ] Grid layout for multiple cards (Four cards per row max)
  - [ ] Toggle Dark Mode and Light Mode support
  - [ ] Better history layout ???
  - [ ] GEt rid of user input size ???
  - [ ] When a close education popup I want to go back to the experiment
- [ ] Instructor video and education overhaul
- [ ] Parallel vs Serial
- [ ] Validate Perf, find a way to run in unix, ways to measure l1 cache. *

### P2

- [ ] GPU algorithms, CUDA
- [ ] Variations of sort algorithms (quick sort, merge sort 1...k, selection)
- [ ] JAVA vs C++ vs RUST vs Python ???
- [ ] CPU vectorization algorithms
- [ ] Clear queue
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

See the [open issues](https://github.com/jzdegrey/AlgoGauge/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

If you would like to contribute to this project, please create a new branch from either main or development and 
create a pull request. ***NEVER*** commit and push directly to main or development!!!

1. Create your Feature Branch from Development or Main
2. Commit and push your Changes
3. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- AUTHORS -->
## Authors

#### Weber State University - School of Computing

- Dr. Brad Peterson
- John Z. DeGrey

<p align="right">(<a href="#readme-top">back to top</a>)</p>
