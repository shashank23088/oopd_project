<!DOCTYPE html>
<html lang="en">

<body>

<h1>Multi-Threaded File Creation</h1>

<p>This repository contains C++ code for multi-threaded file creation. It includes three main programs and a
  script for compilation and execution.</p>

<h2>main1.cpp</h2>

<p>This program creates multiple directories and files concurrently using C++ threads. It supports specifying
  the number of files, the number of threads, and introduces the ability to create files with random data.</p>

<h3>Usage:</h3>

```bash
g++ -o main1 main1.cpp
./main1
```

<h2>main2.cpp</h2>

<p>Similar to <code>main1.cpp</code>, this program creates directories and files concurrently. It includes
  error handling for file creation and provides a function to create directories if they don't exist.</p>

<h3>Usage:</h3>

```bash
g++ -o main2 main2.cpp
./main2
```

<h2>main3.cpp</h2>

<p>This program demonstrates recursive directory creation with an atomic counter for file indexing. It also
  introduces a shell script for compiling and running the programs concurrently.</p>

<h3>Usage:</h3>

```bash
g++ -o main3 main3.cpp
./main3
```

<h2>script.sh</h2>

<p>A shell script for compiling and running the C++ programs. It measures the time taken for program execution
  and displays the results.</p>

<h3>Usage:</h3>

```bash
./script.sh main1.cpp
./script.sh main2.cpp
./script.sh main3.cpp
```

<p>Make sure to replace <code>mainX.cpp</code> with the actual filename you want to compile and run.</p>

</body>

</html>
