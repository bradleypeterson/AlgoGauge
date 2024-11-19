export const runSortingAlgorithm = (algo) => {
  const start = performance.now();
  algo.sortedArray = algo.sortingCommand(algo.array);
  algo.timeTaken = performance.now() - start;
};
