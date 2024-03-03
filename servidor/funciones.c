#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"
#include <time.h>


// Función para autenticar al cliente
struct lista autenticarCliente(int connfd, char codigo[100]) {
    struct Estudiante estudianteEncontrado;
    struct lista ldevolver = {estudianteEncontrado, -1};


    if ((codigo[0] == 'F' || codigo[0] == 'C') && strlen(codigo) > 1) {
        FILE *archivo;
        char archivoNombre[MAX_SIZE];
        char linea[MAX_SIZE];

        // Determinar el archivo correspondiente según el código ingresado
        if (codigo[0] == 'F') {
            strcpy(archivoNombre, "./datos_txt/facultad.txt");
        } else if (codigo[0] == 'C') {
            strcpy(archivoNombre, "./datos_txt/carrera.txt");
        }

        // Abrir el archivo correspondiente en modo lectura
        archivo = fopen(archivoNombre, "r");
        if (archivo == NULL) {
            printf("[SERVER]: Error al abrir el archivo.\n");
            return ldevolver; // Indica error en la autenticación
        }

        int encontrado = 0;

        // Leer el archivo línea por línea buscando el código
        while (fgets(linea, sizeof(linea), archivo)) {
            if (strstr(linea, codigo) != NULL) {
                encontrado = 1;
                break;
            }
        }

        // Cerrar el archivo
        fclose(archivo);

        // Verificar si se encontró el código
        if (encontrado) {
            // Si se encontró el código, solicitamos la matrícula del estudiante
            write(connfd, "Ingrese su matricula:\n", sizeof("Ingrese su matricula:\n"));
            char buffer [10];
            read(connfd, buffer, sizeof(buffer));
            printf("[CLIENTE]: %s\n", buffer);

            // Buscar al estudiante por su matrícula en el archivo estudiantes.txt
            FILE *estudiantes = fopen("./datos_txt/estudiantes.txt", "r");
            if (estudiantes == NULL) {
                printf("[SERVER]: Error al abrir el archivo de estudiantes.\n");
                return ldevolver; // Indica error en la autenticación
            }
 
            int encontrado = 0;

            while (fscanf(estudiantes, "%[^:]:%[^,],%[^,],%[^,],%[^,],%f,%d,%d\n",
                        estudianteEncontrado.matricula, estudianteEncontrado.nombre,
                        estudianteEncontrado.apellido, estudianteEncontrado.codigofacultad,
                        estudianteEncontrado.codigocarrera, &estudianteEncontrado.promedio,
                        &estudianteEncontrado.cant_mat_ap, &estudianteEncontrado.cant_mat_rep) != EOF) {
                if (strcmp(estudianteEncontrado.matricula, buffer) == 0) {
                    // Matrícula encontrada, guardar la información y salir del bucle
                    ldevolver.id = 0;
                    ldevolver.est = estudianteEncontrado;
                    encontrado = 1;
                    
                    break;
                }
            }

            fclose(estudiantes);

            if (encontrado) {
                char mensaje [300];
                sprintf(mensaje, "[SERVER]:\nMatrícula: %s\nNombre: %s %s\nFacultad: %s\nCarrera: %s\nPromedio: %.2f\nMatrículas Aprobadas: %d\nMatrículas Reprobadas: %d\n",
                        estudianteEncontrado.matricula, estudianteEncontrado.nombre,
                        estudianteEncontrado.apellido, estudianteEncontrado.codigofacultad,
                        estudianteEncontrado.codigocarrera, estudianteEncontrado.promedio,
                        estudianteEncontrado.cant_mat_ap, estudianteEncontrado.cant_mat_rep);
                
                write(connfd, mensaje, sizeof(mensaje));
                printf("%s", mensaje);

            } else {
                printf("Estudiante con matrícula %s no encontrado.\n", buffer);
            }


            // Si no se encontró el estudiante, notificamos al cliente
            if (!encontrado) {
                write(connfd, "Estudiante no encontrado.", sizeof("Estudiante no encontrado."));
            }

        } else {
            // Si el código no se encontró en el archivo correspondiente
            write(connfd, "Código inválido o inexistente.", sizeof("Código inválido o inexistente."));
        }
    } else {
        // Si el código no cumple con el formato requerido
        write(connfd, "Código inválido.", sizeof("Código inválido."));
    }

    return ldevolver;
}

struct lista menu(int connfd, char id, struct Estudiante estudiante) {
    struct lista l = {estudiante, 0};

