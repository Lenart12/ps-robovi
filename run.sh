#!/bin/bash

NODES_COUNT=2
CPUS_PER_NODE=2

./build.sh && srun --reservation=fri --mpi=pmix -N$NODES_COUNT -n$CPUS_PER_NODE cmake-build/ps-robovi