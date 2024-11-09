import {Command} from "commander";

import {writeFile, appendFileSync} from "node:fs";
import {runAlgorithm, runSortingAlgorithm} from "./runAlgorithms.mjs"
import readline from 'readline';
import {SortingAlgorithm} from "./AlgogaugeDetails.mjs"
import{verifySort,
} from "./sorting.mjs";

let Max_Number;

const program = new Command();

const SelectedSortingAlgorithms = []



// options.verbose, options.output
const collect = (value, previous) => {
	return previous.concat([value.toLowerCase()]);
}

const writeToFileLocation = (line, file) => {
	if(file == ""){
		return;
	}

	appendFileSync(file, line, (err) => {
		if (err) throw err;
		console.log('The "data to append" was appended to file!');
	}); 
}


 const clearFile = async () => {
	writeFile('../temp/javascript.txt', "", (err) => {
		if (err) throw err;
		console.log('The "data to append" was appended to file!');
	});
}


program
	.name("AlgoGauge NodeJS")
	.description(
		"This is the command line version of the AlgoGauge Program written in Javascript."
	)
	.version("0.5.0")
	.usage("");

program
	.option('-v, --verbose [bool]', 'Verbose mode', false)
	.option('-o, --output [bool]', 'Will output the before and after of the sorted array length should be set to under 100.', false)
	.option('-j, --json [bool]', 'Will output the before and after of the sorted array.', false)
	.option('--ignore', 'does nothing used by c++ and implicit arguments')



//All options are required but default where given to all
program
	.description(
		"Runs a given sort algorithm expecting array strategy and length to be given"
	)
	.option("-f, --file <string>", "The save location for json, json needs to be true", "")
	.option("-a --algorithm <algo>", "Select sort algorithm", collect, [])
	.option("-l, --length [int]", "the number of elements in the array", collect, [])
	.option("-s, --strategy <string>", "the array creation method", collect, [])
	.option("-n, --name <string>", "optional the Canonical Name", collect, [])
	.option("-m, --max [int]", "the largest number the array could have optional", Number.MAX_SAFE_INTEGER)
	.action((options) => {		
		Max_Number = options.max
	});


program.parse();
const options = program.opts();


if(options.file != ""){
	clearFile();
}

if (options.algorithm.length != options.length.length || 
options.algorithm.length != options.strategy.length || 
options.algorithm.length == 0) {     
        throw console.error("Number of provided algorithm(s), length(s), language(s), and strategy(s) arguments do not match!");
		process.exit(1)
}


for(let i = 0; i < options.algorithm.length; i++){
	SelectedSortingAlgorithms.push(new SortingAlgorithm(options.algorithm[i], options.strategy[i], options.length[i], options.name[i], Max_Number));
}



function askQuestion(query) {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
    });

    return new Promise(resolve => rl.question(query, ans => {
        rl.close();
        resolve(ans);
    }))
}


(async () => {
	let jsonResults = "";


  	const startString = await askQuestion("READY?");

	if(options.verbose){
		console.log(`Received: ${startString} starting process`);
	}

	SelectedSortingAlgorithms.forEach(element => {
		if (options.output && options.verbose) console.log(`NodeJS Original Array: ${JSON.stringify(element.array)}`);
	
		if(options.verbose) console.log(`NodeJS Starting sort: \"${element.array}\"`);

		runSortingAlgorithm(element);
		if (options.output && options.verbose) console.log(`NodeJS Sorted Array: ${JSON.stringify(element.sortedArray)}`);

		if(options.verbose) console.log(`NodeJS Verifying sort: \"${element.algorithm}\"`);
	});

	console.log("DONE!")

	SelectedSortingAlgorithms.forEach(element =>{
		const correct = verifySort(element.sortedArray)

		if(!correct){
			console.error(`${element.algorithm} there was an error when sorting`)
		}else{
			if(options.verbose) console.log(`NodeJS Sort: \"${element.algorithm}\" Verified!`);
		}
		if(options.verbose) console.log(`NodeJS Sort: \"${element.algorithm}\" with Algorithm Option \"${element.strategy}\" of length ${element.length}, completed in ${element.timeTaken} milliseconds`);	
		jsonResults += `{"algorithmName": "${element.algorithm}","algorithmOption": "${element.strategy}","algorithmLength": ${element.length},"language": "NodeJS","algorithmCanonicalName": "${element.name}","algorithmRunTime_ms": ${element.timeTaken.toFixed(6)}, "perfData": {}}` + ',';
	})

	
	jsonResults = jsonResults.substring(0, jsonResults.length-1) //removes the last ,
	if(options.file != "" && options.json){
		writeToFileLocation(jsonResults, options.file)
	}

	if(options.json){
		console.log(jsonResults);
	}
})();







