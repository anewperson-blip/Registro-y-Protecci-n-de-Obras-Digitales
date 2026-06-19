/*
 * Punto de entrada del programa. SOLO contiene llamadas a funciones.
 */

#include <iostream>
#include <cstdlib>
#include "obras.h"
#include "utilidades.h"

using namespace std;

// Prototipo del menu principal
void mostrarMenu();

// ==================== PUNTO DE ENTRADA ====================

int main() {
    // Crear carpeta data si no existe (portabilidad Windows/Linux)
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data");
#endif

    // Seeding automatico: si no hay datos, carga 10 obras de prueba
    sembrarDatos();

    int opcion;

    do {
        limpiarPantalla();
        mostrarMenu();
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            mostrarError("Ingrese un numero valido");
            continue;
        }

        limpiarPantalla();

        switch (opcion) {
            case 1: registrarObra();          break;
            case 2: listarObras();            break;
            case 3: modificarObra();          break;
            case 4: eliminarObra();           break;
            case 5: mostrarMenuOrdenamiento();break;
            case 6: menuBusqueda();           break;
            case 7: generarReportePDF();      break;
            case 0:
                cout << "\n+=======================================+\n";
                cout << "|   GRACIAS POR USAR EL SISTEMA        |\n";
                cout << "|   Registro de Proteccion de Obras    |\n";
                cout << "+=======================================+\n\n";
                break;
            default:
                mostrarError("Opcion invalida. Elija entre 0 y 7.");
        }

    } while (opcion != 0);

    return 0;
}

// ==================== MENU PRINCIPAL ====================

void mostrarMenu() {
    cout << "\n";
    cout << "+----------------------------------------------------+\n";
    cout << "|      SISTEMA DE PROTECCION DE OBRAS DIGITALES     |\n";
    cout << "+----------------------------------------------------+\n";
    cout << "|  [1]  REGISTRAR obra nueva                        |\n";
    cout << "|  [2]  LISTAR todas las obras                      |\n";
    cout << "|  [3]  MODIFICAR obra                              |\n";
    cout << "|  [4]  ELIMINAR obra (baja logica)                 |\n";
    cout << "|  [5]  ORDENAR obras (Bubble Sort / Merge Sort)    |\n";
    cout << "|  [6]  BUSCAR obra (Binaria iterativa / recursiva) |\n";
    cout << "|  [7]  GENERAR reporte PDF                         |\n";
    cout << "|  [0]  SALIR                                       |\n";
    cout << "+----------------------------------------------------+\n";
    cout << "Opcion: ";
}
