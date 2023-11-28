#!/bin/bash

# Check if the C++ source file is provided as an argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <cpp_file>"
    exit 1
fi

cpp_file="$1"
program_name="${cpp_file%.*}"

# Compile the C++ code
g++ -o "$program_name" "$cpp_file"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful"

    # Run the C++ program
    ./"$program_name"

    # Measure time taken for C++ program execution
    start_time=$(date +%s.%N)
    ./"$program_name"
    end_time=$(date +%s.%N)
    time_taken=$(echo "$end_time - $start_time" | bc)

    # Display results with precision up to 4 decimals
    printf "Time taken: %.4f seconds\n" "$time_taken"
else
    echo "Compilation failed"
fi

# Clean up
rm "$program_name"
