#!/bin/bash

PS_MAIN="$1"

case "$PS_MAIN" in
    mpi)
        TASK_COUNT="${2:-8}"
        CPUS_PER_TASK=1
        FIRST_ARG="$TASK_COUNT"
        SECOND_ARG="${3}"
        THIRD_ARG="${4}"
        FOURTH_ARG="${5}"
        ;;
    pthread)
        CPUS_PER_TASK="${2:-8}"
        TASK_COUNT=1
        FIRST_ARG="$CPUS_PER_TASK"
        SECOND_ARG="${3}"
        THIRD_ARG="${4}"
        FOURTH_ARG="${5}"
        ;;
    serial)
        CPUS_PER_TASK=1
        TASK_COUNT=1
        FIRST_ARG="${2}"
        SECOND_ARG="${3}"
        THIRD_ARG="${4}"
        ;;
    *)
        echo "Prvi argument ni [mpi/pthread/serial]" >&2
        exit 1
        ;;
esac

mkdir -p output
rm output/*

./build.sh && srun --reservation=fri --mpi=pmix -n"$TASK_COUNT" -c"$CPUS_PER_TASK" cmake-build/ps-robovi "$PS_MAIN" "$FIRST_ARG" "$SECOND_ARG" "$THIRD_ARG" "$FOURTH_ARG"