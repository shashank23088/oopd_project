<!DOCTYPE html>
<html lang="en">

<body>

    <h1>C++ Directory and File Creation</h1>

    <p>This project consists of three C++ programs (<code>main1.cpp</code>, <code>main2.cpp</code>, <code>main3.cpp</code>)
        and a shell script (<code>script.sh</code>) to compile and run them. Each C++ program creates directories and
        files with specific sizes.</p>

    <h2>Contents</h2>

    <ol>
        <li><a href="main1.cpp">main1.cpp</a>: Creates directories <code>dir1</code> and <code>dir2</code> with 100
            files each, where each file is 1 KB in size.</li>
        <li><a href="main2.cpp">main2.cpp</a>: Creates directories <code>dir1</code> and <code>dir2</code> with 10,000
            files each, where each file is 10 bytes in size.</li>
        <li><a href="main3.cpp">main3.cpp</a>: Creates a directory structure with a total of 10,000 files, organized
            into subdirectories. Each file is 10 bytes in size.</li>
        <li><a href="script.sh">script.sh</a>: A shell script to compile and run the C++ programs, measure the time
            taken for execution, and display the results.</li>
    </ol>

    <h2>Usage</h2>

    <h3>Compile and Run</h3>

    <p>To compile and run any of the C++ programs, use the provided shell script:</p>

    <pre><code>./script.sh &lt;cpp_file&gt;</code></pre>

    <p>Replace <code>&lt;cpp_file&gt;</code> with the desired C++ source file (<code>main1.cpp</code>,
        <code>main2.cpp</code>, or <code>main3.cpp</code>).</p>

    <h3>Example</h3>

    <pre><code>./script.sh main1.cpp</code></pre>

    <h3>Output</h3>

    <p>The script will display whether the compilation was successful, run the C++ program, measure the time taken for
        execution, and clean up the compiled executable.</p>

    <h2>Contributing</h2>

    <p>Feel free to contribute to this project by submitting issues or pull requests.</p>

</body>

</html>
