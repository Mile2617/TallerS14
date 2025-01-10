#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int num)
{
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

int leerEnteroPositivo(const char *mensaje)
{
    int valor;
    while (1)
    {
        printf("%s", mensaje);
        if (scanf("%d", &valor) != 1 || valor < 0 || getchar() != '\n')
        {
            printf("Error: Solo se permiten numeros enteros positivos.\n");
            while (getchar() != '\n');
        }
        else
        {
            return valor;
        }
    }
}

float leerFloatPositivo(const char *mensaje)
{
    float valor;
    while (1)
    {
        printf("%s", mensaje);
        if (scanf("%f", &valor) != 1 || valor < 0 || getchar() != '\n')
        {
            printf("Error: Solo se permiten numeros positivos.\n");
            while (getchar() != '\n');
        }
        else
        {
            return valor;
        }
    }
}

void validarCantidadEntera(struct Producto *producto)
{
    float cantidad;
    do
    {
        cantidad = leerFloatPositivo("  Cantidad: ");
        if ((int)cantidad != cantidad)
        {
            printf("Error: La cantidad debe ser un numero entero.\n");
        }
    } while ((int)cantidad != cantidad);
    producto->cantidad = (int)cantidad;
}

void calcularTotalFactura(struct Factura *factura, float impuesto, float descuento)
{
    factura->total = 0;
    float subtotal = 0;
    for (int i = 0; i < factura->numProductos; i++)
    {
        subtotal += factura->productos[i].cantidad * factura->productos[i].precio;
    }
    float impuestos = subtotal * (impuesto / 100);
    float descuentos = subtotal * (descuento / 100);
    factura->total = subtotal + impuestos - descuentos;
    factura->subtotal = subtotal;
    factura->impuestos = impuestos;
    factura->descuentos = descuentos;
}

void imprimirFacturaPorNumero()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    int numeroFactura = leerEnteroPositivo("Ingrese el numero de factura a imprimir: ");
    struct Factura factura;
    int encontrado = 0;

    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = (struct Producto *)malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        if (factura.numeroFactura == numeroFactura)
        {
            imprimirFactura(&factura);
            encontrado = 1;
            free(factura.productos);
            break;
        }

        free(factura.productos);
    }

    if (!encontrado)
    {
        printf("No se encontro la factura con el numero proporcionado.\n");
    }

    fclose(file);
}

void saveFactura(struct Factura *factura)
{
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    fwrite(factura, sizeof(struct Factura), 1, file);
    fwrite(factura->productos, sizeof(struct Producto), factura->numProductos, file);
    fclose(file);
}

void createFactura(int *contadorFactura)
{
    struct Factura factura;
    factura.numeroFactura = ++(*contadorFactura);

    printf("Nombre del cliente: ");
    getchar();  // Limpia el buffer antes de leer el nombre
    leerCadena(factura.nombre, 20);

    factura.cedula = leerEnteroPositivo("Cedula del cliente: ");
    factura.numProductos = leerEnteroPositivo("Numero de productos: ");

    factura.productos = malloc(factura.numProductos * sizeof(struct Producto));

    for (int i = 0; i < factura.numProductos; i++)
    {
        printf("Producto #%d\n", i + 1);
        printf("  Nombre: ");
        getchar();  // Limpia el buffer antes de leer el nombre del producto
        leerCadena(factura.productos[i].nombre, 50);
        validarCantidadEntera(&factura.productos[i]);
        factura.productos[i].precio = leerFloatPositivo("  Precio: ");
    }

    float impuesto = leerFloatPositivo("Ingrese el porcentaje de impuesto: ");
    float descuento = leerFloatPositivo("Ingrese el porcentaje de descuento: ");

    calcularTotalFactura(&factura, impuesto, descuento);
    saveFactura(&factura);
    imprimirFactura(&factura);

    free(factura.productos);
}

void readFactura()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    struct Factura factura;
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = (struct Producto *)malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);
        imprimirFactura(&factura);
        free(factura.productos);
    }

    fclose(file);
}

void buscarFactura()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    int opcion = leerEnteroPositivo("\nBuscar por:\n1. Numero de factura\n2. Cedula\n3. Nombre del cliente\nOpcion: ");
    struct Factura factura;
    char nombre[20];
    int cedula, numeroFactura, encontrado = 0;

    // Corrección en la captura del nombre
    if (opcion == 1)
    {
        numeroFactura = leerEnteroPositivo("Ingrese el numero de factura: ");
    }
    else if (opcion == 2)
    {
        cedula = leerEnteroPositivo("Ingrese la cedula: ");
    }
    else if (opcion == 3)
    {
        printf("Ingrese el nombre del cliente: ");
        getchar();  // Limpia el buffer antes de leer el nombre
        leerCadena(nombre, 20);
    }
    else
    {
        printf("Opcion invalida.\n");
        fclose(file);
        return;
    }
     while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        // Corrección en la comparación de nombres
        if ((opcion == 1 && factura.numeroFactura == numeroFactura) ||
            (opcion == 2 && factura.cedula == cedula) ||
            (opcion == 3 && strcmp(factura.nombre, nombre) == 0))
        {
            imprimirFactura(&factura);
            encontrado = 1;
        }

        free(factura.productos);
    }

    if (!encontrado)
    {
        printf("No se encontro una factura con los datos proporcionados.\n");
    }

    fclose(file);
}


