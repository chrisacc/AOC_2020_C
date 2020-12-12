#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int CountLinesInFile(const char* filename) //note: remove static, place in helper library
{
    FILE *fp;
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
    // Open the file
    fp = fopen(filename, "r");
  
    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
  
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
  
    // Close the file
    fclose(fp);
    printf("The file %s has %d lines\n", filename, count);
    return count;
}

int BootCode(char* opname, int oparg, int accumulator, int *step){
    if (strcmp(opname,"nop")==0){
        *step = 1;
    }else if(strcmp(opname,"acc")==0){
        *step = 1;
        accumulator += oparg;
    }else if(strcmp(opname,"jmp")==0){
        *step = oparg;
    }
    return accumulator;
}
 
int BootCodeDetectInfLoop(char** opnames, int* opargs, int* logOpindex, int* accumulator, int *opIndex, int*step, int *Nops_executed, int Nops){
    int ExitCode=1;
    bool InfLoop=false; //returns 1) inf loop detected, or 0) if access to opnames is out of bounds, 2) if neither condition was satisfied
    
    while(InfLoop==false){ //do stuff exit if - inf loop detected
        char* opname=opnames[*opIndex];
        int oparg=opargs[*opIndex];
        if( (oparg==0) & (strcmp(opname, "jmp")==0) ){
            printf("jmp +0, detected \n");
            ExitCode = 1;
            break;
        }
        //search the log file to check that the operation hasn't
        for (int i=0; i<*Nops_executed; i++){
            //if ((strcmp(logOpname[i], opname)==0) & (logOparg[i]==oparg) ){
            if (logOpindex[i]==*opIndex){
                InfLoop = true;
                ExitCode = 1;
                break;
            }
        }
        
        if(InfLoop==false){
            logOpindex[*Nops_executed]=*opIndex;
            *accumulator = BootCode(opname, oparg, *accumulator, step); //call boot func
            
            *opIndex += *step;
            (*Nops_executed)++;
            
            if ((Nops+1 < *Nops_executed) | (*opIndex > Nops -1)){//reallocate space for logging (shouldn't be needed)
                printf("need to reallocate memory \n");
                ExitCode = 0;
                break;
            }
            
        }
    }
    return ExitCode;
}

void SwapOp(char **Opnames, int ind){
    if (strcmp(Opnames[ind], "jmp")==0){
        Opnames[ind] = "nop";
    }else if (strcmp(Opnames[ind], "nop")==0){
        Opnames[ind] = "jmp";
    }
}

int BootCodePart2(char ** opnames, int* opargs, int* logOpindex, int* accumulator, int Nops){
    int ExitCode = 1;
    int Nswaps = 0;
    int MaxNswaps = Nops;
    int oldIndSwap = -1;
    
    while((ExitCode == 1) & (Nswaps<MaxNswaps)){
        // run the inf detection loop
        *accumulator=0;
        int Nops_executed = 0;
        int step = 0;
        int opIndex = 0;
        ExitCode = BootCodeDetectInfLoop(opnames, opargs, logOpindex, accumulator, &opIndex, &step, &Nops_executed, Nops);
        
        if(ExitCode==1){//Perform a swap //swap back old
            if (oldIndSwap >= 0) SwapOp(opnames, oldIndSwap);
                        
            for(int i=0; i<Nops; i++){
                if( ((strcmp(opnames[i], "jmp")==0) | (strcmp(opnames[i], "nop")==0)) & (i>oldIndSwap) ){
                    oldIndSwap = i; //assign new swap index
                    break;
                }
            }
            SwapOp(opnames, oldIndSwap); // swap new
            Nswaps++;
        }
    }
    return *accumulator;
}

void func_day8(const char *filename){
    int Nops = CountLinesInFile(filename);
    printf("nubmer of ops in %d \n", Nops);
    
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }

    int oparg;
    char* opname = (char*)malloc(4*sizeof(char));
    int loglen = Nops;
    char** logOpname = (char **)malloc(loglen*sizeof(char*));
    int* logOparg = (int*)malloc(loglen*sizeof(int));
    int* logOpindex = (int *)malloc(loglen*sizeof(int));
    char** opnames = (char**)malloc(Nops*sizeof(char*));
    int *opargs = (int*)malloc(Nops*sizeof(int));
    for (int i=0; i<loglen; i++) {
        logOpname[i] = (char *)malloc(4*sizeof(char));
        opnames[i] = (char *)malloc(4*sizeof(char));
    }
   
    int opnum=0;
    while(fscanf(myFile,"%s %d\n", opname, &oparg) != EOF){
        strcpy(opnames[opnum], opname);
        opargs[opnum]=oparg;
        //printf("opname, oparg: %s, %d \n", opnames[opnum], opargs[opnum]);
        opnum++;
    }
    printf("end file import \n");
    fclose(myFile);
    
    
    bool RepeatedOp = false;
    int accumulator=0;
    int Nops_executed=0;
    int step=0;
    int opIndex=0;
    while(!RepeatedOp){
        //if (fscanf(myFile,"%s %d", opname, &oparg) == EOF) break;
        //printf("opname, oparg: %s, %d \n", opname, oparg);
        //***fseek - can't use fseek easily because each line has a diff number of bytes? .. maybe it doesn't actually and it's just the location of the null term
        
        opname=opnames[opIndex];
        oparg=opargs[opIndex];
        //search the log file to check that the operation hasn't
        for (int i=0; i<Nops_executed; i++){
            //if ((strcmp(logOpname[i], opname)==0) & (logOparg[i]==oparg) ){
            if (logOpindex[i]==opIndex){
                RepeatedOp = true;
                break;
            }
        }
        
        if (RepeatedOp==false){
            // this can be deleted, misunderstood question
            strcpy(logOpname[Nops_executed],opnames[opIndex]);
            logOparg[Nops_executed]=opargs[opIndex];
            // this can be deleted
            logOpindex[Nops_executed]=opIndex;
            accumulator = BootCode(opname, oparg, accumulator, &step); //call boot func
                       
            if (loglen+1 < Nops_executed){//reallocate space for logging (shouldn't be needed)
                printf("need to reallocate memory \n");
                break;
            }
            opIndex += step;
            Nops_executed++;
        }
    }
    printf("part 1: accumulator value %d \n", accumulator);
    
    //V2 of part1:
    printf("Better way to do part 1, for part 2: \n");
    accumulator = 0;
    Nops_executed = 0;
    step = 0;
    opIndex = 0;
    BootCodeDetectInfLoop(opnames, opargs, logOpindex, &accumulator, &opIndex, &step, &Nops_executed, Nops);
    
    printf("part 1: accumulator value %d \n", accumulator);
    
    //Part 2:
    BootCodePart2(opnames, opargs, logOpindex, &accumulator, Nops);
    printf("part 2: accumulator value %d \n", accumulator);
    
    printf("I have a memory leak problem here.. i've played fast and loose with my pointers somewhere");
    for (int i=0; i<Nops; i++){
        free(opnames[i]);
        free(logOpname[i]);
    }
    free(logOpname);
    free(opnames);
    
    free(opname);
    free(logOparg);
    free(logOpindex);
    free(opargs);
}
