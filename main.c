#include <stdio.h>
#include <stdlib.h>

void lsh_loop();
char *lsh_readline();

# define LSH_RL_BUFF_SIZE 1024


int main(int argc, char **argv){
	
	lsh_loop();

	return 0;
}

void lsh_loop(){
	char *line;
	char **args;
	int status;

	do{
		printf("> ");
		line = lsh_readline();
		// args= lsh_split_line(line);
		// status=lsh_execute(args);

		free(line);
		free(args);
	} while(status);


}

char *lsh_readline(){

	// fprintf(stdout, "in lsh_readline func\n");

	int buff_size=LSH_RL_BUFF_SIZE;
	int position=0;
	char *buffer=malloc(sizeof(char)*buff_size);
	int c;

	if(!buffer){
		fprintf(stderr, "lsh:allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		c=getchar();

		fprintf(stdout,"%d\n", c);
		
		if(c==EOF || c == "\n"){

			buffer[position]="\0";
		
			return buffer;
		}else{
			buffer[position]=c;
		
	}
		position++;

		if(position>=buff_size){
			buff_size+=LSH_RL_BUFF_SIZE;
			buffer=realloc(buffer,buff_size);

			if(!buffer){
				fprintf(stderr, "lsh:allocation error\n" );
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define LSH_TOK_BUFFSIZE 64
#define LSH_TOK_DELIM "\t\r\n\a"

char **lsh_split_line(char *line){

	int buff_size = LSH_TOK_BUFFSIZE;
	int position = 0;
	int toklen;

	
}