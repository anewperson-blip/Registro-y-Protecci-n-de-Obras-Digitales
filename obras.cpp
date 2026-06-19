/*
 * Implementacion: persistencia binaria, CRUD, seeding,
 * Bubble Sort optimizado, Merge Sort, busqueda binaria
 * iterativa y recursiva con metricas, reporte HTML/PDF.

 */

#include "obras.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// ==================== AYUDA INTERNA ====================

// Copia un string C++ a un char[] con limite de tamano
static void copiarStr(char dest[], const string &src, int tam) {
    strncpy(dest, src.c_str(), tam - 1);
    dest[tam - 1] = '\0';
}

// ==================== PERSISTENCIA BINARIA ====================

/**
 * Propósito  : Lee TODOS los registros del archivo binario (activos e inactivos).
 * Parametro  : obras[] - arreglo pasado POR REFERENCIA
 * Retorno    : cantidad total de registros leidos
 */
int cargarTodas(ObraDigital obras[]) {
    ifstream archivo(ARCHIVO_OBRAS.c_str(), ios::binary);
    if (!archivo.is_open()) return 0;

    int count = 0;
    while (count < MAX_OBRAS) {
        if (!archivo.read(reinterpret_cast<char*>(&obras[count]), sizeof(ObraDigital)))
            break;
        count++;
    }
    archivo.close();
    return count;
}

/**
 * Propósito  : Lee solo registros con activo==1 del archivo binario.
 * Parametro  : obras[] - arreglo POR REFERENCIA (solo activos)
 * Retorno    : cantidad de obras activas
 */
int cargarObras(ObraDigital obras[]) {
    ObraDigital todos[MAX_OBRAS];
    int total   = cargarTodas(todos);
    int activos = 0;
    for (int i = 0; i < total; i++) {
        if (todos[i].activo == 1)
            obras[activos++] = todos[i];
    }
    return activos;
}

/**
 * Propósito  : Escribe el arreglo completo en el archivo binario con structs.
 *              Cada struct se serializa directamente en bytes (campos char[]).
 * Parametros : obras[] POR REFERENCIA, n = numero de elementos a escribir
 */
void guardarObras(ObraDigital obras[], int n) {
    ofstream archivo(ARCHIVO_OBRAS.c_str(), ios::binary | ios::trunc);
    for (int i = 0; i < n; i++) {
        archivo.write(reinterpret_cast<const char*>(&obras[i]), sizeof(ObraDigital));
    }
    archivo.close();
}

// ==================== SEEDING (Carga Minima Automatizada) ====================

/**
 * Propósito  : Detecta si el archivo esta vacio o no existe y carga automaticamente
 *              10 obras de prueba, permitiendo probar ordenacion y busqueda
 *              sin necesidad de ingresar registros manualmente.
 */