    char imprimir[200];
    char opcion[1];
    if (id == 'F') {
        opcion[0] = '4';

        while (opcion[0] != '0') {
            strcpy(imprimir, "[SERVER]: Menu - Facultad:\n[SERVER]: Seleccione la opcion a cambiar\n[0] Salir\n[1] Promedio\n[2] Cant_Mat_Ap\n[3] Camt_Mat_Rep\n");
            write(connfd, imprimir, sizeof(imprimir));
            read(connfd, opcion, sizeof(opcion));

            if (opcion[0] == '1') {
                write(connfd, "[SERVER]: Ingrese su nuevo promedio:\n", sizeof("[SERVER]: Ingrese su nuevo promedio:\n"));
                printf("[SERVER]: Ingrese su nuevo promedio:\n");


                char buffer[10];  // Buffer para almacenar los datos recibidos
                read(connfd, buffer, sizeof(buffer));  // Leer los datos del cliente

                printf("[CLIENTE]: Numero recibido: %s\n", buffer);

                // Convertir la cadena a un número float
                float nuevo_promedio;
                    if (sscanf(buffer, "%f", &nuevo_promedio) == 1) {
                        printf("[CLIENTE]: Numero recibido en float: %f\n", nuevo_promedio);
                        estudiante.promedio = nuevo_promedio;
                        write(connfd, "[SERVER]: Promedio cambiado exitosamente\n", sizeof("[SERVER]: Promedio cambiado exitosamente\n\n"));
                        l.id = 1;

                    } else {
                        printf("[CLIENTE]: Error al convertir a float\n");
                        write(connfd, "[SERVER]: Error al leer el numero, asegurese que lleve un . como separador decimal\n", sizeof("[SERVER]: Error al leer el numero, asegurese que lleve un . como separador decimal\n"));
                    }
            }

            if(opcion[0] == '2'){
                write(connfd, "[SERVER]: Ingrese la cant de materias AP:\n", sizeof("[SERVER]: Ingrese la cant de materias AP:\n"));
                printf("[SERVER]: Ingrese la cant de materias AP:\n");

                char buffer[10];
                read(connfd, buffer, sizeof(buffer));

                printf("[CLIENTE]: Numero recibido: %s\n", buffer);

                // Convertir la cadena a un numero int
                int nuevo_ap_materias;
                if (sscanf(buffer, "%d", &nuevo_ap_materias) == 1) {
                    printf("[CLIENTE]: Numero recibido en int: %d\n", nuevo_ap_materias);
                    estudiante.cant_mat_ap = nuevo_ap_materias;
                    write(connfd, "[SERVER]: Cant de materias AP cambiado exitosamente\n", sizeof("[SERVER]: Cant de materias AP cambiado exitosamente\n"));
                    l.id = 1;

                } else {
                    printf("[CLIENTE]: Error al convertir a int\n");
                    write(connfd, "[SERVER]: Error al leer el numero\n", sizeof("[SERVER]: Error al leer el numero\n"));
                }
            }

            if (opcion[0] == '3'){
                write(connfd, "[SERVER]: Ingrese la cant de materias RP:\n", sizeof("[SERVER]: Ingrese la cant de materias RP:\n"));
                printf("[SERVER]: Ingrese la cant de materias RP:\n");

                char buffer[10];
                read(connfd, buffer, sizeof(buffer));

                printf("[CLIENTE]: Numero recibido: %s\n", buffer);

                // Convertir la cadena a un numero int
                int nuevo_rp_materias;
                if (sscanf(buffer, "%d", &nuevo_rp_materias) == 1) {
                    printf("[CLIENTE]: Numero recibido en int: %d\n", nuevo_rp_materias);
                    estudiante.cant_mat_rep = nuevo_rp_materias;
                    write(connfd, "[SERVER]: Cant de materias RP cambiado exitosamente\n", sizeof("[SERVER]: Cant de materias RP cambiado exitosamente\n"));
                    l.id = 1;

                } else {
                    printf("[CLIENTE]: Error al convertir a int\n");
                    write(connfd, "[SERVER]: Error al leer el numero\n", sizeof("[SERVER]: Error al leer el numero\n"));
                }
            }

        }
    } else {
        opcion[0] = '3';

        while (opcion[0] != '0') {
            strcpy(imprimir, "[SERVER]: Menu - Carrera:\n[SERVER]: Seleccione la opcion a cambiar\n[0] Salir\n[1] Facultad\n[2] Carrera");
            write(connfd, imprimir, sizeof(imprimir));
            read(connfd, opcion, sizeof(opcion));

            char mensaje[200];

            if (opcion[0] == '1') {
                strcpy(mensaje, "[SERVER]: Ingrese su nueva facultad. Opciones disponibles:\nFIEC: Facultad.Ing.ElectricidadyComputacion\nFCSH: Facultad.En.CienciasSOcialesHumanisticas\nFICT: Facultad.Ing.CienciasDeLaTierra\n");
                write(connfd, mensaje, sizeof(mensaje));
                printf("[SERVER]: Ingrese su nueva facultad:\n");


                char buffer[10];  // Buffer para almacenar los datos recibidos
                read(connfd, buffer, sizeof(buffer));  // Leer los datos del cliente

                printf("[CLIENTE]: Facultad recibida: %s\n", buffer);

                strcpy(estudiante.codigofacultad, buffer);
                write(connfd, "[SERVER]: Facultad cambiada exitosamente\n", sizeof("[SERVER]: Facultad cambiada exitosamente\n"));
                l.id = 1;
            }

            if(opcion[0] == '2'){
                strcpy(mensaje, "[SERVER]: Ingrese su nueva carrera. Opciones disponibles:\nCI013: Computacion\nCI014: Electricidad\nCI007: AdminEmpresas\nCI005: Civil\n");
                write(connfd, mensaje, sizeof(mensaje));
                printf("[SERVER]: Ingrese su nueva carrera:\n");


                char buffer[10];  // Buffer para almacenar los datos recibidos
                read(connfd, buffer, sizeof(buffer));  // Leer los datos del cliente

                printf("[CLIENTE]: Carrera recibida: %s\n", buffer);

                strcpy(estudiante.codigofacultad, buffer);
                write(connfd, "[SERVER]: Carrera cambiada exitosamente\n", sizeof("[SERVER]: Carrera cambiada exitosamente\n"));
                l.id = 1;
            }
        }
    }

