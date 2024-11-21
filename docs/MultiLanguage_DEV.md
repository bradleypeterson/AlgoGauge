# MultiLanguage

## Desc

This explains the steps need to create another language and how to go about it. You command line interface (CLI) tool for now should only expect one element to be passed at a time however it should still support multiple. All languages should act as mini version of AlgoGauge supporting most if not all attributes. To call the separate CLI tools [subprocess.h](https://github.com/sheredom/subprocess.h) was used to see more information at that look at the [subprocess.md](##sub). The reason for having c++ call all languages allowed us to do two major things one was having the CLI support choosing whatever language you wanted and running multiple at the same time. While this could easily be done on the frontend having it in one place helped. The other reason was PERF perf library in languages such as javascript and python are non existent to in to see more on PERF look at the PERF.md. When making the mini AlgoGauge CLI you don't need to directly work with PERF as c++ will do all of that. However, for languages such as rust and swift with c/c++ interoperability you should compare how it preforms when perf is run from c++ and when it is backed into the CLI.

## Creating the MINI AlgoGauge

Below is the setups that should be taken to create the mini languages

### Options

The following options need to be implemented.

#### Implicit options

Implicit means if the value is passed ie --output or -o you should assume that that means output=true

`-o --output <bool>` When output is true the CLI tool should print the original and sorted array if verbose is turned on ie: Original Array:[]\n Sorted Array:[]

`-v --verify <bool>` Verifies whether the array was correctly sorted should print and error to stderr on failed sort. Done by comparing i < k. Implicit

`-j, --json <bool>` returns a string json like string. Implicit

`-p, --perf <bool>` This is a way to know if perf is going to be used this is required in order to contain perf to a single function area. Implicit

`--ignore` due to how subprocess.h works we couldn't just pass in an empty string as that still gets consider a value getting passed in. Additionally implicit values made it complicated this is used to pass an argument that doesn't do anything.

#### Algorithm options

`-a --algorithm <string>` Specifics the sorting algorithm to be ran. Needs to at least support ["built_in", "bubble", "selection", "insertion", "quick", "merge", "heap"]. More details below

`-n, --number <int>`, The number of elements being that are to be sorted.

`-s, --strategy <string>` The array strategy being used to generate the number. Needs to at least support [random, repeated, chunks, sorted, sorted_reversed]. More details below

`-m, --max <int>`, The largest number that can be randomly selected. Default: UINT32

if you support multiple algorithms at once you can choose how the JSON gets returned however C++ which will only ever call one algorithm is expecting the following results: `{algorithmName": "${capitalizeFirstLetter(algorithm)}","algorithmOption": "${capitalizeFirstLetter(strategy)}","algorithmLength": ${number},"language": "Deno","algorithmCanonicalName": "${name ?? ""}","algorithmRunTime_ms": ${timeTaken}, "perfData": {}}` ex: `{algorithmName": "Heap","algorithmOption": "Random","algorithm": 200,"language": "Deno", "algorithmCanonicalName": "","algorithmRunTime_ms": 0.31491700000000833, "perfData": {}}`  Time taken is in milliseconds and rounded or fixed at the 6th decimal place. algorithmName and algorithmOption should both have the first letter capitalized. Any number does not have quotes around "" words however should this includes both keys and values.

Other options may be added including the ability to select what the chunk size should be or set the repeating number. You could also allow it the ability to sort things besides numbers.

### Algorithms

The following algorithms need to be implemented ["built_in", "bubble", "selection", "insertion", "quick", "merge", "heap"]. To create these you should use c++ as a base and "copy" the algorithms into the desired language. For languages that have a built in sort method built_in should be the built in method. More options are planned to be supported in the future.

### Array Creation

For array creation the following creation strategies need to be created [random, repeated, chunks, sorted, sorted_reversed]. I recommend looking at Javascript or C++ methods to see how these can be done

-Random: A random number between 0...Max. A completely random array.
-Repeating: Select a random number and repeat for the entire size of the array. [1,1...1]
-Chunk: Select a random number and create chunks of size 5 (could optionally allow the user to select it). 5 random numbers that are the same number followed by 5 different numbers.
-Ordered: Make an array that starts at 1 and counts to length. [1,2...number]
-Reversed: Is an ordered array but reversed: [number,number-1...1]

### PERF

Implementing PERF in your mini AlgoGauge is quite easy if you are choosing to have C++ call and run perf on it. The basic idea is if PERF is true print READY? and expect an input. Once input is passed back start the code you want to run PERF on. Once you are done print DONE! and expect an input. Once you get an input continue. For now you can only ever call READY? and DONE! once. Ideally that would be fixed in the future however, it isn't really a problem as c++ only calls one method at a time.

!Warning do not print anything between READY? and DONE!

#### Javascript

While we first attempted to use Deno we ran into a problem. When CMAKE complies the none c files get lost and it made it difficult to work with. Luckily [Deno](https://deno.com/) solves this. Deno allows us to compile javascript to a binary function AlgoGaugeJS. This is great because now we can move AlgoGaugeJS to the $PATH and make it usable in production. While at the time Deno isn't the only on that supports this we found the Buns binaries are significantly slower (buns creation of the binary is quite a bit faster however). Additionally if we ever move to typescript using Deno will make that transition quick. Deno uses a safety and security first approach which for now hasn't required use to use any special options for compiling but more options can be found here [Compile Docs](https://docs.deno.com/runtime/reference/cli/compiler/#compile-options). Deno does not fully support process however often they have drop in replacement that can be found in their documentation.

#### Python

In order to make python more like an complied language we used [pipx](https://github.com/pypa/pipx). pipx allows our AlgoGaugePY to run in an isolated environment meaning we don't need to worry about venv and virtual environments and it makes it easier to share. With pipx use of $PATH we can also call AlgoGaugePY from anywhere in the system with easy. This is especially important in production were AlgoGauge and AlgoGaugePY both need to reside in the $PATH. While I tried to compile python down to a single binary the state of python makes that impossible.