void sembrarDatos() {
    ifstream prueba(ARCHIVO_OBRAS.c_str(), ios::binary);
    bool vacio = !prueba.is_open() || prueba.peek() == ifstream::traits_type::eof();
    prueba.close();
    if (!vacio) return;

    cout << "\n[SISTEMA] Primera ejecucion detectada. Cargando 10 obras de prueba...\n";

    const int N = 10;
    // Datos de semilla: {id, titulo, autor, tipo, fecha, registro, licencia, activo}
    struct Semilla { int id; const char *titulo, *autor, *tipo, *fecha, *registro, *licencia; };
    Semilla datos[N] = {
        {1,  "Logo Corporativo UCB",        "Ana Flores",    "logotipo",    "15/03/2022", "REG-001", "Copyright"       },
        {2,  "Ilustracion Fantasia",         "Carlos Rios",   "ilustracion", "20/06/2021", "REG-002", "Creative Commons"},
        {3,  "Fotografia Paisaje Andino",    "Maria Vargas",  "fotografia",  "10/01/2023", "REG-003", "Royalty-Free"   },
        {4,  "Animacion Intro Empresarial",  "Pedro Huanca",  "animacion",   "05/09/2020", "REG-004", "Copyright"       },
        {5,  "Diseno Identidad Visual",      "Sofia Mamani",  "logotipo",    "28/11/2022", "REG-005", "Creative Commons"},
        {6,  "Arte Digital Abstracto",       "Luis Condori",  "ilustracion", "14/04/2021", "REG-006", "Copyright"       },
        {7,  "Fotografia Retrato Urbano",    "Carla Ponce",   "fotografia",  "03/07/2023", "REG-007", "Royalty-Free"   },
        {8,  "Animacion Educativa Quimica",  "Diego Alcon",   "animacion",   "19/02/2022", "REG-008", "Copyright"       },
        {9,  "Iconografia App Movil",        "Valeria Cruz",  "logotipo",    "30/08/2020", "REG-009", "Creative Commons"},
        {10, "Composicion Digital Musical",  "Roberto Inca",  "ilustracion", "11/12/2023", "REG-010", "Copyright"       }
    };

    ObraDigital semillas[N];
    for (int i = 0; i < N; i++) {
        semillas[i].id     = datos[i].id;
        semillas[i].activo = 1;
        copiarStr(semillas[i].titulo,   datos[i].titulo,   100);
        copiarStr(semillas[i].autor,    datos[i].autor,    80);
        copiarStr(semillas[i].tipo,     datos[i].tipo,     30);
        copiarStr(semillas[i].fecha,    datos[i].fecha,    12);
        copiarStr(semillas[i].registro, datos[i].registro, 30);
        copiarStr(semillas[i].licencia, datos[i].licencia, 40);
    }

    guardarObras(semillas, N);
    cout << "[SISTEMA] 10 obras de prueba cargadas exitosamente.\n\n";
}

// ==================== CRUD ====================

/**
 * Propósito  : Solicita datos al usuario y agrega una nueva ObraDigital
 *              al archivo binario. El ID se genera automaticamente.
 */
void registrarObra() {
    ObraDigital todos[MAX_OBRAS];
    int total = cargarTodas(todos);

    if (total >= MAX_OBRAS) {
        mostrarError("Limite maximo de obras alcanzado");
        return;
    }

    ObraDigital nueva;
    memset(&nueva, 0, sizeof(nueva));
    nueva.activo = 1;

    // ID = maximo existente + 1
    int maxId = 0;
    for (int i = 0; i < total; i++)
        if (todos[i].id > maxId) maxId = todos[i].id;
    nueva.id = maxId + 1;

    limpiarPantalla();
    cout << "\n+=======================================+\n";
    cout << "|       REGISTRAR NUEVA OBRA            |\n";
    cout << "+=======================================+\n\n";
    cout << "ID asignado: " << nueva.id << "\n\n";

    string input;
    cin.ignore(1000, '\n');

    cout << "Titulo: ";
    getline(cin, input);
    copiarStr(nueva.titulo, input, 100);

    cout << "Autor: ";
    getline(cin, input);
    copiarStr(nueva.autor, input, 80);

    cout << "Tipo (logotipo/ilustracion/fotografia/animacion): ";
    getline(cin, input);
    copiarStr(nueva.tipo, input, 30);

    do {
        cout << "Fecha de creacion (DD/MM/AAAA): ";
        getline(cin, input);
        if (!validarFecha(input)) cout << "   Formato invalido.\n";
    } while (!validarFecha(input));
    copiarStr(nueva.fecha, input, 12);

    cout << "Codigo de registro (propiedad intelectual): ";
    getline(cin, input);
    copiarStr(nueva.registro, input, 30);

    cout << "Licencia (Copyright/Creative Commons/Royalty-Free): ";
    getline(cin, input);
    copiarStr(nueva.licencia, input, 40);

    todos[total] = nueva;
    guardarObras(todos, total + 1);
    mostrarExito("Obra registrada con ID " + to_string(nueva.id));
}

/**
 * Propósito  : Muestra en consola el listado formateado de todas las obras activas.
 */
