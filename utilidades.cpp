/*
 * Implementacion de funciones auxiliares de interfaz, comparacion y validacion.
 */

#include "utilidades.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==================== INTERFAZ ====================

void limpiarPantalla() {   //Detecta el SO y ejecuta el comando adecuado para limpiar la consola.
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {          // Muestra un mensaje y espera Enter del usuario.
    cout << "\n+---------------------------------------+\n";
    cout << "|  Presione Enter para continuar...    |\n";
    cout << "+---------------------------------------+\n";
    cin.ignore(1000, '\n');
    cin.get();
}

/**
 * Propósito  : Muestra mensaje de error con borde visual.
 * Parametro  : mensaje - texto a mostrar
 */
void mostrarError(const string &mensaje) {
    cout << "\n+--------------------------------------------------+\n";
    cout << "|  [ERROR] " << mensaje << "\n";
    cout << "+--------------------------------------------------+\n";
    pausar();
}

/**
 * mostrarExito
 * Propósito  : Muestra mensaje de exito con borde visual.
 * Parametro  : mensaje - texto a mostrar
 */
void mostrarExito(const string &mensaje) {
    cout << "\n+--------------------------------------------------+\n";
    cout << "|  [OK] " << mensaje << "\n";
    cout << "+--------------------------------------------------+\n";
    pausar();
}

// ==================== FECHAS ====================

/**
 * Propósito  : Compara dos fechas en formato DD/MM/AAAA.
 * Parametros : f1, f2 - cadenas de fecha por referencia constante
 * Retorno    : negativo si f1 < f2, 0 si iguales, positivo si f1 > f2
 */
int compararFechas(const string &f1, const string &f2) {
    if (f1.length() < 10 || f2.length() < 10) return 0;

    // Extraer año (indices 6-9)
    int a1 = (f1[6]-'0')*1000 + (f1[7]-'0')*100 + (f1[8]-'0')*10 + (f1[9]-'0');
    int a2 = (f2[6]-'0')*1000 + (f2[7]-'0')*100 + (f2[8]-'0')*10 + (f2[9]-'0');
    if (a1 != a2) return a1 - a2;

    // Extraer mes (indices 3-4)
    int m1 = (f1[3]-'0')*10 + (f1[4]-'0');
    int m2 = (f2[3]-'0')*10 + (f2[4]-'0');
    if (m1 != m2) return m1 - m2;

    // Extraer dia (indices 0-1)
    int d1 = (f1[0]-'0')*10 + (f1[1]-'0');
    int d2 = (f2[0]-'0')*10 + (f2[1]-'0');
    return d1 - d2;
}

/**
 * Propósito  : Verifica que la cadena tenga formato DD/MM/AAAA valido.
 * Parametro  : fecha - cadena a validar
 * Retorno    : true si el formato y rangos son correctos
 */
bool validarFecha(const string &fecha) {
    if (fecha.length() != 10) return false;
    if (fecha[2] != '/' || fecha[5] != '/') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (fecha[i] < '0' || fecha[i] > '9') return false;
    }

    int dia  = (fecha[0]-'0')*10 + (fecha[1]-'0');
    int mes  = (fecha[3]-'0')*10 + (fecha[4]-'0');
    int anio = (fecha[6]-'0')*1000 + (fecha[7]-'0')*100
             + (fecha[8]-'0')*10   + (fecha[9]-'0');

    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (anio < 1900 || anio > 2100) return false;
    return true;
}

/**
 * Propósito  : Obtiene la fecha del sistema.
 * Retorno    : string con formato DD/MM/AAAA
 */
string obtenerFechaActual() {
    time_t ahora = time(0);
    tm *t = localtime(&ahora);
    char buf[40];
    // formato DD/MM/AAAA
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    return string(buf);
}
