#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
/* Prefix to every Output */
#define SALUTATION "commandLine :$-> "
/* Input Buffer Size */
#define MAX 1024
#define len 256
/* Color Codes */
// #define CYAN    "\x1b[36m"
// #define RESET   "\x1b[0m"
/* Heading Banner to be displayed at the start of the Program */
void showBanner(){
    printf("\n");
    printf("******************************************************************\n");
    printf("***                                                            ***\n");
    printf("***                  STUDENT DATABASE : CLI                    ***\n");
    printf("***                                                            ***\n");
    printf("******************************************************************\n");
    printf("\n");
}
/* Menu Display when Program is in State 0 */
void showMenu(){
    printf("\n");
    printf("---To connect to the Database Server, type: connect [host name] [port number]----\n");
    printf("\n");
    printf("-----------------To exit the Program, type: exit-------------------\n");
    printf("\n");
}
/* Menu Display when Client is an authenticated Student */
void showStudentMenu(){
    printf("\n");
    printf("*******************************************************************\n");
    printf("*                                                                 *\n");
    printf("*                      COMMANDS AVAILABLE                         *\n");
    printf("*  ______________________    ___________    ________    ________  *\n");
    printf("* |                      |  |           |  |        |  |        | *\n");
    printf("* | marks [subject name] |--| aggregate |--| minSub |--| maxSub | *\n");
    printf("* |______________________|  |___________|  |________|  |________| *\n");
    printf("*                                                                 *\n");
    printf("*******************************************************************\n");
    printf("\n");
    char *commandList[] = {
        "marks [subject name]", "aggregate", "minSubject", "maxSubject"
    };
}
/* Menu Display when Client is an authenticated Instructor */
void showInstructorMenu(){
    printf("\n");
    printf("*******************************************************************\n");
    printf("*    WELCOME, ALBUS!!                                             *\n");
    printf("*                                                                 *\n");
    printf("*                      COMMANDS AVAILABLE                         *\n");
    printf("*  ______________________             _________________________   *\n");
    printf("* |                      |           |                         |  *\n");
    printf("* | marks [subject name] |     --    | classAvg [subject name] |  *\n");
    printf("* |______________________|           |_________________________|  *\n");
    printf("*  ______________________             _________________________   *\n");
    printf("* |                      |           |                         |  *\n");
    printf("* | numPassed [subject]  |     --    |   numFailed [subject]   |  *\n");
    printf("* |______________________|           |_________________________|  *\n");
    printf("*  ______________________             _________________________   *\n");
    printf("* |                      |           |                         |  *\n");
    printf("* |      BestStudent     |     --    |       WorstStudent      |  *\n");
    printf("* |______________________|           |_________________________|  *\n");
    printf("*    _________________________________________________________    *\n");
    printf("*   |                                                         |   *\n");
    printf("* --| updateMarks [student name (full)] [subject] [new marks] |-- *\n");
    printf("*   |_________________________________________________________|   *\n");
    printf("*                                                                 *\n");
    printf("*******************************************************************\n");
    printf("\n");
}
/* Clear Terminal Input Buffer */
void clearBuffer(){
    char c;
    c = getchar();
    while(c != '\n'){
        c = getchar();
    }
}

/* returns length of string from character array by counting untill null character \0 */
int getStringLength(char *str){
    int length = 0;
    while(str[length] != '\0' && length < MAX){
        length++;
    }
    return length;
}

