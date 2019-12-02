#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

int selectPatternMenu();
int selectSpeed();
void contadorBinarioAscendente(int file, int vel);
void circular(int file, int vel);
void alternando(int file, int vel);

int main() {

    int pattern;
    int ledsFile = open("/dev/chardev_leds", O_WRONLY | O_CREAT | O_TRUNC);

    while(pattern = selectPatternMenu()){

        switch (pattern){

            case 0: return 0;
                break;

            case 1:
                contadorBinarioAscendente(ledsFile, selectSpeed());
                break;

            case 2:
                circular(ledsFile, selectSpeed());
                break;

            case 3:
                alternando(ledsFile, selectSpeed());
                break;

            case 4:
                break;

            default: 
                printf("Introduzca una opción válida.\n");
                break;

        }

    }

    close(ledsFile);
    return 0;

}

int selectPatternMenu(){

    int pattern;

    printf("0- Salir\n");
    printf("1- Contador binario ascendente.\n");
    printf("2- Circular\n");
    printf("3- Alternando\n");
    printf("4- Disk I/O (si sale xD)\n");
    printf("Introduzca su opcion: ");
    scanf("%i", &pattern);
    printf("-----------------------------------\n");

    return pattern;

}

int selectSpeed() {

    int vel = 1;
    int aux;
    printf("Introduzca la velocidad en milisegundos que desea: ");
    scanf("%i", &aux);
    if (aux)
        vel = abs(aux);

    return (1000 * vel);

}

void contadorBinarioAscendente(int file, int vel){

    char* states[8] = {"", "3", "2", "23", "1", "13", "12", "123"};
    int i;

    for (i = 0; i < 8; i++){
        write(file, states[i], strlen(states[i]));
        usleep(vel);
    }

    write(file, "0", 1);

} 

void circular(int file, int vel){

    char* states[8] = {"3", "2", "1"};
    int i;

    for (i = 0; i < 15; i++){
        write(file, states[i%3], strlen(states[i%3]));
        usleep(vel);
    }

    write(file, "0", 1);

}

void alternando(int file, int vel){

    char* states[8] = {"13", "2"};
    int i;

    for (i = 0; i < 10; i++){
        write(file, states[i%2], strlen(states[i%2]));
        usleep(vel);
    }

    write(file, "0", 1);

}