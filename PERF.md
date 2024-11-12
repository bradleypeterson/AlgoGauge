# PERF

PERF is a linux tool to be able to understand the performance of a given process. 

https://man7.org/linux/man-pages/man2/perf_event_open.2.html#EXAMPLES

## Security

https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html

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

### Using PERF with Subprocess

The following is used to create a subprocess and analyze a specific function within PERF. The general idea is to have the PerfEvent get ready to analyze  

```js

const startString = await askQuestion("READY?");

//do some function

const endString = await askQuestion("DONE!");

```

```c++
#include "dependencies/PerfEvent.hpp"

PerfEvent perfObject(process.child);

perfObject.startCounters();

//process completes
perfObject.stopCounters();

perfObject.getPerfJSONString()

```
