# PERF

> [!IMPORTANT]
> PERF is Linux only and other operating systems don't have a perf alternative

PERF is a Linux tool to be able to understand the performance of a given process.

[perf_event_open](https://man7.org/linux/man-pages/man2/perf_event_open.2.html#Arguments)

## Security

[perf-security](https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html)

We recommend setting perf_event_paranoid to -1. This allows perf_event.hpp to use multiple CPUs to analyze the performance. It also allows Algogauge to be run without sudo.

This print what perf_event_paranoid is set to, the default is generally 3+

```shell
cat /proc/sys/kernel/perf_event_paranoid
```

This will temporary change perf_event_paranoid to be -1 (resets at reboot)

```shell
echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid 
```

This will permanently change perf_event_paranoid to be -1.

```shell
echo "kernel.perf_event_paranoid = -1" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

## How Algogauge use PERF

Algogauge use PERF to understand how long it takes a specific function to complete. This is after the function has been compiled to machine code. As perf is primary a CLI tool we use an open source library to accomplish this analyzing within C++. [Github of PERF Event](https://github.com/viktorleis/perfevent)

Perf event has been updated by us to output a more JSON like output. Along with sample data for computers without PERF.

### Example

This is the standard way to call perf when you want to measure a function within c++. The start and stop counters should be as close to what you want to measure as possible.

```c++
#include "dependencies/PerfEvent.hpp"

PerfEvent perfObject();

perfObject.startCounters();
//do something
perfObject.stopCounters();

perfObject.getPerfJSONString()
```

#### Using PERF with Subprocess

Passing in a pid will allow you to run PERF on the entire subprocess execution or any external process

```c++
PerfEvent perfObject(pid_of_tracking_process);
```

child -- is any subprocess

The following is done to analyze a specific child function. The general idea is on C++ to have the PerfEvent get ready to analyze. C++ is waits for the "READY?"  inputting into stdin have the child process print the "READY?" and wait for an input. Once C++ gets the input start PERF alert child to start and wait for DONE! string. Finally when child completes the function needing to be analyzed have it print "DONE!" and wait for an input string. C++ get's DONE! message and stops perf and returns something to acknowledge that it got the completion string.

```js
console.log("READY?");
//Get STDIN from c++

//do some function
//! do not print anything between READY! and DONE!
console.log("DONE!");
//Get STDIN from c++
```

```c++
PerfEvent perfObject(process.child);

//wait for READY?
perfObject.startCounters();
//STDIN Start

//wait for DONE!
perfObject.stopCounters();
//STDIN Done
```

> [!IMPORTANT]
> For now we have not found way to allow printing between READY? and DONE! the reason is unclear
> Note: I was able to get it work on my machine but in production when the number of elements being sorted was then then 500 it broke.
> The solution was not having anything print between the functions.

### What Algogauge collects
