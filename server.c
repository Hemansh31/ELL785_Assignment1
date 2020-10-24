#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <string.h>
#include <unistd.h>
#define MAX 1024
#define len 256
#define slen 8

struct subject{
    char *subjectName;
    int marks;
};


/* struct definition for holding student marks */
struct student{
    char firstname[len];
    char lastname[len];
    struct subject listOfSubjects[slen];
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
void createMarksMemory(struct student *marks_db, int *marks_db_size, char *file_descriptor, char subjectNames[slen][len], int *num_Sub){
    FILE *student_marks;
    char file_buffer[MAX];
    student_marks = fopen("student_marks.txt", "r");
    fgets(file_descriptor, MAX, (FILE*)student_marks);
    int open = -1;
    int curr_len = 0;
    for(int d = 0; d < getStringLength(file_descriptor); d++){
        if(open == -1){
            if(file_descriptor[d] == '('){
                open = 0;
                curr_len = 0;
            }
        }
        else{
            if(file_descriptor[d] != ')'){
                subjectNames[*num_Sub][curr_len] = file_descriptor[d];
                curr_len++;
            }
            else{
                subjectNames[*num_Sub][curr_len] = '\0';
                (*num_Sub)++;
                curr_len = 0;
                open = -1;
            }
        }
    }
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
        while(file_buffer[pointer] == ' '){
            pointer++;
        }
       while(file_buffer[pointer] != ' '){
           temporary.lastname[cnt] = file_buffer[pointer];
           pointer++;
           cnt++;
       }
       temporary.lastname[cnt] = '\0';
       cnt = 0;
         while(file_buffer[pointer] == ' '){
            pointer++;
        }
       char tempo[4] = {'\0'};

        int sub_count = 0;

       while(file_buffer[pointer] != '\0'){
           char number[4] = {'\0'};
           cnt = 0;
           
           while(file_buffer[pointer] == ' '){
               pointer++;
           }
           while(file_buffer[pointer] != ' ' && file_buffer[pointer] != '\n'){
               number[cnt] = file_buffer[pointer];
               pointer++;
               cnt++;
           }
           pointer++;
           number[cnt] = '\0';
           if(sub_count < (*num_Sub)){
                ((temporary.listOfSubjects[sub_count])).subjectName = (subjectNames[sub_count]);      
                ((temporary.listOfSubjects[sub_count])).marks = atoi(number);
                sub_count++;
           }           
       }

       marks_db[*marks_db_size] = temporary;
       (*marks_db_size) = (*marks_db_size) + 1;
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

void addStrings(int size, char *strings[], char *socketBuffer){
    int total_String_size = 0;
    for(int h = 0;h < size;h++){
        int curr_String_size = 0;
        while(strings[h][curr_String_size] != '\0'){
            socketBuffer[total_String_size] = strings[h][curr_String_size];
            curr_String_size++;
            total_String_size++;
        }
    }
    socketBuffer[total_String_size] = '\0';
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
    char file_descriptor[MAX];
    char subjectNames[slen][len];
    int num_Sub = 0;
    createMarksMemory(marks_db, &marks_db_size, file_descriptor, subjectNames, &num_Sub);
    printf("Student marks added to Cache \n");
    // for(int g = 0; g < num_Sub; g++){
    //     printf("%s ", subjectNames[g]);
    // }
    // printf("\n");
    // for(int i = 0; i < marks_db_size ; i++){
    //     printf("%s %s %d %d %d %d %d\n", marks_db[i].firstname, marks_db[i].lastname, marks_db[i].listOfSubjects[0].marks, marks_db[i].listOfSubjects[1].marks, marks_db[i].listOfSubjects[2].marks, marks_db[i].listOfSubjects[3].marks, marks_db[i].listOfSubjects[4].marks);
    // }
    // for(int i = 0; i < 2;i++){
    //     for(int h = 0; h < num_Sub;h++){
    //         printf("%s - %d \n", ((marks_db[i].listOfSubjects[h])).subjectName, ((marks_db[i].listOfSubjects[h])).marks);
    //     }
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
        int client_State = 0; // initial State;
        int clearance = 0;
        int found = -1;

        for( ; ; ){
            if(client_State == 0){

                length = read(newSocket_fd, socket_Buffer, MAX); // reads username
                printf("%s\n", socket_Buffer); // prints username
                /* Searches entered username in the cache */
                for(int h = 0;h < login_db_size;h++){
                    if(compare_Strings(socket_Buffer, login_db[h].username) == 1){
                        found = h;
                        break;
                    }
                }

                if(found != -1){
                    /* Checks is user is instructor */
                    if(compare_Strings("instructor", socket_Buffer) == 1){
                        clearance = 1;
                    }
                    length = write(newSocket_fd, "1", 2); // validates username

                    length = read(newSocket_fd, socket_Buffer, MAX); // reads password
                    printf("%s\n", socket_Buffer); // prints password

                    if(compare_Strings(socket_Buffer, login_db[found].password) == 1){

                        
                        
                        printf("%s\n", "User Authenticated");
                        
                        if(clearance == 1){
                            length = write(newSocket_fd, "2", 2); // validates password
                            client_State = 2;
                        }
                        else{
                            length = write(newSocket_fd, "1", 2); // validates password
                            client_State = 1;
                            printf("%s %s",(*login_db[found].link).firstname," connected \n");
                        }                      
                        
                    }
                    else{
                        length = write(newSocket_fd, "0", 2);
                        printf("%s\n", "Invalid Password");                        
                        break;
                    }
                }
                else{
                    length = write(newSocket_fd, "0", 2);
                    printf("%s\n", "Invalid Username"); 
                    break;
                }

            }
            else if(client_State == 1){

                
                length = read(newSocket_fd, socket_Buffer, MAX);
                printf("%s\n", socket_Buffer);
                
                if(compare_Strings(socket_Buffer,"exit") == 1){
                    break;
                }
                else if(compare_Strings(socket_Buffer, "aggregate") == 1){
                    int totalMarks = 0;
                    for(int x = 0; x < num_Sub; x++){
                        totalMarks = totalMarks + ((*login_db[found].link).listOfSubjects[x].marks);
                    }

                    float percentage = (((float)totalMarks)/5);
                    char cent[MAX];
                    sprintf(cent, "%f", percentage);
                    char *strings[] = {
                        "Name : ",
                        (*login_db[found].link).firstname,
                        " ",
                        (*login_db[found].link).lastname,
                        "\n",
                        "   Aggregate Percentage : ",
                        cent,
                        " %"
                    };
                    addStrings(8, strings, socket_Buffer);
                    length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                }
                else if(compare_Strings(socket_Buffer, "minSub") == 1){
                    int minMarks = 100;
                    for(int a = 0; a < num_Sub; a++){
                        if((*login_db[found].link).listOfSubjects[a].marks < minMarks){
                            minMarks = (*login_db[found].link).listOfSubjects[a].marks;
                        }
                    }
                    //printf("%d\n", minMarks);
                    char *tstrings[] = {
                        "Name : ",
                        (*login_db[found].link).firstname,
                        " ",
                        (*login_db[found].link).lastname,
                        "\n",
                        "   Subjects with Minimum Marks : ",
                    };
                    char strs[MAX];
                    addStrings(6, tstrings, strs);
                    char *strings[64];
                    strings[0] = strs;
                    int curr_size = 1;
                    int withMin = 0;
                    for(int a = 0; a < num_Sub; a++){
                        if((*login_db[found].link).listOfSubjects[a].marks == minMarks){
                            if(withMin > 0){
                                strings[curr_size] = " , ";
                                curr_size++;
                            }
                            strings[curr_size] = (*login_db[found].link).listOfSubjects[a].subjectName;
                            curr_size++;
                            withMin++;
                        }
                    }
                    strings[curr_size] = "\n   Minimum Marks : ";
                    char num[len];
                    sprintf(num, "%d", minMarks);
                    curr_size++;
                    strings[curr_size] = num;
                    curr_size++;
                    //printf("%s\n", num);
                    addStrings(curr_size, strings, socket_Buffer);
                    //printf("%s\n", socket_Buffer);
                    length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);

                }
                else if(compare_Strings(socket_Buffer, "maxSub") == 1){
                    int maxMarks = 0;
                    for(int a = 0; a < num_Sub; a++){
                        if((*login_db[found].link).listOfSubjects[a].marks > maxMarks){
                            maxMarks = (*login_db[found].link).listOfSubjects[a].marks;
                        }
                    }
                    //printf("%d\n", maxMarks);
                    char *tstrings[] = {
                        "Name : ",
                        (*login_db[found].link).firstname,
                        " ",
                        (*login_db[found].link).lastname,
                        "\n",
                        "   Subjects with Maximum Marks : ",
                    };
                    char strs[MAX];
                    addStrings(6, tstrings, strs);
                    char *strings[64];
                    strings[0] = strs;
                    int curr_size = 1;
                    int withMin = 0;
                    for(int a = 0; a < num_Sub; a++){
                        if((*login_db[found].link).listOfSubjects[a].marks == maxMarks){
                            if(withMin > 0){
                                strings[curr_size] = " , ";
                                curr_size++;
                            }
                            strings[curr_size] = (*login_db[found].link).listOfSubjects[a].subjectName;
                            curr_size++;
                            withMin++;
                        }
                    }
                    strings[curr_size] = "\n   Maximum Marks : ";
                    char num[len];
                    sprintf(num, "%d", maxMarks);
                    curr_size++;
                    strings[curr_size] = num;
                    curr_size++;
                    //printf("%s\n", num);
                    addStrings(curr_size, strings, socket_Buffer);
                    //printf("%s\n", socket_Buffer);
                    length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                }
                else{
                    char command[len];
                    char arguement[len];
                    int space = 0;
                    for(int i = 0; socket_Buffer[i] != '\0'; i++){
                        if(space == 0){
                            if(socket_Buffer[i] != ' '){
                                command[i] = socket_Buffer[i];
                            }
                            else{
                                command[i] = '\0';
                                space = i+1;
                            }
                        }
                        else{
                            arguement[i-space] = socket_Buffer[i];
                            arguement[i-space+1] = '\0';
                        }
                    }
                    if(compare_Strings("marks", command) != 1){
                        length = write(newSocket_fd, "Invalid Command", getStringLength("Invalid Command") + 1);
                    }
                    else{
                        int subMatch = -1;
                        char *matchedSubject;
                        int mArks;
                        for(int a = 0; a < num_Sub; a++){
                            if(compare_Strings(arguement, ((*login_db[found].link).listOfSubjects[a].subjectName)) == 1){
                                matchedSubject = ((*login_db[found].link).listOfSubjects[a].subjectName);
                                subMatch = a;
                                mArks = ((*login_db[found].link).listOfSubjects[a].marks);
                                break;
                            }
                        }
                        if(subMatch == -1){
                            length = write(newSocket_fd, "No Such Subject", getStringLength("No Such Subject") + 1);
                        }
                        else{
                            char cent[MAX];
                            sprintf(cent, "%d", mArks);
                            char *strings[] = {
                                "Name : ",
                                (*login_db[found].link).firstname,
                                " ",
                                (*login_db[found].link).lastname,
                                "\n",
                                "   Subject Name : ",
                                matchedSubject,
                                "\n",
                                "   Marks Obtained : ",
                                cent
                            };
                            addStrings(10, strings, socket_Buffer);
                            length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                        }
                    }
                    
                }
            }

            else if(client_State == 2){

                printf("%s %s",login_db[found].firstname," connected \n");
                length = read(newSocket_fd, socket_Buffer, MAX);
                printf("%s\n", socket_Buffer);
                if(compare_Strings(socket_Buffer,"exit") == 1){
                    break;
                }
                else if(compare_Strings(socket_Buffer, "BestStudent") == 1){
                    
                    int maxM = 0;
                    int totals[login_db_size];
                    for(int i=0; i<login_db_size; i++)
                    {
                      if((login_db[i].link) != NULL){
                      int totalMarks=0;
                      for(int x = 0; x < num_Sub; x++){
                        totalMarks = totalMarks + ((*login_db[i].link).listOfSubjects[x].marks);
                      }
                      totals[i] = totalMarks;
                      if(maxM < totalMarks)
                        maxM = totalMarks;
                      }
                    }
                    int indices[login_db_size] ;
                    int counting=0;
                    for(int i=0; i<login_db_size; i++)
                    {
                      if(maxM == totals[i])
                      {
                        indices[counting] = i;
                        counting++;
                      }
                    }
                    float percentage = (((float)maxM)/5);
                    char cent[MAX];
                    sprintf(cent, "%f", percentage);
                    char *strings[64];
                    strings[0] = "The student(s) with best aggregate score is(are) : ";
                    int sizeNow = 1;
                    for(int i=0; i<counting; i++)
                    {
                      if(i != counting-1){
                        strings[sizeNow] = (*login_db[indices[i]].link).firstname;
                        sizeNow++;
                        strings[sizeNow] = " ";
                        sizeNow++;
                        strings[sizeNow] = (*login_db[indices[i]].link).lastname;
                        sizeNow++;
                        strings[sizeNow] = ", ";
                        sizeNow++;
                      }
                      else
                      {
                        strings[sizeNow] = (*login_db[indices[i]].link).firstname;
                        sizeNow++;
                        strings[sizeNow] = " ";
                        sizeNow++;
                        strings[sizeNow] = (*login_db[indices[i]].link).lastname;
                        sizeNow++;
                      }
                    }
                    strings[sizeNow] = "\n   ";
                    sizeNow++;
                    strings[sizeNow] = "Best aggregate score is : ";
                    sizeNow++;
                    strings[sizeNow] = cent;
                    sizeNow++;
                    strings[sizeNow] = "%";
                    sizeNow++;
                    addStrings(sizeNow, strings, socket_Buffer);
                    length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);


                }
                else if(compare_Strings(socket_Buffer, "WorstStudent") == 1){
                    
                    int minM = MAX;
                    int totals[login_db_size];
                    for(int i=0; i<login_db_size; i++)
                    {
                      if((login_db[i].link) != NULL){
                      int totalMarks=0;
                      for(int x = 0; x < num_Sub; x++){
                        totalMarks = totalMarks + ((*login_db[i].link).listOfSubjects[x].marks);
                      }
                      totals[i] = totalMarks;
                      if(minM > totalMarks)
                        minM = totalMarks;
                      }
                    }
                    int indices[login_db_size] ;
                    int counting=0;
                    for(int i=0; i<login_db_size; i++)
                    {
                      if(minM == totals[i])
                      {
                        indices[counting] = i;
                        counting++;
                      }
                    }
                    float percentage = (((float)minM)/5);
                    char cent[MAX];
                    sprintf(cent, "%f", percentage);
                    char *strings[64];
                    strings[0] = "The student(s) with worst aggregate score is(are) : ";
                    int sizeNow = 1;
                    for(int i=0; i<counting; i++)
                    {
                      if(i != counting-1){
                        strings[sizeNow] = (*login_db[indices[i]].link).firstname;
                        sizeNow++;
                        strings[sizeNow] = " ";
                        sizeNow++;
                        strings[sizeNow] = (*login_db[indices[i]].link).lastname;
                        sizeNow++;
                        strings[sizeNow] = ", ";
                        sizeNow++;
                      }
                      else
                      {
                        strings[sizeNow] = (*login_db[indices[i]].link).firstname;
                        sizeNow++;
                        strings[sizeNow] = " ";
                        sizeNow++;
                        strings[sizeNow] = (*login_db[indices[i]].link).lastname;
                        sizeNow++;
                      }
                    }
                    strings[sizeNow] = "\n   ";
                    sizeNow++;
                    strings[sizeNow] = "Worst aggregate score is : ";
                    sizeNow++;
                    strings[sizeNow] = cent;
                    sizeNow++;
                    strings[sizeNow] = "%";
                    sizeNow++;
                    addStrings(sizeNow, strings, socket_Buffer);
                    length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);


              }
              else
              {
                    char command[len];
                    char argument[len];
                    int space = 0;
                    for(int i = 0; socket_Buffer[i] != '\0'; i++)
                    {
                        if(space == 0){
                            if(socket_Buffer[i] != ' '){
                                command[i] = socket_Buffer[i];
                            }
                            else{
                                command[i] = '\0';
                                space = i+1;
                            }
                        }
                        else{
                            argument[i-space] = socket_Buffer[i];
                            argument[i-space+1] = '\0';
                        }
                    }

                    if(compare_Strings("marks", command) == 1)
                    {
                      int subIndex = -1;
                      int counting = 0;
                      char *strings[MAX];
                      int sizeNow = 0;
                      int exists = 0;
                      char mar[MAX];
                      int pointer = 0;

                      for(int i=0; i< login_db_size; i++)
                      {
                        if((login_db[i].link) != NULL)
                        {
                          counting++;
                          if(counting == 1)
                          {
                            for(int j=0; j<num_Sub; j++)
                            {
                              if(compare_Strings(argument, ((*login_db[i].link).listOfSubjects[j].subjectName)) == 1)
                              {
                                exists = 1;
                                subIndex = j;
                                strings[sizeNow] = (*login_db[i].link).firstname;
                                sizeNow++;
                                // printf("%s\n", (*login_db[i].link).firstname);
                                strings[sizeNow] = " ";
                                sizeNow++;
                                strings[sizeNow] = (*login_db[i].link).lastname;
                                sizeNow++;
                                strings[sizeNow] = " : ";
                                sizeNow++;
                              
                                int maR = (*login_db[i].link).listOfSubjects[j].marks;
                                // printf("%d", maR);
                                sprintf(mar + pointer, "%d", maR);
                                // pointer + = 4;
                                // printf("%s\n", mar);
                                strings[sizeNow] = mar + pointer;
                                pointer = pointer + 4;
                                sizeNow++;
                                strings[sizeNow] = "\n   ";
                                sizeNow++;
                                
                              }
                            }
                          }
                          else
                          {
                                strings[sizeNow] = (*login_db[i].link).firstname;
                                sizeNow++;
                                strings[sizeNow] = " ";
                                sizeNow++;
                                strings[sizeNow] = (*login_db[i].link).lastname;
                                sizeNow++;
                                strings[sizeNow] = " : ";
                                sizeNow++;
                                
                                int maR = (*login_db[i].link).listOfSubjects[subIndex].marks;
                                sprintf(mar + pointer, "%d", maR);
                                strings[sizeNow] = mar + pointer;
                                pointer = pointer + 4;
                                sizeNow++;
                                strings[sizeNow] = "\n   ";
                                sizeNow++; 
                          }

                        }
                      }
                      if(exists == 1){
                      addStrings(sizeNow, strings, socket_Buffer);
                      length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                      }
                      else
                      {
                        length = write(newSocket_fd, "No Such Subject", getStringLength("No Such Subject") + 1);

                      }
                    }
                    else if(compare_Strings("classAvg", command) == 1)
                    {
                      int subIndex = -1;
                      int counting = 0;
                      int exists = 0;
                      int totalM = 0;

                      for(int i=0; i< login_db_size; i++)
                      {
                        if((login_db[i].link) != NULL)
                        {
                          counting++;
                          if(counting == 1)
                          {
                            for(int j=0; j<num_Sub; j++)
                            {
                              if(compare_Strings(argument, ((*login_db[i].link).listOfSubjects[j].subjectName)) == 1)
                              {
                                exists = 1;
                                subIndex = j;
                                totalM = totalM + (*login_db[i].link).listOfSubjects[j].marks;
                                
                              }
                            }
                          }
                          else
                          {
                                totalM = totalM + (*login_db[i].link).listOfSubjects[subIndex].marks;
                          }

                        }
                      }
                      char finalper[MAX];
                      float avg = (float)totalM/(login_db_size-1);
                      sprintf(finalper, "%f", avg);
                      char *strings[3] = {"The class average for this subject is : ", finalper, "\n"};

                      if(exists == 1){
                      addStrings(3, strings, socket_Buffer);
                      length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                      }
                      else
                      {
                        length = write(newSocket_fd, "No Such Subject", getStringLength("No Such Subject") + 1);

                      }
                    }
                    else if(compare_Strings("numPassed", command) == 1)
                    {
                      int subIndex = -1;
                      int counting = 0;
                      int exists = 0;
                      int passed = 0;

                      for(int i=0; i< login_db_size; i++)
                      {
                        if((login_db[i].link) != NULL)
                        {
                          counting++;
                          if(counting == 1)
                          {
                            for(int j=0; j<num_Sub; j++)
                            {
                              if(compare_Strings(argument, ((*login_db[i].link).listOfSubjects[j].subjectName)) == 1)
                              {
                                exists = 1;
                                subIndex = j;
                                if((*login_db[i].link).listOfSubjects[j].marks >= 33)
                                  passed++;
                                
                              }
                            }
                          }
                          else
                          {
                            if((*login_db[i].link).listOfSubjects[subIndex].marks >= 33)
                                  passed++;
                          }

                        }
                      }
                      char finalpass[MAX];
                      sprintf(finalpass, "%d", passed);
                      char *strings[2] = {"The number of students who passed this subject is : ", finalpass};

                      if(exists == 1){
                      addStrings(2, strings, socket_Buffer);
                      length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                      }
                      else
                      {
                        length = write(newSocket_fd, "No Such Subject", getStringLength("No Such Subject") + 1);

                      }
                    }
                    else if(compare_Strings("numFailed", command) == 1)
                    {
                      int subIndex = -1;
                      int counting = 0;
                      int exists = 0;
                      int passed = 0;

                      for(int i=0; i< login_db_size; i++)
                      {
                        if((login_db[i].link) != NULL)
                        {
                          counting++;
                          if(counting == 1)
                          {
                            for(int j=0; j<num_Sub; j++)
                            {
                              if(compare_Strings(argument, ((*login_db[i].link).listOfSubjects[j].subjectName)) == 1)
                              {
                                exists = 1;
                                subIndex = j;
                                if((*login_db[i].link).listOfSubjects[j].marks < 33)
                                  passed++;
                                
                              }
                            }
                          }
                          else
                          {
                            if((*login_db[i].link).listOfSubjects[subIndex].marks < 33)
                                  passed++;
                          }

                        }
                      }
                      char finalpass[MAX];
                      sprintf(finalpass, "%d", passed);
                      char *strings[2] = {"The number of students who failed this subject is : ", finalpass};

                      if(exists == 1){
                      addStrings(2, strings, socket_Buffer);
                      length = write(newSocket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                      }
                      else
                      {
                        length = write(newSocket_fd, "No Such Subject", getStringLength("No Such Subject") + 1);

                      }
                    }

            }
            }

        }

        close(newSocket_fd);
        printf("%s\n", "Client Disconnected");

        
    }    
    close(socket_fd);
    return 0;
}
