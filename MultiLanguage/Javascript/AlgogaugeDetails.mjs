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

const UINT32_MAX = Math.pow(2, 32) - 1; 

const capitalizeFirstLetter = (string) => {
    return string.charAt(0).toUpperCase() + string.slice(1);
}


export class SortingAlgorithm {
	timeTaken = null;
	sortedArray = null;
	constructor(algorithm, strategy, length, name, max) {
		this.algorithm = capitalizeFirstLetter(algorithm);
		this.strategy = capitalizeFirstLetter(strategy);
		this.length = length;
		this.name = name ?? "";
		this.max = max ?? UINT32_MAX;

		switch (algorithm) {
			case ("default"):
				this.sortingCommand = builtinSorting;
				break;
			case "merge":
				this.sortingCommand = mergeSort;
				break;
			case "bubble":
				this.sortingCommand = bubbleSort;
				break;
			case "selection":
				this.sortingCommand = selectionSort;
				break;
			case "insertion":
				this.sortingCommand = insertionSort;;
				break;
			case "quick":
				this.sortingCommand = quickSort;
				break;
			case "heap":
				this.sortingCommand = heapSort;
				break;
			default:
				console.error(`error: option '-a --algo --algorithm <string>' argument '${algorithm}' is invalid. Allowed choices are built-in, default, bubble, selection, insertion, quick, merge, heap.`);
				process.exit(1);
		}

		switch (strategy){
			case "random":
				this.array = fullRandomArray(length, max)
				break
			case "chunk":
				this.array  = randomChunkArray(length, max)
				break
			case "repeating":
				this.array  = repeatingValueArray(length, max)
				break
			case "repeating":
				this.array  = repeatingValueArray(length, max)
				break
			case "ordered":
				this.array  = orderedArray(length)
				break
			case "reversed":
				this.array  = orderedReversedArray(length)
				break
			default:
				console.error(`error: option '-s, --strategy <string>' argument '${strategy}' is invalid. Allowed choices are random, chunk, repeating, ordered, reversed.`);
				exit(1)
		}
	}
}