/////////////////////////////////////
// Writtem by Krishna Wadhwani
/////////////////////////////////////

#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int sigint_flag = 0; 

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

void execute_command(char** tokens, pid_t background_pid_list[]){// pid_t background_pid_list[], int* background_pid_index){

	if (strcmp(tokens[0], "exit") == 0){
		// printf("Number of background processes: %i", *background_pid_index);
		// for (p = 0; p < (*background_pid_index); p++){
		// 	printf("Killing %d\n", background_pid_list[p]);
		// 	kill(background_pid_list[p], SIGKILL);
		// }
		int p;
		for(p = 0; p <MAX_NUM_TOKENS; p++){
			if (background_pid_list[p] > 0){
				// printf("#");
				printf("Killing %d\n", background_pid_list[p]);
				kill(background_pid_list[p], SIGKILL);
			}
		}
		printf("Shell: Goodbye.\n");
		// killpg(0, SIGKILL);
		exit(1);
	}

	else if (strcmp(tokens[0], "cd") == 0){
		char buf[1024];
		getcwd(buf, sizeof(buf));
		strcat(buf, "/");
		strcat(buf, tokens[1]);
		int ret = chdir(buf);
		if (ret != 0){
			printf("Shell: Incorrect command.\n");
		}
	}

	else{
		pid_t pid;
		int status;
		pid = fork();
		if (pid < 0){
			// fprintf(stderr, "Unable to fork a child. \n");
			printf("Unable to fork a child.\n");
		}
		else if (pid == 0){
			int flag = execvp(tokens[0], tokens);
			if (flag < 0){
				// fprintf(stderr, "Shell: Incorrect command.\n");
				printf("Shell: Incorrect command.\n");
				exit(flag);
			}
		}
		else{
			wait(NULL);
		}
	}

}

void execute_command_in_background(char** tokens, pid_t background_pid_list[]){// pid_t background_pid_list[], int* background_pid_index){
	pid_t pid;
	int status;
	pid = fork();

	if (pid<0)
		// fprintf(stderr, "Unable to fork a child. \n");
		printf("Unable to fork a child. \n");

	else if(pid == 0){
		setpgid(getpid(), pid);
		// printf("Child %i\n", background_pid_list[0]);

		int flag = execvp(tokens[0], tokens);
		if (flag < 0){
		// fprintf(stderr, "Shell: Incorrect command.\n");
		printf("Shell: Incorrect command.\n");	
		}
	}

	else{
		int p;
		for(p = 0; p <MAX_NUM_TOKENS; p++){
			if (background_pid_list[p] < 0){
				background_pid_list[p] = pid;
				break;
			}
		}
		printf("Process (PID = %d) executing in background\n", pid);

	}
}
//check_for_reaped_children(pid_t backgound_pid_list[], int backgound_pid_index)
void check_for_reaped_children(int *cstatus, pid_t background_pid_list[]){
	int status, i;

	pid_t wait_id;

		wait_id = waitpid(-1, cstatus, WNOHANG); // -1 implies waitpid will wait for any process. 0 for all processes in the process group.

	if (wait_id == -1)
		; //No Child to wait or wait error
	else if (wait_id == 0)
		; // Still Running

	else{
		printf("Background Process (%d) terminated \n", wait_id);
		int p;
		for(p = 0; p< MAX_NUM_TOKENS; p++){
			if (background_pid_list[p] == wait_id){
				background_pid_list[p] = -999;
				break;
			}
		}
	}
}

void sigint_handler(int sig){
   printf("Foreground Processes Terminated. Type exit to close the shell.\n");
   sigint_flag = 1;
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

	pid_t background_pid_list[MAX_NUM_TOKENS];
	for (i = 0; i<MAX_NUM_TOKENS; i++)
		background_pid_list[i] = -999;
	// int flag = 0;
	// int* background_pid_index = &flag;
	// *background_pid_index = 0;

	// printf("!!!!!!!!!!!!!!!!!! %i", *background_pid_index);

	int* cstatus;


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

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		for(i = 0; tokens[i]!=NULL; i++)
		{
			if (strcmp(tokens[i], double_and) == 0)
				is_double_and = 1;
			else if (strcmp(tokens[i], single_and) == 0)
				is_single_and = 1;
			else if (strcmp(tokens[i], triple_and) == 0)
				is_triple_and += 1;

		}

		check_for_reaped_children(cstatus, background_pid_list);

		if (tokens[0] == NULL)
			continue;

		if (is_single_and != 0){
			// printf("Found Single and \n ");
			char* tokens_split = strtok(line, "&");
			tokens = tokenize(tokens_split);
			execute_command_in_background(tokens, background_pid_list);// background_pid_list, background_pid_index);
			printf("%i\n", background_pid_list[0]);
			// printf("background index: %d \n", (*background_pid_index));
		}

		else if (is_triple_and != 0){
			int num_processes = is_triple_and + 1;
			pid_t pid;
			char* tokens_split = strtok(line, "&&&");
			while(tokens_split!=NULL){
				tokens = tokenize(tokens_split);
				pid = fork();

				if (pid == 0){
					if (strcmp(tokens[0], "exit") == 0){
						// kill(0, SIGKILL);
						printf("Shell: Goodbye.\n");
						exit(1);
					}

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
							// fprintf(stderr, "Shell: Incorrect command.\n");
							printf("Shell: Incorrect command.\n");
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

		else{

		char* tokens_split = strtok(line, "&&");

		while(tokens_split!=NULL){

			tokens = tokenize(tokens_split);
			execute_command(tokens, background_pid_list);// background_pid_list, background_pid_index);

			tokens_split = strtok(NULL, "&&");

			if (sigint_flag == 1)
			{
				sigint_flag = 0;
				break;
			}
		}


	}
		check_for_reaped_children(cstatus, background_pid_list);
		// check_for_reaped_children();
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
