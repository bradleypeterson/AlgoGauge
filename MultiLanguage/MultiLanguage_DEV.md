# MultiLanguage

## Desc

This explains the steps need to create another language and how to go about it. You command line interface (CLI) tool should only ever expect one algorithm to be sent at a time but could optionally allow multiple.

## Breakdown

### Options

The following options need to be implemented.

`-o --output <bool>` When output is true the CLI tool should print the original and sorted array ie: Original Array:[]\n Sorted Array:[]

`-v --verify <bool>` Verifies whether the array was correctly sorted should print and error to stderr on failed sort. Done by comparing i < k

`-a --algorithm <string>` Specifics the sorting algorithm to be ran. Needs to at least support ["default", "bubble", "selection", "insertion", "quick", "merge", "heap"] for languages that have a built in sort method default should be that otherwise use set to bubble. 

`-c, --count <int>`, The number of elements being that are to be sorted. Default: 100

`-s, --strategy <string>` The array strategy being used to generate the number. Needs to at least support ["random", "chunk", "repeating", "sorted", "reverse_sorted"]

`-m, --max <int>`, The largest number that can be randomly selected. Default: UINT32

Other options may be added including the ability to select what the chunk size should be or set the repeating number. You may also assume if -o is given output=true however that is not required. You could also allow it the ability to sort things besides number.

### Algorithms

The following algorithms need to be implemented ["default", "bubble", "selection", "insertion", "quick", "merge", "heap"]. To create these you should use c++ as a base and "copy" the algorithms into the desired language. For languages that have a built in sort method default should be the built in method.

### Array Creation

For array creation the following creation strategies need to be created ["random", "chunk", "repeating"]

-Random: A random number between 0...Max.
-Repeating: Select a random number and repeat for the entire size of the array.
-Chunk: Select a random number and create chunks of size 5 (could optionally allow the user to select it).


## Compiled Languages

The complier instructions should be added to the makefile.

## Interpreted Languages
