
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXQUOTES 10000
#define MAXLEN 1000

char *quotes[MAXQUOTES];
int numQuotes=0;

int main() {
	char line[MAXLEN];

	FILE *fp = fopen("quotes.txt", "r");
	int i=0;

	//read one line at a time, allocate memory, then copy the line into array
	while (fgets(line, MAXLEN, fp)) {
		quotes[i] = (char *) malloc (strlen(line)+1);
		strcpy(quotes[i], line);	
		i++;
	}
	numQuotes = i;
	
	char filename[MAXLEN];
	sprintf(filename, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(filename, 0600);
	chmod(filename, 0622);
	printf("Send your requests to %s/n", filename);
	
	while(1){
		FILE *fp = fopen(filename, "r");
		fgets(line, MAXLEN, fp);
		fclose(fp);
		
		char *cptr = strchr(line, '\n');
			if(cptr)
				*cptr = '\0';
		
		FILE *clientfp = fopen(line, "w");
		printf("%s: ", line);
		if (clientfp) {
			int quoteNumber = rand() % numQuotes;
			printf("%s", quotes[quoteNumber]);
			fputs(quotes[quoteNumber], clientfp);
			fclose(clientfp);
		}
		putchar('\n');
	}
}
