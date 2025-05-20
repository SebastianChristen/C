#include <stdio.h>
#include <stdint.h> // <- für datatypes und so
#include <string.h>
#include <stdlib.h> // <- für exit()
#include "definitions.h"

// Init
char* welcomeText = "\n\nWelcome to adventure!\n\n";
const uint8_t COMMANDLENGTH = 0x20; // hexadezi weil fancy: 2*16 = 32; // type aus stdint.h
const uint8_t MAX_LINE_LEN = 0xFF; //255

#define ALEN(arr) (sizeof(arr) / sizeof((arr)[0]))

struct Entity {
    char * type;
    int id;
    char* name;
    char* description;
};

struct Item {
    char * type;
    int id;
    char* name;
    char* description;
    int position;
};


struct Level {
    char * type;
    int id;
    char * name;
    char * description;
    int n;
    int e;
    int s;
    int w;
};


struct Command {
    char* command;
    char* function;
};

struct Command g_commands[10] = {

    {"look", "look"},
    {"quit", "quit_game"},
    {"n", "move_north"},
    {"e", "move_east"},
    {"s", "move_south"},
    {"w", "move_west"},



};



struct Level* g_levels;
struct Entity* g_entities;
struct Item* g_items;
int g_levelCount = 0;
int g_itemCount = 0;
int g_currentLevel = 0;


