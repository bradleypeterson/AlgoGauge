# Subprocess

[Github subprocess.h](https://github.com/sheredom/subprocess.h)

## DESC

Subprocess uses an open source library found on github. It is designed to create subprocess for C/C++. The reason we used this and not standard forking with exec was the ability to run subprocess on macOS, Windows, and Linux without the need to change our code. Subprocess also allows for basic multithreading this is required to allow us to get and send input back and forth from C++ to the subprocess ([PERF](PERF.md#perf)). 
Beyond that while subprocess can use create any process such as node and python these methods make it complicated [linked here]().

## Usage

The basic idea of how to use can be found in [algorithm_caller](../DEV-README.md#algorithm_callerhpp) func: runChildProcess.

### Start

`struct subprocess_s process;`

### Create subprocess

```c++
   int result = subprocess_create_ex(commandLineArguments, subprocess_option_search_user_path | subprocess_option_enable_async | subprocess_option_combined_stdout_stderr, environment, &process);
```

`subprocess_create_ex` create a subprocess that supports custom environment (env) variables. Right now we have no reason to use this as none of the functions require environment variables. But there was no reason to change it.

`commandLineArguments` is a char*[] with the program you are calling.