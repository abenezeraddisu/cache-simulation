# cache-simulation
This is a simple code to simulate cache LRU write-read system written in c. The code takes name of the
file holding the loads and stores and cache size (not including tags or valid bits) in kB, associativity, and the
block size in bytes as command line arguments. It uses LRU replacement policy. For example,
“cache-simulation tracefile 2048 8 64” will simulate a cache that is 2048kB (=2MB), 8-way setassociative, has 64-byte blocks, and uses LRU replacement. This cache will be processing the loads and stores in the file called tracefile. The cache is write-through and write-no-allocate, so the code stores both the state of cache and the entire content of simulated memory; the memory is represented as a simple array of (randomly) 16M bytes. More complex implemenation of the chache heirarchy system I did could be found here. 
