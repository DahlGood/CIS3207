#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    //Loops through files
    for(int i = 1; i < argc; i++) {

        FILE* file = fopen(argv[i], "r");
        if(file == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        char buffer[200];
        //Reads file line by line and prints the current lines as it reads.
        while(fgets(buffer, 200, file)) {
            printf("%s", buffer);
        }

        fclose(file);

    }
    
    return 0;
}