# PERF


PERF is a linux tool to be able to understand the performance of a given process. 

[](https://man7.org/linux/man-pages/man2/perf_event_open.2.html#EXAMPLES)

## Security

[](https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html)

perf_event_paranoid After discussing perf_event_paranoid should be set to -1. This allows use to use multiple CPUs to analyze the performance. It also allows not sudo process to use call perf events. This is really important as it remove the requirement to run Algogauge as sudo.
`cat /proc/sys/kernel/perf_event_paranoid`

This line will change once and will be reset to the original amount at reboot.
`echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid`

This will change permanently.
`echo "kernel.perf_event_paranoid = -1" | sudo tee -a /etc/sysctl.conf`
`sudo sysctl -p`

## How PERF works

This is the standard way to call perf when you want to measure a function within c++. The start and stop counters should be as close to what you want to measure as possible.

```c++
#include "dependencies/PerfEvent.hpp"

PerfEvent perfObject();

perfObject.startCounters();
//do something
perfObject.stopCounters();

perfObject.getPerfJSONString()

```

Measuring process that aren't the calling function. JavaScript, python etc.

```c++
#include "dependencies/PerfEvent.hpp"

PerfEvent perfObject(pid_of_tracking_process);

perfObject.startCounters();

//process completes
perfObject.stopCounters();

perfObject.getPerfJSONString()

```

### Using PERF with Subprocess an Example

child -- is any subprocess

The following is used to create a subprocess and analyze a specific function within PERF. The general idea is on C++ to have the PerfEvent get ready to analyze. C++ is waiting for the "READY?" string have child print the "READY?" and wait for an input. Once C++ gets the input start PERF and wait for DONE! string. Finally when child completes the function needing to be analyzed have it print "DONE!" and wait for input string. C++ get's DONE! message and stops perf and returns something to acknowledge that it got the completion string.

```js
console.log("READY?");
//Get STDIN from c++

//do some function
//! do not print before DONE!
console.log("DONE!");
//Get STDIN from c++
```

```c++
#include "dependencies/PerfEvent.hpp"

PerfEvent perfObject(process.child);

//wait for READY?
perfObject.startCounters();
//pass in START

//wait for DONE!
perfObject.stopCounters();
//pass in START

//Once everything is done print perf details.
perfObject.getPerfJSONString()

```