void editarFactura()
{
    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    int numeroFactura = leerEnteroPositivo("Ingrese el numero de factura a editar: ");
    struct Factura factura;
    int encontrado = 0;

    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = (struct Producto *)malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        if (factura.numeroFactura == numeroFactura)
        {
            encontrado = 1;
            printf("Factura encontrada.\n");
            imprimirFactura(&factura);

            printf("Seleccione una opcion:\n");
            printf("1. Agregar producto\n2. Eliminar producto\n3. Editar producto\n4. Salir\nOpcion: ");

            int opcion;
            scanf("%d", &opcion);
            getchar();

            switch (opcion)
            {
                case 1:
                {
                    factura.numProductos++;
                    factura.productos = realloc(factura.productos, factura.numProductos * sizeof(struct Producto));
                    printf("Ingrese los datos del nuevo producto:\n");
                    printf("  Nombre: ");
                    leerCadena(factura.productos[factura.numProductos - 1].nombre, 50);
                    validarCantidadEntera(&factura.productos[factura.numProductos - 1]);
                    factura.productos[factura.numProductos - 1].precio = leerFloatPositivo("  Precio: ");
                    break;
                }
                case 2:
                {
                    int prodIndex = leerEnteroPositivo("Ingrese el numero del producto a eliminar: ") - 1;
                    if (prodIndex >= 0 && prodIndex < factura.numProductos)
                    {
                        for (int i = prodIndex; i < factura.numProductos - 1; i++)
                        {
                            factura.productos[i] = factura.productos[i + 1];
                        }
                        factura.numProductos--;
                        factura.productos = realloc(factura.productos, factura.numProductos * sizeof(struct Producto));
                        printf("Producto eliminado correctamente.\n");
                    }
                    else
                    {
                        printf("Indice de producto invalido.\n");
                    }
                    break;
                }
                case 3:
                {
                    int prodIndex = leerEnteroPositivo("Ingrese el numero del producto a editar: ") - 1;
                    if (prodIndex >= 0 && prodIndex < factura.numProductos)
                    {
                        printf("Nuevo nombre del producto: ");
                        leerCadena(factura.productos[prodIndex].nombre, 50);
                        validarCantidadEntera(&factura.productos[prodIndex]);
                        factura.productos[prodIndex].precio = leerFloatPositivo("Nuevo precio: ");
                    }
                    else
                    {
                        printf("Indice de producto invalido.\n");
                    }
                    break;
                }
                case 4:
                    printf("Saliendo del menu de edicion.\n");
                    break;
                default:
                    printf("Opcion invalida.\n");
                    break;
            }

            calcularTotalFactura(&factura, 0, 0);
            fseek(file, -((long)sizeof(struct Factura) + (factura.numProductos * sizeof(struct Producto))), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            fwrite(factura.productos, sizeof(struct Producto), factura.numProductos, file);
            fflush(file);

            printf("Factura actualizada correctamente.\n");
        }

        free(factura.productos);
    }

    if (!encontrado)
    {
        printf("No se encontro la factura con el numero proporcionado.\n");
    }

    fclose(file);
}



void eliminarFactura()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo o el archivo no existe.\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (tempFile == NULL)
    {
        printf("Error al crear el archivo temporal.\n");
        fclose(file);
        return;
    }

    int numeroFactura = leerEnteroPositivo("Ingrese el numero de factura a eliminar: ");
    struct Factura factura;
    int encontrado = 0;

    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        if (factura.numeroFactura == numeroFactura)
        {
            encontrado = 1;
            printf("Factura #%d eliminada correctamente.\n", numeroFactura);
            free(factura.productos);
            continue; // No escribir esta factura en el archivo temporal
        }

        fwrite(&factura, sizeof(struct Factura), 1, tempFile);
        fwrite(factura.productos, sizeof(struct Producto), factura.numProductos, tempFile);
        free(factura.productos);
    }

    fclose(file);
    fclose(tempFile);

    if (!encontrado)
    {
        printf("No se encontro una factura con el numero proporcionado.\n");
        remove("temp.dat");
    }
    else
    {
        remove("factura.dat");
        rename("temp.dat", "factura.dat");
    }
}

void reporteResumen()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    float totalFacturado = 0;
    int totalFacturas = 0;
    char mejorCliente[20] = "";
    float mayorTotal = 0;

    struct Factura factura;
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        totalFacturado += factura.total;
        totalFacturas++;

        if (factura.total > mayorTotal)
        {
            mayorTotal = factura.total;
            strcpy(mejorCliente, factura.nombre);
        }

        free(factura.productos);
    }

    fclose(file);

    printf("\n--- RESUMEN ---\n");
    printf("Total facturado: %.2f\n", totalFacturado);
    printf("Numero total de facturas: %d\n", totalFacturas);
    printf("Mejor cliente: %s con un total de %.2f\n", mejorCliente, mayorTotal);
}
