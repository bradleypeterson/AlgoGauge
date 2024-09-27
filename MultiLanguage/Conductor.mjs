import { spawn } from "node:child_process";
import { exec as execCb } from "node:child_process";
import { promisify } from "node:util";


const exec = promisify(execCb);
const { error, stdout, stderr } = await exec("./child.sh");

const child = spawn("./child.sh");
console.log(`${new Date()} : CHILD STARTED`);
child.stdout.on("data", (d) => console.log(`${new Date()} : STDOUT => ${d}`));
child.stderr.on("data", (d) => console.log(`${new Date()} : STDERR => ${d}`));
child.on("close", () => console.log(`${new Date()} : CHILD ENDED`));


import { exec, spawn } from 'node:child_process';

exec('my.bat', (err, stdout, stderr) => {
  if (err) {
    console.error(err);
    return;
  }
  console.log(stdout);
});

// Script with spaces in the filename:
const bat = spawn('"my script.cmd"', ['a', 'b'], { shell: true });
// or:
exec('"my script.cmd" a b', (err, stdout, stderr) => {
  // ...
});

import { promisify } from 'node:util';
import child_process from 'node:child_process';
const execFile = promisify(child_process.execFile);
async function getVersion() {
  const { stdout } = await execFile('node', ['--version']);
  console.log(stdout);
}
getVersion();