<a name="readme-top"></a>
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/jzdegrey/AlgoGauge">
    <img src="https://catalog.weber.edu/mime/media/22/2687/east_horiz.png" alt="Weber State University Logo">
  </a>

<h3 align="center">AlgoGauge CLI</h3>


  <p align="center">
    The CLI version of Dr. Brad Peterson's AlgoGauge Tenure project
    <br />
    <a href="https://github.com/jzdegrey/AlgoGauge/issues"><strong>Known Issues and Bugs »</strong></a>
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
cmake --build ..
./AlgoGauge -h
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Please use ```shell ./AlgoGauge -h``` to see the help message along with options.

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
You can provide all program option flags at once (excluding the help flag)

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

- [ ] Add Perf options
- [ ] Add Tim sort Algorithm
- [ ] Add Hashing comparisons
- [ ] Fix memory SEGFAULT errors
- [ ] Finish documenting code

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