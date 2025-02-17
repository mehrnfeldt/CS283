1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  By calling fork before execvp we preserves the parent process since fork creates a child process that executes a new program. Then the parent still runs and is not overwritten. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork call fails then the operating system was incapable of creating a new process this could be due to eithe insuffiecient system resources or reaching the system's process limit. Also, no child is created when fork fails. I return an exit code that idicates a failure and also printing "fork failed".

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command to execute by searching for the specified program in the directories listed in the PATH environment variable. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  When a parent process forks a child process, calling wait() it ensures that the parent properly gathers the child process, removing it form the system's process table. Wait() also makes the parent pause execution until the child process finishes and allows hte parent to check of the child succeeded, failed or was terminated. If wait() was not called the parent process would run without knowing the data from the child process. Also, the child process remains useless as the parent never recieved its exit status. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() extracts the exit status of a child process after it has been reaped using wait(). It recieves the lower bits of the exit code. It is important as it helps determine the execution of the child process(success or failure). 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My code handles quoted arguments with a loop that checks for quotes. It reads the quoted characters and stores this in an array. A space is treated as part of the argument. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I didn't have the quoted logic to respect spaces in an argument so I had to implement this. I also added the cd logic that does nothing if cd is called with no arguments. Then the implementation of dragon as its own file was implements. For the parsing logic I did this loop directly in the exec_local_cmd_loop() function. This was altered from the previous assignment as I directly in this one function. I implemented this code without using the build_cmd_buff() function so that was a challenge to implement my previous logic without calling it as I didn't exactly know where that would come into place. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are used to motify processes about certain events or conditions. They can cause a notification when a specific event occurs, infor a process about exceptional conditions, and interrupt, pause or kill a process. This is different form other forms of IPC because they are asynchronous and interrupt the normal flow of execution. They also do not carry data or pass data structures in most cases.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is a signal that teminates a process immediately. It is used when a process is stuck like in an infinite loop and need to be terminated. SIGTERM is a default signal that is used to request the termination of a process. It is a little more forgiving than SIGKILL as it allows the process to perform cleanup before being terminated. It is used to shut down a process nicely. This could be like when a user wants to stop a running program but want to git it the chance to cleanup before exiting. SIGSTOP is a signal used to stop a process temporarilty. A process is suspended once it recieve this signal. 
It is commonly used in debugging situations since only temporarily pauses a process and can be resumed with signal SIGCONT. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When receives a process is paused by the operating system. It halts a process and it will no longer continue. The process can be continued using signal SIGCONT. It can not be caught or ignored like SIGINT because SIGSTOP is a special signal designed for controlling process execution at the system level. It provides a mechanism for stopping a process without giving the process the ability to alter the signal suspension. 
