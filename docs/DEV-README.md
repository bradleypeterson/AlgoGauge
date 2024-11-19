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

# AlgoGauge CLI
---
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
│   ├───arrays.hpp # Contains all the array logic
│   ├───hash_algs.cpp  # Contains all the hash table logic 
│   ├───linkedList.hpp # Contains all the linked list logic
│   ├───RandomNum.hpp  # File containing logic to generate random numbers
│   └───sort_7algs.cpp  # Contains all the Sorting Algorithms logic
│
├───CLI_Parsing  # Contains files related to parsing out command line arguments
│   ├───algorithm_caller.hpp # Takes parsed commands from CLI_Parsing.hpp and calls the algorithms.
│   └───CLI_Parsing.hpp  # File for parsing out command line arguments. Also, manages the running algorithms
│
├───dependencies  # Contains any third party libraries (TPL)
│   ├───cxxopts.hpp  # TPL for auto parsing the command line arguments (this does a lot of heavy lifting)
│   ├───PerfEvent.hpp  # Library that I wrote to make interfacing with Perf a lot easier (this is not a TPL)
│   └───subprocess.h # TODO: add info on what this does.
│
├───MultiLanguage  # Contains the files for multi-language logic. (sorting algorithms only)
│
├───CMakeLists.txt  # File that tells cmake or make how to build this project
└───main.cpp  # Main runner. No logic is included here. Only used as a starting point.
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

# algorithms/
---
### /arrays.cpp
TODO: add text.

### /hash_algs.cpp
This file is where all the hash table logic is stored. The only hash table that has been created is closed (array based) hash table. 

##### Design:
  - The design method was to create an class for ClosedHashTable that has all the crud operations like creating, looking-up, changing, and deleting. 
  - Then I created the testing methods that are apart of the class, which are specifically for testing the hash table algorithms. 
  - The getters retrive the privite data members that are passed into the constructor. These data members have to do with the creation of the hash table and the parameters for testing, along with the parameters for how the JSON string is returned and verbose couts.
  - To run/test a hash table object you create an object and pass it into the `runHash()` function. The `runHash()` function returns a string which is in a JSON format. Example bellow:
  ```cpp
  cout << runHash(AlgoGauge::ClosedHashTable<string, string> (100000, "linear", 50, 1000, true, "true")) << endl;
  ```

##### Things to Add:
- [ ] Add Open hash tables
- [ ] Add Quadratic probing 
- [ ] Add more testing functions  

### /linkedList.cpp
TODO: add text.

### /RandomNum.hpp
This file centralizes the random number generator. It primarily uses the Mersenne Twister Engine.
The MTE is not considered to be a *truly* random generator. This is used so as to generate the exact
same set of random numbers every time given a seed. While the MTE does generate random numbers after 
each `getRandomNumber()` call, the set  it generates will always be the same. 

- ##### e.g.
  If I want to generate 5 random numbers, it might produce {3, 524, 65, 325663, 23, 25234}.
  If I restart the program, it will produce {3, 524, 65, 325663, 23, 25234} again. Even though
  the program was restarted, it still produced the same set of random numbers.

- ##### Why do we want to do this?
   This helps ensure consistency when comparing different sorting algorithms while also maintaining 
  randomness within the set. If I want to compare a random Bubble sort with a random Quick sort, I would
  expect to get a more accurate result if the two algorithms had to sort the exact same set. This also
  greatly helps if I compare two algorithms with random start sets, come back at a later time, and then 
  run an additional algorithm with a random start set. No matter the time difference between the 
  first comparison run and the second, the random start set will be the same every time. 

### /sort_7algs.cpp
This is the main file that houses all the sorting algorithms and processing logic.
This file is setup to be object oriented where every algorithm should inherit from the base class 
`BaseSort`. 
<br>
This file also contains perf logic. Since perf only works on Linux, in order to allow this project
to compile on non-linux machines (in order to make things easier for development), there are compiler
commands added throughout in order to make sure that the perf logic is only compiled on Linux 
environments. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

# CLI_Parsing/
---
### /algorithm_caller.hpp
TODO: add text.
### /CLI_Parsing.hpp
This is the main file for managing the program. It parses out the command line arguments primarily,
and then takes those arguments to build each of the algorithms depending on the passed arguments/flags.
It then takes any passed program level flags (verbose, json, perf, etc) and sets up those 
configurations. 
<br>
There are also some compiler commands listed here in case it is compiled on a non-linux environment 
and the flag `-p` or `--perf` is passed without setting it to `sample` (`--perf=sample`).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

