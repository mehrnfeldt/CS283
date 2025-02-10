1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() reads a whole line of input including spaces. This is useful for handling commands that have arguments. Other functions might not read the spaces as included in the input which would not be useful for shell input parsing. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  malloc() allows for a dynamic allocation of memory at runtime. This means that it is not bound to a specific memory buffer size which could be a waste of memory if too large or could cause errors if too small. I am not limited by a predetermined buffer size when using malloc(). 

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If leading spaces are not trimmed, a command name might be incorrectly interpreted as an empty command or invalid. Also, commands could be incorrectly stored as "cmd1 " with an extra space which would interfer with string operations.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection allows users to change the default behaviors by sending output to files, reading input from files, or handling error messages differently. We can use output redirection (> and >>) to eith rewrite a file or append to a file. Some limitations would include the potential for getting the two mixed up.We could use input redirection (<) to read file text as input to a command. A challege would be to remember to close the file after using it. We could also use error redirection. 2> which redirects stderr to a file and overwrites it. 2>> redirects stderr to a file but appends it. And &> redirects both stdout and stderr to output.log. A limitation would be about properly ensuring that error messages are correctly captured without messing with normal output. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection sends input or output to or from files. It either saves outout to a file or reads input from a file. Piping involved sending output of one command directly as input to another command. This is used to process data dynamically. Piping does not involve files. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep STDOUT and STDERR separate in a shell because if it is mixed together then the valid results would be intertwined with the error messages making it confusing to program. Also, this separation make debugging and writing code easier. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Valid commands will appear in STDOUT while error will appear on STDERR. We should display any error messages, and keep the two distinctly separate. Also, I think error codes should be implemented as well. Yes, our shell should provide a way to merge STDOUT and STDERR for flexibile handling of output. This can be done using 2>&1, which redirects STDERR to STDOUT, allowing both streams to be processed together.
