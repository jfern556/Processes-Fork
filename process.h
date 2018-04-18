
/******************************************************************************/
/*PROGRAM DEFINES*/

# include  <stdio.h>
# include  <stdlib.h>                    //Facilitates the use of atoi
# include  <sys/shm.h>                   //Shared memory facility
# include  <sys/types.h>                 //To use pid_t type
# include  <sys/ipc.h>                   //To use IPC_CREAT
# include  <unistd.h>			//to use fork

#define VALID 0                         //Value if valid
#define INVALID 1                       //Value if invalid
#define MIN_NUMBER 48                   //Ascii code of zero
#define MAX_NUMBER 57                   //Ascii code of nine 
#define MIN_VALID 2                     //Minimum of valid arguments
#define MAX_VALID 8                     //Maximum of valid arguments                     

/******************************************************************************/
/*FUNCTIONS PROTOTYPES*/
int valid_argc(int);
int valid_argv (int, char * []);
int request_shared_m (int);
void display_shared_m (int* , int);
int child_process (int*, int);
void detach_shared_m (int*);
int child_id_request (int* , int, int);
