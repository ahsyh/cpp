http://www.cs.sfu.ca/~ashriram/courses/2013/CS431/assignments/Ass2/Ass2.html

g++ -ggdb3 -m64 -std=c++0x -pthread -lrt -Wall -O3 ppp.C random.C TaskGroup.C driver-sort.C -o driver-sort
g++ -ggdb3 -m64 -std=c++0x -pthread -lrt -Wall -O3 ppp.C random.C TaskGroup.C driver-compute.C -o driver-compute
g++ -ggdb3 -m64 -std=c++0x -pthread -lrt -Wall -O3 ppp.C random.C TaskGroup.C driver-reduce.C -o driver-reduce

./driver-sort --particles 40000000 --trials 5 --grainsize 1000 --threads 4
./driver-compute --particles 10000000 --trials 5 --grainsize 1000 --threads 4
./driver-reduce --particles 100000000 --trials 5 --grainsize 1000 --threads 4

