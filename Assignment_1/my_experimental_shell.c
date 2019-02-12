#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>

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

char **tokenize_experimental(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == '\n' || readChar == '\t' || readChar == '&'){
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

	else{
		pid_t pid;
		int status;
		pid = fork();
		if (pid < 0){
			fprintf(stderr, "Unable to fork a child. \n");
		}
		else if (pid == 0){
			// setpgid(0, 0);
			int flag = execvp(tokens[0], tokens);
			// printf("Command Executed in Background\n");
					// int flag = execlp(tokens[i],tokens[i], NULL);
		// printf("Flag is here \n");
			if (flag < 0){
				fprintf(stderr, "Shell: Incorrect command.\n");
				exit(flag);
			}
		}
		else{
			wait(NULL);
			// printf("PChild Process (PID: %i) to be executed in background \n", pid);
			// waitpid(pid, &status, WNOHANG);
			// return;
		}
	}

}

void execute_command_in_background(char** tokens){
	pid_t pid;
	int status;
	pid = fork();

	if (pid<0)
		fprintf(stderr, "Unable to fork a child. \n");

	else if(pid == 0){
		// setpgid(0, 0);

		int flag = execvp(tokens[0], tokens);
		if (flag < 0){
		fprintf(stderr, "Shell: Incorrect command.\n");
		}
	}

	else{

		int parent_terminate = 0;
		pid_t wait_id;

		while(parent_terminate == 0){
			wait_id = waitpid(pid, &status, WNOHANG);

			if (wait_id == -1)
				fprintf(stderr, "Wait Error");
			else if (wait_id == 0){
					// Child still running
				sleep(1);
			}
			else{
				parent_terminate = 1;
				if (WIFEXITED(status))
					printf("Background Process terminated \n");
				else
					printf("Error in Background Process execution\n");
			}
		}


	}
}

void sigint_handler(int sig){
   printf("Type exit to close the shell. \n");
}

int main(int argc, char* argv[]) {

	signal(SIGINT, sigint_handler);

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;  
	char  **real_tokens;             
	int i, j, k;

	char* double_and = "&&";
	char* single_and = "&";
	char* triple_and = "&&&";

	


	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		int is_double_and = 0, is_single_and = 0, is_triple_and = 0 ;
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
		// printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them

		if (strcmp(tokens[0], "exit") == 0){
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		exit(0);
		}


		for(i = 0; tokens[i]!=NULL; i++)
		{
			if (strcmp(tokens[i], double_and) == 0)
				is_double_and = 1;
			else if (strcmp(tokens[i], single_and) == 0)
				is_single_and = 1;
			else if (strcmp(tokens[i], triple_and) == 0)
				is_triple_and += 1;

		}

		if (is_single_and == 1){
			printf("Found Single and");
			tokens = tokenize_experimental(line);

			for(i = 0; tokens[i]!=NULL;i++){
				real_tokens = tokenize(tokens[i]);
				execute_command_in_background(real_tokens);

				for (j=0; real_tokens[j]!=NULL;j++){
				free(real_tokens[j]);
				}
				free(real_tokens);
			}

		}

		else if (is_triple_and != 0)
		{
			printf("Found triple and\n");
			// tokens = tokenize_experimental(line);
			int num_processes = is_triple_and + 1;
			pid_t pid;
			char* tokens_split = strtok(line, "&&&");
			while(tokens_split!=NULL){
				tokens = tokenize(tokens_split);
				pid = fork();

				if (pid == 0){
					if (strcmp(tokens[0], "cd") == 0){
						char buf[1024];
						getcwd(buf, sizeof(buf));
						strcat(buf, "/");
						strcat(buf, tokens[1]);
						int ret = chdir(buf);
						if (ret != 0){
							printf("Unable to change Directory\n");
						}
					}

					else{

						int flag = execvp(tokens[0], tokens);

						if (flag < 0){
							fprintf(stderr, "Shell: Incorrect command.\n");
							exit(flag);
						}
					}
				}

				tokens_split = strtok(NULL, "&&&");
			}

			for (i = 0; i<num_processes; i++){
				wait(NULL);
			}

		}

		// if (is_triple_and == 1){

		// }
		else{
		tokens = tokenize_experimental(line);

		for(i=0;tokens[i]!=NULL;i++){
			// printf("found token %c of length (%lu) \n\n\n", tokens[i][0], strlen(tokens[i]));
			real_tokens = tokenize(tokens[i]);
			for(k=0;real_tokens[k]!=NULL;k++){
			// printf("Found Real Token: %s \n", real_tokens[k]);
			}
			execute_command(real_tokens);

			// printf("Freeing the real token !!!!!\n");
			for (j=0; real_tokens[j]!=NULL;j++){
				// printf("Real Token = %s  ", real_tokens[j]);
				free(real_tokens[j]);
				// printf("Freed!!!!!!!!!!!!\n");
			}
			// printf("Freeing Array.   ");
			free(real_tokens);
			// printf("Freed Array !!!!!\n");
			// printf("found token %s\n", tokens[i]);
		}
	}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