void listarObras() {
    ObraDigital obras[MAX_OBRAS];
    int n = cargarObras(obras);

    limpiarPantalla();
    cout << "\n";
    cout << "+============================================================================================+\n";
    cout << "|                      LISTADO DE OBRAS DIGITALES REGISTRADAS                               |\n";
    cout << "+============================================================================================+\n\n";

    if (n == 0) {
        cout << "   No hay obras activas registradas.\n";
        pausar();
        return;
    }

    cout << " ID  | TITULO                    | AUTOR              | TIPO          | FECHA      | LICENCIA\n";
    cout << "-----+---------------------------+--------------------+---------------+------------+-------------------\n";

    for (int i = 0; i < n; i++) {
        string tit(obras[i].titulo);
        string aut(obras[i].autor);
        string tip(obras[i].tipo);

        cout << " " << obras[i].id;
        for (int j = to_string(obras[i].id).length(); j < 4; j++) cout << " ";
        cout << "| ";

        if (tit.length() > 25) tit = tit.substr(0,22) + "...";
        cout << tit;
        for (int j = tit.length(); j < 26; j++) cout << " ";
        cout << "| ";

        if (aut.length() > 18) aut = aut.substr(0,15) + "...";
        cout << aut;
        for (int j = aut.length(); j < 19; j++) cout << " ";
        cout << "| ";

        if (tip.length() > 13) tip = tip.substr(0,10) + "...";
        cout << tip;
        for (int j = tip.length(); j < 14; j++) cout << " ";
        cout << "| ";

        cout << obras[i].fecha << " | ";
        cout << obras[i].licencia << "\n";
    }

    cout << "\n   Total de obras activas: " << n << "\n";
    pausar();
}

/**
 * Propósito  : Busca una obra activa por ID y permite editar sus campos.
 *              Deja en blanco para conservar el valor actual.
 */
void modificarObra() {
    ObraDigital todos[MAX_OBRAS];
    int total = cargarTodas(todos);

    if (total == 0) { mostrarError("No hay obras registradas"); return; }

    int id;
    cout << "\nID de la obra a modificar: ";
    cin >> id;
    cin.ignore(1000, '\n');

    int indice = -1;
    for (int i = 0; i < total; i++) {
        if (todos[i].id == id && todos[i].activo == 1) { indice = i; break; }
    }

    if (indice == -1) { mostrarError("Obra no encontrada o eliminada"); return; }

    limpiarPantalla();
    cout << "\n+=======================================+\n";
    cout << "|         MODIFICAR OBRA                |\n";
    cout << "+=======================================+\n\n";
    cout << "Obra actual:\n";
    mostrarObra(todos[indice]);
    cout << "\n(Deje en blanco para conservar el valor actual)\n\n";

    string input;

    cout << "Nuevo titulo [" << todos[indice].titulo << "]: ";
    getline(cin, input);
    if (!input.empty()) copiarStr(todos[indice].titulo, input, 100);

    cout << "Nuevo autor [" << todos[indice].autor << "]: ";
    getline(cin, input);
    if (!input.empty()) copiarStr(todos[indice].autor, input, 80);

    cout << "Nuevo tipo [" << todos[indice].tipo << "]: ";
    getline(cin, input);
    if (!input.empty()) copiarStr(todos[indice].tipo, input, 30);

    cout << "Nueva fecha [" << todos[indice].fecha << "]: ";
    getline(cin, input);
    if (!input.empty()) {
        if (validarFecha(input)) copiarStr(todos[indice].fecha, input, 12);
        else cout << "   Fecha invalida, se conserva la anterior.\n";
    }

    cout << "Nuevo registro [" << todos[indice].registro << "]: ";
    getline(cin, input);
    if (!input.empty()) copiarStr(todos[indice].registro, input, 30);

    cout << "Nueva licencia [" << todos[indice].licencia << "]: ";
    getline(cin, input);
    if (!input.empty()) copiarStr(todos[indice].licencia, input, 40);

    guardarObras(todos, total);
    mostrarExito("Obra modificada exitosamente");
}

/**
 * Propósito  : Realiza BAJA LOGICA (activo=0) del registro con el ID dado.
 *              El struct permanece en el archivo pero activo=0 lo oculta.
 */
