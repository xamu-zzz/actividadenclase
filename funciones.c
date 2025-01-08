#include <stdio.h>
#include "funciones.h"
#include <string.h>

int menu() {
    int opcion;
    printf("1. Crear factura\n");
    printf("2. Ver facturas\n");
    printf("3. Editar factura\n");
    printf("4. Eliminar factura\n");
    printf("Ingrese una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void save(struct Factura *factura) {
    FILE *file;
    file = fopen("facturas.dat", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }else{
        fwrite(factura, sizeof(struct Factura), 1, file);
        printf("Factura guardada\n");
    }
    fclose(file);
}

void leerCadena(char *cadena, int numcaracteres) {
    fflush(stdin);
    fgets(cadena, numcaracteres, stdin);
    int len = strlen(cadena) - 1;
    cadena[len] = '\0';
}

void createFactura() {
    struct Factura factura;
    factura.total = 0;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 50);
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &factura.cedula);
    printf("Ingrese la cantidad de productos: ");
    scanf("%d", &factura.numProd);
    for (int i = 0; i < factura.numProd; i++) {
        printf("Ingrese el nombre del producto %d: ", i + 1);
        leerCadena(factura.productos[i].nombre, 50);
        printf("Ingrese la cantidad del producto %d: ", i + 1);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Ingrese el precio del producto %d: ", i + 1);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    factura.estado = 0;
    save(&factura);
}

void readFactura() {
    int opc = 0;
    struct Factura factura;
    FILE *file;

    file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    printf("Cedula\t\tNombre\t\tTotal\n");
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.estado != -1)
        {
            printf("%d\t\t%s\t\t%.2f\n", factura.cedula, 
                                         factura.nombre, 
                                         factura.total);
        }
    }
    fclose(file);
    //ver detalle de la factura
    printf("Desea ver el detalle de la factura? 1. Si 2. No\n");
    scanf("%d", &opc);
    if (opc == 1) {
        printf("Ingrese la cedula del cliente: ");
        int cedula;
        scanf("%d", &cedula);
        findFacturaByCedula(cedula);
    }
}

int findFacturaByCedula(int cedula) {
    struct Factura factura;
    int f=0, pos=0;
    FILE *file;
    file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return -1;
    }
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            f=1;
            printf("----------factura encontrada----------\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("Productos:\n");
            printf("Nombre\t\tCantidad\t\tPrecio\n");
            for (int i = 0; i < factura.numProd; i++) {
                printf("%s\t\t%d\t\t%.2f\n", factura.productos[i].nombre,
                                             factura.productos[i].cantidad,
                                             factura.productos[i].precio);
            }
            printf("Total: %.2f\n", factura.total);
            break;
        }
        pos=ftell(file);
    }
    if (f==0){
        printf("Factura no encontrada\n");
    }
    fclose(file);
    return pos;
}


void editarFactura() {
    struct Factura factura;
    int cedula;
    FILE *file;

    // Abrir el archivo para lectura/escritura
    file = fopen("facturas.dat", "r+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("Ingrese la cedula de la factura que desea editar:\n ");
    scanf("%d", &cedula);

    int opc=0, pos=0; // Posición absoluta en el archivo (en bytes)

    // Buscar la factura con la cédula proporcionada
    while (!feof(file)) {
        // Leer una factura desde el archivo
        fread(&factura, sizeof(struct Factura), 1, file);

        if (feof(file)) break; // Evitar procesar si ya es final del archivo

        if (factura.cedula == cedula) {
            opc = 1;

            // Mostrar los datos actuales de la factura
            printf("\n----------Factura encontrada----------\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("\nProductos:\n");
            printf("Nombre\t\tCantidad\tPrecio\n");
            for (int i = 0; i < factura.numProd; i++) {
                printf("%s\t\t%d\t\t%.2f\n", 
                       factura.productos[i].nombre, 
                       factura.productos[i].cantidad, 
                       factura.productos[i].precio);
            }
            printf("Total: %.2f\n", factura.total);

            // Solicitar nuevos datos para actualizar la factura
            printf("\nIngrese los nuevos datos de la factura:\n");
            printf("Ingrese el nombre del cliente: ");
            leerCadena(factura.nombre, 50);

            printf("Ingrese la cedula del cliente: ");
            scanf("%d", &factura.cedula);

            printf("Ingrese la cantidad de productos: ");
            scanf("%d", &factura.numProd);

            factura.total = 0; // Reinicia el total para recalcular
            for (int i = 0; i < factura.numProd; i++) {
                printf("Ingrese el nombre del producto %d: ", i + 1);
                leerCadena(factura.productos[i].nombre, 50);

                printf("Ingrese la cantidad del producto %d: ", i + 1);
                scanf("%d", &factura.productos[i].cantidad);

                printf("Ingrese el precio del producto %d: ", i + 1);
                scanf("%f", &factura.productos[i].precio);

                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }

            factura.estado=0; // Actualiza el estado si es necesario

            // Calcular la posición de la factura encontrada
            pos=ftell(file) - sizeof(struct Factura);  //obtener la posición actual del puntero de archivo en un archivo abierto. Es decir, devuelve el número de bytes desde el inicio del archivo hasta la posición actual del puntero.

            // Mover el puntero del archivo a la posición de la factura encontrada
            fseek(file, pos, 0);

            // Sobrescribir los datos de la factura en la posición correspondiente
            fwrite(&factura, sizeof(struct Factura), 1, file);

            printf("Factura actualizada correctamente.\n");
            break;
        }
    }

    if (!opc) {
        printf("No se encontro ninguna factura con la cédula proporcionada.\n");
    }

    fclose(file);
}


void eliminarFactura() {
    struct Factura factura;
    int cedula;
    FILE *file;

    // Abrir el archivo en modo lectura/escritura
    file = fopen("facturas.dat", "r+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("Ingrese la cedula del cliente para eliminar la factura: ");
    scanf("%d", &cedula);

    int opc=0, pos=0; // Posición absoluta en el archivo (en bytes)

    // Buscar la factura con la cédula proporcionada
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula && factura.estado != -1) {
            opc=1;

            // Cambiar el estado de la factura a -1 (marcar como eliminada)
            factura.estado=-1;

            // Calcular la posición de la factura en el archivo
            pos=ftell(file) - sizeof(struct Factura);

            // Moverse a la posición correspondiente y sobrescribir la factura
            fseek(file, pos, 0);
            fwrite(&factura, sizeof(struct Factura), 1, file);

            printf("Factura con cedula %d fue eliminada.\n", cedula);
            break;
        }
    }

    if (!opc) {
        printf("No se encontro ninguna factura activa con la cedula proporcionada.\n");
    }

    fclose(file);
}