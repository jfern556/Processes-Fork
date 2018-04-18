// process.c - file name
/****************************************************************************/
/*PROGRAM DOCUMENTATION

 AUTHOR: Jessica Fernandez-Rubio

 PID   : 5795311

 Programming III. Sixth Assignment

 PROGRAM DESCRIPTION: This program will accept a number if integer numbers. It
 will then request a shared memory for the integers entered, attach the memory
 to its own address space and fill this space with the numbers. After this it 
 will create child processes according to the number of inputs,each child 
 process with an unique id. Each number of the shared memory is stored in an 
 array, and corresponds to one of the childs depending on the element number 
 and child Id. For example, element 1 on the array with index 0 corresponds to
 child 1. Each of one of these elements will be multiplied for the correspon-
 ding child ID and stored back in the array. Parent has to wait for each child 
 process to finish and then display the final shared memory, detach it and 
 remove it.  

 INPUT: The program accepts inputs through the command line. Total of inputs
 accepted are between 1 and 7 both inclusive, each input a uniqueinteger
 number between 0 and 9. 

 OUTPUT: The output is a trace statement showing which statements are currently
 executing and who is executing them, whether the parent or the child.  

 Compilation: make
              or
              gcc process.c
 Execute: ./a.out "number" "number"         //Maximum of 7 numbers can be used
          or
          ./process.out "number" "number"   
       
          Note: Where it says number on the execution line, goes the input. 
                Just enter the number(s) without the quotations.

 I hereby certify that this collective work is my own except the share memory, 
 attach, detach and remove functions which were taken from the moodle site,  
 and none of it, is the work of any other person or entity.

 */
/******************************************************************************/
#include  "process.h"

int main (int argc, char  *argv[]){ 
    int shared_m_ID;
    int *shared_m_ptr;
    int count = 0;
    int child_process_id[(argc-1)];
    int processes = 0;
    int status;
    int child_id;
    int child_id_p;
    pid_t pid;
    pid_t wait_pid;
     
    if (valid_argc(argc) == VALID && valid_argv(argc, argv) == VALID){
        printf("Parent: About to request a shared memory for %d integers \n\n", (argc-1));
        shared_m_ID = request_shared_m (argc);
        printf("Parent: Has received a shared memory of %d integers\n\n", (argc-1));
        printf("Parent: Shared memory is about to be attached\n\n");
        shared_m_ptr = (int*)shmat(shared_m_ID, NULL, 0);
        if ((long)shared_m_ptr == -1){
            printf("***shmat ERROR***\n\n");
            exit (INVALID);
        }    
        printf("Parent: Shared memory has been attached\n\n");
        printf("Parent: Shared memory is getting filled with the following integers:  ");
        for ( count = VALID; count < (argc -1) ; count++){
            shared_m_ptr[count] = atoi ( argv[count+1]);
            printf("%d  ", shared_m_ptr[count]);
        }
        printf("\n\nParent: The shared memory has been completely filled\n\n");
        printf("Parent: Initial state of shared memory: ");
        display_shared_m (shared_m_ptr, argc);
        for ( processes = VALID; processes < (argc - 1); processes ++){
            printf("Parent: About to fork a child process\n\n");
            pid = fork();
            child_process_id[processes] = pid;
            if (pid == VALID){
                printf("Child %d: Initial state of shared memory (%d): ", 
                                      (processes + 1), getpid());
                display_shared_m (shared_m_ptr, argc);
                child_id = child_process (shared_m_ptr, processes);
                printf("Child %d: Current state of shared memory (%d): ", child_id,(int)getpid());
                display_shared_m (shared_m_ptr, argc);           
                exit (VALID);
            }
            else if (pid < VALID){
                printf ("Error when forking child process");
            }
            else {
                
            }
        }
        for (count = 0; count < (argc - 1); count ++){
            wait_pid = (int)wait(&status);
            child_id_p = child_id_request (child_process_id, wait_pid, (argc - 1));
            printf("Parent: Detects process %d (child %d) is done\n\n", wait_pid, child_id_p);
            printf("Exit Status: %d\n\n", status);
        }
        printf("Parent: Detected the completion of its children\n\n");
        printf("Parent: Final state of shared memory: ");
        display_shared_m (shared_m_ptr, argc);
        detach_shared_m (shared_m_ptr);      
        shmctl(shared_m_ID, IPC_RMID, NULL);
        printf("Parent: Has removed the shared memory\n\n");         
    }
    else {
        printf("ERROR, invalid arguments!!!\n\n");
        }
    return VALID;
}