void eliminarObra() {
    ObraDigital todos[MAX_OBRAS];
    int total = cargarTodas(todos);

    if (total == 0) { mostrarError("No hay obras registradas"); return; }

    int id;
    cout << "\nID de la obra a eliminar: ";
    cin >> id;
    cin.ignore(1000, '\n');

    int indice = -1;
    for (int i = 0; i < total; i++) {
        if (todos[i].id == id && todos[i].activo == 1) { indice = i; break; }
    }

    if (indice == -1) { mostrarError("Obra no encontrada o ya eliminada"); return; }

    limpiarPantalla();
    cout << "\n+=======================================+\n";
    cout << "|      ELIMINAR OBRA  (Baja Logica)     |\n";
    cout << "+=======================================+\n\n";
    cout << "VA A ELIMINAR:\n";
    mostrarObra(todos[indice]);

    cout << "\nConfirmar eliminacion? (s/n): ";
    char conf; cin >> conf; cin.ignore(1000, '\n');

    if (conf == 's' || conf == 'S') {
        todos[indice].activo = 0;
        guardarObras(todos, total);
        mostrarExito("Obra eliminada (baja logica aplicada)");
    } else {
        mostrarExito("Operacion cancelada");
    }
}

/**
 * Propósito  : Imprime todos los campos de una obra con borde visual.
 * Parametro  : obra - pasada POR REFERENCIA CONSTANTE (solo lectura, sin copia)
 */
void mostrarObra(const ObraDigital &obra) {
    cout << "+-----------------------------------------------+\n";
    cout << "| ID       : " << obra.id        << "\n";
    cout << "| Titulo   : " << obra.titulo    << "\n";
    cout << "| Autor    : " << obra.autor     << "\n";
    cout << "| Tipo     : " << obra.tipo      << "\n";
    cout << "| Fecha    : " << obra.fecha     << "\n";
    cout << "| Registro : " << obra.registro  << "\n";
    cout << "| Licencia : " << obra.licencia  << "\n";
    cout << "| Estado   : " << (obra.activo ? "Activo" : "Eliminado") << "\n";
    cout << "+-----------------------------------------------+\n";
}

// ==================== BUBBLE SORT OPTIMIZADO ====================

/**
 * Propósito  : Ordena obras[] alfabeticamente por titulo usando Bubble Sort
 *              con bandera de intercambio (optimizacion de terminacion temprana).
 * Parametros (POR REFERENCIA):
 *   obras[]       - arreglo a ordenar (se modifica en sitio)
 *   n             - cantidad de elementos
 *   comparaciones - sale con el total de comparaciones realizadas
 *   intercambios  - sale con el total de intercambios realizados
 * Retorno multiple: comparaciones e intercambios via parametros por referencia.
 */
void bubbleSortOptimizado(ObraDigital obras[], int n,
                          int &comparaciones, int &intercambios) {
    comparaciones = 0;
    intercambios  = 0;
    if (n <= 1) return;

    bool huboIntercambio;
    for (int i = 0; i < n - 1; i++) {
        huboIntercambio = false;
        for (int j = 0; j < n - 1 - i; j++) {
            comparaciones++;
            if (string(obras[j].titulo) > string(obras[j+1].titulo)) {
                ObraDigital temp = obras[j];
                obras[j]         = obras[j+1];
                obras[j+1]       = temp;
                intercambios++;
                huboIntercambio  = true;
            }
        }
        if (!huboIntercambio) break;  // ya ordenado: terminar antes
    }
}

// ==================== MERGE SORT (INTERCALACION) ====================

/**
 * Propósito  : Combina dos sub-arreglos ya ordenados en el arreglo principal.
 *              Funcion auxiliar de mergeSort.
 * Parametros (POR REFERENCIA):
 *   obras[]              - arreglo principal
 *   inicio, medio, fin   - limites de los dos sub-arreglos
 *   comparaciones        - acumula el conteo de comparaciones
 */
void merge(ObraDigital obras[], int inicio, int medio, int fin, int &comparaciones) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    ObraDigital izq[MAX_OBRAS], der[MAX_OBRAS];
    for (int i = 0; i < n1; i++) izq[i] = obras[inicio + i];
    for (int j = 0; j < n2; j++) der[j] = obras[medio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        comparaciones++;
        if (string(izq[i].titulo) <= string(der[j].titulo)) obras[k++] = izq[i++];
        else                                                 obras[k++] = der[j++];
    }
    while (i < n1) obras[k++] = izq[i++];
    while (j < n2) obras[k++] = der[j++];
}

