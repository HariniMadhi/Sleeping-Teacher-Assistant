# Sleeping-Teacher-Assistant
A simulation of the Sleeping teacher assistant(STA) problem's solution.
# Backgroud
The STA problem is a real time application of process synchronization. There are many other problems that can be solved using the
concept of Mutual exclusion and Semaphores such as Dining philosopher problem and Producer Consumer problem. If we try to implement
the solution for these problems using threads alone, we might come across the problem of multiple processes accessing the same
memory and this leads to inconsistency of the values stored in a variable. So we have to restrict access to the same variable by
more than one process. The concurrent operations can be done without inconsistency of data by using the concepts like critical
section and mutual exclusion.
# Quick start
First you have to compile the sta.c program.
```
gcc sta.c -lpthread -o sta
```
Now the executable file sta will be created in the folder.
You can run the program by using the following command.
```
./sta
```
# Command line arguments
The -n option is used to specify the number of students in the class.
```
./sta.c -n 7
```
This will make the program run with 7 students.

The -s option is used to run the program for specified number of seconds.
```
./sta -s 15
```
The program will end in 15 seconds.

The -f option to specify the output file.
```
./sta -f outfile
```
The output will be displayed in the file named outfile.

The --help argument can be used for any doubts.
```
./sta --help
```
# Sample Execution
```
./sta -n 10 -s 20 -f sta_output
```
This makes the program run for 20 seconds, with 10 student processes and the output will be present in the sta_output file.

Link for the report : https://docs.google.com/document/d/15bLh_HKQcE-ukSY4qMgWizJpssj_H5kIXVx3yEqpBUo/edit?usp=sharing