# dependencies/
---
### /cxxopts.hpp
This is the TPL that does most of the heavy lifting in terms of parsing out the command line 
arguments. While there are other TPLs that are more sophisticated in parsing CLI args, I chose this one
as it was able to accomplish everything we needed, while also being the simplest to use. 
<br><br>
The official documentation for this TPL is listed here: https://github.com/jarro2783/cxxopts


### /PerfEvent.hpp
This is the library that interfaces with the Perf API on Linux. It should go without saying that this 
was the hardest piece to interface with. My only advice is to put aside a couple of hours to research 
and understand Perf. Unfortunately, the majority of documentation on Perf is on the Command Line 
version, rather than the API. However, the following doc I found to be the most helpful when trying 
to understand Perf: https://www.man7.org/linux/man-pages/man2/perf_event_open.2.html
<br>
Luckily for you, I wrote this library to make interfacing with the Perf API easier and to take on 
the majority of the heavy lifting. 

### /subprocess.h
TODO: add information.


# MultiLanguage/
---
TODO: add files and descriptions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

# More Information:
---
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
docs, Software events do not use CPU registers.
<br>
When this limit is exceeded, no error is generated. Instead, all the perf metrics are returned as all 
0's. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Deploying to an Ubuntu Server
These steps are similar to the **Getting Started -> Installation** section in the other README. However,
these contain the steps I used to build this as a program on Ubuntu so that it could be called from 
the command line interface as a normal Linux command instead of having to specify the path to the 
binary every time. 

## Prerequisites

This project is built on cmake. Please make sure to have cmake installed before continuing.

## Installation
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

## Getting AlgoGauge to run as sudo as a user without sudo access
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
(these changes will only work on new shell sessions. Meaning you'll need to close your active shell session and re-login
in order to get it to work)
These lines allow any user to now run the following command:
```bash
sudo AlgoGauge [flags/args]
```
Which sudo is required in order for the -p/--perf flag to work!

<p align="right">(<a href="#readme-top">back to top</a>)</p>


# Deploying React to an Ubuntu Server
---
*This section provides details on deploying your React app to an Ubuntu server running NGINX*

### Prerequisites:
- ##### NGINX
- ##### Ubuntu
These instructions were made for deploying on an Ubuntu server using the NGINX web daemon. These instructions will
go over some configurations on NGINX and Ubuntu systemd, however, it will be assumed that at this point, you already 
have at least the basic installation of NGINX installed and running on Ubuntu. 

<br>

<h2 id="i-react-home" name="i-react-home">Installation Contents</h2>
<ul>
<li><a href="#i-node">Installing Node</a></li>
<li><a href="#c-react-root-dep">Configuring React for root deployment</a></li>
<li><a href="#c-react-sub-dep">Configuring React for sub-directory or sub-domain deployment</a></li>
<li><a href="#sandbox-subdir-groups">Sandbox sub-directory for Group Based Permissions</a></li>
<li><a href="#c-react-server-daemon">Create React Server Daemon Process</a></li>
<li><a href="#c-nginx-react-router">Configure NGINX to Point React Pages to React Router</a></li>
<li><a href="#i-c-mongodb">Install and Configure MongoDB</a></li>
<li><a href="#react-additional-notes">Additional Notes</a></li>
</ul>


<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Installing Node
---
As you probably already know, Node is required in order to run a React app. However, installing Node on Ubuntu is a 
little bit different if you are wanting to use the most recent version of Node. 

**IMPORTANT:** The majority of package managers (including `apt` and `apt-get`) do not have the latest version of Node.
I do not know the reason for this, but it is ***highly recommended to use the following installation instructions***
instead of installing Node via the stock package manager repo!

To install the ***correct*** and ***latest*** version of Node, please follow the instructions located at their official 
Git repo listed here: https://github.com/nodesource/distributions#debian-and-ubuntu-based-distributions
<br>
If you do not, it is highly probable you ***will*** run into issues later on.

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Configuring React for root deployment
---
If you are planning on deploying your React app at the root level of your website (or if your React app will be the 
sole website running on the server) and you intend to install it at `/var/www/html/`, then continue with this section.

