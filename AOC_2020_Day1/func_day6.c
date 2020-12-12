#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // required by day 2
#include <string.h> //required by day 2

int compchar (const void *elem1, const void *elem2){
     int f = *((char*)elem1);
     int s = *((char*)elem2);
     if (f>s) return 1;
     if (f<s) return -1;
     return 0;
 }

int CountUnique(char* arr, int n){
    int uniqueCnts=n;
    qsort(arr, n, sizeof(char), compchar); //sort
    //check num of unique items in sorted array
    for (int i=1; i<n; i++){
        if (arr[i]==arr[i-1]){
            uniqueCnts--;
        }
    }
    return uniqueCnts;
}

void GetOverlappingChar(char *arr_overlap, char *arr){
    int ioverlap=0;
    bool valfound;
    while(arr_overlap[ioverlap]!='\0'){
        int i=0;
        valfound=false;
        while (arr[i] !='\0'){
            if (arr_overlap[ioverlap]==arr[i]){
                valfound=true;
                break;
            }
            i++;
        }
        if (!valfound) arr_overlap[ioverlap]='0';
        ioverlap++;
    }
}

void func_day6(const char* filename){
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    int n = 100; // careful, this number has to be greater than num of chars in first line
    
    char *arr;
    size_t len = sizeof(char)*n;
    arr = malloc(len);
    if (arr == NULL){printf("Memory allocation error\n"); exit(-1); }
    
    //char *arr_overlap = (char*)calloc(26, sizeof(char));
    char arr_overlap[] = "abcdefghijklmnopqrstuvwxyz";
    
    char *fullstr;
    fullstr = malloc(len*2);
    
    int AllGroupCounts=0;
    int GroupCounts=0;
    
    int offset=0;
    int NGroups=0;
    int SumNumOverlap=0;
    while (fgets(arr, len , myFile) != NULL ){ //using EOF doesn't increment
        if (strlen(arr)==1 & arr[strlen(arr)-1]=='\n'){
            NGroups++;
            //printf("parsing Group \n");
            //printf("%s \n", fullstr);
            GroupCounts = CountUnique(fullstr, strlen(fullstr));
            AllGroupCounts += GroupCounts;
            offset=0; // reset position to copy to
            fullstr[0] = '\0';
            
            int NumOverlap=0;
            for (int i=0; i<strlen(arr_overlap); i++){//part 2, count number of overlapping answers
                if (arr_overlap[i] != '0') NumOverlap++;
            }
            SumNumOverlap+=NumOverlap;
            strcpy(arr_overlap, "abcdefghijklmnopqrstuvwxyz"); //reset overlap accrual array
        }else{//
            strncpy(fullstr+offset, arr, strlen(arr)-1); // copy and remove \n
            fullstr[offset+strlen(arr)-1]='\0'; //add in null term char
            offset=offset+strlen(arr)-1;
            GetOverlappingChar(arr_overlap, arr); //part 2, accrue the overlap of answers
            //printf("overlap %s \n", arr_overlap);
        }
        
    }
    //this is bad, but process last group that was missed due to last new line
    NGroups++;
    GroupCounts = CountUnique(fullstr, strlen(fullstr));
    AllGroupCounts += GroupCounts;
     
    int NumOverlap=0;
    for (int i=0; i<strlen(arr_overlap); i++){//part 2, count number of overlapping answers
        if (arr_overlap[i] != '0') NumOverlap++;
    }
    SumNumOverlap+=NumOverlap;
    //this is bad, but process last group that was missed due to last new line
    
    printf("Part 1 - sum of counts : %d \n", AllGroupCounts);
    printf("Part 2 - sum of everyone answered counts: %d \n", SumNumOverlap);
    fclose(myFile);
    free(arr);
    free(fullstr);
}