void process_command(char* verb, char* arg){
    if (strcmp(verb, "look") == 0 ){
        printf("%s", g_levels[g_currentLevel].description);
    }
    else if (strcmp(verb, "quit") == 0 ){
        exit(0);
    }
    else if (strcmp(verb,"n") == 0){
        g_currentLevel = g_levels[g_currentLevel].n;
    }
    else if (strcmp(verb,"e") == 0){
        g_currentLevel = g_levels[g_currentLevel].e;
    }
    else if (strcmp(verb,"s") == 0){
        g_currentLevel = g_levels[g_currentLevel].s;
    }
    else if (strcmp(verb,"w") == 0){
        g_currentLevel = g_levels[g_currentLevel].w;
    }
    else if (strcmp(verb, "move") == 0 ){
        if (arg == NULL) {
            printf("the parser expected a direction.");
            return;
        }
        if (strcmp(arg,"north") == 0){
            g_currentLevel = g_levels[g_currentLevel].n;
        } else
        if (strcmp(arg,"east") == 0){
            g_currentLevel = g_levels[g_currentLevel].e;
        } else
        if (strcmp(arg,"south") == 0){
            g_currentLevel = g_levels[g_currentLevel].s;
        }
        else
        if (strcmp(arg,"west") == 0){
            g_currentLevel = g_levels[g_currentLevel].w;
        } else {
            printf("this direction is not known.");
            return;
        }
        printf("you move %s",arg);
    }


    else {
        printf("the parser was unable to process the verb.");
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

    process_command(verb, argumentPtr);
    return;

}

// ---
// iterates trough the current Line and store attributes in struct array at current Id index
//
void storeLevel(char fileLine[], int levelId){
    char* tokenPtr = strtok(fileLine, "|");    // init new tokenPtr (start of current Line String)
    int i = 0;
    while (tokenPtr != NULL) {
        printf("tokenptr %d %s",i,tokenPtr);
        switch (i){
            case 0: g_levels[levelId].type =        strdup(tokenPtr);  break;
            case 1: g_levels[levelId].id =          atoi(tokenPtr);  break;
            case 2: g_levels[levelId].name =        strdup(tokenPtr);  break;
            case 3: g_levels[levelId].description = strdup(tokenPtr);  break;
            case 4: g_levels[levelId].n =           atoi(tokenPtr); break;
            case 5: g_levels[levelId].e =           atoi(tokenPtr); break;
            case 6: g_levels[levelId].s =           atoi(tokenPtr); break;
            case 7: g_levels[levelId].w =           atoi(tokenPtr); break;
            default: break;
        }
        tokenPtr = strtok(NULL, "|");
        i++;
    }
}

void storeItem(char fileLine[], int itemId){
    char* tokenPtr = strtok(fileLine, "|");    // init new tokenPtr (start of current Line String)
    int i = 0;
    while (tokenPtr != NULL) {
        printf("tokenptr %d %s",i,tokenPtr);
        switch (i){
            case 0: g_items[itemId].type =        strdup(tokenPtr);  break;
            case 1: g_items[itemId].id =          atoi(tokenPtr);  break;
            case 2: g_items[itemId].name =        strdup(tokenPtr);  break;
            case 3: g_items[itemId].description = strdup(tokenPtr);  break;
            case 4: g_items[itemId].position =    atoi(tokenPtr);  break;
            default: break;
        }
        tokenPtr = strtok(NULL, "|");
        i++;
    }
}


void readFile(){
    FILE* fptr;
    fptr = fopen("world.wad", "r");
    char fileLine[MAX_LINE_LEN];


    //--- looks good but look again ---
    int countLevels = 0, countItems = 0;

    // Set countLevels and countItems
    // if line startwith map, increase countLevels ...
    while (fgets(fileLine, MAX_LINE_LEN, fptr)) {
        if (strncmp("map", fileLine, 3) == 0) countLevels++;
        else if (strncmp("item", fileLine, 4) == 0) countItems++;
    }
    rewind(fptr); // go back to start of file

    // allocate only what we need
    g_levels = malloc(sizeof(struct Level) * countLevels);
    g_items = malloc(sizeof(struct Item) * countItems);
    g_levelCount = countLevels;
    g_itemCount = countItems;
    // ---


    int n = 0;
    while(fgets(fileLine, MAX_LINE_LEN, fptr)) {

        // --- prepare ID
        char fileLineForId[MAX_LINE_LEN];       // set up array
        strcpy(fileLineForId, fileLine);        // create a copy of fileLine and use it only for getting the Id
        // --- Read ID
        strtok(fileLineForId, "|");                  // split until first delimiter
        char* idTokenPtr = strtok(NULL,"|");         // take the part after the delimiter (the Id)
        int id = atoi(idTokenPtr);              // turn Id from string to int


        if (strncmp("map", fileLine, strlen("map")) == 0){ // check if string start with substring function
            storeLevel(fileLine, id);
        } else if (strncmp("item", fileLine, strlen("item")) == 0){ // check if string start with substring function
            storeItem(fileLine, id);
        }

        n++;
    };

    // close file
    fclose(fptr);
}




// main loop
int main(){
    //setup
    readFile();


    // JUST TO MAKE SURE IT WORKS
    for (int i = 0; i < g_levelCount; i++) {
        printf("Type: %s\n", g_levels[i].type);
        printf("ID: %d\n", g_levels[i].id);
        printf("Name: %s\n", g_levels[i].name);
        printf("Description: %s\n", g_levels[i].description);
        printf("N: %d\n", g_levels[i].n);
        printf("E: %d\n", g_levels[i].e);
        printf("S: %d\n", g_levels[i].s);
        printf("W: %d\n\n", g_levels[i].w);
    }

    // JUST TO MAKE SURE IT WORKS
    for (int i = 0; i < g_itemCount; i++) {
        printf("Type: %s\n", g_items[i].type);
        printf("ID: %d\n", g_items[i].id);
        printf("Name: %s\n", g_items[i].name);
        printf("Description: %s\n", g_items[i].description);
        printf("Position: %d\n\n", g_items[i].position);
    } 
 


    char command[COMMANDLENGTH];
    printf("%s",welcomeText);

    //main game loop
    while (1){

        // explain current situation
        printf("\n\n%s",g_levels[g_currentLevel].description);
        int n = g_levels[g_currentLevel].n;
        int e = g_levels[g_currentLevel].e;
        int s = g_levels[g_currentLevel].s;
        int w = g_levels[g_currentLevel].w;
        printf("\nIn the north, you notice a %s.",g_levels[n].name);
        printf("\nIn the east,  you notice a %s.",g_levels[e].name);
        printf("\nIn the south, you notice a %s.",g_levels[s].name);
        printf("\nIn the west,  you notice a %s.",g_levels[w].name);

        // search for g_items
        for (int i = 0; i < g_itemCount; i++){
            if (g_levels[g_currentLevel].id == g_items[i].position){
                printf("\nthere is a %s.", g_items[i].name);
            }
        }




        printf("\n>?");
        fgets(command, sizeof(command), stdin);
        parse(command);
    }

    return 0;
};

