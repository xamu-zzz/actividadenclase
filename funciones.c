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
    } else {
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

// Verifica si ya existe una factura con la misma cédula en el archivo
int existeCedula(int cedula) {
    struct Factura factura;
    FILE *file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        return 0; // Si no se puede abrir el archivo, asumimos que no hay duplicados
    }

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula && factura.estado != -1) {
            fclose(file);
            return 1; // Cedula encontrada
        }
    }

    fclose(file);
    return 0; // Cedula no encontrada
}

void createFactura() {
    struct Factura factura;
    factura.total = 0;

    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 50);

    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &factura.cedula);

    // Verificar si la cédula ya existe en el archivo
    if (existeCedula(factura.cedula)) {
        printf("Error: Ya existe una factura con esta cedula.\n");
        return;
    }

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
        if (factura.estado != -1) {
            printf("%d\t\t%s\t\t%.2f\n", factura.cedula, 
                                          factura.nombre, 
                                          factura.total);
        }
    }
    fclose(file);

    // Ver detalle de la factura
    printf("Desea ver el detalle de la factura? 1. Si 2. No\n");
    scanf("%d", &opc);
    if (opc == 1) {
        printf("Ingrese la cédula del cliente: ");
        int cedula;
        scanf("%d", &cedula);
        findFacturaByCedula(cedula);
    }
}

int findFacturaByCedula(int cedula) {
    struct Factura factura;
    int f = 0;
    FILE *file;

    file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return -1;
    }

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula && factura.estado != -1) {
            f = 1;
            printf("---------- Factura encontrada ----------\n");
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
            fclose(file);
            return 0; // Cedula encontrada
        }
    }

    if (f == 0) {
        printf("Factura no encontrada\n");
    }
    fclose(file);
    return -1; // Cedula no encontrada
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

    printf("Ingrese la cédula de la factura que desea editar:\n");
    scanf("%d", &cedula);

    int pos = -1;

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula && factura.estado != -1) {
            printf("\n---------- Factura encontrada ----------\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);

            printf("\nIngrese los nuevos datos de la factura:\n");
            printf("Ingrese el nombre del cliente: ");
            leerCadena(factura.nombre, 50);

            printf("Ingrese la cantidad de productos: ");
            scanf("%d", &factura.numProd);

            factura.total = 0;
            for (int i = 0; i < factura.numProd; i++) {
                printf("Ingrese el nombre del producto %d: ", i + 1);
                leerCadena(factura.productos[i].nombre, 50);

                printf("Ingrese la cantidad del producto %d: ", i + 1);
                scanf("%d", &factura.productos[i].cantidad);

                printf("Ingrese el precio del producto %d: ", i + 1);
                scanf("%f", &factura.productos[i].precio);

                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }

            pos = ftell(file) - sizeof(struct Factura);
            fseek(file, pos, SEEK_SET);
            fwrite(&factura, sizeof(struct Factura), 1, file);

            printf("Factura actualizada correctamente.\n");
            fclose(file);
            return;
        }
    }

    printf("No se encontró ninguna factura con la cédula proporcionada.\n");
    fclose(file);
}

void eliminarFactura() {
    struct Factura factura;
    int cedula;
    FILE *file;

    file = fopen("facturas.dat", "r+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("Ingrese la cédula del cliente para eliminar la factura: ");
    scanf("%d", &cedula);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula && factura.estado != -1) {
            factura.estado = -1;
            fseek(file, ftell(file) - sizeof(struct Factura), SEEK_SET);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            printf("Factura eliminada.\n");
            fclose(file);
            return;
        }
    }

    printf("No se encontró ninguna factura activa con la cédula proporcionada.\n");
    fclose(file);
}