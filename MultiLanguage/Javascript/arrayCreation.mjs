export const fullRandomArray = (length, max) => {
	return Array.from({ length }, () => Math.floor(Math.random() * max));
};

export const repeatingValueArray = (length, max) => {
	return Array(length).fill(Math.floor(Math.random() * max));
};

export const randomChunkArray = (length, max) => {
	const chunkSize = length < 10 ? 1 : 5; //Update to take length/5 or something
	let chunk = Math.random() >= 0.5; //update to randomly choose true or false every time
	let arr = [];

	//A very C++ way to do it but it works
	for (let i = 0; i < length; chunk = !chunk) {
		//flip-flop the flipFlop
		let randNum = Math.floor(Math.random() * max);
		for (let j = 0; j < chunkSize && i < length; j++, i++) {
			if (chunk) {
				randNum = Math.floor(Math.random() * max);
			}
			arr.push(randNum);
		}
	}

	return arr.flat();
};

export const orderedArray = (length) => {

	return Array.from({ length: length}, (_, index) => index + 1)

}

export const orderedReversedArray = (length) => {
	return Array.from({ length: length }, (_, i) => length - i);
}
