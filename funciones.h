#ifndef FUNCIONES_H
#define FUNCIONES_H

struct Producto
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura
{
    int numeroFactura;
    char nombre[20];
    int cedula;
    int numProductos;
    float subtotal;
    float impuestos;
    float descuentos;
    float total;
    struct Producto *productos;
};

int menu();
void leerCadena(char *cadena, int num);
int leerEnteroPositivo(const char *mensaje);
float leerFloatPositivo(const char *mensaje);
void validarCantidadEntera(struct Producto *producto);
void calcularTotalFactura(struct Factura *factura, float impuesto, float descuento);
void imprimirFactura(struct Factura *factura);
void saveFactura(struct Factura *factura);
void createFactura(int *contadorFactura);
void readFactura();
void buscarFactura();
void editarFactura();
void eliminarFactura();
void reporteResumen();

#endif // FUNCIONES_H
