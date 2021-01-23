#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int displayDirectory ();
int displayCommande (char ** commande);
int countSpace (char * buffer);
void cutCommande (char * buffer, char ** commande);

int main (int argc, char ** argv) {

	char ** commande = NULL;
	char buffer[256];
	int count = 0;
	FILE * fp;

	fp = fopen ("tes_shell_history", "r+b");

	memset(buffer, 0, 256);

	while (strcmp(buffer, "exit")){

		memset(buffer, 0, 256);

		if (displayDirectory())
			return 0;

		fgets(buffer, 256, stdin);
		buffer[strlen(buffer) - 1] = 0;

		if (strcmp(buffer, "exit")) {

			fseek (fp, 0, SEEK_END);
			fprintf(fp, "%s\n", buffer);

			count = countSpace(buffer);

			commande = malloc(sizeof(*commande) * count);

			cutCommande(buffer, commande);

			if(displayCommande(commande))
				return 0;

			free(commande);
			
		}
	}

	fclose(fp);

	return 0;
}

// Cut buffer and affect each part in commande table
void cutCommande (char * buffer, char ** commande) {

	int i;
	char * strToken;

	strToken = strtok(buffer, " ");
	commande[0] = strToken;

	for (i = 1; strToken != NULL; i++) {
		strToken = strtok(NULL, " ");
		commande[i] = strToken;
	}
}

// Count space in buffer
int countSpace (char * buffer) {

	int count;
	long unsigned int i;
	int espace;

	for (i = 0, espace = 0, count = 1; i < strlen(buffer); i++){
		if (buffer[i] == ' ') {
			if (!espace){
				espace = 1;
				count++;
			}
		} else {
			espace = 0;
		}
	}

	return count;
}

// Execute commande
int displayCommande (char ** commande) {

	pid_t childpid = fork();

	if (childpid == 0) {
		execvp(commande[0], &commande[0]);
		return 1;
	}

	wait(&childpid);
	return 0;

}

// Display current directory
int displayDirectory () {

	char *pwd[10] = {"pwd"};
	pid_t childpid = fork();

	if (childpid == 0) {
		execvp(pwd[0], &pwd[0]);
		return 1;
	}

	printf(" > ");
	wait(&childpid);
	return 0;

}
