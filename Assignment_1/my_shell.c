#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <signal.h>

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

void execute_command(char** tokens, int command_index, int start_token, int if_background){

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
		pid = fork();
		if (pid < 0){
			perror("Unable to form a child. \n");
		}
		else if (pid == 0){

			int flag = execvp(tokens[command_index], tokens + start_token);
					// int flag = execlp(tokens[i],tokens[i], NULL);
		// printf("Flag is here \n");
			if (flag < 0){
				perror("Shell: Incorrect command.");
			}
		}
		else{
			if (if_background == 0)
				wait(NULL);
			else
				printf("To be executed in background \n");
		}
	}

}



int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	char* double_and = "&&";
	char* single_and = "&";

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

		int if_execute = 1;
   		
       //do whatever you want with the commands, here we just print them
		int double_and_indices[32];
		int double_and_flag = 0;
		int max_len = 0;
		int if_background = 0;

		for(i=0;tokens[i]!=NULL;i++){
			// printf("found token %s \n", tokens[i]);

			// int is_command = 0;

			// for (int j = 0; j < 3; j++){
			// 	if (strcmp(commands[j], tokens[i]) == 0){
			// 		is_command = 1;
			// 		break;
			// 	}
			// }

			// if (is_command == 0)
			// 	continue;

			// if (strcmp(tokens[i], "&&") == 0)
			// 	if_execute = 0;

			// if (if_execute == 1){

			// }
			max_len = max_len + 1;
			if (strcmp(tokens[i], double_and) == 0){
				printf("Found Something\n");
				double_and_indices[double_and_flag] = i;
				double_and_flag = double_and_flag + 1;
			}
			if (strcmp(tokens[i], single_and) == 0)
				if_background = 1;

	}
	if (double_and_flag == 0){
		execute_command(tokens, 0, 0, if_background);
	}
	else{
		// double_and_indices[double_and_flag] = max_len;
		// double_and_flag = double_and_flag + 1;
		// int j, k, l, split_token_length;
		// for(i = 0; i<double_and_flag; i++){

		// 	if (i == 0)
		// 		split_token_length = double_and_indices[i];
		// 	else
		// 		split_token_length = double_and_indices[i] - double_and_indices[i-1] - 1;

		// 	char* split_tokens[64];
		// 	k = 0;
		// 	if (i == 0)
		// 		j = 0;
		// 	else
		// 		j = double_and_indices[i-1]+1;
		// 	while(j < double_and_indices[i]){
		// 		split_tokens[k] = tokens[j];
		// 		k = k + 1;
		// 		j = j + 1;
		// 	}
		// 	// printf("Command to be executed (Command Length: %i): \n", split_token_length);
			
		// 	// for(l=0;l<split_token_length;l++){
		// 	// 	printf("%s ", split_tokens[l]);
		// 	// }
		// 	// printf("\n");

		// 	execute_command_foreground(split_tokens);
		// 	// free(split_tokens);
		// }

		double_and_indices[double_and_flag] = max_len;
		double_and_flag = double_and_flag + 1;
		int j, split_token_length, k;

		for(j = 0; j < double_and_flag; j++){
			// char **split_tokens = (char **)malloc(64 * sizeof(char *));
			// if (j == 0)
			// 	memcpy(split_tokens, tokens, double_and_indices[i] * sizeof(char*));
			// else
			// 	memcpy(split_tokens, tokens+double_and_indices[i]+1, (double_and_indices[i] - double_and_indices[i-1] - 1) * sizeof(int));
			if (j == 0)
				execute_command(tokens, 0, 0, if_background);
			else
				execute_command(tokens, double_and_indices[j]+1, double_and_indices[j]+1, if_background);

			// free(split_tokens);
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
