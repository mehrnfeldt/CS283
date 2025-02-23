1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

waitpid() ensures that the parent shell waits for all child processes to complete before accepting new input, preventing zombie processes and resource leaks. Without calling waitpid(), child processes could terminate but their exit status wouldn't be collected, leading to incomplete execution and potential errors in the shell. Additionally, forgetting waitpid() could cause the shell to prompt for new input while commands are still executing, disrupting the intended flow.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

After using dup2() to redirect input and output file descriptors, it's necessary to close the unused pipe ends to avoid resource leakage and unintended behavior. If the unused pipe ends are left open, they could cause issues such as deadlocks, where processes wait on each other indefinitely, or data could be mistakenly written or read by the wrong process.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in because it directly modifies the shell’s working directory, which affects the current process’s environment. If cd were implemented as an external command, it would execute in a separate child process, causing the change in directory to only affect that process, not the shell itself. This would lead to unexpected behavior since the shell’s working directory would remain unchanged, and the user would not see the expected results when navigating directories.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To allow an arbitrary number of piped commands, I would need to dynamically allocate memory for the command list and pipes. I could use malloc to allocate memory for command_list_t and the pipe file descriptors, resizing the arrays as needed when new commands are added. However, this introduces trade-offs: dynamic memory allocation increases complexity, as I would need to carefully manage memory, handle potential memory leaks, and ensure proper error handling for allocation failures. 
