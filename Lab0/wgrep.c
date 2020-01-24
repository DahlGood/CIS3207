#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if(argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    if(argc == 2) {
        char content[200];
        while(fgets(content, 200, stdin) != NULL) {
                if(strstr(content, argv[1])) {
                    printf("%s", content);
                }

            }
        
        return 0;
    }


    for(int i = 2; i < argc; i++) {

            FILE* file = fopen(argv[i], "r");
            if(file == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            //Getting size of file.
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            rewind(file);

            //Allocating memory for the content in the file.
            char* content = (char *)malloc(size);
            if(content == NULL) {
                printf("MALLOC failed\n");
                exit(1);
            }

            //+1 accounting for the end of file marker.
            while(fgets(content, size+1, file) != NULL) {
                if(strstr(content, argv[1])) {
                    printf("%s", content);
                }

            }
            
            fclose(file);

            free(content);

    }

    return 0;
}