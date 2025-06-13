/*CLIENTE*/
/*********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_TAM_MENSAJE 512 //Numero de caracteres maximo del mensaje

char mensaje[MAX_TAM_MENSAJE];

/**********************************************************/
/* función catch que captura una interrupción             */
/**********************************************************/
void catch(int sig)
{
	strcpy(mensaje,"terminar();");
}

/**********************************************************/
/* función MAIN                                           */
/* Orden Parametros: IP destino, puerto                   */
/**********************************************************/
int main(int argc, char *argv[]) {
	int descriptor_socket_origen;
	struct sockaddr_in socket_origen, socket_destino;
	char respuesta[MAX_TAM_MENSAJE];//, mensaje[MAX_TAM_MENSAJE];

 	if (argc != 3) {
    printf("\n\nEl número de parámetros es incorrecto\n");
    printf("Use: %s <IP_servidor> <puerto>\n\n",argv[0]);
    exit(EXIT_FAILURE);
  }

	// Creamos el socket del cliente
	descriptor_socket_origen = socket(AF_INET,SOCK_STREAM,0);
	if (descriptor_socket_origen == -1) {
    printf("ERROR: El socket del cliente no se ha creado correctamente!\n");
    exit(EXIT_FAILURE);
  }

  //Se prepara el socket de la máquina cliente
  socket_origen.sin_family = AF_INET;
	socket_origen.sin_port = htons(0); 								//Asigna un puerto disponible dela máquina
	socket_origen.sin_addr.s_addr = htonl(INADDR_ANY); //Asigna una IP de la máquina

	//Asigna una direccion local al socket
	if( bind(descriptor_socket_origen, (struct sockaddr*)&socket_origen, sizeof(socket_origen)) == -1) {
    printf("ERROR al configurar el socket del cliente\n");
    close(descriptor_socket_origen);
    exit(EXIT_FAILURE);
  }

  //Se prepara la configuracion de socket de la máquina servidora
	socket_destino.sin_family = AF_INET;
	socket_destino.sin_addr.s_addr = inet_addr(argv[1]);
	socket_destino.sin_port = htons(atoi(argv[2]));

	//Establece la conexión con la máquina remota
  printf("***Estableciendo conexion con servidor %s en puerto %s ....\n",argv[1],argv[2]);
	if( connect(descriptor_socket_origen, (struct sockaddr*)&socket_destino, sizeof(socket_destino)) == -1) {
    printf("ERROR en la solicitud de conexion del cliente al servidor\n");
    close(descriptor_socket_origen);
    exit(EXIT_FAILURE);
  } else {
    printf("***conexion establecida. \n");
  }
  signal(SIGINT, &catch);
  do {
    //Envía el mensaje
    printf("<<Client>>: ");
    scanf("%s",mensaje);
    if(send(descriptor_socket_origen,mensaje, strlen(mensaje)+1, 0) == -1) {
      printf("ERROR al enviar el mensaje del cliente al servidor\n");
      close(descriptor_socket_origen);
      exit(EXIT_FAILURE);
    } else {
      printf("***Cliente envía: %s a: %s en puerto: %s \n",mensaje, argv[1], argv[2]);
    }

	  //Recibe la respuesta
    if(recv(descriptor_socket_origen, respuesta, sizeof(respuesta), 0) == -1) {
      printf("ERROR al recibir el mensaje del servidor al cliente\n");
      close(descriptor_socket_origen);
      exit(EXIT_FAILURE);
    } else {
      printf("<<Server>>: %s\n", respuesta);
    }

    //Se cierra la conexión (socket)
    if(strcmp(mensaje,"terminar();") == 0) {
      printf("***Cliente terminó conexion con servidor.\n");
      close(descriptor_socket_origen);
      exit(EXIT_FAILURE);
    }

  } while(1);
	//Se cierra la conexión (socket)
  printf("\nCliente termina.\n");
	close(descriptor_socket_origen);
  exit(EXIT_SUCCESS);
}