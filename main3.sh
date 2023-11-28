#!/bin/bash

j=2

# Function to create directory with 10 MB files
function create_dir {
  local dir_name=$1
  local total_files=$2
  
  if [ $total_files -eq 0 ]; then
    return
  fi

  mkdir -p $dir_name
  for i in {1..100}; do
    if ! dd if=/dev/zero of=${dir_name}/file${i}.txt bs=1 count=10; then
      echo "Error creating file in ${dir_name}/file${i}.txt"
      exit 1
    else
      echo "File created successfully: ${dir_name}/file${i}.txt"
    fi
    ((total_files--))
  done

  create_dir ${dir_name}/subdir${j} $total_files
  ((j++))

}

# Measure time taken for directory creation
time_taken=$( { time create_dir dir1 10000; } 2>&1 | grep real | awk '{print $2}' )

# Display results
echo "Time taken: ${time_taken} seconds"