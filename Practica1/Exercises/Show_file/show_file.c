#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int c;
        char buffer[1]; 

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	// We use c as a control variable 
    while ((c = fread(buffer, sizeof(char), 1, file))){

		fwrite(buffer, sizeof(char), 1, stdout);

    }

	fclose(file);
	return 0;
	
}