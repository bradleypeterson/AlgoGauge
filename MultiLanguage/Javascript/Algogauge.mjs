import { Command, Argument, Option } from "commander";
import {randomChunkArray, repeatingValueArray, fullRandomArray} from "./arrayCreation.mjs";

import { exit, stdin, stdout } from "process";
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
	"built-in",
	"bubble",
	"selection",
	"insertion",
	"quick",
	"merge",
	"heap",
];

const arrayOption = ["random", "repeating", "chunk"];
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
	.command("run")
	.description(
		"Runs a given sort algorithm expecting array strategy and count to be given"
	)
	.addArgument(
		new Argument("<algorithm>", "Select sort algorithm").choices(
			supportedAlgorithms
		)
	)
	.requiredOption("-c, --count <int>", "the number of", 100)
	.addOption(
		new Option("-s, --strategy <string>", "the array creation method")
			.default("random")
			.choices(arrayOption)
			.makeOptionMandatory()
	)
	.requiredOption("-m, -max <int>", "the largest number the array could have optional", Number.MAX_SAFE_INTEGER)
	.action((algorithm, options) => {
		Algorithm = algorithm;
		ArrayDetails.Count = options.count;
		ArrayDetails.Strategy = options.strategy
	});

program
	.command("sort")
	.description(
		"Runs a given sort algorithm expecting array to come from stdin"
	)
	.addArgument(
		new Argument("<algorithm>", "Select sort algorithm").choices(
			supportedAlgorithms
		)
	)
	// .option(
	// 	"-n, --name <string>",
	// 	"The name of what you are running, doesn't do anything right now"
	// )
	.action((algorithm, options) => {
		Algorithm = algorithm;
		// sorting.Name = options.name;
	});

program.parse();



const options = program.opts();
let sortingCommand = null
switch (Algorithm) {
	case ("built-in", "default"):
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

if(program.args.includes("run")){
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

	if(options.verify){
		const correct = verifySort(sortedArray)
		stdout.write(`${Algorithm.toUpperCase()} ${correct ? "sorted correctly" : "AHHH!!!!"} \n`)
	}

	
	
	exit()
}

// Read input from stdin
let input = "";
stdin.on("data", (chunk) => {
	input += chunk;
});

stdin.on("end", () => {
	const array = JSON.parse(input);
	if (options.output) {
		stdout.write(JSON.stringify(array));
	}

	const sortedArray = sortingCommand(array)

	// Output the sorted array as JSON to stdout
	if (options.output) {
		stdout.write(JSON.stringify(sortedArray));
	}
	if(options.verify){
		const correct = verifySort(sortedArray)
		stdout.write(`${Algorithm.toUpperCase()} ${correct ? "sorted correctly" : "AHHH!!!!"} \n`)
	}
});

