const { program, Option, Argument } = require('commander');

program
  .name('AlgoGauge NodeJS')
  .description('This is the command line version of the AlgoGauge Program written in Javascript.')
  .version('0.5.0')
  .usage("")
//   .addArgument(new Argument('<algorithm>', 'Selected Algorithm').choices(supportedAlgorithms).default(supportedAlgorithms[0]))
//   .argument('[length]', 'length', 20)
//   .action((algorithm, length) => {
//     console.log('username:', algorithm);
//     console.log('password:', length);
//   });

program
	.name("")
	.option('-v, --verbose', 'Runs the program in verbose mode')
	.option('-i, --include-values', 'Include values in the output. Recommended to use with length < 100')
	.option('-f, --file <path/file>',"Save output to a file", "")
	.option('-j, --json', "Prints the output as a json formatted objects")
	// .addOption(new Option('-p, --perf <argument>', "Show performance details"))


program.command('Algorithm')
  .description('Name of the algorithm to run')
  .argument('<string>', 'string to split')
  .option('--first', 'display just the first substring')
  .option('-s, --separator <char>', 'separator character', ',')
  
  .action((str, options) => {
    const limit = options.first ? 1 : undefined;
    console.log(str.split(options.separator, limit));
  });

program
//   .option('-a, --algorithm <algorithm>', 'Name of the first algorithm to run. Supported algorithms: bubble, selection, insertion, quick, merge, heap')
  .addOption(new Option('-a, --algorithm <algorithm> <length>', 'Selected Algorithm').choices(supportedAlgorithms).default("built-in"))
  .option('-l, --length <int>', 'Amount to Sort 0-4294967295')
  .option('-n, --name <string>', 'A canonical name that will be returned in output if provided.', "")

	

program.parse();

// version('1.0.0', '-v, --version')
//   .usage('[OPTIONS]...')
//   .option('-f, --flag', 'Detects if the flag is present.')
//   .option('-c, --custom <value>', 'Overwriting value.', 'Default')
//   .parse(process.argv);

// const options = opts();



/*

This is the command line version of the AlgoGauge Program.
Usage:
  AlgoGauge --algo (some algorithm) --len [some int > 0][OPTIONALS: -r | -e | -c | -s | -o] [-f "some file", -v, -j, -i, -p]

 Algorithm Options [ONE REQUIRED] options:
  -r, --random [=arg(=true)]    Generated data will be a random set 
                                (default: false)
  -e, --repeated [=arg(=true)]  Generated data set will have repeated 
                                values (default: false)
  -c, --chunks [=arg(=true)]    Generated data set will have various 
                                subsets that will contain both random and 
                                in order values (default: false)
  -s, --reversed [=arg(=true)]  Generated data set will be in reverse order 
                                e.g. (9-0) (default: false)
  -o, --ordered [=arg(=true)]   Generated data set will be ordered e.g. 
                                (0-9) (default: false)

 Program Output [OPTIONAL] options:

 Program Settings [OPTIONAL] options:
  -p, --perf [=If you are not on Linux and want to use this anyways, you can set this to "sample". e.g. --perf=sample(=true)]
                                Includes Perf data in the output. Actual 
                                Perf data only works on Linux. (default: 
                                false)


*/

import {Command, Argument} from 'commander';
import "./sorting.js"
import { bubbleSort, defaultSort } from './sorting.js';
import { config } from 'dotenv';

const supportedAlgorithms = [
	"built-in",
	"bubble",
	"selection",
	"insertion",
	"quick",
	"merge",
	"heap",
];
const program = new Command();

let runningAlgorithms = [
	// {algorithm: "", length: 0, config: "rand"}
]
const selectedSorts = []

program
	.name("AlgoGauge NodeJS")
	.description(
		"This is the command line version of the AlgoGauge Program written in Javascript."
	)
	.version("0.5.0")
	.usage("");

program
	.option("-v, --verbose", "Runs the program in verbose mode")
	.option(
		"-i, --include-values",
		"Include values in the output. Recommended to use with length < 100"
	)
	.option("-f, --file <path/file>", "Save output to a file", "")
	.option("-j, --json", "Prints the output as a json formatted objects");

program
	.addArgument(
		new Argument(
			"-a, --algorithm <algorithm> <length>",
			"Selected Algorithm"
		)
			.choices(supportedAlgorithms)
			.default("built-in")
	)
	.option("-l, --length <int>", "Amount to Sort 0-4294967295")
	.option(
		"-n, --name <string>",
		"A canonical name that will be returned in output if provided.",
		""
	)

	.action((algorithm, options) => {
		console.log(algorithm, options);
	});

program
	.command("run")
	// ['built-in', 'bubble', 'selection', 'insertion', 'quick', 'merge', 'heap'];
	.option("-d, --default <length>", "Runs the built in sort method")
	.option("-b, --bubble <length>", "Length of bubble sort")
	.option("-s, --selection <length>", "Length of merge sort")
	.option("-i, --insertion <length>", "Length of merge sort")
	.option("-q, --quick <length>", "Length of merge sort")
	.option("-m, --merge <length>", "Length of merge sort")
	.option("-h, --heap <length>", "Length of merge sort")
	.option("-c, config <type>", 'Configuration type (e.g., random, chunk)')

	.action((options) => {
        const { default: def, bubble, selection, insertion, quick, merge, heap, config } = options;

        // Collect all provided sorting options
        if (def) selectedSorts.push({ type: 'default', length: def, config: config });
        if (bubble) selectedSorts.push({ type: 'bubble', length: bubble, config: config });
        if (selection) selectedSorts.push({ type: 'selection', length: selection, config: config });
        if (insertion) selectedSorts.push({ type: 'insertion', length: insertion, config: config });
        if (quick) selectedSorts.push({ type: 'quick', length: quick, config: config });
        if (merge) selectedSorts.push({ type: 'merge', length: merge, config: config });
        if (heap) selectedSorts.push({ type: 'heap', length: heap, config: config });

        // Check if at least one sorting option is provided
        if (selectedSorts.length === 0) {
            console.error('Error: At least one sorting option must be specified.');
            process.exit(1);
        }

		if(def) selectedSorts.push({algorithm: defaultSort, length: def, arrayConfig: config});
		
		if (bubble) selectedSorts.push({algorithm: bubbleSort, length: bubble, arrayConfig: config});

		if (selection);
		if(insertion);
		if(quick);	
		if(merge);
		if(heap);
		if (selectedSorts.length === 0) {
            console.error('Error: At least one sorting option must be specified.');
            process.exit(1);
        }
		console.log(selectedSorts)
	});

program.parse(process.argv);

const runSort = ({
	sortType = defaultSort,
	randomType = fullRandomArray,
	length = 100,
	max = Number.MAX_SAFE_INTEGER,
} = {}) => {
	console.log(sortType(randomType(length, max)));
};

console.log(runningAlgorithms, selectedSorts)

const fullRandomArray = (length, max) => {
	return Array.from({ length }, () => Math.floor(Math.random() * max));
};

const repeatingValueArray = (length, max) => {
	return Array(length).fill(Math.floor(Math.random() * max));
};

const randomChunkArray = (length, max) => {
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