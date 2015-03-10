# FaSE

    //                                                
    //  88888888888           ad88888ba   88888888888  
    //  88                   d8"     "8b  88           
    //  88                   Y8,          88           
    //  88aaaaa  ,adPPYYba,  `Y8aaaaa,    88aaaaa      
    //  88"""""  ""     `Y8    `"""""8b,  88"""""      
    //  88       ,adPPPPP88          `8b  88           
    //  88       88,    ,88  Y8a     a8P  88           
    //  88       `"8bbdP"Y8   "Y88888P"   88888888888  
    //                                                 

_Pedro {Paredes, Ribeiro} - DCC/FCUP_

## Version Information
FaSE Version 0.3 - Launched March 8 2014

## Compilation and Usage
To compile the Source Code use the `make` command (at least on Linux).
If you wish to cleanup extra compilation files run the command `make clean`.

To use the FaSE software run `./FASE -h` to display a small help information
with instructions on what arguments should be provided.

Main Settings: `./FASE -s <Subgraph Size> -i <input file> [arguments...]`

All commands:

    -h : Displays this help information
	-s <Integer> : Subgraph Size
	-i <Filename> : Name of input file (Format in Readme)
	-d : Directed Subgraph (Default undirected)
	-o : Name of output file (Default is stdout)
	-dt : Detailed Result (Displays all subgraph types and occurrences)
	-z : Use 0-based input (Suitable for input files starting at node 0)
	-tm : Use Adjacency Matrix LS-Labeling (Default is Adjacency List Labeling)
	-l : Use Adjacency List Only (Suitable for Large Scale or large networks [>10^5 nodes])
	-p <P1> <P2> ... <Ps> : Sets the sampling probabilities by depth (note that -s must have been selected first and this flag is optional)
	-pl <Psamp> : Sets the probabilities like in -p but with the low sampling setup
	-pm <Psamp> : Sets the probabilities like in -p but with the medium sampling setup
	-ph <Psamp> : Sets the probabilities like in -p but with the high sampling setup
	-q : Ignore arguments and prompt input

## Input Information
The inputed networks should be in an edge list format, meaning that the input
file should only contain lines with information about an edge, starting with
node 1 or 0 (can be specified in the arguments) and with all edges from 0/1
to `<Number of edges> - 1 / <Number of edges>` being used.

## Folder Information
The main folder contains all source files from the FaSE Source Code, this Read Me
file and two folders. The "nauty" folder contains the code from Brendan McKay's
Nauty algorithm for isomorphism tests. The "Networks" folder contains the used
networks in the process of testing the FaSE algorithm, organized by "Undirected"
and "Directed" folders straightforwardly in .edges files as described above (1-based).

## Sampling Information
Version 0.3 includes the sampling version of the FaSE algorithm. To use it just run the command with the "-p" flag followed by as many probabilities (numbers between 0 and 1) as the searched subgraph size. To know more about these probabilities refer to our second paper submitted to SNAM.

If you don't want to figure out the individual probabilities use the flags `-pl <per>`, `-pm <per>` or `-ph <per>`, where the `<per>` is the sampling percentage (the average percentage of sampled subgraphs, better defined in the SNAM paper), and the `pl`, `pm` and `ph` are respectively the low, medium and high sampling setups (also defined in the SNAM paper, but briefly speaking, the low setup has the most variance, but is slightly faster, the high is the one with least variance, but is slightly slower and the medium is in the middle of both).

Note that, as indicated in the helper file, you need to specify the `-s` flag before the `-p` (or any `-pl`, `-pm`, `-ph`).

Also, if you want to run the full enumeration, the `-p` command is optional, so if you run the command with no `-p` it will perform a full enumeration as it did in previous versions.

## Extra Information
The Source Code was developed and tested on a Linux environment, however it
should work on other OSs too.

This Source Code was developed along the following articles:

* [ASONAM'2013](http://asonam.cpsc.ucalgary.ca/): Pedro Paredes and Pedro Ribeiro - [Towards a Faster
Network-Centric Subgraph Census](http://dl.acm.org/citation.cfm?doid=2492517.2492535).
* (Submitted to) [SNAM](https://www.springer.com/computer/database+management+%26+information+retrieval/journal/13278): Pedro Paredes and Pedro Ribeiro - FaSE: Fast Exact and Approximate Subgraph Census

This software uses the nauty program version 2.4 by Brendan McKay. Therefore, nauty's
license restrictions apply to the usage of FaSE.

## Website Link
[http://www.dcc.fc.up.pt/gtries/fase/](http://www.dcc.fc.up.pt/gtries/fase/).

## Authors
The Source Code was created by Pedro Paredes and Pedro Ribeiro from CRACS & INESC-TEC
DCC-FCUP, Universidade do Porto, Portugal.

Their contacts are:

* [pparedes@dcc.fc.up.pt](mailto:pparedes@dcc.fc.up.pt)
* [pribeiro@dcc.fc.up.pt](mailto:pribeiro@dcc.fc.up.pt)
