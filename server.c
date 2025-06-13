//funcionalidades
#include "base.h"
//conexion
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
//estos 4 tiraran errores en windows porque estan pensados para linux, donde deberia probarse toda esta madre
//#include <sys/socket.h> 
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>

#define MAX_MSG 500 //cant max mensajes
int ServerDescSock;

int main(int argc, char *argv[]){
    //freopen("output2.txt", "w+", stdout);
    fill(); //va primero porque setea toda la base de sistemas que se usara para gestionar a los usuarios

    socklen_t TargetSize;
    struct sockaddr_in socket_servidor, socket_cliente;
    char mensaje_entrada[MAX_MSG], mensaje_salida[MAX_MSG]; //modificar estos valores a centro de entrada de mensajes y centro de salida
    int recibidos, enviados; //bytes recibidos 

    //revisar mañana bien esto, aun no entiendo su uso correcto
  if (argc != 2) {
    printf("\n\nEl número de parámetros es incorrecto\n");
    printf("Use: %s <puerto>\n\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Creamos el socket del servidor
    ServerDescSock = socket(AF_INET, SOCK_DGRAM,0); //crea el socket para el server
	if (ServerDescSock == -1) {
    printf("ERROR: problemas en la creacion del socket del servidor\n");
    exit(EXIT_FAILURE);
  }

    socket_servidor.sin_family = AF_INET; //define familia
    socket_servidor.sin_port = htons(atoi(argv[1])); //hace las conversiones para recibir un string y usarlo como valor de puerto (entregado manualmente)
    socket_servidor.sin_addr.s_addr = htonl(INADDR_ANY); //conversion para recibir una IP cualquiera


    return 0;
}