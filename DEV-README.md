<a id="readme-top" name="readme-top"></a>
<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/jzdegrey/AlgoGauge">
    <img src="https://catalog.weber.edu/mime/media/22/2687/east_horiz.png" alt="Weber State University Logo">
  </a>

<h2 align="center">AlgoGauge<br>DEVELOPER MANUAL</h3>


  <h6 align="center">
    The CLI version of Dr. Brad Peterson's AlgoGauge Tenure project.

  </h6>
  <p align="left">
    This is the README for developers who are looking to improve this project. If you are only looking
    to use this program, please read the <a href="./README.md">README.md</a> file instead. This readme
    also contains information on deploying not just this program, but also the React app to an 
    Ubuntu server.
    <br><br>
    Use this file to SUPPLEMENT the <a href="./README.md">README.md</a>. 
    <br>    
    <em>It makes little sense to improve something you do not know how to use.</em>
  </p>
</div>

<br>
<h4>&gt;&gt;&gt; <a href="#react-deploy">Jump to Deploying React to an Ubuntu Server</a></h4>

## AlgoGauge CLI
****
*This section provides details on the C++ Code*

### Project Structure
This project is organized in categorized folders that stores similar files. In order to keep this
project clean and easy to navigate, files should not contain an overwhelming number of lines.
Even if a large collection of functions reference each other, you can generally isolate some and
move them to their own file. Look at the already existing files to gain an understanding of what 
constitutes a normal number of lines for a given file. 

