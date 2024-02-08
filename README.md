# Description 


## Usage:
Run the following commands
-  `make`
- `./a.out`
---
### Prompt
- **Terminal** comes with a sleek prompt displaying the username and the hostname of the machine, and the current working directory

## Features:

### `;` seperated commands ###
- Concatenate multiple commands in a single line by separating them with a semicolon `;`
---

### `&` terminated background processes
- Run processes in background by appending an ampersand `&` at the end of the command
- **Terminal** displays the pid of such processes before executing them in the background
- **Child handling**: **Terminal** displays the exit status of such processes when they are terminated, suspended or continued.
---



### warp - Change current working directory

Usage : `warp [DIR]`
- Here, `[DIR]` can take on any of the following values
  - `~` Changes directory to the home directory i.e. the directory from which the shell was executed
  - `-` Changes directory to the previous working directory
  - Any relative path from the home directory is valid
  - Any absolute path (i.e. from the root directory) is valid
  - Passing no arguments changes directory to the home directory
  - Note that these are valid as long as the user has the appropriate permissions for the directory passed
---


### peek - List files/folders and their information from one or multiple directories

Usage : `peek [OPTIONS]... [FILE]...`

- Here, `[OPTIONS]` can take any of the following values
  - Specifying no options prints only the names of the non-hidden files/folders from the FILEs passed
  - `-a` (all) Behaves similar to no options but additionally prints hidden files
  - `-l` (long format) Prints additional info such as permissions, date modified, size for each non-hidden file
  - Note that multiple options can be used at the same time
- The `[FILE]` argument can be
  - A relative or absolute path to a file or a folder
  - Note that the user is required to have appropriate permissions to read the details/contents of the file/folder
---

### pastevents - Displays the previous 15 commands executed on the Terminal

Usage : `pastevents execute [0<=index<=15]`
- Here, specifying only pastevents, will print previous 15 commands and are stored in ***past.txt***
- If the new command entered is same as ***previous*** command, then it will not be stored in past.txt
- ***pastevents purge*** clears past.txt file
- ***pastevents execute*** index will Execute the command at position in pastevents
---

### proclore - Prints the process related info of a shell program

Usage : `proclore [pid]`

- Displays the pid, process status, memory consumption and executable path for the specified process
- Passing a valid `pid` displays the info related to the specified `pid`
- If no arguments are passed, instead displays the info related to the current **Terminal** process

---
### seek - Looks for files and directory in the specified target directory 
Usage : `seek <flags> <search> <target_directory>`

- Flags :
  - -d : Only look for directories (ignore files even if name matches)
  - -f : Only look for files (ignore directories even if name matches)
  - -d and -f flag can’t be used at the same time.
  - If No flags are given, then it will look for both files and directory
  - -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it

- search :
  - Write the file or directory that needs to be searched inside the `<target_directory>`  tree

- target_directory :
  - Write the folder in whose the file or folder is to be printed

- “No match found!” will be printed in case no matching files/directories is found.


-  It returns a list of relative paths (from target directory) of all matching files/directories ***(files in green and directories in blue)*** separated with a newline character.


---
### File redirection
- `< [FILE]` can be used to redirect input from the specified `[FILE]` for a command
- `> [FILE]` and `>> [FILE]` can be used to redirect the output to the specified `[FILE]` for a command. `>` Overwrites the output file and `>>` appends to the output file
- Both input and output redirections can be used on a single command
  - Example: `cat < input.txt > output.txt`
---
### Piping commands
- `<command> | <command>` can be used to pipe the output of the command on the left side of the pipe to the command on the right side of the pipe
- Two or more commands can be chained together by piping
- Piping can be used alongside input and output redirections
  - Example: `cat < in.txt | wc -l > lines.txt`
- Note that in case of a conflict between taking input/output from the pipe or through a redirection, the redirection is given higher preference
  - Example: `cat in.txt | sort > out.txt | wc -l`
  - This shall output 0 since the `sort` command redirects the output to `out.txt` and not the pipe

### Signals
- `<ctrl>C` or `SIGINT` does not kill **Terminal**
- `<ctrl>Z` or `SIGTSTP` does not kill **Terminal**
- `<ctrl>D` or an `EOF` safely logs out of the shell (functionally equivalent to the `exit` command)
---
### Activities - Displays the processes spawned by the shell which are either running or suspended 
- ***activities*** print a list of all the processes currently running that were spawned by shell in ***lexicographic*** order.
- This list will contain the following information about all processes :
  - Command Name
  - pid
  - state : running, stopped and finished

- If a process is ***finished***, then it will be printed only once. 
---
### Ping - Sends a specified signal to the process with the specified job number

Usage : `ping [pid] [signal_number]`

- The `pid` can be any of the job numbers listed using the command `jobs`
- The `signal_number` can be any of the signals listed under the manpage signal(7)

---
### fg - Brings the running or stopped process specified by the pid to the foreground

Usage: `fg [pid]`

- The `pid` can be any of the pids listed using the command `activities`


### bg - Sends the running or stopped process specified by the pid to the background
---
Usage: `bg [pid]`

- The `job_number` can be any of the pids listed using the command `activities`
---
### Neonate - Prints a specific system data periodically for a specified period

Usage: `neonate [OPTION] N`

- Here, the `[OPTION]` must include exactly one instance of
  - `-n` followed by a positive integer `[N > 0]` which specifies the interval for printing the command output
  - This prints the PID of the most recently created process on the system after every `N` seconds
- A keypress of `x` stops execution of the command 

### exit : Exits out of the shell





# Assumptions
1. Assuming the input size will be maximum of 1024 bytes.
2. In the pastevents command, if the command contains pastevents, then it wont be stored in past.txt, or else the corresponding command will be check with the last command and if both of them are not same, then they are stored in ***past.txt***.

3. All paths involved are less than ***1000 characters***.
4. Each command is less than ***1000 characters***.
5. History is stored in `txt` file in the same folder as where the executable is stored.
