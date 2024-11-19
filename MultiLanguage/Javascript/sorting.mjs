export const builtinSorting = (arr) => {
  return arr.sort((a, b) => a - b);
};

export const mergeSort = (arr, firstIndex = 0, lastIndex = arr.length) => {
  if (lastIndex - firstIndex < 2) {
    return;
  }

  // Find a middle
  let middleIndex = Math.floor((lastIndex - firstIndex) / 2) + firstIndex;

  mergeSort(arr, firstIndex, middleIndex);
  mergeSort(arr, middleIndex, lastIndex);

  let leftHalfSize = middleIndex - firstIndex;

  let leftArray = [];

  for (let i = 0; i < leftHalfSize; i++) {
    leftArray[i] = arr[firstIndex + i];
  }
  let rightHalfSize = lastIndex - middleIndex;
  let rightArray = [];
  for (let i = 0; i < rightHalfSize; i++) {
    rightArray[i] = arr[middleIndex + i];
  }
  let leftIndex = 0;
  let rightIndex = 0;
  let arrIndex = firstIndex;

  while (leftIndex < leftHalfSize && rightIndex < rightHalfSize) {
    if (leftArray[leftIndex] <= rightArray[rightIndex]) {
      arr[arrIndex] = leftArray[leftIndex];
      leftIndex++;
    } else {
      arr[arrIndex] = rightArray[rightIndex];
      rightIndex++;
    }
    arrIndex++;
  }
  while (leftIndex < leftHalfSize) {
    arr[arrIndex] = leftArray[leftIndex];
    leftIndex++;
    arrIndex++;
  }
  while (rightIndex < rightHalfSize) {
    arr[arrIndex] = rightArray[rightIndex];
    rightIndex++;
    arrIndex++;
  }

  return arr;
};

const quickSortPartition = (arr, first = 0, last = arr.length) => {
  let pivot = arr[first],
    smallIndex = first;
  for (let index = first + 1; index < last; index++) {
    if (arr[index] < pivot) {
      smallIndex++;
      [arr[smallIndex], arr[index]] = [arr[index], arr[smallIndex]];
    }
  }

  // swap pivot into its final spot
  [arr[first], arr[smallIndex]] = [arr[smallIndex], arr[first]];

  return smallIndex;
};

export const quickSort = (arr, firstIndex = 0, length = arr.length) => {
  if (arr.length <= 1) {
    return arr;
  }

  if (firstIndex < length) {
    let pivotLocation = quickSortPartition(arr, firstIndex, length);
    quickSort(arr, firstIndex, pivotLocation);
    quickSort(arr, pivotLocation + 1, length);
  }
  return arr;
};

export const bubbleSort = (arr) => {
  let swapped = false;
  let n = arr.length;

  do {
    swapped = false;
    let lastUnsorted = 0; // Track the last unsorted position

    for (let i = 0; i < n - 1; i++) {
      if (arr[i] > arr[i + 1]) {
        // Swap using destructuring
        [arr[i], arr[i + 1]] = [arr[i + 1], arr[i]];

        swapped = true;
        lastUnsorted = i + 1; // Update last unsorted position
      }
    }
    // Reduce the range for the next pass
    n = lastUnsorted > 0 ? lastUnsorted : n; // If lastUnsorted is zero, keep n unchanged
  } while (swapped && n > 1); // Continue if swaps were made and there's more than one element
  return arr;
};

export const insertionSort = (arr) => {
  // The outer loop's job is to...
  // Obtain the leftmost unsorted value, iterates one index to the right each round.  Starts at 1
  for (let i = 1; i < arr.length; i++) {
    // The inner loop's job is to...
    // Take the leftmost unsorted value, walk to the left (back to index 0), swapping along the way until
    // no swaps are needed, or until we get to index 0.
    let j = i;
    while (j > 0 && arr[j] < arr[j - 1]) {
      [arr[j], arr[j - 1]] = [arr[j - 1], arr[j]];
      j--;
    }
  }
  return arr;
};

export const selectionSort = (arr) => {
  // The outer loop's job is to...
  // Each time the outer loop iterators, it works with the leftmost/unsorted index value
  for (let i = 0; i < arr.length - 1; i++) {
    // The inner loop's job is to...
    // Compare the current index to the rest of the unsorted region
    for (let j = i + 1; j < arr.length; j++) {
      // We have an i (the index we hope to sort) and a j (a candidate to check)
      if (arr[j] < arr[i]) {
        [arr[i], arr[j]] = [arr[j], arr[i]];
      }
    }
  }
  return arr;
};

const heapify = (arr, index, lastUnsortedIndex) => {
  // Find largest among root, left child and right child
  let largest = index,
    left = index * 2 + 1,
    right = index * 2 + 2;

  if (left < lastUnsortedIndex && arr[left] > arr[largest]) {
    largest = left;
  }

  if (right < lastUnsortedIndex && arr[right] > arr[largest]) {
    largest = right;
  }

  // Swap and continue heapifying if root is not largest
  if (largest !== index) {
    [arr[index], arr[largest]] = [arr[largest], arr[index]];

    heapify(arr, largest, lastUnsortedIndex);
  }

  return arr;
};

export const heapSort = (arr) => {
  let i = Math.floor(arr.length / 2);
  while (i > 0) {
    i--;
    heapify(arr, i, arr.length);
  }

  // Swap root to sorted position, re-heap, repeat.
  let lastUnsortedIndex = arr.length;
  while (lastUnsortedIndex > 0) {
    lastUnsortedIndex--;
    [arr[0], arr[lastUnsortedIndex]] = [arr[lastUnsortedIndex], arr[0]];

    heapify(arr, 0, lastUnsortedIndex);
  }

  return arr;
};

export const verifySort = (arr) => {
  if (!arr) {
    return false;
  }
  for (let i = 0; i < arr.length - 1; i++) {
    if (arr[i + 1] < arr[i]) {
      return false;
    }
  }
  return true;
};