/**
 * Propósito  : Ordena obras[] por titulo usando Intercalacion (divide y venceras).
 * Parametros (POR REFERENCIA):
 *   obras[]         - arreglo a ordenar
 *   inicio, fin     - limites del sub-arreglo actual
 *   comparaciones   - acumula el conteo de comparaciones en todas las llamadas
 */
void mergeSort(ObraDigital obras[], int inicio, int fin, int &comparaciones) {
    if (inicio < fin) {
        int medio = (inicio + fin) / 2;
        mergeSort(obras, inicio, medio,   comparaciones);
        mergeSort(obras, medio+1, fin,    comparaciones);
        merge    (obras, inicio, medio, fin, comparaciones);
    }
}

/**
 * Propósito  : Submenu que ejecuta ambos algoritmos sobre copias independientes
 *              del arreglo, muestra reporte comparativo de eficiencia y guarda
 *              el resultado del algoritmo elegido.
 */
void mostrarMenuOrdenamiento() {
    ObraDigital obras[MAX_OBRAS];
    int n = cargarObras(obras);

    if (n == 0) { mostrarError("No hay obras para ordenar"); return; }

    limpiarPantalla();
    cout << "\n+=======================================+\n";
    cout << "|   MODULO COMPARATIVO DE ORDENACION    |\n";
    cout << "+=======================================+\n";
    cout << "| 1. Bubble Sort Optimizado             |\n";
    cout << "| 2. Merge Sort (Intercalacion)         |\n";
    cout << "| 3. Reporte comparativo (ambos)        |\n";
    cout << "+=======================================+\n";
    cout << "Opcion: ";

    int opcion; cin >> opcion; cin.ignore(1000, '\n');

    ObraDigital copia1[MAX_OBRAS], copia2[MAX_OBRAS];
    for (int i = 0; i < n; i++) { copia1[i] = obras[i]; copia2[i] = obras[i]; }

    int comp1 = 0, inter1 = 0, comp2 = 0;

    if (opcion == 1) {
        bubbleSortOptimizado(copia1, n, comp1, inter1);
        guardarObras(copia1, n);
        cout << "\n[Bubble Sort] Comparaciones: " << comp1 << " | Intercambios: " << inter1 << "\n";
        mostrarExito("Obras ordenadas con Bubble Sort");

    } else if (opcion == 2) {
        mergeSort(copia2, 0, n-1, comp2);
        guardarObras(copia2, n);
        cout << "\n[Merge Sort] Comparaciones: " << comp2 << "\n";
        mostrarExito("Obras ordenadas con Merge Sort");

    } else if (opcion == 3) {
        bubbleSortOptimizado(copia1, n, comp1, inter1);
        mergeSort(copia2, 0, n-1, comp2);

        limpiarPantalla();
        cout << "\n+=====================================================+\n";
        cout << "|          REPORTE COMPARATIVO DE ORDENACION          |\n";
        cout << "+=====================================================+\n";
        cout << "|  Registros ordenados : " << n << "\n";
        cout << "|  Criterio            : Titulo (alfabetico)\n";
        cout << "+-----------------------------------------------------+\n";
        cout << "|  Algoritmo          | Comparaciones | Intercambios  |\n";
        cout << "+---------------------+---------------+---------------+\n";
        cout << "|  Bubble Sort Opt.   | " << comp1;
        for(int j=to_string(comp1).length();j<14;j++) cout<<" ";
        cout << "| " << inter1 << "\n";
        cout << "|  Merge Sort         | " << comp2;
        for(int j=to_string(comp2).length();j<14;j++) cout<<" ";
        cout << "| N/A\n";
        cout << "+---------------------+---------------+---------------+\n";
        cout << "|  VEREDICTO: ";
        if (comp2 < comp1) cout << "Merge Sort fue mas eficiente               |\n";
        else if (comp1 < comp2) cout << "Bubble Sort fue mas eficiente           |\n";
        else cout << "Ambos algoritmos tuvieron igual eficiencia   |\n";
        cout << "+=====================================================+\n";

        cout << "\nGuardar con Merge Sort? (s/n): ";
        char g; cin >> g; cin.ignore(1000, '\n');
        if (g == 's' || g == 'S') {
            guardarObras(copia2, n);
            mostrarExito("Archivo guardado ordenado con Merge Sort");
        }
        pausar();
    } else {
        mostrarError("Opcion invalida");
    }
}

