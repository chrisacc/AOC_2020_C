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

void func_day9(const char *filename){
    int Nlines = CountLinesInFile(filename);
    printf("nubmer of ops in %d \n", Nlines);
    
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    int iLine=0;
    int dum;
    int* arr = (int*)malloc(Nlines*sizeof(int));
    while(fscanf(myFile,"%d\n", &dum) != EOF){
        arr[iLine]=dum;
        //printf("opname, oparg: %s, %d \n", opnames[opnum], opargs[opnum]);
        iLine++;
    }
    printf("end file import \n");
    fclose(myFile);
    
    int BufLen=25;
    int num1; //solution to part 1;
    //queryBuf(arr, BugLen);
    for (int offset=BufLen; offset<Nlines-BufLen; offset++){
        bool Valid = false;
        for (int i=offset-BufLen; i<offset; i++){
            for (int j=offset-BufLen; j<offset; j++){
                if (arr[i]==arr[j]){
                    continue;
                }else{
                    if (arr[i]+arr[j]==arr[offset]){
                        Valid = true;
                    }
                }
            }
        }
        if (Valid==false){
            num1 = arr[offset];
            printf("Part 1 answer: the invalid number found in sequence, %d, \n", num1);
        }
    }
    
    //// Part 2
    //for every offset position start building a running sum of numbers,
    //stop with the running sum when it exceeds
    int num2;
    int sum;
    int startind, stopind;
    bool exitcond=false;
    for (int offset=0; offset<Nlines-1; offset++){
        int sum=0;
        for (int i=offset; i<Nlines; i++){
            sum += arr[i];
            exitcond = (sum == num1);
            if (exitcond) {
                printf("sumval is %d \n", sum);
                startind=offset;
                stopind=i;
                num2 = arr[i] + arr[offset];
                //find min and max val between arr[offset:i]                
                break;
            }
            if (sum>num1) break;
        }
        if (exitcond==true) break;
    }
    
    int minval;
    int maxval;
    for (int i=startind; i<stopind+1; i++){
        if (i==startind){
            minval=arr[i];
            maxval=arr[i];
        }else{
            if (arr[i]<minval) minval=arr[i];
            if (arr[i]>maxval) maxval=arr[i];
        }
    }
    num2 = minval+maxval;
    printf("Part 2 answer: %d, \n ", num2);
}
