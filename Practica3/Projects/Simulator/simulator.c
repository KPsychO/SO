#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>

# define N_PARADAS 5 // número de paradas de la ruta
# define EN_RUTA 0 // autobús en ruta
# define EN_PARADA 1 // autobús en la parada
# define MAX_USUARIOS 40 // capacidad del autobús
# define USUARIOS 4 // numero de usuarios

// estado inicial
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus
int n_ocupantes = 0; // ocupantes que tiene el autobús

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};
// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

// Otras definiciones globales (comunicación y sincronización)
pthread_t autobus;
pthread_t usuarios[USUARIOS];

pthread_mutex_t cerrojo;

pthread_cond_t cond_pas = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_bus = PTHREAD_COND_INITIALIZER;

void * thread_autobus(void * args) {
  printf("Creando autobus...\n");
  while ( /*condicion*/ 1) {
    // esperar a que los viajeros suban y bajen
	Autobus_En_Parada();
    // conducir hasta siguiente parada
	if(!esperando_parada[(int) args] || !esperando_bajar[(int) args])
      Conducir_Hasta_Siguiente_Parada();
  }
}

void * thread_usuario(void * arg) {

  int a, b;

  // obtener el id del usario
  int id_usuario = (int) arg;

  printf("Creando usuario %d ...\n", id_usuario);

  while ( /*condicion*/ 1) {
    a = rand() % N_PARADAS;
    do {
      b = rand() % N_PARADAS;
    } while (a == b);
    Usuario(id_usuario, a, b);
  }
}

void Usuario(int id_usuario, int origen, int destino) {
  // Esperar a que el autobus esté en parada origen para subir
  if(parada_actual == origen)
    Subir_Autobus(id_usuario, origen);
  // Bajarme en estación destino
  if(parada_actual == destino)
    Bajar_Autobus(id_usuario, destino);
}

int main(int argc, char * argv[]) {
  int i;
  // Definición de variables locales a main
  // Opcional: obtener de los argumentos del programa la capacidad del
  // autobus, el numero de usuarios y el numero de paradas
  // Crear el thread Autobus

  for(i = 0; i < N_PARADAS; i++){
	  esperando_bajar[i] = 0;
	  esperando_parada[i] = 0;
  }

  pthread_mutex_init(&cerrojo, NULL);
  pthread_create(&autobus, NULL, thread_autobus, NULL);

  // Crear thread para el usuario i
  // Esperar terminación de los hilos
  for(i = 0; i < USUARIOS; i++)
    pthread_create(&usuarios[i], NULL, thread_usuario, (void*)i);

  return 0;
  
}

void Autobus_En_Parada() {
  /* Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
  quieran bajar y/o subir la parada actual. Después se pone en marcha */
  pthread_mutex_lock(&cerrojo);
  while(esperando_parada[parada_actual] || esperando_bajar[parada_actual]){
	
	if (esperando_parada[parada_actual]){
	  pthread_cond_broadcast(&cond_pas);
      pthread_cond_wait(&cond_bus, NULL);
	}
	if(esperando_bajar[parada_actual]){
      pthread_cond_broadcast(&cond_pas);
      pthread_cond_wait(&cond_bus, NULL);
	}
  }
}

void Conducir_Hasta_Siguiente_Parada() {
  /* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
  printf("Saliendo de %d ...\n", parada_actual);
  pthread_mutex_lock(&cerrojo);
  estado = EN_RUTA;
  pthread_mutex_unlock(&cerrojo);
  printf("Circulando...\n");
  sleep(4);
  parada_actual = (parada_actual + 1) % N_PARADAS;
  printf("Llegando a %d ...\n", parada_actual);
  pthread_mutex_lock(&cerrojo);
  estado = EN_PARADA;
  pthread_mutex_unlock(&cerrojo);
}

void Subir_Autobus(int id_usuario, int origen) {
  /* El usuario indicará que quiere subir en la parada ’origen’, esperará a que
  el autobús se pare en dicha parada y subirá. El id_usuario puede utilizarse para
  proporcionar información de depuración */
  pthread_mutex_lock(&cerrojo);
  esperando_parada[origen] ++;
  printf("%d Esperando en %d ... \n", id_usuario, origen);
  pthread_cond_wait(&cond_pas, NULL);
  n_ocupantes++;
  esperando_parada[origen] --;
  printf("%d Subiendo al autobus en %d ... \n", id_usuario, origen);
  pthread_mutex_unlock(&cerrojo);

  if(!esperando_parada[origen])
    pthread_cond_signal(&cond_bus);

}

void Bajar_Autobus(int id_usuario, int destino) {
  /* El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
  el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
  proporcionar información de depuración */
  pthread_mutex_lock(&cerrojo);
  esperando_parada[destino] ++;
  printf("%d Quiere bajar en %d ... \n", id_usuario, destino);
  pthread_cond_wait(&cond_pas, NULL);
  n_ocupantes--;
  esperando_parada[destino] ++;
  printf("%d Bajando en %d ... \n", id_usuario, destino);
  pthread_mutex_unlock(&cerrojo);

  if(!esperando_parada[destino])
    pthread_cond_signal(&cond_bus);
}