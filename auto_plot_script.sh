#!/bin/bash
set -e  # Exit immediately if any command fails

# === Check input ===
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 /absolute/path/to/file.csv [1BB|2BB]"
    exit 1
fi

CSV_PATH="$1"
TAG="$2"

# === Validate inputs ===
if [[ ! "$CSV_PATH" =~ \.csv$ || ! -f "$CSV_PATH" ]]; then
    echo "Error: First argument must be an existing .csv file"
    exit 1
fi

if [[ "$TAG" != "1BB" && "$TAG" != "2BB" ]]; then
    echo "Error: Tag must be either 1BB or 2BB"
    exit 1
fi

# === Derive variables ===
CSV_FILENAME=$(basename "$CSV_PATH")
CSV_NAME="${CSV_FILENAME%.csv}"
INPUT_SUBDIR="./input_dir/$CSV_NAME"
CSV_DEST="$INPUT_SUBDIR/$CSV_FILENAME"
XBB_DIR="$INPUT_SUBDIR/csv_$TAG"
PLOTS_DIR="$XBB_DIR/plots"
ROOT_PATH="input_dir/$CSV_NAME/csv_$TAG/"

# === Prevent overwrite ===
if [[ -d "$INPUT_SUBDIR" ]]; then
    echo "Error: Directory '$INPUT_SUBDIR' already exists. Aborting to prevent overwrite."
    exit 1
fi

if [[ -f "$CSV_DEST" ]]; then
    echo "Error: Destination CSV file '$CSV_DEST' already exists. Aborting."
    exit 1
fi

# === Check dependencies ===
if [[ ! -f "bash/ProcessSQL.sh" ]]; then
    echo "Error: Missing script 'bash/ProcessSQL.sh'"
    exit 1
fi

if [[ ! -f "python/merge_timing_files.py" ]]; then
    echo "Error: Missing script 'python/merge_timing_files.py'"
    exit 1
fi

if [[ ! -f "scripts/runAllTiming.C" ]]; then
    echo "Error: Missing ROOT script 'scripts/runAllTiming.C'"
    exit 1
fi

# === Proceed ===
echo "Creating directory $INPUT_SUBDIR..."
mkdir -p "$INPUT_SUBDIR"
cp "$CSV_PATH" "$CSV_DEST"

echo "Running SQL processing..."
source bash/ProcessSQL.sh "input_dir" "$TAG"

echo "Creating plots directory..."
mkdir -p "$PLOTS_DIR"

echo "Merging timing files..."
python python/merge_timing_files.py -i "$XBB_DIR/" -max 1000000

echo "Running ROOT script on $ROOT_PATH..."
root -l -b -q "scripts/runAllTiming.C(\"$ROOT_PATH\")"

echo "All operations complete for $CSV_NAME with tag $TAG :)"
