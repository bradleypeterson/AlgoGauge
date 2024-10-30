# MultiLanguage

## Desc

This explains the steps need to create another language and how to go about it. You command line interface (CLI) tool should only ever expect one algorithm to be sent at a time but could optionally allow multiple.

## Breakdown

### Options

The following options need to be implemented.

#### Implicit options

Implicit means if the value is passed ie --output or -o you should assume that that means output=true

`-o --output <bool>` When output is true the CLI tool should print the original and sorted array if verbose is turned on ie: Original Array:[]\n Sorted Array:[]

`-v --verify <bool>` Verifies whether the array was correctly sorted should print and error to stderr on failed sort. Done by comparing i < k

`-j, --json <bool>`, returns a string json you may assume that if -j is passed it should be set as true

`--ignore` due to how c++ calls the function and implicit arguments this is used to pass an argument that doesn't do anything

#### Algorithm options

`-a --algorithm <string>` Specifics the sorting algorithm to be ran. Needs to at least support ["default", "bubble", "selection", "insertion", "quick", "merge", "heap"] for languages that have a built in sort method default should be that otherwise use set to bubble.

`-l, --length <int>`, The number of elements being that are to be sorted. Default: 100

`-s, --strategy <string>` The array strategy being used to generate the number. Needs to at least support ["random", "chunk", "repeating", "sorted", "reverse_sorted"]

`-m, --max <int>`, The largest number that can be randomly selected. Default: UINT32

if you support multiple algorithms at once you can choose how the JSON gets returned however C++ which will only ever call one algorithm is expecting the following results: `{algorithmName": "${capitalizeFirstLetter(algorithm)}","algorithmOption": "${capitalizeFirstLetter(strategy)}","algorithmLength": ${length},"language": "NodeJS", "verified": ${+correct},"algorithmCanonicalName": "${name ?? ""}","algorithmRunTime_ms": ${timeTaken}, "perfData": {}}` ex: `{algorithmName": "Heap","algorithmOption": "Random","algorithmLength": 200,"language": "NodeJS", "verified": 1,"algorithmCanonicalName": "","algorithmRunTime_ms": 0.31491700000000833, "perfData": {}}` Note verified is an int not a boolean True = 1, false = 2. Verified is whether the algorithm sorted correctly. Time taken is in milliseconds and rounded or fixed at the 6th decimal place. algorithmName and algorithmOption should both have the first letter capitalized. Any number does not have quotes around "" however, words should both keys and values.

Other options may be added including the ability to select what the chunk size should be or set the repeating number. You could also allow it the ability to sort things besides numbers.

### Algorithms

The following algorithms need to be implemented ["default", "bubble", "selection", "insertion", "quick", "merge", "heap"]. To create these you should use c++ as a base and "copy" the algorithms into the desired language. For languages that have a built in sort method default should be the built in method.

### Array Creation

For array creation the following creation strategies need to be created ["random", "chunk", "repeating"]

-Random: A random number between 0...Max. A completely random array.
-Repeating: Select a random number and repeat for the entire size of the array. [1,1...1]
-Chunk: Select a random number and create chunks of size 5 (could optionally allow the user to select it). 5 random numbers that are the same number followed by 5 different numbers.
-Ordered: Make an array that starts at 1 and counts to length. [1,2...length]
-Reversed: Is an ordered array but reversed: [length,length-1...1]

## Compiled Languages

The complier instructions should be added to the makefile.

## Interpreted Languages
