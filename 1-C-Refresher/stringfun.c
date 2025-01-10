#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *dest = buff;  // Pointer to the buffer
    char *src = user_str;  // Pointer to the user input string
    int char_count = 0;
    int word_flag = 0;  // Tracks if we are inside a word

    while (*src != '\0' && char_count < len) {
        if (*src != ' ' && *src != '\t') {
            *dest++ = *src;
            char_count++;
            word_flag = 1;  // Mark we are inside a word
        } else if (word_flag) {
            // Only add a single space between words
            *dest++ = ' ';
            char_count++;
            word_flag = 0;  // Reset flag for new word
        }
        src++;
    }

    if (char_count >= len) {
        return -1;  // User string is too long
    }

    // Fill the remaining buffer space with dots
    while (char_count < len) {
        *dest++ = '.';
        char_count++;
    }

    return char_count;  // Return the user string length
}
	


void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    (void) len;
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        char current = *(buff + i);
        if (current == ' ') {
            if (in_word) {
                in_word = 0; // End of a word
            }
        } else {
            if (!in_word) {
                word_count++; // Start of a new word
                in_word = 1;
            }
        }
    }

    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      If argc < 2, then argv[1] would not exist and would be unsafe. The second condition makes sure that the first argument after the program's name is an option flag starting with a -. If either condition fails, it safely exits with an error message.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This ensures that the user provides a string as an argument. If the number of arguments (argc) is less than 3, it means the user has not provided the string, so the program exits with a usage message. This avoids future errors caused by trying to process a missing string argument.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
    	printf("Error: Memory allocation failed.\n");
    	exit(99);
    }
	

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        case 'r': {
 	// Reverse string functionality
    	for (int i = 0; i < user_str_len / 2; i++) {
        	char temp = *(buff + i);
        	*(buff + i) = *(buff + user_str_len - 1 - i);
        	*(buff + user_str_len - 1 - i) = temp;
    	}
    	printf("Reversed String: ");
    	for (int i = 0; i < user_str_len; i++) {
        	putchar(*(buff + i));
    	}
    	putchar('\n');
    	break;
}
	case 'w': {
    	// Print words and their lengths
    	printf("Word Print\n----------\n");
    	int word_count = 1;
    	int char_count = 0;

    	for (int i = 0; i < user_str_len; i++) {
        	if (*(buff + i) == ' ' || i == user_str_len - 1) {
            		if (i == user_str_len - 1 && *(buff + i) != ' ') {
                		char_count++;
            }
            printf("%d. ", word_count++);
            for (int j = i - char_count; j < i; j++) {
                putchar(*(buff + j));
            }
            printf(" (%d)\n", char_count);
            char_count = 0;
        } else {
            char_count++;
        }
    }
    break;
}
	
	
	
	
	
	
	//       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    free(buff);
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
