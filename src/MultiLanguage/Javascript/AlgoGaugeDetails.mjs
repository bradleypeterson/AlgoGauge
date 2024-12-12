import {
  bubbleSort,
  builtinSorting,
  heapSort,
  insertionSort,
  mergeSort,
  quickSort,
  selectionSort,
} from "./sorting.mjs";
import {
  fullRandomArray,
  orderedArray,
  orderedReversedArray,
  randomChunkArray,
  repeatingValueArray,
} from "./arrayCreation.mjs";

const UINT32_MAX = Math.pow(2, 32) - 1;

const capitalizeFirstLetter = (string) => {
  return string.charAt(0).toUpperCase() + string.slice(1);
};

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
        this.sortingCommand = insertionSort;
        break;
      case "quick":
        this.sortingCommand = quickSort;
        break;
      case "heap":
        this.sortingCommand = heapSort;
        break;
      default:
        throw console.error(
          `error: option '-a --algo --algorithm <string>' argument '${algorithm}' is invalid. Allowed choices are built-in, default, bubble, selection, insertion, quick, merge, heap.`,
        );
    }

    switch (strategy) {
      case "random":
        this.array = fullRandomArray(length, max);
        break;
      case "chunks":
        this.array = randomChunkArray(length, max);
        break;
      case "repeating":
        this.array = repeatingValueArray(length, max);
        break;
      case "sorted":
        this.array = orderedArray(length);
        break;
      case "sorted_reversed":
        this.array = orderedReversedArray(length);
        break;
      default:
        console.error(
          `error: option '-s, --strategy <string>' argument '${strategy}' is invalid. Allowed choices are random, chunk, repeating, sorted, sorted_reverse.`,
        );
        exit(1);
    }
  }
}
