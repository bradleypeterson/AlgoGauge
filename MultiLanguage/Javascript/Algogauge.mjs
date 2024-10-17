import { Command, Argument, Option } from "commander";
import {randomChunkArray, repeatingValueArray, fullRandomArray, orderedArray, orderedArrayReversed} from "./arrayCreation.mjs";

import { exit, stderr, stdin, stdout } from "process";
import {
	builtinSorting,
	bubbleSort,
	heapSort,
	insertionSort,
	mergeSort,
	quickSort,
	selectionSort,
	verifySort,
} from "./sorting.mjs";
import {writeFile, appendFileSync} from "node:fs";

const UINT32_MAX = (1 << 31) >>> 0 | (1 << 31) - 1;

let Max_Number = Number.MAX_SAFE_INTEGER




const program = new Command();


const collect = (value, previous) => {
	return previous.concat([value.toLowerCase()]);
}

const writeToFileLocation = (line, file) => {
	if(file == ""){
		return;
	}

	appendFileSync(file, line, (err) => {
		if (err) throw err;
		console.log('The "data to append" was appended to file!');
	}); 
}

const runAlgorithm = (algorithm, strategy, count, name, options) => {
	let sortingCommand = null
	switch (algorithm) {
		case ("default"):
			sortingCommand = builtinSorting;
			break;
		case "merge":
			sortingCommand = mergeSort;
			break;
		case "bubble":
			sortingCommand = bubbleSort;
			break;
		case "selection":
			sortingCommand = selectionSort;
			break;
		case "insertion":
			sortingCommand = insertionSort;;
			break;
		case "quick":
			sortingCommand = quickSort;
			break;
		case "heap":
			sortingCommand = heapSort;
			break;
		default:
			console.error(`error: option '-a --algo --algorithm <string>' argument '${algorithm}' is invalid. Allowed choices are built-in, default, bubble, selection, insertion, quick, merge, heap.`);
			process.exit(1)
	}

	let array = []
	switch (strategy){
		case "random":
			array = fullRandomArray(count, Max_Number)
			break
		case "chunk":
			array = randomChunkArray(count, Max_Number)
			break
		case "repeating":
			array = repeatingValueArray(count, Max_Number)
			break
		case "repeating":
			array = repeatingValueArray(count, Max_Number)
			break
		case "ordered":
			array = orderedArray(count)
			break
		case "reversed":
			array = orderedArrayReversed(count)
			break
		default:
			console.error(`error: option '-s, --strategy <string>' argument '${strategy}' is invalid. Allowed choices are random, chunk, repeating, ordered, reversed.`);
			exit()
	}

	if (options.output) {
		stdout.write(`Original Array: ${JSON.stringify(array)}\n`);
	}
	const start = performance.now();
	const sortedArray = sortingCommand(array)

    const timeTaken = performance.now() - start;

	if (options.output) {
		stdout.write(`Sorted Array: ${JSON.stringify(sortedArray)}\n`);
	}

	verify: if(options.verbose){
		const correct = verifySort(sortedArray)
		if(correct){
			console.log(`${algorithm.toUpperCase()} sorted correctly\n`)
			break verify;
		}
		
		console.error(`${algorithm.toUpperCase()} there was an error when sorting\n`)
	}

	return `{"algorithmName": "${algorithm}","algorithmOption": "${strategy}","algorithmLength": ${count},"language": "javascript", "algorithmCanonicalName": "${name ?? ""}","algorithmRunTime_ms": ${timeTaken}, "perfData": {}}`
}

 const clearFile = async () => {
	writeFile('../temp/javascript.txt', "", (err) => {
		if (err) throw err;
		console.log('The "data to append" was appended to file!');
	});
}


program
	.name("AlgoGauge NodeJS")
	.description(
		"This is the command line version of the AlgoGauge Program written in Javascript."
	)
	.version("0.5.0")
	.usage("");

program
	.option('-v, --verbose [bool]', 'Verify mode', (value) => {
	  return value.toLowerCase() === 'true'; // Convert string to boolean
	}, false)
  
	.option('-o, --output [bool]', 'Will output the before and after of the sorted array.', (value) => {
	  return value.toLowerCase() === 'true';
	}, false)
  
	.option('-j, --json [bool]', 'Will output the before and after of the sorted array.', (value) => {
	  return value.toLowerCase() === 'true';
	}, false);


//All options are required but default where given to all
program
	.description(
		"Runs a given sort algorithm expecting array strategy and count to be given"
	)
	.option("-f, --file <string>", "The save location for json, json needs to be true", "")
	.option("-a --algorithm <algo>", "Select sort algorithm", collect, [])
	.requiredOption("-c, --count [int]", "the number of", collect, [])
	.option("-s, --strategy <string>", "the array creation method", collect, [])
	.option("-n, --name <string>", "optional the Canonical Name", collect, [])
	.option("-m, --max [int]", "the largest number the array could have optional", Number.MAX_SAFE_INTEGER)
	.action((options) => {		
		Max_Number = options.max
	});


program.parse();
const options = program.opts();


if(options.file != ""){
	let clearFilePromise =  clearFile();
}

if (options.algorithm.length != options.count.length || 
options.algorithm.length != options.strategy.length || 
options.algorithm.length == 0) {     
        throw console.error("Number of provided algorithm(s), count(s), language(s), and strategy(s) arguments do not match!");
		process.exit(1)
    }

let jsonResults = "";
for(let i = 0; i < options.algorithm.length; i++){
	jsonResults += (runAlgorithm(options.algorithm[i],options.strategy[i], options.count[i], options.name[i], options) + ",\n")
}

if(options.file != "" && options.json){
	writeToFileLocation(jsonResults, options.file)
}

if(options.json){
	console.log(jsonResults);
}


	

	
exit(0)