/********************************************************************************/
/**Function: valid_argc
 This function checks if the total inputs through the command line is ok. It
 checks if argc is between 2 and 8 which is the number of expected arguments,
 one for program name and the rest for input(s).

 Returns: The return is 0 (VALID) if valid or 1(INVALID) if not valid.
 */
/********************************************************************************/
int valid_argc(int argc){
    if (argc >= MIN_VALID && argc <= MAX_VALID){
        return VALID;
    }
    else return INVALID;
}



/********************************************************************************/
/**Function: valid_argv
 This function checks each value entered, once the argc has been  validated. 
 It makes sure each value is an integer between zero and nine as well as unique
 values, which means different values. 

 Returns: The return is 0 (VALID) if valid or 1(INVALID) if not valid.
 */
/********************************************************************************/
int valid_argv (int argc, char *array []){
    int count;
    int valid = 0;
    int count_if_diff = 0;
    int index = 0;
    for (count = 1; count < argc; count ++){
        if (*(array[count]+1) == '\0'){
            if (*array[count] >= MIN_NUMBER  && *array[count]<= MAX_NUMBER){
                valid ++;
            }
            for (index = (count + 1); index < argc; index++){
                if (*array[count] == *array[index])
                    return INVALID;
            }
        }       
    }
    if (valid == (argc - 1)) 
         return VALID;
}



/********************************************************************************/
/**Function: request_shared_m
 This function request a shared memory according to the number of integers entered
 in the command line. It checks the output of shmget, if its valid or if it fails.

 Returns: The return is an integer, the shared memory id.
 */
/********************************************************************************/
int request_shared_m (int args){
    int shared_m_ID;
    int numbers = args - 1;
    shared_m_ID = shmget(IPC_PRIVATE, numbers * sizeof(int), IPC_CREAT|0666);
    if (shared_m_ID < 0){
        printf ("***shmget ERROR***\n\n");
        exit (INVALID);
    } 
    else {
        return shared_m_ID;
    }
}



/********************************************************************************/
/**Function: display_shared_m
 This function displays the elements stored in shared memory in a type of array.

 Returns: N/A
 */
/********************************************************************************/
void display_shared_m (int *pointer, int args){
    int count = 0;
    int inputs = args - 1;
    for (count = 0; count < inputs; count ++){
        printf("| %d  ", pointer [ count ]);
    }
    printf("| \n\n");
}



/********************************************************************************/
/**Function: child_process
 This function assigns a unique id to the child. It gets the element stored in 
 the array that corresponds to the child Id and multiply both values, then the 
 resulting value gets stored in the same place in the array where the element was
 before the multiplication. 

 Returns: An integer number which is the child id. 
 */
/********************************************************************************/
int child_process (int *pointer, int index){
    int child_id = index + 1;
    pointer [index] = pointer [index] * child_id;
    return child_id;
}



/********************************************************************************/
/**Function: detach_shared_m
 This function detaches the shared memory from a process. It checks if the 
 operation was succesful

 Returns: N/A
 */
/********************************************************************************/
void detach_shared_m(int *pointer){
    if(shmdt(pointer)== VALID){
        printf("Parent: Has detached its shared memory\n\n");
    }
    else {
        printf("Parent: Error detaching shared memory\n\n");
    }
}



/********************************************************************************/
/**Function: child_id_request
 This function receives as parameters an array of integers which contains process
 numbers, receives a specific process number and the length of the array. Then it 
 checks if the process number entered as an argument is found in the array, if so, 
 returns the position+1 which will be the child id. 

 Returns: child unique id which is an integer
 */
/********************************************************************************/
int child_id_request(int* array, int process, int length){
    int count = 0;
    for (count = 0; count < length; count++){
        if (array [count] == process){
            return (count + 1);
        }
        else {}
    }
}
