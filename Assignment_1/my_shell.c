#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

// Terminal Command executions

void execute_command(char** tokens){

	if (strcmp(tokens[0], "cd") == 0){
		char buf[1024];
		// char* gdir, dir, to;

		getcwd(buf, sizeof(buf));

		strcat(buf, "/");

		strcat(buf, tokens[1]);

		int ret = chdir(buf);
		// printf("55555 %i\n", ret);
		if (ret != 0){
			printf("Unable to change Directory\n");
		}
	}

	
	pid_t pid;
	pid = fork();
	if (pid < 0){
		perror("Unable to form a child. \n");
	}
	else if (pid == 0){

		int flag = execvp(tokens[0], tokens);
					// int flag = execlp(tokens[i],tokens[i], NULL);
		// printf("Flag is here \n");
		if (flag < 0){
			perror("Shell: Incorrect command.");
		}
	}
	else{
		wait(NULL);
	}

}



int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	char* echo = "echo";
	char* pwd = "pwd";
	char* ls = "ls";
	char* cat = "cat";
	char* sleep = "sleep";
	char* ps = "ps";
	char* commands[] = {echo, pwd, ls, sleep, cat, ps};

	// printf("Flag is here: %s \n", echo);

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		// printf("Command entered: %s \n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		execute_command(tokens);
   
       //do whatever you want with the commands, here we just print them

		// for(i=0;tokens[i]!=NULL;i++){
		// 	// printf("found token %s \n", tokens[i]);

		// 	// int is_command = 0;

		// 	// for (int j = 0; j < 3; j++){
		// 	// 	if (strcmp(commands[j], tokens[i]) == 0){
		// 	// 		is_command = 1;
		// 	// 		break;
		// 	// 	}
		// 	// }

		// 	// if (is_command == 0)
		// 	// 	continue;

		// 	pid_t pid;

		// 	pid = fork();
		// 	if (pid < 0){
		// 		perror("Unable to form a child. \n");
		// 	}
		// 	else if (pid == 0){
		// 		int flag = execvp(tokens[i], tokens);
		// 			// int flag = execlp(tokens[i],tokens[i], NULL);
		// 		printf("Flag is here \n");
		// 		if (flag < 0){
		// 			perror("Could not execute this command. Try Again. \n");
		// 		}
		// 	}
		// 	else{
		// 		wait(NULL);
		// 	}

		// }
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
