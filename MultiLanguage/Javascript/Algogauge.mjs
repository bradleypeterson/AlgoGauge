import {Command} from "commander";


import { exit} from "process";

import {writeFile, appendFileSync} from "node:fs";
import {runAlgorithm} from "./runAlgorithms.mjs"

const UINT32_MAX = (1 << 31) >>> 0 | (1 << 31) - 1;

let Max_Number = UINT32_MAX

const program = new Command();

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
	.option('-v, --verbose [bool]', 'Verbose mode', (value) => {
	  return value.toLowerCase() === 'true'; // Convert string to boolean
	}, false)
	.option('-o, --output [bool]', 'Will output the before and after of the sorted array length should be set to under 100.', (value) => {
	  return value.toLowerCase() === 'true';
	}, false)
	.option('-j, --json [bool]', 'Will output the before and after of the sorted array.', (value) => {
	  return value.toLowerCase() === 'true';
	}, false);


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

let jsonResults = "";
for(let i = 0; i < options.algorithm.length; i++){
	jsonResults += (runAlgorithm(options.algorithm[i],options.strategy[i], options.length[i], options.name[i], options.verbose, options.output, Max_Number) + ",")
}
jsonResults = jsonResults.substring(0, jsonResults.length-1) //removes the last ,
if(options.file != "" && options.json){
	writeToFileLocation(jsonResults, options.file)
}

if(options.json){
	console.log(jsonResults);
}


	

	
exit(0)