#include <stdio.h>
#include "funciones.h"

int menu()
{
    int opcion;
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1. Crear factura\n");
    printf("2. Leer facturas\n");
    printf("3. Editar factura\n");
    printf("4. Eliminar factura\n");
    printf("5. Buscar factura\n");
    printf("6. Reporte resumen\n");
    printf("7. Salir\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

int main()
{
    int opcion;
    int contadorFactura = 0;

    do
    {
        opcion = menu();
        switch (opcion)
        {
        case 1:
            createFactura(&contadorFactura);
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
        case 5:
            buscarFactura();
            break;
        case 6:
            reporteResumen();
            break;
        case 7:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 7);

    return 0;
}
