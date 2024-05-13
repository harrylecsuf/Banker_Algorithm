# Banker_Algorithminput.txt file represents the input that users or other programs can edit:
### its structure is the following:
##### line 1:
 indicates the number of processes and then the number of resources
##### the next 5 lines
 represent the allocation matrix (number of lines is the number of processes)
##### the next 5 lines
 represent the maximum matrix (number of lines is the number of processes)
the last line represent the available resources
current example:
5 3  
0 1 0  
2 0 0  
3 0 2  
2 1 1  
0 0 2  
7 5 3  
3 2 2  
9 0 2  
2 2 2  
4 3 3  
3 3 2  
##### Output of this example is:  
Safe State 1 3 4 0 2  
### Another example:  
3 3  
0 1 0   
2 0 0  
3 0 2  
7 5 3  
3 2 2  
9 0 2  
2 2 2  
4 3 3  
3 3 2  
##### 3 processes and 3 resources on the first line
##### next 3 line: 
allocation matrix  
##### next 3 line: 
maximum matrix  
last line is resources available  
##### Output of this example is:
Safe State  
## Please replicate the same working directory tree:
- input text file, makefile are outside in the main working directory
- three sub directories src, obj, bin
- src will contain header files and implementations, test code and main file
- obj will have the o files, containing compiled code before linking
- bin has the executables, for test code and the main program

# GitHub Link for Project work: 
https://github.com/harrylecsuf/Banker_Algorithm.git
