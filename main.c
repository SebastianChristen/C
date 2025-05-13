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
    char* type;
    uint8_t id;
    char* name;
    char* description;
    uint8_t n;
    uint8_t e;
    uint8_t s;
    uint8_t w;  
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
    //struct Level write_struct = { "map", 1, "forest", "ur in a forest", 1,2,3,4 }; // testdaten
    // writing to file
    //  fwrite(&write_struct, sizeof(write_struct), 1, infile);


    struct Level read_struct;

   // Store the content of the file
    char myString[100];
    char words[100];
    //char *tokenPtr;

    // Read the content and print it

    
    int n;
    while(fgets(myString, 100, fptr)) {
        printf("%s", myString);
        char *tokenPtr = strtok(myString, "|");
        int i = 0;
        while (tokenPtr != NULL) {
            

            switch (i)
            {
            case 0:
                printf("type: %s\n", tokenPtr);
                break;
            case 1:
                printf("id: %s\n", tokenPtr);
                break;
            case 2:
                printf("name: %s\n", tokenPtr);
                break;
            case 3:
                printf("description: %s\n", tokenPtr);
                break;
            
            default:
                break;
            }




            tokenPtr = strtok(NULL, "|");
            i++;
        }
    };

   


    //while ( words ){
    //    printf("%s\n",words);
    //    words = strtok(myString, "|"); // first word in sentence
    //}


   // printf("%s %d %s %s %d %d %d %d", 
    //    read_struct.type,
   //     read_struct.id,
   //     read_struct.name,
   //     read_struct.description,
   //     read_struct.n, read_struct.e, read_struct.s, read_struct.w );

 // close file
    fclose(fptr);


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