***IMPORTANT:*** **If you are planning on installing this in a subdirectory or as a subdomain,** ***do NOT*** **follow
this section!!! Instead, follow the next section titled** <a href="#c-react-sub-dep">Configuring React for 
sub-directory or sub-domain deployment</a>.

Once you clone your React repo, first, make sure that you create the .env files for both your frontend and backend 
folders. After you do that, all you have to do is `cd` into your frontend folder and then run the following command:

```shell
npm run build
```

After it builds, it should create a `build` folder within your frontend folder. Copy or move all the contents from that 
folder into the root NGINX html folder.

```shell
mv ./build/* /var/www/html/
```
The above command assumes your present working directory is the frontend/client folder.
<br>
After you move or copy the files to the `html` folder, cd into that folder and change the file modifications to have 
read and execute permissions recursively.

```shell
cd /var/www/html
sudo chmod -R +rx .
```

***IMPORTANT:*** **It is critical to make sure ALL folders and files have both READ (r) and EXECUTE (x) permissions, 
especially on the world flags. If they do not, NGINX will assume they should not be accessed and when you go to visit 
your website, you will get a 403 FORBIDDEN error!**
<br>
If there are *any* folders starting from root down to *any* web file that needs to be displayed that doesn't have 
read and execute flags on world, then NGINX will **ALWAYS** return a 403 error. 
<br>
E.g.
If all files listed in `/var/www/html/` have read and execute flags set on world, but the folder `/var/www/` does not 
have these permissions, then NGINX will return 403 for every single file attempted to access on your website. Even
though your files under `html/` have the appropriate permissions, because one of the parent folders `www/` does not have
the appropriate permissions, NGINX assumes everything under that folder should inherit those permissions without
exception. 
<br>
In reality, this isn't abnormal or weird behavior that NGINX is doing to make things frustrating or hard, this is 
actually standard permission practice in the world of Linux which is called the precedence of ACLS. You can read more 
about Linux file permissions here: https://linuxhandbook.com/linux-file-permissions/

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Configuring React for sub-directory or sub-domain deployment
---
If you are planning on deploying your React app in a subdirectory or subdomain of your website (or if your React app 
will be one of many websites running on the server) and you intend to install it at `/var/www/html/[some-folder-name]`, 
then continue with this section.
<br>

***IMPORTANT:*** **If you are planning on installing this at the root level,** ***do NOT*** **follow
this section!!! Instead, follow the previous section titled** <a href="#c-react-root-dep">Configuring React for root 
deployment</a>.

There is some important configurations you need to make to your React project before you deploying it to a subfolder is
possible. First, in your frontend/client folder, you need to make sure any resource links are **relative**. This 
means any links to pictures, videos, or other files you're referencing that you are also storing. 
<br>
Next, your index.html file and add the following line as the first line right after the `<head>` tag:
```html
<head>
    <base href="%PUBLIC_URL%" />
    ...
</head>
```
The `%PUBLIC_URL%` string is a React environment variable that tells React to use the public URL set for your React app.
<br><br>
**Note: If for whatever reason, relative links aren't working for your React app once you build and deploy it, you will 
need to instead convert them to absolute, but prepend it with:** `${process.env.PUBLIC_URL}/[path to resource]`
<br>
E.g.
Instead of
```jsx
<img src={`pics/img/logo.png`}/>
```

Use:
```jsx
<img src={`${process.env.PUBLIC_URL}/img/logo.png`}/>
```

<br>

Next, you'll need to modify any file that has a `<BrowserRouter>` AND/OR `<Routes>` tag.

<br>

For each `<BrowserRouter>` occurrence, you'll need to change the tags as such:
```jsx
import { BrowserRouter } from "react-router-dom";
...
const basename = process.env.PUBLIC_URL;
...
<BrowserRouter basename={basename}>
    ...
</BrowserRouter>
```

<br>

For each `<Routes>` occurrence, you'll need to change the tags as such:
```jsx
import { Route, Routes } from "react-router-dom";
...
const basename = process.env.PUBLIC_URL;
...
<Routes basename={basename}>
    ...
</Routes>
```

Only modify the `<Routes>` tags, do **NOT** modify the `<Route>` tags!
If your app uses both `<BrowserRouter>` and `<Routes>` tags, you'll need to modify both.

<br>