// ==================== BUSQUEDA BINARIA ====================

/**
 * Propósito  : Busca un titulo en el arreglo ORDENADO usando ciclo while.
 * Parametros (POR REFERENCIA - retorno multiple):
 *   indice        - posicion donde se encontro la obra (-1 si no existe)
 *   comparaciones - cantidad de iteraciones del ciclo realizadas
 */
void busquedaBinariaIterativa(ObraDigital obras[], int n,
                              const string &titulo,
                              int &indice, int &comparaciones) {
    indice = -1; comparaciones = 0;
    int inicio = 0, fin = n - 1;

    while (inicio <= fin) {
        comparaciones++;
        int medio = (inicio + fin) / 2;
        string tituloMedio(obras[medio].titulo);

        if (tituloMedio == titulo)       { indice = medio; return; }
        else if (tituloMedio > titulo)     fin = medio - 1;
        else                               inicio = medio + 1;
    }
}

/**
 * Propósito  : Busca un titulo en el arreglo ORDENADO de forma RECURSIVA.
 *              Se llama a si misma reduciendo el subarreglo en cada paso.
 * Parametros (POR REFERENCIA - retorno multiple):
 *   indice   - posicion encontrada (-1 si no existe)
 *   llamadas - contador de llamadas recursivas realizadas
 */
void busquedaBinariaRecursiva(ObraDigital obras[], int inicio, int fin,
                              const string &titulo,
                              int &indice, int &llamadas) {
    llamadas++;

    if (inicio > fin) { indice = -1; return; }

    int medio = (inicio + fin) / 2;
    string tituloMedio(obras[medio].titulo);

    if (tituloMedio == titulo) {
        indice = medio;
    } else if (tituloMedio > titulo) {
        busquedaBinariaRecursiva(obras, inicio, medio-1, titulo, indice, llamadas);
    } else {
        busquedaBinariaRecursiva(obras, medio+1, fin,    titulo, indice, llamadas);
    }
}

/**
 * Propósito  : Submenu que ordena el arreglo por titulo, solicita el termino
 *              de busqueda y ofrece tres modos: iterativo, recursivo y comparativa.
 */
void menuBusqueda() {
    ObraDigital obras[MAX_OBRAS];
    int n = cargarObras(obras);

    if (n == 0) { mostrarError("No hay obras registradas"); return; }

    // Ordenar por titulo (condicion previa para busqueda binaria)
    int c = 0, x = 0;
    bubbleSortOptimizado(obras, n, c, x);

    limpiarPantalla();
    cout << "\n+=======================================+\n";
    cout << "|      BUSQUEDA BINARIA DE OBRAS        |\n";
    cout << "+=======================================+\n";
    cout << "| 1. Busqueda Binaria ITERATIVA         |\n";
    cout << "| 2. Busqueda Binaria RECURSIVA         |\n";
    cout << "| 3. Comparar ambos metodos             |\n";
    cout << "+=======================================+\n";
    cout << "Opcion: ";

    int opcion; cin >> opcion; cin.ignore(1000, '\n');
    if (opcion < 1 || opcion > 3) { mostrarError("Opcion invalida"); return; }

    cout << "\nTitulo exacto a buscar: ";
    string titulo; getline(cin, titulo);

    int indIter = -1, compIter = 0;
    int indRec  = -1, llamRec  = 0;

    if (opcion == 1 || opcion == 3)
        busquedaBinariaIterativa(obras, n, titulo, indIter, compIter);
    if (opcion == 2 || opcion == 3) {
        indRec = -1; llamRec = 0;
        busquedaBinariaRecursiva(obras, 0, n-1, titulo, indRec, llamRec);
    }

    limpiarPantalla();
    cout << "\n+=====================================================+\n";
    cout << "|              RESULTADO DE BUSQUEDA                 |\n";
    cout << "+=====================================================+\n";

    if (opcion == 1) {
        cout << "| Metodo: ITERATIVO\n";
        cout << "| Iteraciones realizadas: " << compIter << "\n";
        if (indIter != -1) { cout << "| OBRA ENCONTRADA:\n"; mostrarObra(obras[indIter]); }
        else cout << "| Obra NO encontrada.\n";

    } else if (opcion == 2) {
        cout << "| Metodo: RECURSIVO\n";
        cout << "| Llamadas recursivas: " << llamRec << "\n";
        if (indRec != -1) { cout << "| OBRA ENCONTRADA:\n"; mostrarObra(obras[indRec]); }
        else cout << "| Obra NO encontrada.\n";

    } else {
        cout << "| Metodo        | Resultado     | Pasos\n";
        cout << "+---------------+---------------+------------------\n";
        cout << "| Iterativo     | " << (indIter!=-1?"Encontrado    ":"No encontrado ")
             << "| " << compIter << " iteraciones\n";
        cout << "| Recursivo     | " << (indRec!=-1 ?"Encontrado    ":"No encontrado ")
             << "| " << llamRec << " llamadas\n";
        if (indIter != -1) { cout << "\nOBRA ENCONTRADA:\n"; mostrarObra(obras[indIter]); }
        else cout << "\nObra NO encontrada.\n";
    }

    pausar();
}

