


export const builtinSorting = (arr) => {
	return arr.sort((a, b) => a - b);
};

export const mergeSortElements = (right, left) => {
	let resultArray = [],
		leftIndex = 0,
		rightIndex = 0;

	// Loop through both arrays, comparing elements and adding the smaller one to the resultArray
	while (leftIndex < left.length && rightIndex < right.length) {
		if (left[leftIndex] < right[rightIndex]) {
			resultArray.push(left[leftIndex]);
			leftIndex++; // Move to the next element in the `left` array
		} else {
			resultArray.push(right[rightIndex]);
			rightIndex++; // Move to the next element in the `right` array
		}
	}

	// Concatenate the remaining elements from either `left` or `right` (if any)
	return [
		...resultArray,
		...left.slice(leftIndex),
		...right.slice(rightIndex),
	];
};

export const mergeSort = (arr) => {
	//Break out
	if (arr.length <= 1) {
		return arr;
	}

	const middle = Math.floor(arr.length / 2); // Find the middle index
	const left = arr.slice(0, middle); // Split the array into left half
	const right = arr.slice(middle); // Split the array into right half

	// Recursively call mergeSort on the left and right halves
	return mergeSortElements(
		mergeSort(left), // Recursively sort the left half
		mergeSort(right) // Recursively sort the right half
	);
};

export const quickSort = (arr) => {
	if (arr.length <= 1) {
		return arr;
	}

	let pivot = arr[0];
	let leftArr = [];
	let rightArr = [];

	for (let i = 1; i < arr.length; i++) {
		if (arr[i] < pivot) {
			leftArr.push(arr[i]);
		} else {
			rightArr.push(arr[i]);
		}
	}

	return [...quickSort(leftArr), pivot, ...quickSort(rightArr)];
};

export const bubbleSort = (arr) => {
	let swapped = false;

	do {
		swapped = false;
		for (let i = 0; i < arr.length - 1; i++) {
			if (arr[i] > arr[i + 1]) {
				let temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				swapped = true;
			}
		}
	} while (swapped);

	return arr;
};

export const insertionSort = (arr) => {
	for (let i = 1; i < arr.length; i++) {
		let currentValue = arr[i];
		let j = i - 1;
		while (j >= 0 && arr[j] > currentValue) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = currentValue;
	}
	return arr;
};


export const selectionSort = (arr) => {
	for (let i = 0; i < arr.length - 1; i++) {
  
	  let minIndex = i;
	  for (let j = i + 1; j < arr.length; j++) {
		if (arr[j] < arr[minIndex]) {
		  minIndex = j;
		}     
	  }
	  [arr[i], arr[minIndex]] = [arr[minIndex], arr[i]];
	}
	return arr;
}


const swap = (array, index1, index2) => {
    [array[index1], array[index2]] = [array[index2], array[index1]];
}

const heapify  = (array, index, length = array.length) => {
    let largest = index,
        left = index * 2 + 1,
        right = index * 2 + 2;

    // compare element to its left and right child 
    if (left < length && array[left] > array[largest]) {
        largest = left;
    }
    if (right < length && array[right] > array[largest]) {
        largest = right;
    }

    // if the parent node isn't the largest element, swap it with the largest child
    if (largest !== index) {
        swap(array, index, largest);

        // continue to heapify down the heap
        heapify(array, largest, length);
    }

    return array;
}



export const heapSort = (arr) =>{
    for (let i = Math.floor(arr.length / 2); i >= 0; i--) {
        heapify(arr, i)
    }

    // work backwards, moving max elements to the end of the array
    for(let i = arr.length - 1; i > 0; i--){
        // max element of unsorted section of array is at index 0, swap with element at last index in unsorted array
        swap(arr, 0, i);

        // re-heapify array, from beginning to the end of the unsorted section
        heapify(arr, 0, i);
    }

    return arr;
}


export const verifySort = (arr) =>{
	for (let i = 0; i < arr.length-1; i++) {
		if (arr[i + 1] < arr[i]) {
			return false
		}
	}
	return true
}