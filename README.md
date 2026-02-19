# TCP-CHAT
TCP-CHAT is a lightweight tcp chat server and client written in C using only the GNU C Library (glibc).  
  
It is meant to function similar to Internet Relay Chat (IRC) servers, and uses similar commands.  
  
TCP-CHAT was written for the 2026 IU Indianapolis Computer Science Club Hackathon. 

An example can be found here: https://www.youtube.com/watch?v=4MCgdPXsQhc  
## Installation
If you are running a UNIX/POSIX system, the only requirement is a C compiler and glibc(the GNU C Compiler(gcc) should come with glibc by default).  

If you are using Windows, you must use the Windows Subsystem for Linux (WSL) for compiling and running the program. The system does not compile or run correctly on Windows, as the socket programming libraries differ.  

For those unfamiliar with WSL, an installation guide can be found [here](https://learn.microsoft.com/en-us/windows/wsl/install)  

This program was tested on Windows with WSL Ubuntu, the default for WSL, with the only package installed being gcc.  

## Usage
Once an instance of the server is running, multiple clients can connect to it and interact with each other using a series of commands:  
| Command | Description |
|-----:|-----------|
|`msg:`|Message to all other users.|
|`pmsg@name:`|Private Message to User|
|`msgbold:`|A bold and colorful version of `msg:`|
|`list:`|Lists all users online.|
|`help:`|Lists all available commands|
|`clear:`|Clears terminal|
|`name:`|Changes nickname|
|`exit:`|Leave Server and close program|

### Notes:
Every command uses a `:` at the end of it.  
Commands like `msg:` and `name:` take in characters after the colon as the argument.  
Commands are not case sensitive.  

# Disclaimer:
This is a functional chat server. It can be exposed over the wider internet and it will function. That being said, I do not recommend anyone to use this as an actual IRC-style chat server over the internet, even if it technically can be.  
It sends plaintext traffic and was specifically written to only use glibc. It is not meant to be a modern internet communication service in it's current form.    
As such, the client and server files are configured by default to only function on localhost. This can be easily changed by reading the commend in runserver and changing your runclient file to whatever address you want to connect to.  
