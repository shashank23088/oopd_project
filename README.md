<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Multi-Threaded C++ Shell - README</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 20px;
        }

  h1, h2, h3 {
      color: #333;
  }

  code {
      background-color: #f4f4f4;
      border: 1px solid #ddd;
      padding: 5px;
      font-family: "Courier New", Courier, monospace;
  }

  pre {
      background-color: #f8f8f8;
      border: 1px solid #ddd;
      padding: 10px;
      overflow: auto;
  }

  ul {
      list-style-type: none;
      padding: 0;
  }

  ul li::before {
      content: "▹";
      margin-right: 10px;
      color: #777;
  }
</style>
</head>

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
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
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