/* Check Validity of entered command and Return valid command codes */
int check_Command_Validity(int program_State, int curr_Buffer_Size, char *command_Buffer){
    if(program_State == 0){
        if((curr_Buffer_Size != 4) && (curr_Buffer_Size != 7)){
            return -1;
        }
        else if(curr_Buffer_Size == 4){
            char exit[] = {"exit"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != exit[h]){
                    return -1;
                }
            }
            return 1;
        }
        else{
            char connect[] = {"connect"};
            for(int h = 0; h < 8; h++){
                if(command_Buffer[h] != connect[h]){
                    return -1;
                }
            }
            return 2;
        }
    }
    else if(program_State == 1){
        if(curr_Buffer_Size == 4){
            char exit[] = {"exit"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != exit[h]){
                    return -1;
                }
            }
            return 1;
        }
        else if(curr_Buffer_Size == 5){
            char marks[] = {"marks"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != marks[h]){
                    return -1;
                }
            }
            return 2;
        }
        else if(curr_Buffer_Size == 9){
            char aggregate[] = {"aggregate"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != aggregate[h]){
                    return -1;
                }
            }
            return 3;
        }
        else if(curr_Buffer_Size == 6){
            char minSub[] = {"minSub"};
            char maxSub[] = {"maxSub"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != minSub[h]){
                    for(int g = 0; g < curr_Buffer_Size; g++){
                        if(command_Buffer[g] != maxSub[g]){
                            return -1;
                        }
                    }
                    return 5;
                }
            }
            return 4;
        }
        else{
            return -1;
        }
    }
    else if(program_State == 2){
        if(curr_Buffer_Size == 4){
            char exit[] = {"exit"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != exit[h]){
                    return -1;
                }
            }
            return 1;
        }
        else if(curr_Buffer_Size == 5){
            char marks[] = {"marks"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != marks[h]){
                    return -1;
                }
            }
            return 2;
        }
        else if(curr_Buffer_Size == 8){
            char classAvg[] = {"classAvg"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != classAvg[h]){
                    return -1;
                }
            }
            return 3;
        }
        else if(curr_Buffer_Size == 9){
            char numP[] = {"numPassed"};
            char numF[] = {"numFailed"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != numP[h]){
                    for(int g = 0; g < curr_Buffer_Size; g++){
                        if(command_Buffer[g] != numF[g]){
                            return -1;
                        }
                    }
                    return 5;
                }
            }
            return 4;
        }
        else if(curr_Buffer_Size == 11){
            char best[] = {"BestStudent"};
            char updt[] = {"updateMarks"};
            int counting = 0;
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != best[h]){
                    break;
                }
                else
                {
                  counting++;
                }
            }
            if(counting == 11)
              return 6;
            counting = 0;
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != updt[h]){
                    break;
                }
                else
                {
                  counting++;
                }
            }
            if(counting == 11)
              return 8;
            
            return -1;
        }
        else if(curr_Buffer_Size == 12){
            char worst[] = {"WorstStudent"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(command_Buffer[h] != worst[h]){
                    return -1;
                }
            }
            return 7;
        }
        
        else{
            return -1;
        }
    }
    return 0;
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

