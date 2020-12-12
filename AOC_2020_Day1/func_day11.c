#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//this is a perfect example where a struct or a class would have been very useful for organized, faster coding

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

void SurroundingSeatInds(int *indarr, int i, int j, int Ni, int Nj){
    //-1 ind out of bounds?
    //ind ul, u, ur, l, r, ll, l, lr;
    //fcn to return indices of adjacent seats
    
    int rowm = (i-1);
    int rowp = (i+1);
    int colm = (j-1);
    int colp = (j+1);
    
    if ((rowm<0) | (colm<0)) {  indarr[0]=-1;}
    else indarr[0] = rowm*Nj + colm;
    
    if (rowm<0){indarr[1]=-1;}
    else indarr[1] = rowm*Nj +j   ;
    
    if (rowm<0 | colp>Nj-1) {indarr[2]=-1;}
    else indarr[2] = rowm*Nj + colp; //indarr[0] + 2;
    
    if (colm<0){indarr[3]=-1;}
    else indarr[3] = i*Nj + colm;
    
    if (colp>Nj-1){indarr[4]=-1;}
    else indarr[4] = i*Nj + colp;  //indarr[3] + 2;
    
    if ((rowp>Ni-1) | (colm<0)){indarr[5]=-1;}
    else indarr[5] = rowp*Nj + colm;
    
    if (rowp>Ni-1){indarr[6]=-1;}
    else indarr[6] = rowp*Nj +j   ; //indarr[5] + 1;
    
    if ((rowp>Ni-1) | (colp>Nj-1)){indarr[7]=-1;}
    else indarr[7] = rowp*Nj + colp; //indarr[5] + 2;
    //out of bounds checking
}

int CountSeatsOccAdj(char *mat, int i, int j, int Ni, int Nj){
    int count=0;
    int indarr[8]; //only way this size would change is if the radius of comparison changed...
    int ind_adj;
    SurroundingSeatInds(indarr, i, j, Ni, Nj);
    for (int i=0; i<8; i++){
        ind_adj=indarr[i];
        if (ind_adj>=0){ //-1 indicates out of bounds
            if (mat[ind_adj]=='#'){
                count++;
            }
        }
    }
    //printf("i,j: %d, %d \n", i,j);
    return count;
}

void goLos(int direction, int *i, int *j, int Ni, int Nj){ //update the i,j for a step taken in the given look direction
    //-1 ind out of bounds?
    //ind ul, u, ur, l, r, ll, l, lr;
    //int rowinc, colinc;
    if (direction==0){
        (*i)--;
        (*j)--;
    }else if (direction == 1){
        (*i)--;
    }else if (direction == 2){
        (*i)--;
        (*j)++;
    }else if (direction == 3){
        (*j)--;
    }else if (direction == 4){
        (*j)++;
    }else if (direction == 5){
        (*i)++;
        (*j)--;
    }else if (direction == 6){
        (*i)++;
    }else if (direction == 7){
        (*i)++;
        (*j)++;
    }
    
    //flag for out of bounds
    if (*i<0 | *i>Ni-1){
        *i=-1;
    }
    if (*j<0 | *j>Nj-1){
        *j=-1;
    }
}

int CountSeatsOccLos(char *mat, int i, int j, int Ni, int Nj){ //Line of site version
    int count=0;
    int itemp, jtemp;
    
    for (int lookdirn=0; lookdirn<8; lookdirn++){
        bool cond=true;
        itemp=i;
        jtemp=j;
        while (cond){
            goLos(lookdirn, &itemp, &jtemp, Ni, Nj);
            if ((itemp>=0) & (jtemp>=0)) {//IF in bounds
                if (mat[itemp*Nj + jtemp]=='#'){
                    count++;
                    cond=false;
                }else if (mat[itemp*Nj + jtemp]=='L'){
                    cond=false;
                }
            }else{//ELSE, break
                cond=false;
            }
        }
    }
    return count;
}

