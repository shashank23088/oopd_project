#!/bin/bash

# Create directory with 2 files of 1GB each
mkdir -p dir1
for i in {1..2}; do
  if ! dd if=/dev/zero of=dir1/file${i}.txt bs=1G count=1; then
    echo "Error creating file in dir1/file${i}.txt"
    exit 1
  else
    echo "File created successfully: dir1/file${i}.txt"
  fi
done

# Create directory with 2 files of 10MB each
mkdir -p dir2
for i in {1..2}; do
  if ! dd if=/dev/zero of=dir2/file${i}.txt bs=10M count=1; then
    echo "Error creating file in dir2/file${i}.txt"
    exit 1
  else
    echo "File created successfully: dir2/file${i}.txt"
  fi
done

# Create directory with only 2 files (10MB each)
mkdir -p dir3
for i in {1..2}; do
  if ! dd if=/dev/zero of=dir3/file${i}.txt bs=10M count=1; then
    echo "Error creating file in dir3/file${i}.txt"
    exit 1
  else
    echo "File created successfully: dir3/file${i}.txt"
  fi
done

# Function to recursively create directories with 2 files until 2 files are reached
function create_recursive_dir {
  local dir_name=$1
  local file_count=$2
  
  if [ $file_count -ge 2 ]; then
    return
  fi

  mkdir -p $dir_name
  for i in {1..2}; do
    if ! dd if=/dev/zero of=${dir_name}/file${file_count}.txt bs=10M count=1; then
      echo "Error creating file in ${dir_name}/file${file_count}.txt"
      exit 1
    else
      echo "File created successfully: ${dir_name}/file${file_count}.txt"
    fi
    ((file_count++))
  done

  for i in {1..2}; do
    create_recursive_dir ${dir_name}/subdir${i} $file_count
  done
}

# Create directory with 2 files recursively
create_recursive_dir dir4 0

# Measure time taken for each command
time_taken_dir1=$(du -sh dir1 | cut -f1)
time_taken_dir2=$(du -sh dir2 | cut -f1)
time_taken_dir3=$(du -sh dir3 | cut -f1)
time_taken_dir4=$(du -sh dir4 | cut -f1)

# Display results
echo "Time taken for dir1: $time_taken_dir1"
echo "Time taken for dir2: $time_taken_dir2"
echo "Time taken for dir3: $time_taken_dir3"
echo "Time taken for dir4: $time_taken_dir4"