    l.est = estudiante;
    return l;
}

int guardar(struct Estudiante est_antes, struct Estudiante est_despues){
    int confirmacion = 0; // 0: algun error 1: todo bien

    FILE *estudiantesFile, *logFile;
    char buffer[50];
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char fecha[128];
    strftime(fecha, 128, "%Y/%m/%d %H:%M:%S", tlocal);

    // Abrir el archivo de log para escribir cambios
    logFile = fopen("./datos_txt/log.txt", "a");
    if (logFile == NULL) {
        printf("Error al abrir el archivo log.txt.\n");
        confirmacion = 0;
        return confirmacion;
    }

    // Escribir en el archivo de log
    fprintf(logFile, "Fecha_Cambio (%s): %s/%s\n", fecha, est_despues.codigofacultad, est_despues.codigocarrera);
    fprintf(logFile, "1: Datos de estudiantes antes del cambio\n");
    fprintf(logFile, "Matricula:%s,Nombre:%s,%s,Facultad:%s,Carrera:%s,Promedio:%.2f,Cant_Mat_Ap:%d,Cant_Mat_Rep:%d\n",
            est_antes.matricula, est_antes.nombre, est_antes.apellido, est_antes.codigofacultad, est_antes.codigocarrera,
            est_antes.promedio, est_antes.cant_mat_ap, est_antes.cant_mat_rep);
    fprintf(logFile, "2: Datos de estudiantes despues del cambio\n");
    fprintf(logFile, "Matricula:%s,Nombre:%s,%s,Facultad:%s,Carrera:%s,Promedio:%.2f,Cant_Mat_Ap:%d,Cant_Mat_Rep:%d\n",
            est_despues.matricula, est_despues.nombre, est_despues.apellido, est_despues.codigofacultad, est_despues.codigocarrera,
            est_despues.promedio, est_despues.cant_mat_ap, est_despues.cant_mat_rep);

    fclose(logFile);

    // Abrir el archivo de estudiantes para actualizar la información del estudiante
    estudiantesFile = fopen("./datos_txt/estudiantes.txt", "r");
    if (estudiantesFile == NULL) {
        printf("Error al abrir el archivo estudiantes.txt.\n");
        return 0;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error al abrir el archivo temporal.\n");
        fclose(estudiantesFile);
        return 0;
    }

    // Buscar y actualizar la información del estudiante en el archivo estudiantes.txt
    while (fgets(buffer, sizeof(buffer), estudiantesFile)) {
        struct Estudiante temp;
        sscanf(buffer, "%[^:]:%[^,],%[^,],%[^,],%[^,],%f,%d,%d\n",
               temp.matricula, temp.nombre, temp.apellido, temp.codigofacultad, temp.codigocarrera,
               &temp.promedio, &temp.cant_mat_ap, &temp.cant_mat_rep);

        if (strcmp(temp.matricula, est_antes.matricula) != 0) {
            fprintf(tempFile, "%s:%s,%s,%s,%s,%.2f,%d,%d\n",
                    temp.matricula, temp.nombre, temp.apellido, temp.codigofacultad, temp.codigocarrera,
                    temp.promedio, temp.cant_mat_ap, temp.cant_mat_rep);
        } else {
            fprintf(tempFile, "%s:%s,%s,%s,%s,%.2f,%d,%d\n",
                    est_despues.matricula, est_despues.nombre, est_despues.apellido, est_despues.codigofacultad, est_despues.codigocarrera,
                    est_despues.promedio, est_despues.cant_mat_ap, est_despues.cant_mat_rep);
        }
    }

    fclose(estudiantesFile);
    fclose(tempFile);

    remove("./datos_txt/estudiantes.txt");
    rename("temp.txt", "./datos_txt/estudiantes.txt");

    return 1;
}