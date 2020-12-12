#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int parseBSP(int n, char* seq){
    //n is how many rows there are
    int lower = 0;
    int upper = n-1;
    //seq will be of length log2(n)
    int i=0;
    while (upper != lower){
        if ((seq[i]=='F') | (seq[i]=='L')){
            upper = (upper+lower)/2;
        }else{
            lower = (upper+lower)/2 + 1;
        }
        i++; //printf("lower, upper, %d %d \n", lower, upper);
    }
    return lower;
}

int comp (const void *elem1, const void *elem2){
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f>s) return 1;
    if (f<s) return -1;
    return 0;
}

void func_day5(const char* filename){ //Identifying seatID on plane
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    int Ni=128;
    int Nj=8;
    int seatID_max;
    int seatID_min;
    int *seatID_arr;
    seatID_arr = (int*)calloc(Ni*Nj, sizeof(int));
    
    /*
    int row = parseBSP(Ni, "FBFBBFF");
    int col = parseBSP(Nj, "RLR"); //printf("row, col %d %d \n", row, col);
    int seatID = row*Nj + col; printf("set id %d \n", seatID);
    */
    
    char seq[11]; //11th char is for null term
    char *subseq1; //char subseq1[7];
    char *subseq2; //char subseq2[3];
    
    int Nseats = 0;
    while (fscanf(myFile,"%s\n", seq) != EOF){
        Nseats++;
        //printf("%s \n", seq);
        subseq1=&seq[0]; //for (int i=0; i<7; i++) {subseq1[i]=seq[i];}
        subseq2=&seq[7]; //for (int i=0; i<3; i++) {subseq2[i]=seq[i+7];}
        int row = parseBSP(Ni, subseq1);
        int col = parseBSP(Nj, subseq2);
        int seatID = row*Nj + col;
        seatID_arr[Nseats-1]=seatID;
        
        if (Nseats==1) {seatID_max=seatID; seatID_min=seatID;}
        if (seatID>seatID_max) seatID_max=seatID;
        if (seatID<seatID_min) seatID_min=seatID;
    }
    printf("total number of seats %d \n", Nseats);
    printf("min seat id is %d \n", seatID_min);
    
    //Part 2
    qsort(seatID_arr, Ni*Nj, sizeof(int), comp);
    int mySeatID;
    for (int i=1; i<Ni*Nj; i++){
        if ((seatID_arr[i]-seatID_arr[i-1])>1) mySeatID=seatID_arr[i] - 1;
    }
    
    printf("Part 1, max seat id is %d \n", seatID_max);
    printf("Part 2, my seat id is %d \n", mySeatID);
    fclose(myFile);
    free(seatID_arr);
}
////////////////////////////////////////////////////////////////////////////////////
