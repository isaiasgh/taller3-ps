#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_SIZE 100  // Tamaño máximo de las cadenas

// Estructura para los datos de estudiante
struct Estudiante{
    char matricula[MAX_SIZE];
    char nombre[MAX_SIZE];
    char apellido[MAX_SIZE];
    char codigofacultad[MAX_SIZE];
    char codigocarrera[MAX_SIZE];
    float promedio;
    int cant_mat_ap;
    int cant_mat_rep;
};

struct lista{
    struct Estudiante est;
    int id;
};


// Prototipos de funciones
struct lista autenticarCliente(int connfd, char codigo[100]);
struct lista menu(int connfd, char id, struct Estudiante estudiante);
int guardar(struct Estudiante est_antes, struct Estudiante est_despues);


#endif