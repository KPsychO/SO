#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes) {
	// Complete the function

	int nCopied = 0;
	int c;

	if(origin == NULL || destination == NULL) 
		return -1;

	do {

		c = getc(origin); 
		if(c != EOF) { 
			putc(c, destination);
			nCopied++;
		}

	} while((c != EOF) && (nCopied != nBytes));

	return nCopied;

}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file) {
	// Complete the function

	char *buffer = NULL;
	int l;
	int c;

	if (file == NULL) return NULL;

	buffer = malloc(PATH_MAX);
	l = 0;

	do {
		c = getc(file);
		if(c != '\n')
		buffer[l++] = c;

	} while(c != '\n');

	buffer[l] = '\0';

	return buffer;

}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles) {
	// Complete the function

	stHeaderEntry *arr = NULL;

	int n_files = 0;
	int i;

	if (tarFile == NULL) return NULL;

	fread(&n_files, sizeof(int), 1, tarFile);
	fscanf(tarFile, "\n");

	arr = (stHeaderEntry*) malloc(sizeof(stHeaderEntry) * n_files);

	for(i = 0; i < n_files; i++){

		arr[i].name = malloc(PATH_MAX);
		arr[i].name = loadstr(tarFile);
		fread(&arr[i].size, sizeof(int), 1, tarFile);
		fscanf(tarFile, "\n");

	}

	(*nFiles) = n_files;

	return arr;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[]) {
	// Complete the function

	FILE *iFile = NULL;
	FILE *tFile = NULL;
	stHeaderEntry *arr = NULL;

	int nBytes = 0;
	int i;

	if ((tFile = fopen(tarName, "wb")) == NULL) return EXIT_FAILURE;

	arr = malloc(sizeof(stHeaderEntry) * nFiles);

	nBytes = (nFiles + 1) + sizeof(int) + nFiles * sizeof(unsigned int);

	for(i = 0; i < nFiles; i++){

		arr[i].name = malloc(strlen(fileNames[i]) + 1);
		strcpy(arr[i].name, fileNames[i]);
		nBytes += strlen(fileNames[i]) + 1;

	}

	fseek(tFile, nBytes, SEEK_SET);

	for(i = 0; i < nFiles; i++){

		iFile = fopen(fileNames[i], "rb");
		if (iFile == NULL) return EXIT_FAILURE;
		nBytes = copynFile(iFile, tFile, INT_MAX);
		fclose(iFile);
		arr[i].size = nBytes;

	}

	rewind(tFile);

	fwrite(&nFiles, sizeof(int), 1, tFile);
	fwrite("\n", sizeof(char), 1, tFile);

	for(i = 0; i < nFiles; i++){

		fwrite(arr[i].name, strlen(fileNames[i]), 1, tFile);
		fwrite("\n", sizeof(char), 1, tFile);
		fwrite(&arr[i].size, sizeof(unsigned int), 1, tFile);
		fwrite("\n", sizeof(char), 1, tFile);
		free(arr[i].name);

	}

	printf("Fichero Mytar creado con exito\n");
	free(arr);
	fclose(tFile);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[]) {
	// Complete the function

	FILE *oFile = NULL;
	FILE *tFile = NULL;
	stHeaderEntry *arr = NULL;

	int nCopied;
	int n_files = 0;
	int i;

	if((tFile = fopen(tarName, "rb")) == NULL) return EXIT_FAILURE;

	arr = readHeader(tFile, &n_files);

	for(i = 0; i < n_files; i++){

		oFile = fopen(arr[i].name, "wb");
		nCopied = copynFile(tFile, oFile, arr[i].size);
		printf("[%d]: Creando fichero %s, tamano %d Bytes...Ok\n", i, arr[i].name, nCopied);
		fclose(oFile);

	}

	free(arr);
	fclose(tFile);

	return EXIT_SUCCESS;
}
