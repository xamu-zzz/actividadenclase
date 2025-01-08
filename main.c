#include <stdio.h>
#include "funciones.h"


int main (int argc, char *argv[]) {

    int opc=0,opc2=0;
    do{
        opc=menu();
        switch(opc){
            case 1:
                createFactura();
                break;
            case 2:
                readFactura();
                break;
            case 3:
                editarFactura();
                break;
            case 4: 
                eliminarFactura();
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
    printf("Desea continuar? 1. Si 2. No\n");
    scanf("%d",&opc2);
    }while(opc2==1);
    return 0;
}