import { Command } from "npm:commander";

import { runSortingAlgorithm } from "./runAlgorithms.mjs";
import { SortingAlgorithm } from "./AlgogaugeDetails.mjs";
import { verifySort } from "./sorting.mjs";

let Max_Number;

const program = new Command();

const SelectedSortingAlgorithms = [];

// options.verbose, options.output
const collect = (value, previous) => {
  return previous.concat([value.toLowerCase()]);
};



program
  .name("AlgoGauge NodeJS")
  .description(
    "This is the command line version of the AlgoGauge Program written in Javascript.",
  )
  .version("0.5.0")
  .usage("");

program
  .option("-v, --verbose [bool]", "Verbose mode", false)
  .option(
    "-o, --output [bool]",
    "Will output the before and after of the sorted array length should be set to under 100.",
    false,
  )
  .option(
    "-j, --json [bool]",
    "Will output the before and after of the sorted array.",
    false,
  )
  .option("--ignore", "does nothing used by c++ and implicit arguments")
  .option("-p, --perf", "This let's javascript know that you are calling from c++", false)

;

//All options are required but default where given to all
program
  .description(
    "Runs a given sort algorithm expecting array strategy and length to be given",
  )
//   .option(
//     "-f, --file <string>",
//     "The save location for json, json needs to be true",
//     "",
//   )
  .option("-a --algorithm <algo>", "Select sort algorithm", collect, [])
  .option(
    "-n, --number [int]",
    "the number of elements in the array",
    collect,
    [],
  )
  .option("-s, --strategy <string>", "the array creation method", collect, [])
  .option("-y, --name <string>", "optional the Canonical Name", collect, [])
  .option(
    "-m, --max [int]",
    "the largest number the array could have optional",
    Number.MAX_SAFE_INTEGER,
  )
  .action((options) => {
    Max_Number = options.max;
  });

program.parse();
const options = program.opts();


if (
  options.algorithm.length != options.number.length ||
  options.algorithm.length != options.strategy.length ||
  options.algorithm.length == 0
) {
  throw console.error(
    "Number of provided algorithm(s), length(s), language(s), and strategy(s) arguments do not match!",
  );
}

for (let i = 0; i < options.algorithm.length; i++) {
  SelectedSortingAlgorithms.push(
    new SortingAlgorithm(
      options.algorithm[i],
      options.strategy[i],
      options.number[i],
      options.name[i],
      Max_Number,
    ),
  );
}



(() => {
  let jsonResults = "";

  // console.log("DONE!")

  // console.log("DONE!")

  for (const element of SelectedSortingAlgorithms) {
    if (options.output && options.verbose) {
      console.log(`NodeJS Original Array: ${JSON.stringify(element.array)}`);
    }

    if (options.verbose) {
      console.log(`NodeJS Starting sort: \"${element.algorithm}\"`);
    }

    if(options.perf){
		const buf = new Uint8Array(100);
		const encoder = new TextEncoder();
		const data = encoder.encode("READY?");
		const bytesWritten = Deno.stdout.writeSync(data); // 11

    	//   console.log("READY?")
		const _ = Deno.stdin.readSync(buf); // 11 bytes
		const text = new TextDecoder().decode(buf);  // "hello world"

		if (options.verbose) {
			console.log(`Received: ${text} starting process`);
		}
      	// Deno.stdin.close();

    }
    runSortingAlgorithm(element);
    if(options.perf){
		const encoder = new TextEncoder();
		const data = encoder.encode("DONE!");
		const bytesWritten = Deno.stdout.writeSync(data); // 11
		const buf = new Uint8Array(100);
		console.log("DONE!")
		const _ = Deno.stdin.readSync(buf); // 11 bytes
		const text1 = new TextDecoder().decode(buf);  // "hello world"
		if (options.verbose) {
			console.log(`Received: ${text1} stopping process`);
		}
    }
	  Deno.stdin.close();
  }

  SelectedSortingAlgorithms.forEach((element) => {
    if (options.output && options.verbose) {
      console.log(
        `NodeJS Sorted Array: ${JSON.stringify(element.sortedArray)}`,
      );
    }

    if (options.verbose) {
      console.log(`NodeJS Verifying sort: \"${element.algorithm}\"`);
    }
    const correct = verifySort(element.sortedArray);

    if (!correct) {
      console.error(`${element.algorithm} there was an error when sorting`);
    } else {
      if (options.verbose) {
        console.log(`NodeJS Sort: \"${element.algorithm}\" Verified!`);
      }
    }
	if (options.verbose) {
      console.log(
        `NodeJS Sort: \"${element.algorithm}\" with Algorithm Option \"${element.strategy}\" of number ${element.length}, completed in ${element.timeTaken} milliseconds`,
      );
    }
    jsonResults +=
      `{"algorithmName": "${element.algorithm}","algorithmOption": "${element.strategy}","algorithmLength": ${element.length},"language": "NodeJS","algorithmCanonicalName": "${element.name}","algorithmRunTime_ms": ${
        element.timeTaken.toFixed(6)
      }, "perfData": {}}` + ",";
  });

  jsonResults = jsonResults.substring(0, jsonResults.length - 1); //removes the last ,


  if (options.json) {
    console.log(jsonResults);
  }
})();

