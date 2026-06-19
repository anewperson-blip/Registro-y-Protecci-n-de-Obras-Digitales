# Sistema de Registro y Protección de Obras Digitales

**Universidad Católica Boliviana "San Pablo"**  
Facultad de Ingeniería · Departamento de Ingeniería de Sistemas  
Asignatura: Programación I — SIS-112 | Gestión 1/2026  

---

## Integrantes del Grupo 8

| _Alejandra Romina Arguedas Abastoflor_ |
| _Sebastian Garnica Kauht_ |

---

## Dominio de datos y structs utilizados

El sistema gestiona **obras digitales** registradas para protección de propiedad intelectual.  
La estructura principal es:

```cpp
struct ObraDigital {
    int  id;           // Identificador único autoincremental
    char titulo  [100]; // Nombre de la obra
    char autor   [80];  // Nombre del creador
    char tipo    [30];  // Categoría: logotipo, ilustración, fotografía, animación
    char fecha   [12];  // Fecha de creación: DD/MM/AAAA
    char registro[30];  // Código de propiedad intelectual
    char licencia[40];  // Tipo: Copyright, Creative Commons, Royalty-Free
    int  activo;        // Baja lógica: 1 = activo, 0 = eliminado
};
```

Los campos de texto usan `char[]` de tamaño fijo para permitir serialización binaria directa con `fstream` en modo `ios::binary`.

Los datos se almacenan en `data/obras.dat` como archivo binario de structs consecutivos.

---

## Estructura de carpetas

```
ObraDigital/
├── include/
│   ├── obras.h        → prototipos y definición del struct
│   └── utilidad.h     → prototipos de funciones auxiliares
├── src/
│   ├── obras.cpp      → implementación del dominio
│   └── utilidad.cpp   → funciones de interfaz y fechas
├── data/
│   └── obras.dat      → archivo binario de datos (auto-generado)
├── main.cpp           → SOLO llamadas a funciones
└── README.md
```

---

## Instrucciones de compilación y ejecución

### Requisitos previos
- Compilador `g++` con soporte C++11 o superior  
- Sistema operativo: Windows (MinGW) o Linux/macOS

### Compilar desde consola

**Linux / macOS:**
```bash
g++ -std=c++11 main.cpp src/obras.cpp src/utilidad.cpp -I include -o ObraDigital
```

**Windows (MinGW):**
```cmd
g++ -std=c++11 main.cpp src\obras.cpp src\utilidad.cpp -I include -o ObraDigital.exe
```

### Ejecutar
```bash
# Linux / macOS
./ObraDigital

# Windows
ObraDigital.exe
```

> La carpeta `data/` y el archivo binario `obras.dat` se crean automáticamente en la primera ejecución.

---

## Guía de uso rápido

Al iniciar por primera vez, el sistema carga **10 obras de prueba automáticamente** (seeding), listas para probar todas las funciones.

### Menú principal

```
[1] REGISTRAR obra nueva          → Ingresa los datos de una nueva obra
[2] LISTAR todas las obras        → Muestra la tabla de obras activas
[3] MODIFICAR obra                → Edita campos por ID (Enter = conservar)
[4] ELIMINAR obra (baja lógica)   → Marca como inactiva sin borrar del archivo
[5] ORDENAR obras                 → Bubble Sort / Merge Sort con reporte comparativo
[6] BUSCAR obra                   → Búsqueda binaria iterativa o recursiva
[7] GENERAR reporte PDF           → Exporta HTML imprimible como PDF
[0] SALIR
```

### Flujo recomendado para demostración

1. `[2]` Listar → ver los 10 registros de prueba  
2. `[5]` → Opción `3` (reporte comparativo) → ver métricas de Bubble Sort vs Merge Sort  
3. `[6]` → Opción `3` (comparar métodos) → buscar `"Arte Digital Abstracto"` → ver iteraciones vs llamadas recursivas  
4. `[1]` → Registrar una obra nueva  
5. `[4]` → Eliminar una obra (baja lógica)  
6. `[7]` → Generar reporte HTML → abrir en navegador → `Ctrl+P` → Guardar como PDF  

---

## Decisiones de diseño destacadas

| Decisión | Justificación |
----------------------------
| `char[]` en lugar de `string` en el struct | Permite serialización binaria directa sin punteros dinámicos |
| Baja lógica (`activo = 0`) | Conserva el historial sin fragmentar el archivo |
| Parámetros por referencia en búsqueda y ordenación | Permite retorno múltiple (índice + comparaciones/llamadas) sin usar variables globales |
| Bubble Sort + Merge Sort en copias independientes | El reporte comparativo no altera el orden original del archivo |
| Seeding automático | Primera ejecución no requiere ingreso manual de datos |

---

## Compilado y probado con

```
g++ (Ubuntu/MinGW) — estándar C++11
Sin warnings con flags: -Wall -Wextra
```
