//funcionalidades
#include "base.h"
//conexion
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
//estos 4 tiraran errores en windows porque estan pensados para linux, donde deberia probarse toda esta madre
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAX_MSG 500 //cant max mensajes
int ServerDescSock; //

//mensaje de notificacion atrapada.
void catch(int sig){
	printf("***Señal: %d atrapada!\n", sig);
    printf("***Cerrando servicio ...\n");
    close(ServerDescSock);
    printf("***Servicio cerrado.\n");
    exit(EXIT_SUCCESS);
}



int main(int argc, char *argv[]){
        //freopen("output2.txt", "w+", stdout);
        fill(); //va primero porque setea toda la base de sistemas que se usara para gestionar a los usuarios

        socklen_t TargetSize;
        struct sockaddr_in socket_servidor, cliente;
        char mensaje_entrada[MAX_MSG], MSG_OUT[MAX_MSG]; //modificar estos valores a centro de entrada de mensajes y centro de salida
        int recibidos, enviados; //bytes recibidos 

    //el puerto que le correspondera debe recibir solo eso 
    if (argc != 2) {
        printf("\n\nEl número de parámetros es incorrecto\n");
        printf("Use: %s <puerto>\n\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    // Creamos el socket del servidor
        ServerDescSock = socket(AF_INET, SOCK_DGRAM,0); //crea el socket para el server y lo configuramos para UDP
	if (ServerDescSock == -1) {
        printf("ERROR: problemas en la creacion del socket del servidor\n");
        exit(EXIT_FAILURE);
    }
        socket_servidor.sin_family = AF_INET; //define familia
        socket_servidor.sin_port = htons(atoi(argv[1])); //hace las conversiones para recibir un string y usarlo como valor de puerto (entregado manualmente)
        socket_servidor.sin_addr.s_addr = htonl(INADDR_ANY); //conversion para recibir una IP cualquiera


	if( bind(ServerDescSock, (struct sockaddr*)&socket_servidor, sizeof(socket_servidor)) == -1) {
        printf("ERROR al unir el socket a la dirección de la máquina servidora\n");
        close(ServerDescSock);
        exit(EXIT_FAILURE);
    }

    // revisa que todo este en orden y que todo funcione bien 
    signal(SIGINT, &catch);

    printf("\nnodo principal en funcionamiento en el puerto: %s ...\n",argv[1]);    

    do{ //Quedo a la espera de algun mensaje
        TargetSize=sizeof(cliente);
        recibidos= recvfrom(ServerDescSock, mensaje_entrada, MAX_MSG, 0, (struct sockaddr*)&cliente, &TargetSize);
        if(recibidos <0) {
            printf("<<SERVER STATUS>>: ERROR de recvfrom() \n");
            exit(EXIT_FAILURE);
        } else{
        int index =findUser(cliente.sin_addr.s_addr); 
        if(0>index){
            addUser(mensaje_entrada,cliente.sin_addr.s_addr,atoi(argv[1]));
            printf("a ingresado al servidor el usuario %s con la direccion %d", mensaje_entrada, cliente.sin_addr.s_addr);
        }else{
            int option = atoi(mensaje_entrada)        
            switch (option){ //
            case 1: //recibir mensajes para enviar 
                MSG_OUT = "<<SERVER>>: indique a quien quiere enviar el mensaje";  //intenta enviar un mensaje pidiendo el nombre del usuario al que quiere buscar.
                char sender[50] = Usuarios[index]->user;
                int receiver;

                enviados = sendto(ServerDescSock, strcat(MSG_OUT,"\0"), MAX_MSG, 0, (struct sockaddr*)&cliente, TargetSize);
                    if (enviados<0) {
                        printf("<<MSG STATUS>>: Error sendto() \n");
                    } else{
                        printf("<<MSG STATUS>>: msg send\n");
                        //espera a que el usuario envie la respuesta
                        recibidos= recvfrom(ServerDescSock, mensaje_entrada, MAX_MSG, 0, (struct sockaddr*)&cliente, &TargetSize);
                            if(recibidos <0) {
                                printf("<<MSG STATUS>>: ERROR de recvfrom() \n");
                            }else{
                                if(0<FindUser(mensaje_entrada)){
                                    receiver = Usuarios[FindUser(mensaje_entrada)]->IP;
                                    MSG_OUT = "<<MSG STATUS>>: ingrese el mensaje que quiere enviar";
                                    sendto(ServerDescSock, strcat(MSG_OUT,"\0"), MAX_MSG, 0, (struct sockaddr*)&cliente, TargetSize);    
                                    recibidos= recvfrom(ServerDescSock, mensaje_entrada, MAX_MSG, 0, (struct sockaddr*)&cliente, &TargetSize);
                                    char mail[ MAX_MSG];

                                
                                }else{
                                    MSG_OUT = "<<MSG STATUS>>: el nombre ingresado no existe, ingrese un nombre existente";
                                    sendto(ServerDescSock, strcat(MSG_OUT,"\0"), MAX_MSG, 0, (struct sockaddr*)&cliente, TargetSize);
                                    break;
                                }
                            }
                    }
            
            break;
            
            default:
                break;
            }
        
        
        
        }

        }
            //Envia el mensaje al cliente
	printf(MSG_OUT, "El mensaje recibido fue --- %s ---.",mensaje_entrada);


    }while(1);


    printf("cerrando sistema\n");
    close(ServerDescSock);
    printf("loby cerrado\n");
    exit(EXIT_SUCCESS);


        return 0;
}