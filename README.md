<!DOCTYPE html>
<html lang="en">

<body>

<h1>Multi-Threaded C++ Shell</h1>

<p>Welcome to the documentation for the Multi-Threaded C++ Shell project! This C++ program implements a multi-threaded shell
    with basic file and directory manipulation commands. It allows users to perform operations like changing directories,
    listing directory contents, moving, removing, and copying files and directories concurrently using threads.</p>

<h2>Table of Contents</h2>

<ul>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#commands">Commands</a></li>
    <li><a href="#multithreading">Multi-Threading</a></li>
    <li><a href="#examples">Examples</a></li>
</ul>

<h2 id="usage">Usage</h2>

<p>To use the multi-threaded shell, compile the program and run the executable. Once launched, it will provide a command-line
    interface where you can enter commands.</p>

<pre><code>./your_executable_name</code></pre>

<h2 id="commands">Commands</h2>

<p>The following commands are supported:</p>

<ul>
    <li><strong>cd</strong>: Change current working directory.</li>
    <li><strong>ls</strong>: List directory contents with options for hidden files, long format, and recursion.</li>
    <li><strong>mv</strong>: Move or rename files or directories.</li>
    <li><strong>rm</strong>: Remove files or directories with options for recursion, interactive mode, and force.</li>
    <li><strong>cp</strong>: Copy files or directories with options for recursion and interactive mode.</li>
</ul>

<h2 id="multithreading">Multi-Threading</h2>

<p>The program utilizes multi-threading to concurrently perform file and directory operations, providing improved efficiency.
    Multi-threading allows the shell to handle multiple tasks simultaneously, enhancing performance and responsiveness.</p>

<h2 id="examples">Examples</h2>

<pre><code>ls -l</code></pre>

<p>This command lists the contents of the current directory in long format using multiple threads.</p>

<pre><code>mv file.txt new_directory/ -r</code></pre>

<p>This command moves the file named <code>file.txt</code> to the directory <code>new_directory</code> recursively using
    multiple threads.</p>

</body>

</html>
