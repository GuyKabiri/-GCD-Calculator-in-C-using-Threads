# GCD Calculator in C using Threads

A small Unix / Linux assignment in the 'Operation Systems' course which calculates the GCD of pairs of numbers.
There are 2 versions, one that using a process for each pair, and one that uses a constant number of processes and communicates with the main process using pipes.


## V1

V1 reads pairs of numbers from STDIN, but can get a file name in the command line argument to read from it.
All the pairs are saved in a shared memory array so each thread can access it. Each thread gets specific indexes in that array to read and write to so no need for any lock.
The main thread joins all of the threads and then prints the pairs and their calculated gcd.

#### Usage
Run `./v1` in the command line in the `v1` folder, or `./v1/v1` in the main folder, to user STDIN as an input source.
Press `ctrl + d` to stop.

##### Optional:
Run `./v1 file_name` in the command line in `v1` folder, or `./v1/v1 file_name` in the main folder.
Where `file_name` is the file to read from, it should contain pairs of numbers separated with spaces or tabs, each pair in a separate row.


## V2

V2 reads pairs of numbers from STDIN, but can get a file name in the command line argument to read from it.
All the pairs are saved in a shared memory array so each thread can access it. Each thread takes one pair from the array and calculates it's gcd, a mutex lock is used to synchronize uses of the array.
Each thread prints the pair and the calculated gcd when done.

#### Usage

Run `./v2` in the command line in the `v2` folder, or `./v2/v2` in the main folder, to user STDIN as an input source.
Press `ctrl + d` to stop.

##### Optional:
Run `./v2 file_name` in the command line in `v2` folder, or `./v2/v2 file_name` in the main folder.
Where `file_name` is the file to read from, it should contain pairs of numbers separated with spaces or tabs, each pair in a separate row.