#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

//////////////////////////////////////////////////////////////////////////////////// DAY 7 /////////
int CountLinesInFile(const char* filename)
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



struct bag{ //should probably have been named 'Rule'
    char *bagtype;
    char **bagcontents;
    int *bagvals;
    int Nbagtypes;
};

int FindBagsContaining(char *queryString, struct bag *bags, int *RulesFoundNew, int NRules){
    int Nfinds=0;
    for (int iRule=0; iRule<NRules; iRule++){
        for (int ibag=0; ibag<(bags+iRule)->Nbagtypes; ibag++){
            if (strcmp(queryString, (bags+iRule)->bagcontents[ibag])==0){
                RulesFoundNew[iRule]++;
                Nfinds++;
                break;
            }
        }
    }
    return Nfinds;
}

int FindBagsContainedIn(char *queryString, struct bag *bags, int *RulesFoundNew, int NRules){
    int Nfinds=0;
    for (int iRule=0; iRule<NRules; iRule++){
        if (strcmp(queryString, (bags+iRule)->bagtype)==0){
            for (int ibag=0; ibag<(bags+iRule)->Nbagtypes; ibag++){
                Nfinds += (bags+iRule)->bagvals[ibag];
                
                int subfinds=0;
                for (int iiRule=0; iiRule<NRules; iiRule++){
                    if (strcmp( (bags+iiRule)->bagtype, (bags+iRule)->bagcontents[ibag])==0){
                        RulesFoundNew[iiRule] = (bags+iRule)->bagvals[ibag];
                        subfinds++;
                    }
                    if (subfinds == (bags+iRule)->Nbagtypes){
                        break; //conditionally escape could be done here to make it faster... i.e. when number of finds = Nbagtypes
                    }
                }
            }
            break;
        }
    }
    
    return Nfinds;
}

int CountAllBagsNeeded(char *queryString, struct bag *bags, int NRules) { // not the best speed, but height traversal still (lateral would not work)
    int total=0;
    //printf("queryString %s \n", queryString);
    for (int i=0; i<NRules; i++){
        bool stringmatch = (strcmp(queryString, (bags+i)->bagtype)==0);
        if (stringmatch){
            for (int subi=0; subi<(bags+i)->Nbagtypes; subi++){
                //temp = bagval + bagval*DumFunc();
                total = total + (bags+i)->bagvals[subi] + (bags+i)->bagvals[subi] * CountAllBagsNeeded( (bags+i)->bagcontents[subi], bags, NRules);
            }
        }
    }
    return total;
}


void InitRulesFound(int *RulesFound, int NRules){
    for (int i=0; i<NRules; i++) RulesFound[i]=0;
}

void DeleteBags(struct bag *bags, int NRules){
    for (int i=0; i<NRules; i++){
        for (int j=0; j<(bags+i)->Nbagtypes; j++){
            free((bags+i)->bagcontents[j]);
        }
        free((bags+i)->bagcontents);
        free((bags+i)->bagtype);
        free((bags+i)->bagvals);
    }
    free(bags);
}
 
