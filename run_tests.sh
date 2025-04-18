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

    # Check if expected stderr is non-empty
    expect_exit_65=false
    if [ -s "$err_expected" ]; then
        expect_exit_65=true
    fi

    # Run the program, capturing stdout, stderr, and exit code
    $PROGRAM tokenize "$testfile" > "$out_actual" 2> "$err_actual"
    prog_exit_code=$?

    # Compare outputs and capture diffs
    diff -u "$out_expected" "$out_actual" > "${base}.out.diff"
    out_ok=$?
    diff -u "$err_expected" "$err_actual" > "${base}.err.diff"
    err_ok=$?

    # Check exit code if expected stderr is non-empty
    exit_ok=1
    if $expect_exit_65; then
        if [ $prog_exit_code -eq 65 ]; then
            exit_ok=0
        else
            exit_ok=1
        fi
    else
        if [ $prog_exit_code -eq 0 ]; then
            exit_ok=0
        else
            exit_ok=1
        fi
    fi

    if [ $out_ok -eq 0 ] && [ $err_ok -eq 0 ] && [ $exit_ok -eq 0 ]; then
        echo "[PASS] $testfile"
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
        if [ $exit_ok -ne 0 ]; then
            echo "  [exit code differs]: Expected $([ $expect_exit_65 = true ] && echo 65 || echo 0), got $prog_exit_code"
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
