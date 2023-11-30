<!DOCTYPE html>
<html lang="en">
<!-- <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>C++ Shell Commands</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
        }
        code {
            background-color: #f4f4f4;
            padding: 4px;
            border-radius: 4px;
            font-family: 'Courier New', Courier, monospace;
        }
    </style>
</head> -->
<body>

<h1>C++ Shell Commands</h1>

<p>This HTML page provides an overview of the C++ Shell Commands implementation.</p>

<h2>Overview</h2>

<p>The program is structured as a C++ class called <code>Shell</code>, which encapsulates the shell functionalities. It includes methods for each supported command, such as changing directories, listing directory contents, moving and renaming files or directories, removing files or directories, and copying files or directories.</p>

<h2>Commands</h2>

<h3>cd - Change Directory</h3>

<p>Change the current working directory.</p>
<code>cd &lt;directory&gt;</code>

<!-- Repeat similar sections for other commands -->

<h2>Usage</h2>

<p>To use the shell, compile the program using a C++ compiler and run the compiled executable. Enter commands when prompted (<code>MyShell&gt;</code>). Use <code>-h</code> or <code>--help</code> with any command for detailed usage information.</p>

<p>Example:</p>
<pre>
MyShell> ls -l
MyShell> cd /path/to/directory
MyShell> mv file1.txt newfile.txt
MyShell> rm -r directory
MyShell> cp -r source destination
</pre>

<h2>Building and Running</h2>

<p>Compile the program using your preferred C++ compiler. For example:</p>
<pre>
g++ -o main main.cpp
</pre>

<p>Run the compiled executable:</p>
<pre>
./main
</pre>

</body>
</html>
