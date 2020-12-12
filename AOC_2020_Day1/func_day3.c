#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/////////////////////////////////
void func_day3(const char* filename){
    //read in the full array of chars (alternative is to only read in 2 lines at a time)
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    int nreads=0;
    int n = 100; // careful, this number has to be greater than num of chars in first line
    
    char *arr;
    arr = malloc(sizeof(char)*n);
    if (arr == NULL){printf("Memory allocation error\n"); exit(-1); }
    
    int Ni,Nj;
    while (fscanf(myFile,"%s\n", &arr[nreads]) != EOF){
        if (nreads==0) Nj = strlen(arr);
        nreads = strlen(arr);
        printf("%d \n", nreads);
        
        if (nreads + Nj >= n-1){
            n += 100;
            arr = realloc(arr, sizeof(char)*n);
        }
        
    }
    fclose(myFile);
    Ni=nreads/Nj;
    
    for (int i=0; i<22; i++){printf("%c", arr[i]);}
    printf("matrix is of size %d x %d \n",Ni,Nj);
    
    // launch your toboggan
    int Ntreeshit=0;
    int i=0;
    int j=0;
    while (i<Ni){
        // each iter, j=j+3; i=i+1;
        j += 3;
        i += 1;
        // end loop when i>=Ni-1
        if (arr[i*Nj + j%Nj]=='#') Ntreeshit++;
    }
    printf("Part 1: number of treeshit: %d \n",Ntreeshit);
    
    //Part 2 - launch toboggan
    int rights[5] = {1,3,5,7,1};
    int lefts[5] = {1,1,1,1,2};
    long solmult=1;
    for (int iLaunch=0; iLaunch<5; iLaunch++){
        i=0; j=0; Ntreeshit=0;
        while (i<Ni){
            j += rights[iLaunch];
            i += lefts[iLaunch];
            if (arr[i*Nj + j%Nj]=='#') Ntreeshit++;
        }
        printf("ntreeshit: %d\n", Ntreeshit);
        solmult = solmult * Ntreeshit;
    }
    printf("Part 2: solutions multiplied together: %li \n", solmult);
    
    free(arr);
}


