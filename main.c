#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>

void lsh_loop();
char *lsh_readline();
char **lsh_split_line(char *line);

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

int lsh_execute(char **args);

# define LSH_RL_BUFF_SIZE 1024

int main(int argc, char **argv){
	lsh_loop();
	return EXIT_SUCCESS;
}

void lsh_loop(void){
	char *line;
	char **args;
	int status;

	do {
		fprintf(stdout,"> ");
		line = lsh_readline();
		args= lsh_split_line(line);
		status=lsh_execute(args);

		// fprintf(stdout,"---------debugging start---------\n");
		// fprintf(stdout,"read_line : %s\n", line);
		// fprintf(stdout,"args : %s\n", args);
		// fprintf(stdout,"status : %d\n", status);
		// fprintf(stdout,"---------debugging end---------\n");

		free(line);
		free(args);
	} while(status);


}

char *lsh_readline(){

	// fprintf(stdout, "\nin lsh_readline func\n");

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

		// fprintf(stdout,"%d\n", c);
		
		if(c==EOF || c == '\n'){

			buffer[position]='\0';
		
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
	char **tokens=malloc(buff_size * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr, "lsh alocation error \n" );
		exit(EXIT_FAILURE);
	}

	token=strtok(line,LSH_TOK_DELIM);
	while(token!=NULL){
		tokens[position]=token;
		position++;

		if (position>=buff_size)
		{
			/* code */
			buff_size+=LSH_TOK_BUFFSIZE;
			tokens=realloc(tokens,buff_size * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "lsh alocation error \n"  );
				exit(EXIT_FAILURE);
			}
		}

		token=strtok(NULL,LSH_TOK_DELIM);
	}

	tokens[position]=NULL;
	return tokens;
}

int lsh_launch(char **args){
	pid_t pid;
	pid_t wpid;

	int status;

	pid=fork();

	if(pid==0){
		if(execvp(args[0],args)==-1){
			perror("lsh");
		}
		exit(EXIT_FAILURE);

	}else if(pid<0){
		perror("lsh");
	}else{
		do {
			wpid=waitpid(pid,&status,WUNTRACED);
		} while(!WIFEXITED(status)&&!WIFSIGNALED(status));
	}

	return 1;
}

char *builtin_str[]={
	"cd",
	"help",
	"exit"
};

int (*builtin_func[])(char **)={
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

int lsh_num_builtins(){
	return sizeof(builtin_str)/sizeof(char *);
}

int lsh_cd(char **args){

	if(args[1]==NULL){
		fprintf(stderr, "lsh:expected arg to \"cd\"\n");
	}else{
		if(chdir(args[1])!=0){
			perror("lsh");
		}
	}
	return 1;
}

int lsh_help(char **args){

	int i;
	fprintf(stdout, "HELP info\n");
}

int lsh_exit(char **args){
	return 0;
}

int lsh_execute(char **args){
	int i;

	if(args[0]==NULL){
		return 1;
	}

	for(i=0;i<lsh_num_builtins();i++){
		if(strcmp(args[0],builtin_str[i])==0){
			return (*builtin_func[i])(args);
		}
	}

	return lsh_launch(args);
}
