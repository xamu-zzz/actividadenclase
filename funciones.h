//primero esto para que no se repita el codigo
struct Producto{
    char nombre[50];
    int cantidad;
    float precio;
};
struct Factura{
    char nombre[50];
    int cedula;
    int numProd;
    struct Producto productos[5];
    float total;
    int estado;
};
int menu();
void save(struct Factura *factura);
void leerCadena(char *cadena, int numcaracteres);
void createFactura();
void readFactura();
int findFacturaByCedula(int cedula);
void editarFactura();
void eliminarFactura();
int existeCedula(int cedula);x