The final thing you'll need to modify is the `package.json` file within your frontend/client folder (do not do this in 
your backend/server folder!)

<br>

Add the following key/value pair at the bottom of this file:

```json
{
  ...,
  "homepage": "[full URL to your webserver including path-to-subfolder. e.g. https://example.com/react-app]"
}
```
It is important to only make this last change on the production server!

<br>

With all these changes made, we can finally build and deploy our React script. First, `cd` into your frontend folder and
then run the following command:

```shell
npm run build
```

After it builds, it should create a `build` folder within your frontend folder. Copy or move all the contents from that
folder into the html subdirectory folder.

```shell
mv ./build/* /var/www/html/[path to your subdirectory]
```
The above command assumes your present working directory is the frontend/client folder.
<br>
After you move or copy the files to the `/var/www/html/[path to your subdirectory]` folder, cd into that folder and 
change the file modifications to have read and execute permissions recursively.

```shell
cd /var/www/html/[path to your subdirectory]
sudo chmod -R +rx .
```

***IMPORTANT:*** **It is critical to make sure ALL folders and files have both READ (r) and EXECUTE (x) permissions,
especially on the world flags. If they do not, NGINX will assume they should not be accessed and when you go to visit
your website, you will get a 403 FORBIDDEN error!**
<br>
If there are *any* folders starting from root down to *any* web file that needs to be displayed that doesn't have
read and execute flags on world, then NGINX will **ALWAYS** return a 403 error.
<br>
E.g.
If all files listed in `/var/www/html/` have read and execute flags set on world, but the folder `/var/www/` does not
have these permissions, then NGINX will return 403 for every single file attempted to access on your website. Even
though your files under `html/` have the appropriate permissions, because one of the parent folders `www/` does not have
the appropriate permissions, NGINX assumes everything under that folder should inherit those permissions without
exception.
<br>
In reality, this isn't abnormal or weird behavior that NGINX is doing to make things frustrating or hard, this is
actually standard permission practice in the world of Linux which is called the precedence of ACLS. You can read more
about Linux file permissions here: https://linuxhandbook.com/linux-file-permissions/

<br>

The following article may be helpful for reference if problems arise: 
https://skryvets.com/blog/2018/09/20/an-elegant-solution-of-deploying-react-app-into-a-subdirectory/

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Sandbox sub-directory for Group Based Permissions
---
This section details instructions for the current deployment of the AlgoGauge React app on the Morpheus server as well 
as deploying individual group's React projects to their own segregated/sandboxed folders


The current Morpheus server is setup so that each group that had an opportunity to deploy their project, was sandboxed 
into their own project folder. This provided certain security boundaries so that each group could only access and 
make changes to their folder and not the others. The following explains how this was accomplished.
<br><br>

1. A user group is created for the student team using their full team name
2. All students belonging to that team are then added to the group
3. A folder is created within `/var/www/html` with their group name as the folder name
4. The `chown` command is then used on that newly created folder to make the group ownership part of the new group we
just created
5. A `setgid` bit is then added to that newly created folder to the **group ONLY** using the following command
```shell
sudo chmod -R 775 /var/www/html/[folder name]
sudo chown -R [myself]:[group name] /var/www/html/[folder name]
sudo chmod g+s /var/www/html/[folder name]
```
***IMPORTANT:*** **It is absolutely critical that the you make sure to use `g+s` instead of just `+s` so that the 
`setgid` bit is placed on the group part only! Using just `+s` will set it on both the group AND the user meaning that 
not only will all newly created files inherit the group, but also the user who created the folder!**

<br>