// ==================== REPORTE PDF (HTML imprimible) ====================

/**
 * generarReportePDF
 * Propósito  : Exporta el listado de obras activas a un archivo HTML con
 *              estilos de impresion. Desde el navegador, Ctrl+P -> "Guardar como PDF"
 *              genera el PDF final requerido por el examen.
 */
void generarReportePDF() {
    ObraDigital obras[MAX_OBRAS];
    int n = cargarObras(obras);

    string archivo = "reporte_obras.html";
    ofstream html(archivo.c_str());
    if (!html.is_open()) { mostrarError("No se pudo crear el reporte"); return; }

    string fechaHoy = obtenerFechaActual();

    html << "<!DOCTYPE html><html lang='es'><head>\n"
         << "<meta charset='UTF-8'><title>Reporte Obras Digitales</title>\n"
         << "<style>\n"
         << "body{font-family:Arial,sans-serif;font-size:12px;margin:20px}\n"
         << "h1{text-align:center;color:#2c3e50;font-size:18px}\n"
         << "h2{text-align:center;color:#555;font-size:13px}\n"
         << "p.info{text-align:center;color:#777}\n"
         << "table{width:100%;border-collapse:collapse;margin-top:20px}\n"
         << "th{background:#2c3e50;color:#fff;padding:8px;text-align:left}\n"
         << "td{padding:6px 8px;border-bottom:1px solid #ddd}\n"
         << "tr:nth-child(even){background:#f2f2f2}\n"
         << ".total{font-weight:bold;margin-top:10px}\n"
         << "@media print{body{margin:10mm}}\n"
         << "</style></head><body>\n"
         << "<h1>SISTEMA DE REGISTRO Y PROTECCION DE OBRAS DIGITALES</h1>\n"
         << "<h2>Universidad Catolica Boliviana \"San Pablo\" &mdash; Grupo 8</h2>\n"
         << "<p class='info'>Reporte generado el: " << fechaHoy << "</p>\n"
         << "<table><tr><th>ID</th><th>Titulo</th><th>Autor</th><th>Tipo</th>"
         << "<th>Fecha</th><th>Registro</th><th>Licencia</th></tr>\n";

    for (int i = 0; i < n; i++) {
        html << "<tr>"
             << "<td>" << obras[i].id       << "</td>"
             << "<td>" << obras[i].titulo   << "</td>"
             << "<td>" << obras[i].autor    << "</td>"
             << "<td>" << obras[i].tipo     << "</td>"
             << "<td>" << obras[i].fecha    << "</td>"
             << "<td>" << obras[i].registro << "</td>"
             << "<td>" << obras[i].licencia << "</td>"
             << "</tr>\n";
    }

    html << "</table>\n"
         << "<p class='total'>Total de obras registradas: " << n << "</p>\n"
         << "</body></html>\n";
    html.close();

    mostrarExito("Reporte generado: " + archivo + " -> Abrir en navegador y Ctrl+P para PDF");
}
