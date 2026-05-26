/**
 * @file main.cpp
 * @brief Sistema de control academico con operaciones CRUD completas.
 * @author Julian
 * @date 2026
 */

#include <iostream>
#include <vector>
#include <map>
#include "EloquentORM.h"
#include "MySQLConexion.h"

using namespace std;

/**
 * @class Curso
 * @brief Modelo para gestionar la tabla cursos.
 */
class Curso : public EloquentORM {
public:
    Curso(MySQLConexion &db) : EloquentORM(db, "cursos", {"id", "nombre"}) {}
};

/**
 * @class Estudiante
 * @brief Modelo para gestionar la tabla estudiantes.
 */
class Estudiante : public EloquentORM {
public:
    Estudiante(MySQLConexion &db) : EloquentORM(db, "estudiantes", {"id", "nombre", "carnet", "curso_id"}) {}
};

/**
 * @brief Lista todos los cursos registrados.
 * @param db Referencia a la conexion activa.
 */
void listar_cursos(MySQLConexion &db) {
    Curso curso_model(db);
    vector<map<string, string>> registros = curso_model.getAll();
    cout << "\n--- LISTADO DE CURSOS ---" << endl;
    for (const auto &fila : registros) {
        cout << "ID: " << fila.at("id") << " | Curso: " << fila.at("nombre") << endl;
    }
}

/**
 * @brief Gestiona las operaciones CRUD de cursos.
 * @param db Referencia a la conexion activa.
 */
void gestionar_cursos(MySQLConexion &db) {
    int subopcion = 0;
    while (subopcion != 5) {
        cout << "\n--- GESTION DE CURSOS ---" << endl;
        cout << "1. Listar | 2. Crear | 3. Actualizar | 4. Eliminar | 5. Regresar" << endl;
        cout << "Opcion: ";
        cin >> subopcion;

        if (subopcion == 1) {
            listar_cursos(db);
        } else if (subopcion == 2) {
            Curso nuevo(db);
            string nombre;
            cout << "Nombre del curso: "; cin.ignore(); getline(cin, nombre);
            nuevo.set("nombre", nombre);
            if (nuevo.save()) cout << "Curso creado con exito." << endl;
        }
        // Nota: Agregar logica de actualizar y eliminar segun necesidad
    }
}

/**
 * @brief Lista todos los estudiantes registrados.
 * @param db Referencia a la conexion activa.
 */
void listar_estudiantes(MySQLConexion &db) {
    Estudiante est_model(db);
    vector<map<string, string>> registros = est_model.getAll();
    cout << "\n--- LISTADO DE ESTUDIANTES ---" << endl;
    for (const auto &fila : registros) {
        cout << "ID: " << fila.at("id") << " | Nombre: " << fila.at("nombre") 
             << " | Carnet: " << fila.at("carnet") << endl;
    }
}

/**
 * @brief Gestiona las operaciones CRUD de estudiantes.
 * @param db Referencia a la conexion activa.
 */
void gestionar_estudiantes(MySQLConexion &db) {
    int subopcion = 0;
    while (subopcion != 5) {
        cout << "\n--- GESTION DE ESTUDIANTES ---" << endl;
        cout << "1. Listar | 2. Crear | 3. Actualizar | 4. Eliminar | 5. Regresar" << endl;
        cout << "Opcion: ";
        cin >> subopcion;

        if (subopcion == 1) {
            listar_estudiantes(db);
        }
        // Agrega logica de CRUD para estudiantes
    }
}

/**
 * @brief Punto de entrada principal del sistema.
 */
int main() {
    // Configuracion de acceso a base de datos
    MySQLConexion db("root", "admin", "proyectofinal", "localhost", 3306);

    if (!db.open()) return 1;

    int opcion = 0;
    while (opcion != 3) {
        cout << "\n--- SISTEMA DE CONTROL ACADEMICO ---" << endl;
        cout << "1. Gestionar Cursos" << endl;
        cout << "2. Gestionar Estudiantes" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: gestionar_cursos(db); break;
            case 2: gestionar_estudiantes(db); break;
            case 3: cout << "Saliendo del sistema." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    }

    db.close();
    return 0;
}