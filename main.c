#include <stdio.h>
#include <stdint.h> // <- für datatypes und so
#include <string.h> 
#include "definitions.h"

// Init
char* welcomeText = "\n\nWelcome to adventure!\n\n";

struct Item {
    char* name;
    char* description;
};


struct Level {
    char * type;
    char * id;
    char * name;
    char * description;
    //uint8_t n;
    //uint8_t e;
    //uint8_t s;
    //uint8_t w;  
    //struct Item item;
};




char verbs[0x3][0xF]  = {// 4x16
        "look",
        "take",
        "throw"
    };


uint8_t commandLength = 0x20; // hexadezi weil fancy: 2*16 = 32; // type aus stdint.h

void process_verb(char* verb){
    if (strcmp(verb, "look") == 0 ){
        printf("You are located in a forest. you look around and see a house.");
    }
}


// parser
void parse(char* input){
    printf("%s",input);

    char* verb = strtok(input, " "); // first word in sentence

    uint8_t i; // loop durch array
    for (i = 0; i < sizeof(verbs)/0xF; i++){
        if (strcmp(verbs[i], verb) == 0 ) {
            printf("%s", verb);
            printf("!\n\n");
            process_verb(verb);
        }
    }


    
}


void readFile(){
    FILE* fptr;
    fptr = fopen("world.wad", "r");
   

    struct Level levels[3];
    char myString[100];



    
    int n = 0;
    while(fgets(myString, 100, fptr)) {
        printf("Setting up data for level index %d...", n);
        char *tokenPtr = strtok(myString, "|");
        // tokenPtr ist nur ein Pointer, und wird die value nicht behalten. deshalb strdup, um den wert endgültig auszulesen
        int i = 0;
        while (tokenPtr != NULL) {
            
            printf("n ist %d ",n);

            switch (i)
            {
            case 0:
                printf("type: %s\n", tokenPtr);
                levels[n].type = strdup(tokenPtr);
                break;
            case 1:
                printf("id: %s\n", tokenPtr);
                levels[n].id = strdup(tokenPtr);
                break;
            case 2:
                printf("name: %s\n", tokenPtr);
                levels[n].name = strdup(tokenPtr);
                break;
            case 3:
                printf("description: %s\n", tokenPtr);
                levels[n].description = strdup(tokenPtr);

                break;
            
            default:
                break;
            }




            tokenPtr = strtok(NULL, "|");
            i++;
        }
        n++;
    };

    // close file
    fclose(fptr);


    
    for (int i = 0; i < n; i++) {
        printf("\n\nLevel %d\n", i);
        printf("Type: %s\n", levels[i].type);
        printf("ID: %s\n", levels[i].id);
        printf("Name: %s\n", levels[i].name);
        printf("Description: %s\n", levels[i].description);
    }
   


    


}




// main loop
int main(){
    char command[commandLength];
    printf("%s",welcomeText);

    printf(">?");
    //fgets(command, sizeof(command), stdin);
    //parse(command);
    readFile();

    return 0;
}

