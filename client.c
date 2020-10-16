#include<stdio.h>
/* Prefix to every Output */
#define SALUTATION "commandLine :$-> "
/* Input Buffer Size */
#define MAX 1024
/* Color Codes */
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
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
    printf("---To connect to the Database Server, type: connect [host name]----\n");
    printf("\n");
    printf("-----------------To exit the Program, type: exit-------------------\n");
    printf("\n");
}
/* Menu Display when Client is an authenticated Student */
void showStudentMenu(){
    printf("\n");
    printf("-------------------------COMMANDS AVAILABLE-----------------------\n");
    printf("------To know more about a command type : help [command name]-----\n");
    char *commandList = {
        "connect"
    };
}
/* Menu Display when Client is an authenticated Instructor */
void showInstructorMenu(){
    printf("\n");
    printf("-------------------------COMMANDS AVAILABLE-----------------------\n");
    printf("------To know more about a command type : help [command name]-----\n");
    char *commandList = {
        "connect"
    };
}
/* Clear Terminal Input Buffer */
void clearBuffer(){
    char c;
    c = getchar();
    while(c != '\n'){
        c = getchar();
    }
}
/* Check Validity of entered command and Return valid command codes */
int check_Command_Validity(int program_State, int curr_Buffer_Size, char input_Buffer[]){
    if(program_State == 0){
        if((curr_Buffer_Size < 4) || ((curr_Buffer_Size > 4) && (curr_Buffer_Size <= 8))){
            return -1;
        }
        else if(curr_Buffer_Size == 4){
            char exit[] = {"exit"};
            for(int h = 0; h < curr_Buffer_Size; h++){
                if(input_Buffer[h] != exit[h]){
                    return -1;
                }
            }
            return 1;
        }
        else{
            char connect[] = {"connect "};
            for(int h = 0; h < 8; h++){
                if(input_Buffer[h] != connect[h]){
                    return -1;
                }
            }
            return 2;
        }
    }
    return 0;
}

int main(){
    /* Initial Display */    
    showBanner();
    showMenu();
    printf(CYAN SALUTATION RESET);

    /* State variable denotes whether client is student or instructor ( if logged in yet )  or  if the Program has Just begun */
    /* The commands available to execute depend on the state of the Program */
    int program_State = 0; // intial state
    char char_Input;
    char input_Buffer[MAX] = {'\0'}; // Buffer Array
    int curr_Buffer_Size = 0; // Stores size of entered command string

    while(char_Input = getchar()){
        curr_Buffer_Size = 0;
        if(program_State == 0){            
            while(char_Input != '\n'){
                if(curr_Buffer_Size >= MAX){
                    printf("Buffer Limit Exceeded");
                    clearBuffer();
                    curr_Buffer_Size = 0;
                    break;
                }
                else{
                    input_Buffer[curr_Buffer_Size] = (char)char_Input;
                    curr_Buffer_Size++;
                    char_Input = getchar();
                }
            }
            if(curr_Buffer_Size > 0){
                int validity = check_Command_Validity(program_State, curr_Buffer_Size, input_Buffer);
                if(validity == 1){
                    break;
                }
                else if(validity == -1){
                    printf("Invalid Command");
                }
                else{
                    printf("%.*s",curr_Buffer_Size, input_Buffer);
                }
                printf("\n");
            }           
            printf(CYAN SALUTATION RESET);
        }
    }
    return 0;
}