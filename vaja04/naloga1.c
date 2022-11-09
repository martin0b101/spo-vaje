
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000

//Writes data from stanadrd input to file.
int append_text(FILE* file){
    char buf[BUFFER_SIZE];
    printf("Write code: \n");
    while (fgets(buf, sizeof buf, stdin) && buf[0] != '\n') {
        fputs(buf, file);
    }
    fclose(file);
    return 0;
}

//Deletes line on number n.
int delete_n_line(char* file_name, int* line_number){
    char line[BUFFER_SIZE];
    FILE* file = fopen(file_name, "r");
    FILE* temp = fopen("temp", "w");
    int line_counter = 1;
    char buff[BUFFER_SIZE];
    bool read = true;

    do{
        fgets(line, BUFFER_SIZE, file);
        if (feof(file))
            read = false;
        else if(line_counter != line_number)
            fputs(line, temp);

        line_counter++;
    }while(read);

    fclose(temp);
    fclose(file);
    remove(file_name);
    rename("temp", file_name);
    return 0; 

}

int insert_on_nth_line(char* file_name, int* line_number, char* newline_content){
    char line[BUFFER_SIZE];
    FILE* file = fopen(file_name, "r");
    FILE* temp = fopen("temp", "w");
    int line_counter = 1;
    char buff[BUFFER_SIZE];
    bool read = true;

    do{
        fgets(line, BUFFER_SIZE, file);
        if (feof(file))
            read = false;
        if(line_counter == line_number){
            fputs(newline_content, temp);
            fputs(line, temp);
        }else{
            fputs(line, temp);
        }
        line_counter++;
        
    }while(read);
    fclose(temp);
    fclose(file);
    remove(file_name);
    rename("temp", file_name);
    return 0;
}

int main(int argc, char *argv[]){

    if (argc > 4)
        printf("Error: to much arguments!\n");
    if (argc < 3)
        printf("Error: to few arguments!\n");
    //append
    if (argv[2][0] == 'a')
    {
        FILE* file = fopen(argv[1], "a+");
        append_text(file);

    }else if(argv[2][0] == 'd'){
        //delete 
        //FILE* file = fopen(argv[1], "r");
        printf("delete\n");
        int n = atoi(argv[3]);
        delete_n_line(argv[1], n);

    }else if(argv[2][0] == 'i'){
        //insert
        char newline[BUFFER_SIZE];
        printf("insert line: ");
        int n = atoi(argv[3]);
        fgets(newline, BUFFER_SIZE, stdin);
        insert_on_nth_line(argv[1], n, newline);
    }else{
        printf("Wrong arguments!\n");
    }

    return 0;
}