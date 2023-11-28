#!/bin/bash

function create_dir {
  local dir_name=$1
  
  mkdir -p $dir_name
  for i in {1..100}; do
    if ! dd if=/dev/zero of=${dir_name}/file${i}.txt bs=1K count=1; then
      echo "Error creating file in ${dir_name}/file${i}.txt"
      exit 1
    else
      echo "File created successfully: ${dir_name}/file${i}.txt"
    fi
  done
}

# Measure time taken for directory creation
time_taken_dir1=$( { time create_dir dir1; } 2>&1 | grep real | awk '{print $2}' )
time_taken_dir2=$( { time create_dir dir2; } 2>&1 | grep real | awk '{print $2}' )

# Display results
echo "Time taken for dir1: ${time_taken_dir1} seconds"
echo "Time taken for dir2: ${time_taken_dir2} seconds"