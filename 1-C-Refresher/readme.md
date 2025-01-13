//TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist? 

The code first checks if argc is less than 2 which means at least one command-line argument is provided. If argc < 2, then argv[1] would not exist and would be unsafe. The second condition (*argv[1] != '-') ensures that the first argument after the program's name is an option flag starting with a -. If either condition fails, it safely exits with an error message.



//TODO:  #2 Document the purpose of the if statement below 

This ensures that the user provides a string as an argument. If the number of arguments (argc) is less than 3, it means the user has not provided the string, so the program exits with a usage message. This avoids future errors caused by trying to process a missing string argument.  



//TODO:  #7  Notice all of the helper functions provided in the starter take both the buffer as well as the length. Why do you think providing both the pointer and the length is a good practice, after allwe know from main() that the buff variable will have exactly 50 bytes?

Providing both the pointer and the length to helper functions is a good practice because it makes the function more robust and easier to maintain. In this case the buffer size is always 50 bytes but explicitly passing the length ensures the code can be debugged easier.  
