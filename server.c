#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <string.h>
#include <unistd.h>
#define MAX 1024
#define len 256

/* struct definition for holding student marks */
struct student{
    char firstname[len];
    char lastname[len];
    int transfiguration;
    int charms;
    int potions;
    int dada;
    int herbology;
};

/* struct definition for holding user pass data */
struct auth{
    char username[len];
    char password[len];
    char firstname[len];
    char lastname[len];
    int clearance;
    struct student *link;
};

/* returns length of string from character array by counting untill null character \0 */
int getStringLength(char *str){
    int length = 0;
    while(str[length] != '\0'){
        length++;
    }
    return length;
}

/* returns 1 if the two strings are equal 0 otherwise */
int compare_Strings(char *str1, char *str2){
    int len1 = getStringLength(str1);
    int len2 = getStringLength(str2);
    if(len1 != len2){
        return 0;
    }
    else{
        for(int h = 0;h < len1; h++){
            if(str1[h] != str2[h]){
                return 0;
            }
        }
        return 1;
    }
}

/* Parses the student marks text file and stores the data in an array of struct student. 
Caution: The following function assumes that the data in the given file occurs in a predefined order */
/* The following code currently assumes that the files opened exist and the data in the them is arranged in a particular order and there are no duplicate entries in the file */
void createMarksMemory(struct student *marks_db, int *marks_db_size){
    FILE *student_marks;
    char file_buffer[MAX];
    student_marks = fopen("student_marks.txt", "r");
    fgets(file_buffer, MAX, (FILE*)student_marks);
    while(fgets(file_buffer, MAX, (FILE*)student_marks)){
       struct student temporary;
       int pointer = 0;
       int cnt = 0;
       while(file_buffer[pointer] != ' '){
           temporary.firstname[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.firstname[cnt] = '\0';
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != ' '){
           temporary.lastname[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.lastname[cnt] = '\0';
       cnt = 0;
       pointer++;
       char tempo[3] = {'\0'};
       while(file_buffer[pointer] != ' '){
           tempo[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
        temporary.transfiguration = atoi(tempo);
        cnt = 0;
        pointer++;
        while(file_buffer[pointer] != ' '){
           tempo[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }       
       temporary.dada = atoi(tempo);
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != ' '){
           tempo[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.potions = atoi(tempo);
       cnt = 0;
       pointer++;
        while(file_buffer[pointer] != ' '){
           tempo[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.herbology = atoi(tempo);
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != '\n' && file_buffer[pointer] != '\0'){
           tempo[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.charms = atoi(tempo);
       cnt = 0;
       marks_db[*marks_db_size] = temporary;
       *marks_db_size = (*marks_db_size) + 1;
    }
    fclose(student_marks);
}

/* Parses the user pass text file and stores the data in an array of struct auth. 
Caution: The following function assumes that the data in the given file occurs in a predefined order */
/* The following code currently assumes that the files opened exist and the data in the them is arranged in a particular order and there are no duplicate entries in the file */
void createAuthMemory(struct auth *login_db, int *login_db_size, struct student *marks_db, int marks_db_size ){
    FILE *user_pass;
    char file_buffer[MAX];
    user_pass = fopen("user_pass.txt", "r");
    fgets(file_buffer, MAX, (FILE*)user_pass);
    while(fgets(file_buffer, MAX, (FILE*)user_pass)){
       struct auth temporary;
       int pointer = 0;
       int cnt = 0;
       if(file_buffer[pointer] == 'P'){
           temporary.clearance = 1;
           temporary.link = NULL;
       }
       else{
           temporary.clearance = 0;
       }
       while(file_buffer[pointer] != ' '){
           pointer++;
       }
       pointer++;
       while(file_buffer[pointer] != ' '){
           temporary.firstname[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.firstname[cnt] = '\0';
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != ' '){
           temporary.lastname[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.lastname[cnt] = '\0';
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != ' '){
           temporary.username[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.username[cnt] = '\0';
       cnt = 0;
       pointer++;
       while(file_buffer[pointer] != '\n' && file_buffer[pointer] != '\0'){
           temporary.password[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.password[cnt] = '\0';
       cnt = 0;
       pointer++;
       if(temporary.clearance == 0){
           for(int h = 0; h < marks_db_size; h++){
               if((compare_Strings(temporary.firstname, marks_db[h].firstname) == 1) && (compare_Strings(temporary.lastname, marks_db[h].lastname) == 1)){
                   temporary.link = &marks_db[h];
                   break;
               }
           }
       }
       login_db[*login_db_size] = temporary;
       *login_db_size = (*login_db_size) + 1;
    }
    fclose(user_pass);
}

int main(int total_Arguments, char *argument_Pointers[]){

    int socket_fd, newSocket_fd, port_Number, client_Address_Length, length;
    char socket_Buffer[MAX];
    struct sockaddr_in server_Address, client_Address;
    if(total_Arguments < 2){
        fprintf(stderr, "ERROR: no port number provided\n");
        return 0;
    }
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        fprintf(stderr, "ERROR opening socket\n");
        return 0;
    }
    printf("Socket successfully opened \n");

    bzero((char *) &server_Address, sizeof(server_Address));
    port_Number = atoi(argument_Pointers[1]);
    server_Address.sin_family = AF_INET;
    server_Address.sin_port = htons(port_Number);
    server_Address.sin_addr.s_addr = INADDR_ANY;
    if(bind(socket_fd, (struct sockaddr *) &server_Address, sizeof(server_Address)) < 0){
        fprintf(stderr, "ERROR binding\n");
        return 0;
    }
    printf("Socket binding successfull \n");

    struct student marks_db[MAX];
    int marks_db_size = 0;
    createMarksMemory(marks_db, &marks_db_size);
    printf("Student marks added to Cache \n");
    // for(int i = 0; i < marks_db_size ; i++){
    //     printf("%s %s %d %d %d %d %d\n", marks_db[i].firstname, marks_db[i].lastname, marks_db[i].transfiguration, marks_db[i].dada, marks_db[i].potions, marks_db[i].herbology, marks_db[i].charms);
    // }
    struct auth login_db[MAX];
    int login_db_size = 0;
    createAuthMemory(login_db, &login_db_size, marks_db, marks_db_size);
    printf("Authorization details added to cache \n");
    // for(int i = 0; i < login_db_size; i++){
    //     printf("%s %s ", login_db[i].username, login_db[i].password);
    //     if(!login_db[i].clearance){
    //         printf("%s", (*login_db[i].link).firstname);
    //     }
    //     printf("\n");
    // }
    if( listen(socket_fd, 4) < 0){
        fprintf(stderr, "ERROR listening\n");
        return 0;
    }
    printf("Listening on port %d \n", port_Number);
    client_Address_Length = sizeof(client_Address);
    for( ; ; ){
        newSocket_fd = accept(socket_fd, (struct sockaddr *) &client_Address, &client_Address_Length);
        if(newSocket_fd < 0){
            fprintf(stderr, "ERROR on accept\n");
            return 0;
        }
        printf("%s\n", "Client Connected");
        length = read(newSocket_fd, socket_Buffer, MAX);
        printf("%s\n", socket_Buffer);
        int found = -1;
        int authenticated_User = -1;
        for(int h = 0;h < login_db_size;h++){
            if(compare_Strings(socket_Buffer, login_db[h].username) == 1){
                found = h;
                break;
            }
        }
        if(found != -1){
            length = write(newSocket_fd, "1", 2);
            if(length < 0){printf("%s\n", "ERROR in Writing"); }
            length = read(newSocket_fd, socket_Buffer, MAX);
            printf("%s\n", socket_Buffer);
            if(length < 0){printf("%s\n", "ERROR in Reading"); }
            if(compare_Strings(socket_Buffer, login_db[found].password) == 1){
                length = write(newSocket_fd, "1", 2);
                if(length < 0){printf("%s\n", "ERROR in Writing"); }
                printf("%s\n", "User Authenticated");
                authenticated_User = found;

                for( ; ; ){
                    length = read(newSocket_fd, socket_Buffer, MAX);
                    printf("%s\n", socket_Buffer);
                    if(length < 0){printf("%s\n", "ERROR in Reading"); }
                }
                
            }
            else{
                length = write(newSocket_fd, "0", 2);
                if(length < 0){printf("%s\n", "ERROR in Writing"); }
            }
        }
        else{
            length = write(newSocket_fd, "0", 2);
            if(length < 0){printf("%s\n", "ERROR in Writing"); }
        }
        close(newSocket_fd);
        printf("%s\n", "Client Disconnected");
    }    
    close(socket_fd);
    return 0;
}