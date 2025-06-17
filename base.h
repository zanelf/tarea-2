#ifndef BASE_H
#define BASE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define n 50

typedef enum {
    DISPONIBLE,
    OCUPADO
}Estado;

typedef struct {
    char user[50];
    int IP;
    int puerto;
    Estado estado;
}usuario;

//========================================================
//  FUNCIONALIDADES PARA CREAR Y MANTENER LA LISTA DE USUARIOS DE FORMA MANUAL 
usuario* Usuarios[n]; //el espacio donde iran las personas
int last = 0; //ultimo espacio disponible
int currentUsers; //tamaño de usaurios

void fill(){ //inicializador de la variable, para el correcto funcionamiento debe ser una variable vacia
//    for(int i = 0; i <n;i++)strcpy(mesas[i].user,"\0");
    for(int i = 0; i <n;i++)Usuarios[i] = NULL;
}
void show(){
    for(int i = 0; i <n;i++){
            if(Usuarios[i]!=NULL) printf("%s\n",Usuarios[i]->user);
    }printf("\n==============\n");
}

//|X|X|X|Y|X|X|X|X|X|X|Q|Q| |

void addUser(char user[50], int IP,int puerto){ // (char user[50], int IP){
    if(currentUsers != n){
        usuario * aux;
        aux = (usuario*)malloc(sizeof(usuario)); 
        strcpy(aux->user,user);aux->IP = IP;aux->puerto = puerto;aux->estado = 1;
        Usuarios[last] = aux;
        last++; 
        while (Usuarios[last] != NULL)last++;
        currentUsers++;
    }
}
void DeleteUser(char target[]){
    for (int i = 0; i < n; i++){
         if(Usuarios[i]->user == target){
            usuario* aux = Usuarios[i];
            free(aux);
            Usuarios[i] = NULL;
            last = i;
            currentUsers--;
            break;
         }
    }
}



int FindUser(char user[]){
    for (int i = 0; i < n; i++){
        int res = strcmp(user, Usuarios[i]->user);
        if(0 == res)
            return i;
    }
    return -1;
}

int findUser(int IP){
    for (int i = 0; i < n; i++){
        if(IP == Usuarios[i]->IP)
            return i;
    }
    return -1;
}

#endif 