##### Project Tree
```bash
AlgoGaugeCLI:
│
├───algorithms  # Contains files related to running any algorithms
│   ├───RandomNum.hpp  # File containing logic to generate random numbers
│   └───sort_7algs.cpp  # Contains all the Sorting Algorithms logic
│
├───CLI_Parsing  # Contains files related to parsing out command line arguments
│   └───CLI_Parsing.hpp  # File for parsing out command line arguments. Also, manages the running algorithms
│
├───dependencies  # Contains any third party libraries (TPL)
│   ├───cxxopts.hpp  # TPL for auto parsing the command line arguments (this does a lot of heavy lifting)
│   └───Perf.hpp  # Library that I wrote to make interfacing with Perf a lot easier (this is not a TPL)
│
├───CMakeLists.txt  # File that tells cmake or make how to build this project
└───main.cpp  # Main runner. No logic is included here. Only used as a starting point.
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### algorithms/
#### /RandomNum.hpp
This file centralizes the random number generator. It primarily uses the Mersenne Twister Engine.
The MTE is not considered to be a *truly* random generator. This is used so as to generate the exact
same set of random numbers every time given a seed. While the MTE does generate random numbers after 
each `getRandomNumber()` call, the set  it generates will always be the same. 

##### e.g.
If I want to generate 5 random numbers, it might produce {3, 524, 65, 325663, 23, 25234}.
If I restart the program, it will produce {3, 524, 65, 325663, 23, 25234} again. Even though
the program was restarted, it still produced the same set of random numbers.

##### Why do we want to do this?
This helps ensure consistency when comparing different sorting algorithms while also maintaining 
randomness within the set. If I want to compare a random Bubble sort with a random Quick sort, I would
expect to get a more accurate result if the two algorithms had to sort the exact same set. This also
greatly helps if I compare two algorithms with random start sets, come back at a later time, and then 
run an additional algorithm with a random start set. No matter the time difference between the 
first comparison run and the second, the random start set will be the same every time. 

#
#### /sort_7algs.cpp
This is the main file that houses all the sorting algorithms and processing logic.
This file is setup to be object oriented where every algorithm should inherit from the base class 
`BaseSort`. 
<br>
This file also contains perf logic. Since perf only works on Linux, in order to allow this project
to compile on non-linux machines (in order to make things easier for development), there are compiler
commands added throughout in order to make sure that the perf logic is only compiled on Linux 
environments. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#
### CLI_Parsing/
#### /CLI_Parsing.hpp
This is the main file for managing the program. It parses out the command line arguments primarily,
and then takes those arguments to build each of the algorithms depending on the passed arguments/flags.
It then takes any passed program level flags (verbose, json, perf, etc) and sets up those 
configurations. 
<br>
There are also some compiler commands listed here in case it is compiled on a non-linux environment 
and the flag `-p` or `--perf` is passed without setting it to `sample` (`--perf=sample`).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## dependencies/
#### /cxxopts.hpp
This is the TPL that does most of the heavy lifting in terms of parsing out the command line 
arguments. While there are other TPLs that are more sophisticated in parsing CLI args, I chose this one
as it was able to accomplish everything we needed, while also being the simplest to use. 
<br><br>
The official documentation for this TPL is listed here: https://github.com/jarro2783/cxxopts

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#
#### /Perf.hpp
This is the library that interfaces with the Perf API on Linux. It should go without saying that this 
was the hardest piece to interface with. My only advice is to put aside a couple of hours to research 
and understand Perf. Unfortunately, the majority of documentation on Perf is on the Command Line 
version, rather than the API. However, the following doc I found to be the most helpful when trying 
to understand Perf: https://www.man7.org/linux/man-pages/man2/perf_event_open.2.html
<br>
Luckily for you, I wrote this library to make interfacing with the Perf API easier and to take on 
the majority of the heavy lifting. 

<br>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Perf
A couple more items to mention with Perf. Since Perf records performance metrics for a program or a 
running block of code, there is a concern with adding to the run time complexity by also having to 
record the complexity. The way Perf minimizes this (not eliminates) is by recording the performance 
metrics on special CPU registers.
<br><br>
This means that the number of metrics we're able to record is completely dependent on the number 
of these physical performance registers on the CPU. While there is a way to check how many your 
CPU has, I found it's way easier to just conduct trial and error to find the number. (Morpheus 
server has 5). This is because you have to lookup the manual for the specific CPU you're using which 
I found to be very difficult to find one that actually contains this data. 
<br><br>
There are two main Perf Events we are using for this project. Hardware and Software. According to the
docs, Software events do not use registers, however, we found that the Software events still added 
to our total limit anyways.
<br>
When this limit is exceeded, no error is generated. Instead, all the perf metrics are returned as all 
0's. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#
### Deploying to an Ubuntu Server
These steps are similar to the **Getting Started -> Installation** section in the other README. However,
these contain the steps I used to build this as a program on Ubuntu so that it could be called from 
the command line interface as a normal Linux command instead of having to specify the path to the 
binary every time. 

### Prerequisites

This project is built on cmake. Please make sure to have cmake installed before continuing.

### Installation
```shell
cd [source code root directory]
cmake ..
make .
```
After it builds, note the path of where it builds the binary file then execute the following:
```shell
sudo cp [path to binary] /usr/bin/AlgoGauge
sudo chmod +rx /usr/bin/AlgoGauge
```
The last step (`chmod...`) isn't always necessary, however, it doesn't hurt and may prevent problems
in the future.
<br>
This builds the binary and then moves it into the binaries folder which is where almost all shells 
look in when you execute a command. This means that you can now execute `AlgoGauge [flags/args]` like 
a normal linux command. 

### Getting AlgoGauge to run as sudo as a user without sudo access
In order to do this, there is an extra step that needs to be run after completing the above steps.
You have to edit the sudoers file. I found this site to be useful while completing these actions.
https://ostechnix.com/run-particular-commands-without-sudo-password-linux/
To edit the sudoers file, do ***not*** edit it directly! Instead, run the following command:
```bash
sudo visudo
```
Then, add the following lines at the end of the file:
```bash
# Allow perf to be ran without password:
%sudo ALL=NOPASSWD:/usr/bin/AlgoGauge
root ALL=NOPASSWD:/usr/bin/AlgoGauge
%root ALL=NOPASSWD:/usr/bin/AlgoGauge
```
What these three lines mean is to allow all users to run the AlgoGauge program as sudo without 
also having to have sudo access as well as not having to input any password when running it as sudo.
These lines allow any user to now run the following command:
```bash
sudo AlgoGauge [flags/args]
```
Which sudo is required in order for the -p/--perf flag to work!

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#

<h1 id="react-deploy" name="react-deploy">
Deploying React to an Ubuntu Server
</h1>

****
*This section provides details on deploying your React app to an Ubuntu server running NginX*


<p align="right">(<a href="#readme-top">back to top</a>)</p>