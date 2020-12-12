#include <stdio.h>
#include <stdlib.h>

void func_day1(const char* filename){
    FILE *myFile;
    //const char* filename = "input_day1.txt";
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        //printf("Error Reading File \n");
        //return 1;
        fprintf(stderr,"Error Reading File \n");
        exit (1);
    }
    
    int nreads=0;
    int n = 20;
    int *array_in;
    array_in = (int*)malloc(sizeof(int)*n);
    
    if (array_in == NULL){printf("Memory allocation error\n"); exit(1); }
    
    while (fscanf(myFile,"%d\n", &array_in[nreads]) != EOF){
        if (nreads >= n-1){
            n += 10;
            array_in = (int *)realloc(array_in, sizeof(int)*n);
        }
        nreads++;
    }
    //for (int i=0; i<nreads; i++){printf("%d\n", array_in[i]);}
    fclose(myFile);
    
    
    //find the numbers that sum to 2020
    for (int i=0; i<nreads; i++){
        for (int j=i+1; j<nreads; j++){
            if (array_in[i]+array_in[j]==2020){
                printf("Part 1: answer is %d \n", array_in[i]*array_in[j]);
            }
        }
    }
    
    //find 3 number that sum to 2020
    int sum=0;
    for (int i=0; i<nreads; i++){
        for (int j=i+1; j<nreads; j++){
            sum = array_in[i]+array_in[j];
            if (sum>2020){continue;}
            for (int k=j+1; k<nreads; k++){
                if (array_in[i]+array_in[j]+array_in[k]==2020){
                    printf("Part 2: answer is %d \n", array_in[i]*array_in[j]*array_in[k]);
                }
            }
        }
    }
    
    
    free(array_in);
}









