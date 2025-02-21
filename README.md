# Process and Thread Management

A project to explore process and thread management using POSIX and Pthreads by implementing both process-based and threaded solutions, and then comparing their performance.

## Summary
In this task we implemented two versions of a parallelizable computational task using both processes and threads. In Part 1, we created a program that uses fork() to spawn child processes and employs pipes (or shared memory) for inter-process communication (IPC). In Part 2, we implemented multithreaded processing using Pthreads to coordinate concurrent task execution and manage access to shared data. Part 3 involves measuring and comparing the execution times of the process-based and thread-based solutions, varying the number of processes/threads to evaluate scalability. Finally, Part 4 focuses on managing threads by creating joinable and detached threads and analyzing the impact of join versus detach on throughput. Throughout the project, proper error handling, resource cleanup, and documentation are required.

## Specifications
- **Part 1: Process Management**
  - Create child processes using `fork()`.
  - Implement IPC using pipes (or shared memory).
  - Manage process execution, termination, and cleanup.
  - Include comprehensive error handling and in-code documentation.

- **Part 2: Multithreaded Processing**
  - Create threads using Pthreads.
  - Coordinate threads to execute a common task.
  - Ensure safe shared data access using proper synchronization techniques.
  - Manage thread creation, joining/detachment, and cleanup.
  - Provide detailed comments and documentation.

- **Part 3: Performance Measurement**
  - Implement a parallelizable computational task (e.g., matrix multiplication) with both process-based and thread-based approaches.
  - Measure execution times and compare performance.
  - Vary the number of processes/threads to analyze scalability.
  - Present clear performance comparison data in your report.

- **Part 4: Thread Management**
  - Implement both joinable and detached threads.
  - Measure throughput with various join/detach configurations.
  - Analyze the impact of joining versus detaching on overall performance.
  - Document the implementation details and analysis clearly.

- **Submission Requirements:**
  - **Source Code:** Submit your C source code (.c) files.
  - **Report:** Provide a PDF report (6–8 pages) explaining your design, implementation, test results, and analysis.
  - **Packaging:** Zip all files and submit by the deadline.

- **Learning Objectives:**
  - Gain hands-on experience with POSIX process and thread management.
  - Learn how to implement and coordinate inter-process communication.
  - Understand synchronization mechanisms in multithreaded applications.
  - Analyze and compare performance trade-offs between process-based and thread-based designs.

## Author

Qusay Taradeh
