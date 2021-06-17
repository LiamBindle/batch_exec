# batch_exec

batch_exec is an MPI program for running batches of one-liners (one-line shell expressions). This is useful for things like validating large lists of files. 

A batch of one-liners is a list of shell commands like so

```
echo 1; sleep 0.1s
echo 2; sleep 0.2s
echo 3; sleep 0.3s
echo 4; sleep 0.4s
```

For this demo, lets assume that list is saved in `example_batch.txt`. We can execute this batch of commands like so

```console
$ mpirun -np 3 batch_exec -i example_batch.txt
1
2
3
4
```

### Some notes

- Each one-liner is executed by a worker process (one of the MPI processes).
- Your one-liners' output should be a single-line.
  - If a trailing newline character is present, it is automatically stripped.
- The **output is not sequential**. One-liners are dispatched sequentially, but their output is reported immediately.
- The minimum number of processes is 2 (one dispatcher, one worker).
- Empty lines in the one-liner-list are ignored (e.g., a trailing newline in the list file is okay).
- See `batch_exec -h` for usage and arguments details.


