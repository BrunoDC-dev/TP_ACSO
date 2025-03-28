#!/bin/bash

# TO RUN:
# chmod +x compare_rdump.sh
# ./compare_rdump.sh <program_file1> [program_file2 ...] [--cycles=N] [--mdump_low=0xLOW] [--mdump_high=0xHIGH]

set -e

# Get the absolute path of the script
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Check if program files are provided
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <program_file1> [program_file2 ...] [--cycles=N] [--mdump_low=0xLOW] [--mdump_high=0xHIGH]"
    exit 1
fi

# Default values
CYCLES=4
MDUMP_LOW=0x10000000
MDUMP_HIGH=0x10001000
ARGS=()

# Parse arguments
for arg in "$@"; do
    if [[ "$arg" == --cycles=* ]]; then
        CYCLES="${arg#--cycles=}"
    elif [[ "$arg" == --mdump_low=* ]]; then
        MDUMP_LOW="${arg#--mdump_low=}"
    elif [[ "$arg" == --mdump_high=* ]]; then
        MDUMP_HIGH="${arg#--mdump_high=}"
    else
        ARGS+=("$arg")
    fi
done

# Check if the program file exists
if [ ! -f "${ARGS[0]}" ]; then
    echo "❌ Error: Program file '${ARGS[0]}' not found!"
    exit 1
fi

### 🔹 Run ref_sim_x86 and save its dumpsim
echo "Running ref_sim_x86..."
./ref_sim_x86 "${ARGS[@]}" <<EOF
run $CYCLES
rdump
mdump $MDUMP_LOW $MDUMP_HIGH
q
EOF

# Ensure dumpsim exists
if [ ! -f "$SCRIPT_DIR/dumpsim" ]; then
    echo "❌ No dumpsim file found after running ref_sim_x86"
    exit 1
fi

# Copy the reference dumpsim for comparison
cp "$SCRIPT_DIR/dumpsim" "$SCRIPT_DIR/ref_dumpsim"

### 🔹 Run sim and save its dumpsim
echo "Running sim..."
cd src
./sim "../${ARGS[@]}" <<EOF
run $CYCLES
rdump
mdump $MDUMP_LOW $MDUMP_HIGH
q
EOF

# Ensure dumpsim exists
if [ ! -f "$SCRIPT_DIR/src/dumpsim" ]; then
    echo "❌ No dumpsim file found after running sim"
    exit 1
fi

# Copy the sim dumpsim for comparison
cp "$SCRIPT_DIR/src/dumpsim" "$SCRIPT_DIR/sim_dumpsim"

# Go back to the main directory
cd "$SCRIPT_DIR"

### 🔹 Compare the two dumpsim files
echo "Comparing dumpsim files..."
diff -u ref_dumpsim sim_dumpsim > diff_output.txt || true

if [ -s diff_output.txt ]; then
    echo "❌ Differences found! See diff_output.txt for details."
    cat diff_output.txt
    exit 1
else
    echo "✅ No differences found in dumpsim files."
    exit 0
fi