Currently on the Morpheus server, there is a bit of a problem that should be changed at a later date (I would have 
changed it, but we were so late in the semester that it wasn't really worth doing so). When I created the individual 
sandboxed group folders, I created them within `/var/www/html/` and meant those to be the sole folder where the 
groups could do whatever they wanted in their own sandboxed folders. However, I later learned that this created a 
nuisance as when a group built their React app, they would have to put it at the root of their group folder which is 
where their git repo was also located. Considering no one wanted their git repo to be accessible by NGINX which would 
also make it available by web browser, the work around we created was to have an `etc` folder at the root group folder. 
This `etc` folder would house all files not wanting to be displayed by NGINX including their git repo. This worked
because we would only set the `etc` folder to **NOT** have any permissions on the world bits. This is all fine 
except whenever we have to build the React app. This is because we would have to redo the `chmod -R +rx` on the root 
group folder which would also add read and execute permissions to the `etc` folder which is not what we wanted. Meaning 
that every time we would have to manually specify each of the files in our `chmod` command so as to not touch the `etc` 
folder as well.

<br>

The best solution to this problem is to create two folders for each group. One group folder that would be the same as
above in `/var/www/html/` and then another one somewhere other than `/var/www/html/`. This would eliminate the need for
the `etc` folder as it would move the `etc` folder somewhere else in the file system.

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Create React Server Daemon Process
---
This section explains how to set up the React backend/server/express/etc server to run without taking over your shell 
and without requiring you to maintain an active shell session.

When we run `npm start` within our development environments for our backend/server/express process, you may have 
noticed that the command `npm start` takes over your current console/shell and can only be regained by terminating the 
process. This of course, is completely impractical to use on the server. To get around this, we create a daemon via 
Ubuntu's `systemd` daemons and allow Ubuntu to manage it for us. To do this run the following commands:

<br>

First, make note of the absolute path of where your backend/server/express `js` file is located. If you don't know, 
look within your package.json file within the backend/server/express/whatever folder (not your frontend folder) and 
look at the value for the `start` key. 

<br>

Next, edit that file and add the following line to the **1st line** of the file (it ***must*** be the first line!):
```jsx
#!/user/bin/env node
```

Then run the following command:
```bash
chmod +x [path to your backend/server/express js file]
```

Now you can create the daemon file using the following:

```bash
sudo [vim or nano] /etc/systemd/system/[group name or whatever name]-backend.service
```

Add the following lines to this file:
```bash
[Unit]
Description=CHANGEME

[Service]
ExecStart=/usr/bin/env node /var/www/html/CHANGEME
Restart=always
User=nobody
# Note Debian/Ubuntu uses 'nogroup', RHEL/Fedora uses 'nobody'
Group=nogroup
Environment=PATH=/usr/bin:/usr/local/bin
Environment=NODE_ENV=production
WorkingDirectory=/var/www/html/CHANGEME

[Install]
WantedBy=multi-user.target
```
Making sure to change all the `CHANGEME` to the appropriate values.

<br>

The following is an example of this service file for a group called `example` where their root group folder is located 
at `/var/www/html/example` and their backend JS file is located at 
`/var/www/html/example/etc/AlgoGauge/backend/server.js`:
```bash
[Unit]
Description=Example AlgoGauge Backend

[Service]
ExecStart=/usr/bin/env node /var/www/html/example/etc/AlgoGauge/backend/server.js
Restart=always
User=nobody
# Note Debian/Ubuntu uses 'nogroup', RHEL/Fedora uses 'nobody'
Group=nogroup
Environment=PATH=/usr/bin:/usr/local/bin
Environment=NODE_ENV=production
WorkingDirectory=/var/www/html/example/etc/AlgoGauge/backend

[Install]
WantedBy=multi-user.target
```

After you created and saved your `.service` file, you need to tell `systemd` to enable it. You can do this with the 
following command:
```bash
sudo "_systemctl_" enable [name of .service file you created WITHOUT .service]
```

E.g. if the service file you created is called `example-backend.service`, you would run:
```bash
sudo systemctl enable example-backend
```

Now that it is enabled, you can test it by running:
```bash
sudo systemctl start [name of .service file you created WITHOUT .service]
```

If you need to stop it, run the following:
```bash
sudo systemctl stop [name of .service file you created WITHOUT .service]
```

If you need to restart it, run the following:
```bash
sudo systemctl restart [name of .service file you created WITHOUT .service]
```

After you start it, it is important to make sure that it did start correctly. To check to see if it started correctly 
or to just check if it's running at anytime, run the following command:
```bash
systemctl status [name of .service file you created WITHOUT .service]
```

If you want to view the logs or stdout of your daemon, the `systemctl status` will display the last 5 or so lines from 
your daemon. To view more, you'll need to use the `journalctl` command like so:
```bash
journalctl -u [name of .service file you created WITHOUT .service]
```

**Note: if you get a permissions error when running this, you may need to run `journalctl` as root.**


<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Configure NGINX to Point React Pages to React Router
---

Another fun issue with React and React Router is that when you navigate to a different page that's handled 
by your React Router and try to refresh, you'll notice you'll get a 404 error. This is because when you 
navigate to a page from your React app, React Router handles the page indexing and page loading. However, 
upon refreshing the page, the browser then asks NGINX for that page instead of React Router. This results 
in a 404 as that page does in fact not actually exist. Once you build your React App, only 1 index.html file 
is created. 


To fix this, you'll need to configure NGINX to point any paths directed to your React App subdirectory to 
point to the React Router instead of having NGINX try to find the path on the server. 

First, open up `/etc/nginx/sites-available/default` in your favorite text editor.

There should be a `location /{ ...` block already listed (Morpheus server will have multiple. Look at the 
Morpheus Setup for info on the current configuration). Make sure the initial `location` field is set like so:

```yaml
location / {
      # First attempt to serve request as file, then
      # as directory, then fall back to displaying a 404.
      try_files $uri $uri/ $uri/index.html /index.html;
}
```
The above lines will fix refresh issue with a React App that's installed at the html root.

To get your React App in a subdirectory to not return a 404 when refreshed, insert the following lines 
for your React app:

```yaml
location /PATH-TO-YOUR-REACT-APP/ {
    try_files $uri $uri/ /PATH-TO-YOUR-REACT-APP/index.html;
}
```

It is critical that you make sure the `PATH-TO-YOUR-REACT-APP{` `location /PATH-TO-YOUR-REACT-APP/ {` is 
wrapped in `/`. Otherwise, it will NOT work!

#### Morpheus Current NGINX Configuration:
The current Morpheus configuration at the time of this writing has 4 `location ...` entries. One for the 
root html path and 3 for each of the groups that were to publish their project to Morpheus. 

#### Useful links for NGINX Directives:
https://www.keycdn.com/support/nginx-location-directive
https://www.digitalocean.com/community/tutorials/nginx-location-directive
https://stackforgeeks.com/blog/reactjs-application-showing-404-not-found-in-nginx-server


<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Install and Configure MongoDB
---

MongoDB has been installed on the Morpheus server so as to not have to worry about any paid cloud 
subscriptions or free tier limits. 

### Installing MongoDB on Ubuntu
Please use the following tutorial on installing MongoDB on Ubuntu: https://www.mongodb.com/docs/manual/tutorial/install-mongodb-on-ubuntu/

Make sure you also follow the (hidden) instructions on adjusting UNIX `ulimit` Settings listed on the same page 
or here: https://www.mongodb.com/docs/manual/reference/ulimit/

### Configuring Security for MongoDB
By default, the newly installed instance of MongoDB is completely open on localhost where any user connecting to 
it will have unlimited, admin access. This is not good. very not good. very super duper not good. so much not good that
it bad. very bad. very, very bad. very very super duper bad.

To fix this, we can setup an initial admin account for MongoDB by following the instructions here:
https://www.mongodb.com/docs/manual/tutorial/configure-scram-client-authentication/#std-label-create-user-admin
(Note: This has already been configured on Morpheus)

### Creating Users on MongoDB
To create a user on MongoDB, follow the instructions listed here: https://www.mongodb.com/docs/manual/tutorial/create-users/

It should be noted that when you create a new user, if you create the user whiling using a db other than 
`admin` then you will need to specify that db within your connection string.

### Morpheus Setup
The Morpheus server is currently configured with the fun-gang db only. All the other groups did not have time 
to move their DB instance from Cloud Atlas to Morpheus localhost db. 

Only one user was created to interface with this single db.

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

# Additional Notes
---

If there are no additional notes to be added, but yet a section for additional notes is added anyways 
where the only content is "there is no additional notes as of right now" or some other long winded content 
under the section "Additional Notes" just for the sole purpose of wasting the reader's time and attention, 
then the long winded content that is super descriptive and a waste of time that is included in the section 
"Additional Notes" for the sole purpose of wasting the reader's time and attention, will be just that: a 
complete disregard for the reader's time by wasting their time by providing bland and pointless and 
redundant and runons, or inconsistent content. Yet, you read this whole thing. And that's what I like 
about you. You and I can waste the universe's time together as father and son. Trust your instincts. You 
know it to be true. Live, long, laugh, love and prosper. 

<p align="right">(<a href="#i-react-home">back to install react contents</a>)</p>
<p align="right">(<a href="#readme-top">back to top</a>)</p>