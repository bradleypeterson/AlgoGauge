import { Command, Argument, Option } from "commander";
import {randomChunkArray, repeatingValueArray, fullRandomArray} from "./arrayCreation.mjs";

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

const UINT32_MAX = (1 << 31) >>> 0 | (1 << 31) - 1;


const supportedAlgorithms = [
	"default",
	"bubble",
	"selection",
	"insertion",
	"quick",
	"merge",
	"heap",
];

const arrayOptions = ["random", "repeating", "chunk"];
const program = new Command();
let Algorithm = ""
const ArrayDetails = {
	Count: 100,
	Strategy: "random",
	Max: Number.MAX_SAFE_INTEGER
};

program
	.name("AlgoGauge NodeJS")
	.description(
		"This is the command line version of the AlgoGauge Program written in Javascript."
	)
	.version("0.5.0")
	.usage("");

program
	.option("-v, --verify <bool>", "Verify mode", false)
	.option(
		"-o, --output <bool>",
		"Will output the before and after of the sorted array.",
		false
	);



//All options are required but default where given to all
program
	.description(
		"Runs a given sort algorithm expecting array strategy and count to be given"
	)
	.option("-a --algorithm <string>", "Select sort algorithm", "default")
	.requiredOption("-c, --count <int>", "the number of", 100)
	.option("-s, --strategy <string>", "the array creation method", "random")
	.option("-m, --max <int>", "the largest number the array could have optional", Number.MAX_SAFE_INTEGER)
	.action((options) => {
		Algorithm = options.algorithm.toLowerCase();

		if(!supportedAlgorithms.includes(Algorithm)){
			console.error(`error: option '-a --algo --algorithm <string>' argument '${Algorithm}' is invalid. Allowed choices are built-in, default, bubble, selection, insertion, quick, merge, heap.`);
			process.exit(1)
		}		
		ArrayDetails.Count = options.count;
		ArrayDetails.Strategy = options.strategy.
		toLowerCase()
		ArrayDetails.Max = options.max
		if(!arrayOptions.includes(ArrayDetails.Strategy)){
			console.error(`error: option '-s, --strategy <string>' argument '${ArrayDetails.Strategy}' is invalid. Allowed choices are random, repeating, chunk.`)
		}
	});

program.parse();

const options = program.opts();
let sortingCommand = null
switch (Algorithm) {
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
		console.log("No sort method found");
		exit();
}


	let array = []
	switch (ArrayDetails.Strategy){
		case "random":
			array = fullRandomArray(ArrayDetails.Count, ArrayDetails.Max)
			break
		case "chunk":
			array = randomChunkArray(ArrayDetails.Count, ArrayDetails.Max)
			break
		case "repeating":
			array = repeatingValueArray(ArrayDetails.Count, ArrayDetails.Max)
			break
		default:
			console.log("No Strategy was specified")
			exit()
	}
	if (options.output) {
		stdout.write(`Original Array: ${JSON.stringify(array)}\n`);
	}
	const sortedArray = sortingCommand(array)


	if (options.output) {
		stdout.write(`Sorted Array: ${JSON.stringify(sortedArray)}\n`);
	}

	verify: if(options.verify){
		const correct = verifySort(sortedArray)
		if(correct){
			console.log(`${Algorithm.toUpperCase()} sorted correctly\n`)
			break verify;
		}
		
		console.error(`${Algorithm.toUpperCase()} there was an error when sorting\n`)
	}

	
	
	exit(0)



