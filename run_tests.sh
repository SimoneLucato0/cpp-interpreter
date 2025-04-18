#!/bin/bash

PROGRAM=./tokenize

all_passed=true
failed_tests=()

for f in tests/*.expected; do
    sed -i 's/\r$//' $f
    if [ -f "$f" ] && [ -n "$(tail -c1 "$f")" ]; then
        echo "WARNING: $f does not end with a newline"
    fi
done

for testfile in tests/*.txt; do
    base=${testfile%.txt}
    out_actual="${base}.out.actual"
    err_actual="${base}.err.actual"
    out_expected="${base}.out.expected"
    err_expected="${base}.err.expected"

    # Run the program, capturing stdout and stderr
    $PROGRAM tokenize "$testfile" > "$out_actual" 2> "$err_actual"

    # Compare outputs and capture diffs
    diff -u "$out_expected" "$out_actual" > "${base}.out.diff"
    out_ok=$?
    diff -u "$err_expected" "$err_actual" > "${base}.err.diff"
    err_ok=$?

    if [ $out_ok -eq 0 ] && [ $err_ok -eq 0 ]; then
        echo "[PASS] $testfile"
        # Clean up diff files since there's no difference
        rm -f "${base}.out.diff" "${base}.err.diff"
    else
        echo "[FAIL] $testfile"
        failed_tests+=("$testfile")
        if [ $out_ok -ne 0 ]; then
            echo "  [stdout differs]:"
            cat "${base}.out.diff"
        fi
        if [ $err_ok -ne 0 ]; then
            echo "  [stderr differs]:"
            cat "${base}.err.diff"
        fi
    fi
done

if [ ${#failed_tests[@]} -eq 0 ]; then
    echo "All tests passed."
    rm -f tests/*.out.actual
    rm -f tests/*.err.actual
    exit 0
else
    echo
    echo "Some tests failed:"
    for t in "${failed_tests[@]}"; do
        echo "  $t"
    done
    exit 1
fi