char UpdateSeatRule(char *mat, int i, int j, int Ni, int Nj, int ProbPart){
    int SeatThresh;
    int seats_seen_occ=0;
    char NewSeatVal=mat[i*Nj+j];
    
    if (ProbPart==1) {
        SeatThresh=4;
        seats_seen_occ=CountSeatsOccAdj(mat, i, j, Ni, Nj);
    }else if (ProbPart==2) {
        SeatThresh=5;
        seats_seen_occ=CountSeatsOccLos(mat, i, j, Ni, Nj);
    }
    
    if ((mat[i*Nj + j]=='L') & (seats_seen_occ==0) ){ //if seat empty and no occupied seats adjacent
        NewSeatVal = '#';
    }else{
        if((mat[i*Nj + j]=='#') & (seats_seen_occ >= SeatThresh)  ){
            NewSeatVal = 'L';
        }
    }
    return NewSeatVal;
}

void CopyMatrix(char *dest, char*src, int Ni, int Nj){
    for (int i=0; i<Ni; i++){
        for (int j=0; j<Nj; j++){
            dest[i*Nj + j] = src[i*Nj + j];
        }
    }
}

void UpdateSeating(char *mat, int Ni, int Nj, int ProbPart){
    char *tempmat = (char*)malloc(Ni*Nj*sizeof(char)); // gotta be a better way than copying entire matrix
    CopyMatrix(tempmat, mat, Ni, Nj);
    for (int i=0; i<Ni; i++){
        for (int j=0; j<Nj; j++){
            //mat[i]=UpdateSeatRule(mat, i, j, Ni, Nj);
            if (tempmat[i*Nj+j] != '.'){
                mat[i*Nj + j]=UpdateSeatRule(tempmat, i, j, Ni, Nj, ProbPart);
            }
        }
    }
    free(tempmat);
}

int CountSeatsOcc(char *arr, int Ni, int Nj){
    int NSeatsOcc=0;
    for (int i=0; i<Ni*Nj; i++){
        if (arr[i]=='#') NSeatsOcc++;
    }
    return NSeatsOcc;
}

bool Comparator(char *mat1, char *mat2, int Ni, int Nj){
    bool SameMat = true; //assume they are the same
    for (int i=0; i<Ni*Nj; i++){
        if (mat1[i] != mat2[i]){
            SameMat = false;
            break;
        }
    }
    return SameMat;
}

void PrintSeatingChart(char *arr, int Ni, int Nj){
    printf("matrix, of size %d x %d is: \n",Ni,Nj);
    for (int i=0; i<Ni; i++){
        for (int j=0; j<Nj; j++){
            printf("%c", arr[i*Nj + j]);
        }
        printf("\n");
    }
}

int FindEquilibriumSeating(char *mat, int Ni, int Nj, int ProbPart){
    bool SameMat=false;
    char *tempmat = (char*)malloc(Ni*Nj*sizeof(char));
    while(SameMat==false){
        //PrintSeatingChart(mat, Ni, Nj); printf("\n");
        CopyMatrix(tempmat, mat, Ni, Nj); //store old state
        UpdateSeating(mat, Ni, Nj, ProbPart); //update seating
        SameMat = Comparator(tempmat, mat, Ni, Nj); //check if the seating has changed with the update
        //if (SameMat==true){
        //    break;
        //}
        
        //don't need to copy matrix to know if something has changed, could just track if a change was set
    }
    int Nseatocc = CountSeatsOcc(mat, Ni, Nj);

    free(tempmat);
    return Nseatocc;
}

// function to eval diff of mats
void func_day11(const char* filename){
    int NLines = CountLinesInFile(filename);
    
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
        //printf("%d \n", nreads);
        
        if (nreads + Nj >= n-1){
            n += 100;
            arr = realloc(arr, sizeof(char)*n);
        }
        
    }
    fclose(myFile);
    Ni=nreads/Nj;
    
    int ProbPartNum = 1;
    int Nseatocc;
    
    char *initMat1=malloc(Ni*Nj*sizeof(char));
    CopyMatrix(initMat1, arr, Ni, Nj);
    char *initMat2=malloc(Ni*Nj*sizeof(char));
    CopyMatrix(initMat2, arr, Ni, Nj);
    
    Nseatocc = FindEquilibriumSeating(initMat1, Ni, Nj, ProbPartNum);
    printf("Part 1 answer: Seats occupied = %d \n",Nseatocc);

    ProbPartNum = 2;
    Nseatocc = FindEquilibriumSeating(initMat2, Ni, Nj, ProbPartNum);
    printf("Part 2 answer: Seats occupied = %d \n",Nseatocc);
    
    free(arr);
    free(initMat1);
    free(initMat2);
}

