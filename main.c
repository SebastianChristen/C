#include <stdio.h>
#include <stdint.h> // <- für datatypes und so
#include <string.h>
#include <stdlib.h> // <- für exit()
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
    char* n;
    char* e;
    char* s;
    char* w;
    //struct Item item;
};

struct Level levels[3];
int currentLevel = 0;


char verbs[0x5][0xF]  = {// 4x16
        "look",
        "take",
        "throw",
        "quit",
        "move"
    };


uint8_t commandLength = 0x20; // hexadezi weil fancy: 2*16 = 32; // type aus stdint.h

void process_command(char* verb, char* arg){
    if (strcmp(verb, "look") == 0 ){
        printf("%s", levels[currentLevel].description);
    }
    else if (strcmp(verb, "quit") == 0 ){
        exit(0);
    }
    else if (strcmp(verb, "move") == 0 ){
        if (arg == NULL) {
            printf("the parser expected a direction.");
            return;
        }
        printf("you go %s",arg);

    }
}


// parser
void parse(char* input){
    //printf("%s",input);

    char *argumentPtr = strstr(input, " "); // nimm alles inkl. nach dem leerzeichen

    if (argumentPtr == NULL || strcmp(argumentPtr, "") == 0){
        //printf("chicken jockey");
        argumentPtr = NULL; // boi what
    } else {
        argumentPtr = argumentPtr+1; //starte einen char weiter vorne, also ohne das leerzeichen
    }

    char* verb = strsep(&input, " ");
    verb = strsep(&verb, "\n");

    uint8_t i; // loop durch array
    for (i = 0; i < sizeof(verbs)/0xF; i++){
        if (strcmp(verbs[i], verb) == 0 ) {
            process_command(verb, argumentPtr);
            return;
        }
    }

    printf("the parser was unable to process the verb.");

}


void readFile(){
    FILE* fptr;
    fptr = fopen("world.wad", "r");
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


    printf("*** Level setup finished ***");


}




// main loop
int main(){
    //setup
    readFile();
    char command[commandLength];
    printf("%s",welcomeText);

    //main game loop
    while (1){
        printf("\n>?");
        fgets(command, sizeof(command), stdin);
        parse(command);
    }

    return 0;
}

