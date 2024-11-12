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
import {randomChunkArray, repeatingValueArray, fullRandomArray, orderedArray, orderedReversedArray} from "./arrayCreation.mjs";

const capitalizeFirstLetter = (string) => {
    return string.charAt(0).toUpperCase() + string.slice(1);
}

export const runAlgorithm = (algorithm, strategy, length, name, verbose, output, max) => {
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
	const runningAlgorithm = capitalizeFirstLetter(algorithm);

	let array = []
	switch (strategy){
		case "random":
			array = fullRandomArray(length, max)
			break
		case "chunk":
			array = randomChunkArray(length, max)
			break
		case "repeating":
			array = repeatingValueArray(length, max)
			break
		case "repeating":
			array = repeatingValueArray(length, max)
			break
		case "ordered":
			array = orderedArray(length)
			break
		case "reversed":
			array = orderedReversedArray(length)
			break
		default:
			console.error(`error: option '-s, --strategy <string>' argument '${strategy}' is invalid. Allowed choices are random, chunk, repeating, ordered, reversed.`);
			exit(1)
	}
	if(verbose) console.log(`NodeJS Starting sort: \"${runningAlgorithm}\"`);

	if (output && verbose) console.log(`NodeJS Original Array: ${JSON.stringify(array)}`);

	const start = performance.now();
	const sortedArray = sortingCommand(array)

    const timeTaken = performance.now() - start;

	if (output && verbose) console.log(`NodeJS Sorted Array: ${JSON.stringify(sortedArray)}`);

	if(verbose) console.log(`NodeJS Verifying sort: \"${runningAlgorithm}\"`);

	const correct = verifySort(sortedArray)

	if(!correct){
		console.error(`${runningAlgorithm} there was an error when sorting`)
	}else{
		if(verbose) console.log(`NodeJS Sort: \"${runningAlgorithm}\" Verified!`);
		
	}


	if(verbose) console.log(`NodeJS Sort: \"${runningAlgorithm}\" with Algorithm Option \"${capitalizeFirstLetter(strategy)}\" of length ${length}, completed in ${timeTaken} milliseconds`);

	//"verified": ${+correct}
	return `{"algorithmName": "${runningAlgorithm}","algorithmOption": "${capitalizeFirstLetter(strategy)}","algorithmLength": ${length},"language": "NodeJS","algorithmCanonicalName": "${name ?? ""}","algorithmRunTime_ms": ${timeTaken.toFixed(6)}, "perfData": {}}`
}