int main(){
    /* Initial Display */    
    showBanner();
    showMenu();
    printf(SALUTATION);

    /* State variable denotes whether client is student or instructor ( if logged in yet )  or  if the Program has Just begun */
    /* The commands available to execute depend on the state of the Program */
    int program_State = 0; // intial state
    char char_Input;
    char command_Buffer[MAX] = {'\0'}; // Buffer Array
    int curr_Buffer_Size = 0; // Stores size of entered command string
    int socket_fd, port_Number, length;
    struct sockaddr_in server_Address;
    struct hostent *server;
    char socket_Buffer[MAX];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        fprintf(stderr, "ERROR opening socket\n");
        return 0;
    }   
    while(scanf("%s", command_Buffer)){
      if(program_State == 0){
          close(socket_fd);
          socket_fd = socket(AF_INET, SOCK_STREAM, 0);
          if(socket_fd < 0){
              fprintf(stderr, "ERROR opening socket\n");
              return 0;
          }
          int check = check_Command_Validity(program_State, getStringLength(command_Buffer), command_Buffer);
          if(check == -1){
              printf("Invalid Command\n");
              clearBuffer();
          }
          else if(check == 1){
              break;
          }
          else{
              char_Input = getchar();
              char hostname[MAX] = {'\0'};
              char port[MAX] = {'\0'};
              int space = -1;
              while(char_Input != '\n'){
                  if(char_Input != ' ' && space <= 0){
                      space = 0;
                      hostname[curr_Buffer_Size] = char_Input;
                      char_Input = getchar();
                      curr_Buffer_Size++;
                  }
                  else if(char_Input != ' ' && space == 1){
                      port[curr_Buffer_Size] = char_Input;
                      char_Input = getchar();
                      curr_Buffer_Size++;
                  }
                  else if(char_Input == ' ' && space == 0){
                      space = 1;
                      char_Input = getchar();
                      curr_Buffer_Size = 0;
                  }
                  else{
                      char_Input = getchar();
                  }
              }
              curr_Buffer_Size = 0;
              if(getStringLength(hostname) == 0 || getStringLength(port) == 0){
                  printf("Syntax Error\n");
              }
              else{
                    port_Number = atoi(port);
                    server = gethostbyname(hostname);
                    if (server == NULL) {
                        printf("ERROR, no such host\n");
                        printf(SALUTATION); 
                        continue;
                    }
                    bzero((char *) &server_Address, sizeof(server_Address));
                    server_Address.sin_family = AF_INET;
                    bcopy((char *) server->h_addr, (char *) &server_Address.sin_addr.s_addr, server->h_length);
                    server_Address.sin_port = htons(port_Number);
                    if(connect(socket_fd, (struct sockaddr *) &server_Address, sizeof(server_Address)) < 0){
                        printf("ERROR connecting to host\n");
                        printf(SALUTATION); 
                        continue;
                    }
                    printf("[username] : ");
                    scanf("%s", socket_Buffer);
                    clearBuffer();
                    length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                    if(length < 0){printf("%s\n%s", "ERROR in Writing", SALUTATION); continue;}
                    length = read(socket_fd, socket_Buffer, MAX);
                    if(length < 0){printf("%s\n%s", "ERROR in Reading", SALUTATION); continue;}
                    if(socket_Buffer[0] == '0'){
                        printf("ERROR Invalid Username\n");
                        printf(SALUTATION);
                        continue;
                    }
                    else{
                        printf("[password] : ");
                        scanf("%s", socket_Buffer);
                        clearBuffer();
                        length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                        if(length < 0){printf("%s\n%s", "ERROR in Writing", SALUTATION); continue;}
                        length = read(socket_fd, socket_Buffer, MAX);
                        if(length < 0){printf("%s\n%s", "ERROR in Reading", SALUTATION); continue;}
                        if(socket_Buffer[0] == '0'){
                            printf("ERROR Invalid Password\n");
                            printf(SALUTATION);
                            continue;
                        }
                        else if(socket_Buffer[0] == '1'){
                            program_State = 1;
                            showStudentMenu();
                        }
                        else{
                            program_State = 2;
                            showInstructorMenu();
                        }
                    }
                }
          }
      }
    
      else if(program_State == 1){
          int check = check_Command_Validity(program_State, getStringLength(command_Buffer), command_Buffer);
          if(check == -1){
              printf("Invalid Command\n");
              clearBuffer();
          }
          else if(check == 1){
              length = write(socket_fd, "exit", getStringLength("exit") + 1);
              break;
          }
          else if(check == 2){
              char argument[MAX] = {'\0'};
              char c = getchar();
              int curr_size = 0;
              while(c != '\n'){
                  argument[curr_size] = (char)c;
                  curr_size++;
                  c = getchar();
              }
              argument[curr_size] = '\0';
              if(getStringLength(argument) == 0){
                  printf("Syntax Error\n");
              }
              else{
                  char *strings[2] = {
                      command_Buffer,
                      argument
                  };
                  addStrings(2, strings, socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
              
          }
          else if(check == 3){
              char c = getchar();
              int valid = 1;
              while(c != '\n'){
                  if(c != ' '){
                      valid = 0;
                  }
                  c = getchar();
              }
              if(valid != 1){
                  printf("Syntax Error\n");
              }
              else{
                  length = write(socket_fd, command_Buffer, getStringLength(command_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 4){
              char c = getchar();
              int valid = 1;
              while(c != '\n'){
                  if(c != ' '){
                      valid = 0;
                  }
                  c = getchar();
              }
              if(valid != 1){
                  printf("Syntax Error\n");
              }
              else{
                  length = write(socket_fd, command_Buffer, getStringLength(command_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 5){
              char c = getchar();
              int valid = 1;
              while(c != '\n'){
                  if(c != ' '){
                      valid = 0;
                  }
                  c = getchar();
              }
              if(valid != 1){
                  printf("Syntax Error\n");
              }
              else{
                  length = write(socket_fd, command_Buffer, getStringLength(command_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else{
              ;
          }

      }

     else if(program_State == 2){
          int check = check_Command_Validity(program_State, getStringLength(command_Buffer), command_Buffer);
          if(check == -1){
              printf("Invalid Command\n");
              clearBuffer();
          }
          else if(check == 1){
              length = write(socket_fd, "exit", getStringLength("exit") + 1);
              break;
          }
          else if(check == 2){
              char argument[MAX] = {'\0'};
              char c = getchar();
              int curr_size = 0;
              while(c != '\n'){
                  argument[curr_size] = (char)c;
                  curr_size++;
                  c = getchar();
              }
              argument[curr_size] = '\0';
              if(getStringLength(argument) == 0){
                  printf("Syntax Error\n");
              }
              else{
                  char *strings[2] = {
                      command_Buffer,
                      argument
                  };
                  addStrings(2, strings, socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 3){
              char argument[MAX] = {'\0'};
              char c = getchar();
              int curr_size = 0;
              while(c != '\n'){
                  argument[curr_size] = (char)c;
                  curr_size++;
                  c = getchar();
              }
              argument[curr_size] = '\0';
              if(getStringLength(argument) == 0){
                  printf("Syntax Error\n");
              }
              else{
                  char *strings[2] = {
                      command_Buffer,
                      argument
                  };
                  addStrings(2, strings, socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 4){
              char argument[MAX] = {'\0'};
              char c = getchar();
              int curr_size = 0;
              while(c != '\n'){
                  argument[curr_size] = (char)c;
                  curr_size++;
                  c = getchar();
              }
              argument[curr_size] = '\0';
              if(getStringLength(argument) == 0){
                  printf("Syntax Error\n");
              }
              else{
                  char *strings[2] = {
                      command_Buffer,
                      argument
                  };
                  addStrings(2, strings, socket_Buffer);
               //   printf("%s\n", socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 5){
              char argument[MAX] = {'\0'};
              char c = getchar();
              int curr_size = 0;
              while(c != '\n'){
                  argument[curr_size] = (char)c;
                  curr_size++;
                  c = getchar();
              }
              argument[curr_size] = '\0';
              if(getStringLength(argument) == 0){
                  printf("Syntax Error\n");
              }
              else{
                  char *strings[2] = {
                      command_Buffer,
                      argument
                  };
                  addStrings(2, strings, socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 6){
              char c = getchar();
              int valid = 1;
              while(c != '\n'){
                  if(c != ' '){
                      valid = 0;
                  }
                  c = getchar();
              }
              if(valid != 1){
                  printf("Syntax Error\n");
              }
              else{
                  length = write(socket_fd, command_Buffer, getStringLength(command_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 7){
              char c = getchar();
              int valid = 1;
              while(c != '\n'){
                  if(c != ' '){
                      valid = 0;
                  }
                  c = getchar();
              }
              if(valid != 1){
                  printf("Syntax Error\n");
              }
              else{
                  length = write(socket_fd, command_Buffer, getStringLength(command_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else if(check == 8)
          {
              char argument1[MAX] = {'\0'};
              char c = getchar();
              if(c != '\n'){
                  c = getchar();
              }              
              int curr_size1 = 0;
              while(c != ' ' && c != '\n'){
                  argument1[curr_size1] = (char)c;
                  curr_size1++;
                  c = getchar();
              }
              argument1[curr_size1] = '\0';
              
              char argument2[MAX] = {'\0'};
              
              if(c != '\n'){
                  c = getchar();
              }
              int curr_size2 = 0;
              while(c != ' ' && c != '\n'){
                  argument2[curr_size2] = (char)c;
                  curr_size2++;
                  c = getchar();
              }
              argument2[curr_size2] = '\0';
              
              char argument3[MAX] = {'\0'};
              if(c != '\n'){
                  c = getchar();
              }
              int curr_size3 = 0;
              while(c != ' ' && c != '\n'){
                  argument3[curr_size3] = (char)c;
                  curr_size3++;
                  c = getchar();
              }
              argument3[curr_size3] = '\0';

              char argument4[MAX] = {'\0'};
              if(c != '\n'){
                  c = getchar();
              }
              int curr_size4 = 0;
              while(c != '\n'){
                  argument4[curr_size4] = (char)c;
                  curr_size4++;
                  c = getchar();
              }
              argument4[curr_size4] = '\0';
              // printf("%s\n", argument1);
              // printf("%s\n", argument2);
              // printf("%s\n", argument3);
              
              if(getStringLength(argument1) == 0){
                  printf("Syntax Error\n");
              }
              else if(getStringLength(argument2) == 0){
                  printf("Syntax Error\n");
              }
              else if(getStringLength(argument3) == 0){
                  printf("Syntax Error\n");
              }
             else if(getStringLength(argument4) == 0){
                  printf("Syntax Error\n");
              }
              else
              {
                 char *strings[9] = {
                      command_Buffer, " ",
                      argument1, " ",
                      argument2, " ",
                      argument3, " ",
                      argument4
                  };
                  addStrings(9, strings, socket_Buffer);
                  // printf("%s\n", socket_Buffer);
                  length = write(socket_fd, socket_Buffer, getStringLength(socket_Buffer) + 1);
                  length = read(socket_fd, socket_Buffer, MAX);
                  printf("\n   ");
                  printf("%s\n",socket_Buffer);
                  printf("\n");
              }
          }
          else{

          }

      }

       printf(SALUTATION);  
    }
    close(socket_fd);
    return 0;
}
