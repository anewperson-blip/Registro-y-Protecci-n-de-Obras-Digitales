#ifndef OBRAS_H
#define OBRAS_H

/*
 * Definicion del struct ObraDigital y prototipos de funciones.
 * Usa char[] de tamano fijo para permitir serializacion binaria directa.
 */

#include <string>
using namespace std;

// ==================== STRUCT PRINCIPAL ====================

/**
 * Representa una obra registrada en el sistema de proteccion.
 * Los campos de texto usan char[] de tamano fijo para poder escribirse
 * y leerse directamente con fstream en modo binario (fwrite/fread equivalente).
 *
 * Campos:
 *   id        - Identificador unico autoincremental
 *   titulo    - Nombre de la obra (max 100 chars)
 *   autor     - Nombre del creador (max 80 chars)
 *   tipo      - Categoria de la obra (max 30 chars)
 *   fecha     - Fecha en formato DD/MM/AAAA (max 12 chars)
 *   registro  - Codigo de propiedad intelectual (max 30 chars)
 *   licencia  - Tipo de licencia (max 40 chars)
 *   activo    - Baja logica: 1=activo, 0=eliminado
 */
struct ObraDigital {
    int  id;
    char titulo   [100];
    char autor    [80];
    char tipo     [30];
    char fecha    [12];
    char registro [30];
    char licencia [40];
    int  activo;
};

// ==================== CONSTANTES ====================

const int    MAX_OBRAS     = 200;
const string ARCHIVO_OBRAS = "data/obras.dat";

// ==================== PERSISTENCIA ====================

int  cargarObras (ObraDigital obras[]);   // solo activos
int  cargarTodas (ObraDigital obras[]);   // todos (incluye bajas)
void guardarObras(ObraDigital obras[], int n);

// ==================== SEEDING ====================

void sembrarDatos();

// ==================== CRUD ====================

void registrarObra();
void listarObras();
void modificarObra();
void eliminarObra();
void mostrarObra(const ObraDigital &obra);

// ==================== ORDENACION ====================

void bubbleSortOptimizado(ObraDigital obras[], int n,
                          int &comparaciones, int &intercambios);
void mergeSort(ObraDigital obras[], int inicio, int fin, int &comparaciones);
void merge    (ObraDigital obras[], int inicio, int medio, int fin, int &comparaciones);
void mostrarMenuOrdenamiento();

// ==================== BUSQUEDA ====================

void busquedaBinariaIterativa(ObraDigital obras[], int n,
                              const string &titulo,
                              int &indice, int &comparaciones);
void busquedaBinariaRecursiva(ObraDigital obras[], int inicio, int fin,
                              const string &titulo,
                              int &indice, int &llamadas);
void menuBusqueda();

// ==================== REPORTE ====================

void generarReportePDF();

#endif 