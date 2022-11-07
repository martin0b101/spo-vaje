
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

//Writes data from stanadrd input to file.
int append_text(char* filename){
    FILE* file;
    file = fopen(filename, "a");
    int bytes_read = 0;
    char buffer[BUFFER_SIZE];
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        printf("Bytes read %d", bytes_read);
        fputc(bytes_read, file);
    }
    fputs(buffer, file);
    if (bytes_read < 0)
    {
        perror("Napaka branja: ");
    }
    fclose(filename);
    
    return 0;
}

//Deletes line on number n.
int delete_n_line(FILE* file, int* line_number){

    return 0;
}

int main(int argc, char *argv[]){

    if (argc > 4)
        printf("Error: to much arguments!\n");
    if (argc < 3)
        printf("Error: to few arguments!\n");

    
    FILE *file;

    //printf("File desc o: %d\n", file);
    //append
    if (argv[2][0] == 'a')
    {
        printf("Writing:\n");
        append_text(argv[1]);
        //fclose(file);
        /* code */
    }else if(argv[2][0] == 'd'){
        //delete 
        /*file = fopen(argv[1], "r");
        printf("delete\n");
        int n = atoi(argv[3]);
        delete_n_line(file, n);
        fclose(file);*/

    }else if(argv[2][0] == 'i'){
        //insert
    }else{
        printf("Wrong arguments!\n");
    }

    return 0;
}