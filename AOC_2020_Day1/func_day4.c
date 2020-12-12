#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "AOCFuncs.h"

void func_day4(const char* filename){ // Passport validity
    //read in the full array of chars (alternative is to only read in 2 lines at a time)
    
    //read in as many lines until empty line
    //parse for validity, continue loop
    
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    int nreads=0;
    int n = 100; // careful, this number has to be greater than num of chars in first line
    
    char *arr;
    size_t len = sizeof(char)*n;
    arr = malloc(len);
    if (arr == NULL){printf("Memory allocation error\n"); exit(-1); }
    
    char *fullstr;
    fullstr = malloc(len*2);
    
    //while (fscanf(myFile,"%s\n", &arr[0]) != EOF){
    //    printf("%s \n",arr);
    //}
    
    ///*
    
    int Nfields = 7; //8
    char fields[] = "byr:iyr:eyr:hgt:hcl:ecl:pid:"; //"byr:iyr:eyr:hgt:hcl:ecl:pid:cid:";
    int fieldlen = 4;
    char field[]="byr:"; //how would i just point to a 4 element memory address rather than copying
    
    int Ninvalid=0;
    int Nvalid=0;
    int offset=0;
    int nppt=0;
    while (fgets(arr, len , myFile) != NULL){ //using EOF doesn't increment
        //printf("%s",arr);
        //printf("%d",strlen(arr));
        
        //strncat(fullstr, arr, strlen(arr));
        //printf("concatenated");
        //strcpy(fullstr+offset, arr);
        
        //if (arr[strlen(arr)-1] != '\n'){
        //    printf("problem");
        //}
        
        if (strlen(arr)==1 & arr[strlen(arr)-1]=='\n'){
            nppt++;
            //printf("parsing input passport \n");
            for (int iField=0; iField<Nfields; iField++){
                for (int dum=0; dum<fieldlen; dum++) field[dum]=fields[iField*fieldlen + dum];
                if (match(fullstr,field) == -1){
                    //printf("missing field");
                    Ninvalid++;
                    break;
                }
            }
            offset=0; // reset position to copy to
        }else{//
            strcpy(fullstr+offset, arr);
            offset=offset+strlen(arr);
        }
    }
    Nvalid = nppt - Ninvalid;
    //*/
    
    /*
    //////////// using getline - specific to POSIX systems
    char *line = NULL;
    size_t linelen = 0;
    
    while (getline(&line, &linelen, myFile) != -1){
        printf("%s", line);
        if (line[strlen(line)-1]=='\n'){
            printf("newline");
        }
    }
    */
     
    printf("Part 1 - Nvalid is: %d \n", Nvalid);
    fclose(myFile);
}
