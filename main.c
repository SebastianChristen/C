#include <stdio.h>
#include <stdint.h> // <- für datatypes und so
#include <string.h> 
//#include <definitions.h>

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
    FILE* infile;
    infile = fopen("world.wad", "wb+");
    struct Level write_struct = { "map", 1, "forest", "ur in a forest", 1,2,3,4 }; // testdaten
    // writing to file
    fwrite(&write_struct, sizeof(write_struct), 1, infile);


    struct Level read_struct;

    // setting pointer to start of the file
    rewind(infile);

    // reading to read_struct
    fread(&read_struct, sizeof(read_struct), 1, infile);



    printf("%s %d %s %s %d %d %d %d", 
        read_struct.type,
        read_struct.id,
        read_struct.name,
        read_struct.description,
        read_struct.n, read_struct.e, read_struct.s, read_struct.w );

 // close file
    fclose(infile);


}




// main loop
void main(){
    char command[commandLength];
    printf("%s",welcomeText);

    printf(">?");
    fgets(command, sizeof(command), stdin);
    //parse(command);
    readFile();

}

