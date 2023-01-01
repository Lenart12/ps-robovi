#!/bin/bash

PS_MAIN="$1"

case "$PS_MAIN" in
    mpi)
        TASK_COUNT="${2:-8}"
        CPUS_PER_TASK="${3:-1}"
        ;;
    pthread)
        CPUS_PER_TASK="${2:-8}"
        TASK_COUNT="${3:-1}"
        ;;
    serial)
        CPUS_PER_TASK=1
        TASK_COUNT=1
        ;;
    *)
        echo "Prvi argument ni [mpi/pthread/serial]" >&2
        exit 1
        ;;
esac

mkdir -p output

./build.sh && srun --reservation=fri --mpi=pmix -n"$TASK_COUNT" -c"$CPUS_PER_TASK" cmake-build/ps-robovi "$PS_MAIN"