void func_day7(const char* filename){
    int NRules = CountLinesInFile(filename);
    printf("%d rules in the file \n", NRules);
    
    struct bag *bags; //should be name Rules
    // initialization - make this a function
    bags = (struct bag*) malloc(NRules*sizeof(struct bag));
    for (int i=0; i<NRules; i++){
        (bags+i)->bagtype = malloc(sizeof(char)*40);
        (bags+i)->bagcontents = malloc(sizeof(char*)*20); //assumes every bag contains at least 1 other bag type
        (bags+i)->bagvals = malloc(sizeof(int)*20);
        (bags+i)->Nbagtypes = 0;
    }
    
    FILE *myFile;
    myFile = fopen(filename, "r");
    
    if (myFile == NULL){
        fprintf(stderr,"Error Reading File \n");
        exit (-1);
    }
    
    size_t n_matches = 3; //
    regex_t regex_bagtype, regex_bagcontents;
    regcomp(&regex_bagtype, "(.*?) bags", REG_EXTENDED | REG_ENHANCED);
    regcomp(&regex_bagcontents, "(\\d+) (.*?) bags?", REG_EXTENDED | REG_ENHANCED);
    regmatch_t pMatch[n_matches];
    char bagtype[40], bagcontents[40];
    
    char*line = NULL;
    size_t len = 0;
    int iRule = 0;
    while (getline(&line, &len, myFile) != -1) {//read the whole line
        //printf("%s", line); //note, line includes \n
        
        //parse inputs
        //parse bagtype (only first hit, no need for while loop
        if (regexec(&regex_bagtype, line, n_matches, pMatch, 0)==0){
            for (int i=0; i<n_matches; i++){
                if (pMatch[i].rm_so < pMatch[i].rm_eo){
                    strncpy(bagtype, &line[pMatch[i].rm_so], pMatch[i].rm_eo - pMatch[i].rm_so);
                    bagtype[pMatch[i].rm_eo-pMatch[i].rm_so] = '\0';
                    //printf("%s \n", bagtype);
                }
            }
        }
        //(bags + iRule)->bagtype = bagtype; //don't understand why this wouldn't work...but ok
        strcpy((bags+iRule)->bagtype, bagtype); //printf("%s \n", (bags+iRule)->bagtype);
        
        //parse bagcontents (record all hits
        //int lineOffset=0;
        char *temp = line; //&line[lineOffset];
        bool contentsEmpty=true;
        int ibagtype=0;
        while (regexec(&regex_bagcontents, temp, n_matches, pMatch, 0)==0){
            contentsEmpty=false;
            ibagtype++;
            (bags+iRule)->Nbagtypes=ibagtype;
            (bags+iRule)->bagcontents = realloc((bags+iRule)->bagcontents, sizeof(char *)*ibagtype);
            (bags+iRule)->bagvals = realloc((bags+iRule)->bagvals, sizeof(int)*ibagtype);
            for (int i=0; i<n_matches; i++){ // i=0, not needed as it is the full str match (versus group)
                if (pMatch[i].rm_so < pMatch[i].rm_eo){ //originaill != cond   ... why would it ever return rm_so > rm_eo ??
                    //printf("%d \n", pMatch[i].rm_eo - pMatch[i].rm_so);
                    strncpy(bagcontents, &temp[pMatch[i].rm_so], pMatch[i].rm_eo - pMatch[i].rm_so);
                    //strncpy(bagcontents, &line[pMatch[i].rm_so], pMatch[i].rm_eo - pMatch[i].rm_so);
                    bagcontents[pMatch[i].rm_eo-pMatch[i].rm_so] = '\0';
                    //printf("BAG CONTENTS FOUND: %s \n", bagcontents);
                    
                    if (i==1) (bags+iRule)->bagvals[ibagtype-1] = atoi(bagcontents);
                    if (i==2) {
                        ((bags+iRule)->bagcontents[ibagtype-1]) = (char *)malloc(sizeof(char)*40); //(char *)malloc(sizeof(char)*strlen(bagcontents));
                        strcpy((bags+iRule)->bagcontents[ibagtype-1], bagcontents);
                    }
                }
            }
            temp += pMatch[0].rm_eo;
            //lineOffset += pMatch[0].rm_eo;
        }
        
        //printf("number of bag types contained: %d \n", (bags+iRule)->Nbagtypes);
        if (contentsEmpty==true) {
            strcpy(bagcontents, "no other bags");
            ((bags+iRule)->bagcontents[0]) = (char *)malloc(sizeof(char)*strlen(bagcontents));
            strcpy((bags+iRule)->bagcontents[0], bagcontents);
            //printf("empty bag: %s \n", (bags+iRule)->bagtype);
        }
        iRule++;
    }
    fclose(myFile);
    
    printf("File parsing complete \n");
    
    ///Check the bag types
    int CheckFileParseResults=0;
    if (CheckFileParseResults==1){
        printf("\n printing extracted rules \n");
        for (int i=0; i<NRules; i++){
            printf("bagtype %s \n", (bags+i)->bagtype);
            for (int j=0; j<(bags+i)->Nbagtypes; j++){
                printf("nbags %d \n", (bags+i)->bagvals[j]);
                printf("of %s \n", (bags+i)->bagcontents[j]);
            }
        }
    }
    
    //// Part 1 - begin the search //////////
    char queryString[40];
    strcpy(queryString, "shiny gold");
    //int totalRulesFound[NRules], RulesFoundNew[NRules], RulesFoundOld[NRules], RulesFoundSingleIter[NRules];
    int *totalRulesFound, *RulesFoundNew, *RulesFoundOld, *RulesFoundSingleIter;
    totalRulesFound = (int*) calloc(NRules, sizeof(int));
    RulesFoundNew = (int *) calloc(NRules, sizeof(int));
    RulesFoundOld = (int *) calloc(NRules, sizeof(int));
    RulesFoundSingleIter = (int *) calloc(NRules, sizeof(int));
    
    int Nfound = FindBagsContaining(queryString, bags, RulesFoundNew, NRules);
    for (int i=0; i<NRules; i++) totalRulesFound[i]=RulesFoundNew[i];
    
    printf("found %s in bag type: \n", queryString);
    for (int i=0; i<NRules; i++){
        if (RulesFoundNew[i] != 0){
            printf("%s \n", (bags+i)->bagtype);
        }
    }
    
    while (Nfound>0){
        Nfound=0; //nfound per while loop
        //memcpy(&RulesFoundOld, &RulesFoundNew, NRules*sizeof(int));//copy rules found new into old
        for (int i=0; i<NRules; i++) RulesFoundOld[i]=RulesFoundNew[i];
        InitRulesFound(RulesFoundNew,NRules); //set rules found new to zeros
        for (int i=0; i<NRules; i++){
            if (RulesFoundOld[i] != 0){
                InitRulesFound(RulesFoundSingleIter, NRules);
                //strcpy(queryString, (bags+i)->bagtype);
                strcpy(queryString, (bags+i)->bagtype);
                Nfound += FindBagsContaining(queryString, bags, RulesFoundSingleIter, NRules);
                
                for (int j=0; j<NRules; j++) RulesFoundNew[j] += RulesFoundSingleIter[j]; //add RulesFoundSingle to rules found new
                
                /*
                printf("found %s in bag type: \n", queryString);
                for (int iprint=0; iprint<NRules; iprint++){
                    if (RulesFoundSingleIter[iprint] != 0){
                        printf("%s \n", (bags+iprint)->bagtype);
                    }
                }
                */
            }
        }
        //printf("moving onto next level of search \n\n");
        for (int j=0; j<NRules; j++) totalRulesFound[j]+=RulesFoundNew[j];//update totalRulesFound
    }
    
    int Nouterbags=0;
    for (int i=0; i<NRules; i++){
        if (totalRulesFound[i] != 0) Nouterbags++;
    }
    printf("Part 1 solution : %d \n", Nouterbags);
    
    
    //////////////// Part 2:
    InitRulesFound(RulesFoundNew, NRules);
    strcpy(queryString, "shiny gold");
    int Ntotal=0;
    Ntotal = CountAllBagsNeeded(queryString, bags, NRules);
    printf("Part 2 solution: %d \n", Ntotal);
    
    /* //This is NOT the way to do part 2, but i thought i would keep it anyway. only goes one level down
    strcpy(queryString, "shiny gold");
    InitRulesFound(RulesFoundNew, NRules);
    Nfound = FindBagsContainedIn(queryString, bags, RulesFoundNew,NRules);
    
    printf("%d bags in total \n", Nfound);
    for (int i=0; i<NRules; i++){
        if (RulesFoundNew[i]!=0){
            printf("%d of %s \n", RulesFoundNew[i] , (bags+i)->bagtype);
        }
    }
    */
    
    free(line);
    DeleteBags(bags, NRules);
    regfree(&regex_bagtype);
    regfree(&regex_bagcontents);
}


