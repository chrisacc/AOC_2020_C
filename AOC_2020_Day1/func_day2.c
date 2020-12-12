#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void func_day2(const char* filename){
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (1);
    }
    //loop over each new line, read line in, count how many times the letter occurs
    int mincnt, maxcnt, charCnt, Npwd=0, Nvalid=0, Ninvalid=0;
    char ch;
    char *pwd = malloc(sizeof(char)*30); //i'm not sure if this is the best way to do this
    bool badpwd;
    
    ///Part 1
    while (fscanf(myFile,"%d-%d %c: %s\n", &mincnt, &maxcnt, &ch, pwd) != EOF){
        Npwd++;
        charCnt=0;
        badpwd=false;
        //printf("length %d", strlen(pwd));
        //for (int i=0; i<30; i++){printf("%c",pwd[i]);}
        
        for (int i=0; i<strlen(pwd); i++){
            if (ch == pwd[i]){
                charCnt++;
                if (charCnt>maxcnt){
                    badpwd = true;
                    break;
                }
            }
        }
        if (charCnt<mincnt) badpwd = true;
        if (badpwd){ Ninvalid++;}
        else{Nvalid++;}
        //printf("%d-%d %c: %s \n", mincnt, maxcnt, ch, pwd);
    }
    printf("file search complete \n");
    printf("%d invalid passwords found \n", Ninvalid);
    printf("Part 1 - number of valid passwords is %d \n", Nvalid);
    
    ///Part 2:
    rewind(myFile); //reset the pointer to the start of the file
    //fseek(myFile, 0, SEEK_SET);//alternative
    Nvalid=0;
    while (fscanf(myFile,"%d-%d %c: %s\n", &mincnt, &maxcnt, &ch, pwd) != EOF){
        if (pwd[mincnt-1]==ch ^ pwd[maxcnt-1]==ch){ // the '-1' is b/c they are 1 based index
            Nvalid++;
        }
    }
    printf("Part 2 - number of valid pwd is %d \n", Nvalid);
    fclose(myFile);
    free(pwd);
}
