#!/usr/bin/env bash
set -u

if ! make clean || ! make all; then
    echo "build failed" >&2
    exit 1
fi

dlc_output=$(./check_ops.py bits.c 2>&1)
dlc_status=$?
if [ "$dlc_status" -ne 0 ]; then
    echo "operator check failed" >&2
    printf '%s\n' "$dlc_output" >&2
    exit 1
fi
printf '%s\n' "$dlc_output"
echo "operator check passed"

btest_output=$(./btest 2>&1)
btest_status=$?
printf '%s\n' "$btest_output"
if [ "$btest_status" -ne 0 ]; then
    echo "btest failed" >&2
    exit 1
fi

score=$(printf '%s\n' "$btest_output" | sed -n 's/.*Total points: *\([0-9][0-9]*\/[0-9][0-9]*\).*/\1/p' | tail -n 1)
if [ "$score" != "110/110" ]; then
    echo "incomplete score: ${score:-not found}" >&2
    exit 1
fi

echo "all checks passed: $score"
