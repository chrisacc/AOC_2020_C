//
//  main.c
//  AOC_2020_Day1
//
//  Created by Chris Acconcia on 2020-12-01.
//

#include <stdio.h>
#include <stdlib.h>
#include "AOCFuncs.h"

//could be a utilities grouping of functions
int readCommaSeparatedFile(const char*, int**);

int main() {
    
    int DayNum = 4;
    
    if (DayNum==1) func_day1("input_day1.txt");
    if (DayNum==2) func_day2("input_day2.txt");
    //if (DayNum==3) func_day3("test_day3.txt");
    if (DayNum==3) func_day3("input_day3.txt");
    //if (DayNum==4) func_day4("test_day4.txt");
    if (DayNum==4) func_day4("input_day4.txt");
    if (DayNum==5) func_day5("input_day5.txt");
    //if (DayNum==6) func_day6("test_day6.txt");
    if (DayNum==6) func_day6("input_day6.txt");
    //if (DayNum==7) func_day7("test_day7.txt");
    if (DayNum==7) func_day7("input_day7.txt");
    //if (DayNum==8) func_day8("test_day8.txt");
    if (DayNum==8) func_day8("input_day8.txt");
    //if (DayNum==9) func_day9("test_day9.txt");
    if (DayNum==9) func_day9("input_day9.txt");
    //if (DayNum==10) func_day10("input_day10.txt");
    if (DayNum==11) func_day11("input_day11.txt");
    //if (DayNum==11) func_day11("test_day11.txt");
    //if (DayNum==12) func_day12("input_day12.txt");
    
    /*
    int *arr;
    int n;
    n = testAlloc(&arr);
    for (int i=0; i<n; i++){
        printf("%d \n", arr[i]);
    }
    free(arr);
    */
    
    /*
    int* array_in;
    printf("address %p \n", array_in);
    int nreads = readCommaSeparatedFile("test.txt", &array_in);
    for (int i=0; i<nreads; i++){
        printf("%d\n", array_in[i]);
    }
    free(array_in);
    */
    
    return 0;
}

int readCommaSeparatedFile(const char* filename, int** array_in){
    //const char* filename = "input_day1.txt";
    //int *array_in;
    
    printf("address in func %p \n", array_in);
    
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        //printf("Error Reading File \n");
        //return 1;
        fprintf(stderr,"Error Reading File \n");
        exit (1);
    }
    
    int nreads=0;
    int n = 20;
    
    *array_in = malloc(sizeof(int)*n); //malloc has changed the value of the pointer
    
    printf("address in func after malloc %p \n", array_in);
    
    if (*array_in == NULL){printf("Memory allocation error\n"); exit(1); }
    
    while (fscanf(myFile,"%d,\n", &(*array_in)[nreads]) != EOF){
        if (nreads >= n-1){
            n += 10;
            *array_in = realloc(*array_in, sizeof(int)*n);
        }
        nreads++;
    }
    //for (int i=0; i<nreads; i++){printf("%d\n", array_in[i]);}
    fclose(myFile);
    
    printf("address in func after realloc %p \n", array_in);
    
    return nreads;
}




