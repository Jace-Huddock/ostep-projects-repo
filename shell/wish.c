#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>



//Error message provided by project description       
void print_error_message(){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}


void exeCmd(char * line, char ** pathv, int * pathc, size_t * pathcLim, char ** prevRtPipe){

    char * start;
    char * end;
    
    int isPipe=0;
    char * pipeErr;

    //Handling redirection
    char * rtPipe = line; //Separates input upon ">" character. Left side for command and right side for files.  
    strsep(&rtPipe, ">");
    if(rtPipe!=NULL){
        
        //check for multiple >
        pipeErr=rtPipe;
        strsep(&pipeErr, ">");
        if(pipeErr != NULL){
            print_error_message();
            return;
        }
        //Case handling multiple files or 0 files
        start=rtPipe;
        end=rtPipe;
        int filec=0;

        while(start != NULL){
            strsep(&end," \n\t");
            if((*start != '\0')&&(*start != ' ')){
                rtPipe = start; //first arg 
                filec += 1;
            }    
            start=end;
        }
        if(filec!=1){
            print_error_message();
            return;
        }

        isPipe=1;
    }

    start = line;
    end = line;

    int myArgc = 0;
    int argvSz = 10;
    char ** myArgv = malloc(argvSz*sizeof(char *));

    //store each argument in my_argv array
    while(start != NULL){
        strsep(&end," \n\t");

        if((*start != ' ')&&(*start != '\0')){

            if(myArgc+1 >= argvSz){ //Reallocating memory to handle more input

                argvSz *= 2;
                char ** tmpArgv = realloc(myArgv, argvSz*sizeof(char *));
                if (tmpArgv != NULL) {
                    myArgv = tmpArgv;
                } else {
                    fprintf(stderr, "Failed to reallocate\n");
                    exit(1);
                }
                
            }
            myArgv[myArgc++] = start;
        }
        
        start=end;

    }
    myArgv[myArgc] = NULL;

    if(isPipe && myArgc ==0 ){
        print_error_message();
        return;
    }

    //if no arguments, return and read more lines
    if(myArgc==0){
        return;
    }
    //Handles exit command
    if (strcmp(myArgv[0], "exit") == 0){
        if(myArgc > 1){
            print_error_message();
            return;
        }
        exit(0);
    //Handles Change directory command
    }else if (strcmp(myArgv[0], "cd") == 0){

        if(myArgc != 2){
            print_error_message();
            return;
        }
        if (chdir(myArgv[1])!=0){
            print_error_message();
            return;
        }

    //Handles path command
    }else if (strcmp(myArgv[0], "path") == 0){
        
        //free memory for each char * pointer in pathv
        for(int i=0; i<*pathc; i++){
            free(pathv[i]);
        }

        *pathc=0;
        int reallocFlg = 0;
        //allocating memory in case of exceeding pathcLim
        while(myArgc-1 > *pathcLim){
            reallocFlg = 1;
            *pathcLim*=2;
        }

        if(reallocFlg != 0){
            char ** tmp_pathv = realloc(pathv,*pathcLim*sizeof(char*));
            if (tmp_pathv != NULL) {
                pathv = tmp_pathv;
            } else {
                fprintf(stderr, "realloc failed\n");
                exit(1);
            }
            
        }

        //store paths in path array
        for(int i=1; i<myArgc;i++){
            pathv[*pathc] = strdup(myArgv[i]); 
            *pathc+=1;
        }
    
    }else{
        //finding program in path array

        int progFd = 0;
        char progPath[100];

        for(int i=0; i<*pathc; i++){
            strcpy(progPath,pathv[i]);

            if(pathv[i][strlen(pathv[i])-1] != '/'){
                strcat(progPath, "/");
            }
            strcat(progPath, myArgv[0]);
            ;

            if(access(progPath, X_OK)==0){
                progFd = 1;
                break;
            }
        }

        if(!progFd){
            print_error_message();
            return;
        }

        if(isPipe){
            if(*prevRtPipe == NULL){
                    *prevRtPipe = strdup(rtPipe);

            }else{
                if(strcmp(rtPipe,*prevRtPipe)==0){
                    return;
                }
            }
        }
        
        
        int rc= fork();
        if(rc<0){
            fprintf(stderr, "fork failed\n");
            exit(1);

        }else if(rc==0){
            //child process runs external commands.    
            
            if(isPipe){
                
                int fd;
                if((fd= open(rtPipe, O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1){
                    printf("open:%s\n",strerror(errno));
                    printf("directory:%s\n",rtPipe);
                    exit(1);
                }
                int dup_err;
                if((dup_err=dup2(fd, STDOUT_FILENO))==-1){
                    printf("dup2:%s",strerror(errno));
                    exit(1);
                };
                if((dup_err=dup2(fd, STDERR_FILENO))==-1){
                    printf("dup2:%s",strerror(errno));
                    exit(1);
                };
                if(close(fd)==-1){
                    printf("close:%s",strerror(errno));
                    exit(1);
                }     
            }
            execv(progPath, myArgv);
            exit(0);
        }else{
            //int rc_wait = wait(NULL);
            printf("wish>");
        }
    }
    
    free(myArgv);

}

int main(int argc, char *argv[]) {
    
    if(argc > 2){
        print_error_message();
        exit(1);
    }
    int interactiveMode = argc==1 ? 1 : 0; //If argument count ==1, interactiveMode is set to 1, else set to 0.
    //Enter shell in interactive mode
    FILE *stream;
    if(interactiveMode){
        stream = stdin;
        printf("wish> ");
    //Enters shell in batch mode by opening provided file.     
    }else{
        stream = fopen(argv[1], "r");
        if(stream == NULL){
            print_error_message();
            exit(1);
        }
    }

    int pathCount = 1; //number of paths stored

    size_t pathcLim = 10; //Maximum paths allowed
    char ** pathv = malloc(pathcLim * sizeof(char*));

    //copy "/bin" into pathv
    pathv[0] = strdup("/bin"); 

    size_t n=0;
    char * line = NULL;
    
    char * lineRight = NULL;
    while((n=getline(&line,&n,stream))!= -1){ //Handles arbitrarily long input
        
        char * start=line;
        char * end=line;
        while(start != NULL){ //Executes all commands provided
            strsep(&end,"&");
            exeCmd(start,pathv,&pathCount,&pathcLim, &lineRight);
            start = end;
        }
    //Printing shell prompt after all commands have executed
        if(interactiveMode){
            printf("wish> ");
        }
        
    }
  
    free(lineRight);
    //free array containing pointers to paths
    free(pathv);
    //free line of commands
    free(line);
    //Exits program elegantly.
    exit(0);
}