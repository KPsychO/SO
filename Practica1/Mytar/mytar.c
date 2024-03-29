#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
       
#include "mytar.h"
       
char use[]="Usage: tar -c|x|l|a|r -f file_mytar [file1 file2 ...]\n";

int main(int argc, char *argv[]) {

  int opt, nExtra, retCode=EXIT_SUCCESS;
  flags flag=NONE;
  char *tarName=NULL;
  char *newTarName=NULL;
  
  //Minimum args required=3: mytar -tf file.tar
  if(argc < 2){
    fprintf(stderr,"%s",use);
    exit(EXIT_FAILURE);
  }
  //Parse command-line options
  while((opt = getopt(argc, argv, "cxlarf:")) != -1) {
    switch(opt) {
      case 'c':
        flag=(flag==NONE)?CREATE:ERROR;
        break;
      case 'x':
        flag=(flag==NONE)?EXTRACT:ERROR;
        break;
      case 'l':
        flag=(flag==NONE)?LIST:ERROR;
        break;
      case 'f':
        tarName = optarg;
        break;
      case 'a':
       flag=(flag==NONE)?ADD:ERROR;
        break;
      case 'r':
       flag=(flag==NONE)?REMOVE:ERROR;
        break;
      default:
        flag=ERROR;
    }
    //Was an invalid option detected?
    if(flag==ERROR){
      fprintf(stderr,"%s",use);
      exit(EXIT_FAILURE);
    }
  }
  
  //Valid flag + arg + file[s]
  if(flag==NONE || tarName==NULL) {
    fprintf(stderr,"%s",use);
    exit(EXIT_FAILURE);
  }
  
  //#extra args
  nExtra=argc-optind;
  
  //Execute the required action
  switch(flag) {
    case CREATE:
      retCode=createTar(nExtra, &argv[optind], tarName);
      break;
    case EXTRACT:
      if(nExtra!=0){
        fprintf(stderr,"%s",use);
        exit(EXIT_FAILURE);
      }
      retCode=extractTar(tarName);
      break;
    case LIST:
  retCode=listTar(tarName);
    break;
     case ADD:
      newTarName = malloc(strlen(tarName) + 6);
      strncpy(newTarName, tarName, strlen(tarName) - 5);
      strcat(newTarName, ".added.mtar");
      retCode=addFileToTar(nExtra, &argv[optind], tarName, newTarName);
      break;
    case REMOVE:
      newTarName = malloc(strlen(tarName) + 6);
      strncpy(newTarName, tarName, strlen(tarName) - 5);
      strcat(newTarName, ".removed.mtar");
      retCode=removeFileFromTar(nExtra, &argv[optind], tarName, newTarName);
      break;
    default:
      retCode=EXIT_FAILURE;
  }
  exit(retCode);
}