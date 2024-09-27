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
	.command("run")
	.option('-a, --algorithms <json>', 'Pass an array of objects in JSON format', '[{"algorithm": "default", "length": 100, "config": "rand"}]')

	.action((options) => {
		const { algorithms } = options;
		console.log(options)
		let parsedData;
        try {
            parsedData = JSON.parse(algorithms);
			if (typeof parsedData !== 'object') {
                throw new Error('Input must be an object or an array.');
            }
        } catch (error) {
            console.error('Invalid JSON input:', error.message);
            process.exit(1);
        }
		console.log(parsedData)
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

// console.log(runningAlgorithms, selectedSorts)

