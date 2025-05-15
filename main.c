#include <stdio.h>
#include <stdint.h> // <- für datatypes und so
#include <string.h>
#include <stdlib.h> // <- für exit()
#include "definitions.h"

// Init
char* welcomeText = "\n\nWelcome to adventure!\n\n";
const uint8_t COMMANDLENGTH = 0x20; // hexadezi weil fancy: 2*16 = 32; // type aus stdint.h
const uint8_t MAX_LINE_LEN = 0xFF; //255

struct Entity {
    char* name;
    char* description;
};


struct Level {
    char * type;
    char * id;
    char * name;
    char * description;
    int n;
    int e;
    int s;
    int w;
    struct Entity item;
};

struct Level levels[3];
struct Entity entities[3];

int currentLevel = 0;


char verbs[0x5][0xF]  = {// 4x16
        "look",
        "take",
        "throw",
        "quit",
        "move"
    };

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
        if (strcmp(arg,"north") == 0){
            currentLevel = levels[currentLevel].n;
        } else
        if (strcmp(arg,"east") == 0){
            currentLevel = levels[currentLevel].e;
        } else
        if (strcmp(arg,"south") == 0){
            currentLevel = levels[currentLevel].s;
        }
        else
        if (strcmp(arg,"west") == 0){
            currentLevel = levels[currentLevel].w;
        } else {
            printf("this direction is not known.");
            return;
        }
        printf("you move %s",arg);

    }
}


// parser
void parse(char* input){
    //printf("%s",input);

    char *argumentPtr = strstr(input, " "); // nimm alles inkl. nach dem leerzeichen
    argumentPtr = strsep(&argumentPtr, "\n"); // trailing enter wegnehmen

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
    char fileLine[MAX_LINE_LEN];

    int n = 0;
    while(fgets(fileLine, MAX_LINE_LEN, fptr)) {
        printf("Setting up data for level index %d...", n);
        char *tokenPtr = strtok(fileLine, "|");
        // tokenPtr ist nur ein Pointer, und wird die value nicht behalten. deshalb strdup, um den wert endgültig auszulesen

        int i = 0;
        while (tokenPtr != NULL) {
            switch (i){
                case 0: levels[n].type =        strdup(tokenPtr);  break;
                case 1: levels[n].id =          strdup(tokenPtr);  break;
                case 2: levels[n].name =        strdup(tokenPtr);  break;
                case 3: levels[n].description = strdup(tokenPtr);  break;
                case 4: levels[n].n =           atoi(strdup(tokenPtr)); break;
                case 5: levels[n].e =           atoi(strdup(tokenPtr)); break;
                case 6: levels[n].s =           atoi(strdup(tokenPtr)); break;
                case 7: levels[n].w =           atoi(strdup(tokenPtr)); break;
                default: break;
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
    char command[COMMANDLENGTH];
    printf("%s",welcomeText);

    //main game loop
    while (1){
        printf("\n\n%s",levels[currentLevel].description);
        printf("\n>?");
        fgets(command, sizeof(command), stdin);
        parse(command);
    }

    return 0;
};

