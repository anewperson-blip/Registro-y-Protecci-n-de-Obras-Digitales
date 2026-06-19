#ifndef UTILIDADES_H
#define UTILIDADES_H

/*
 * Prototipos de funciones auxiliares de interfaz y comparacion de fechas.
 */

#include <string>
using namespace std;

void limpiarPantalla();   //Limpia la consola segun el sistema operativo (Windows/Linux).

void pausar();   //Espera a que el usuario presione Enter para continuar.

/**
 * Compara dos fechas en formato DD/MM/AAAA.
 * Retorno: negativo si f1 < f2, 0 si iguales, positivo si f1 > f2.
 */
int compararFechas(const string &f1, const string &f2);

void mostrarError(const string &mensaje);   //Muestra un mensaje de error con formato visual y pausa.

void mostrarExito(const string &mensaje);    //Muestra un mensaje de exito con formato visual y pausa.

/**
 * Verifica que la cadena cumpla el formato DD/MM/AAAA.
 * Retorno: true si es valida.
 */
bool validarFecha(const string &fecha);

string obtenerFechaActual();   //Retorna la fecha del sistema como string en formato DD/MM/AAAA.

#endif 
