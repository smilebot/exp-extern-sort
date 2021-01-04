# External Sort

This is a learning exercise to understand some of the limitations of external sorting.

The goal is to omptimally sort a large dataset, files of int values for simplicity, with a small working memory.

Learning objectives:

1. Understand chunking and merging
2. Handle the following edge cases:
    * The working memory is smaller than the (number of chunks * 4 bytes)
    * The number of files you can open (`sysctl kern.maxfiles`) on your machine is smaller than your working memory.
    * More to encounter..
3. Build a custom min-heap implementation to keep track of the file stream pointer when popped?


Learnings:

Current Metrics:
File size: 137mb
Buffer size: 1mb
Time: 225.553 seconds
