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

import {SortingAlgorithm} from "./AlgogaugeDetails.mjs"


export const runSortingAlgorithm = (algo) => {
	const start = performance.now();
	algo.sortedArray = algo.sortingCommand(algo.array)
	algo.timeTaken = performance.now() - start;
}

