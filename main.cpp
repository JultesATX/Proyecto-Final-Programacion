/**
 * @file main.cpp
 * @brief Sistema de control de cursos y estudiantes usando EloquentORM y MySQLConexion.
 * @author Julian
 * @date 2026
 */

#include <iostream>
#include <vector>
#include <map>
#include <mysql.h>
#include "EloquentORM.h"
#include "MySQLConexion.h"

using namespace std;

/**
 * @brief Verifica si un ID ya existe.
 */
bool id_existe(MySQLConexion &db, string tabla, int id) {
    string query = "SELECT id FROM " + tabla + " WHERE id = " + to_string(id);
    mysql_query(db.getConnection(), query.c_str());
    MYSQL_RES *res = mysql_store_result(db.getConnection());
    bool existe = (mysql_num_rows(res) > 0);
    mysql_free_result(res);
    return existe;
}

/**
 * @brief Tabla de cursos heredado de EloquentORM.
 */
class Curso : public EloquentORM {
public:
    Curso(MySQLConexion &db) : EloquentORM(db, "cursos", {"id", "nombre"}) {}
};

/**
 * @brief Tabla de estudiantes heredado de EloquentORM.
 */
class Estudiante : public EloquentORM {
public:
    Estudiante(MySQLConexion &db) : EloquentORM(db, "estudiantes", {"id", "nombre", "carnet", "curso_id"}) {}
};

/**
 * @brief Lista de cursos.
 */
void listar_cursos(MySQLConexion &db) {
    Curso c(db);
    vector<map<string, string>> regs = c.getAll();
    cout << "\033[1;36m\n--- LISTADO DE CURSOS ---\033[0m" << endl;
    for (const auto &f : regs) cout << "ID: " << f.at("id") << " | Curso: " << f.at("nombre") << endl;
}

/**
 * @brief Menu para gestionar cursos.
 */
void gestionar_cursos(MySQLConexion &db) {
    int sub = 0;
    while (sub != 5) {
        cout << "\033[1;33m\n--- GESTION DE CURSOS ---\033[0m" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Crear" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "5. Regresar" << endl;
        cout << "Opcion: "; cin >> sub;

        if (sub == 1) listar_cursos(db);
        else if (sub == 2) {
            int id; string nombre;
            cout << "Nuevo ID: "; cin >> id;
            if (id_existe(db, "cursos", id)) cout << "\033[1;31mError: ID ya existe.\033[0m" << endl;
            else {
                cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                string q = "INSERT INTO cursos (id, nombre) VALUES (" + to_string(id) + ", '" + nombre + "')";
                if (mysql_query(db.getConnection(), q.c_str()) == 0) cout << "\033[1;32mCurso creado con exito.\033[0m" << endl;
            }
        } 
        else if (sub == 3) {
            int id; cout << "ID a actualizar: "; cin >> id;
            Curso c(db);
            if (c.find(id)) {
                int n_id; string nombre;
                cout << "Nuevo ID: "; cin >> n_id;
                cout << "Nuevo nombre: "; cin.ignore(); getline(cin, nombre);
                string q = "UPDATE cursos SET id = " + to_string(n_id) + ", nombre = '" + nombre + "' WHERE id = " + to_string(id);
                if (mysql_query(db.getConnection(), q.c_str()) == 0) cout << "\033[1;32mCurso actualizado con exito.\033[0m" << endl;
            } else cout << "\033[1;31mID no encontrado.\033[0m" << endl;
        } 
        else if (sub == 4) {
            int id; cout << "ID a eliminar: "; cin >> id;
            Curso c(db);
            if (c.find(id) && c.remove()) cout << "\033[1;32mCurso eliminado.\033[0m" << endl;
        }
    }
}

/**
 * @brief Lista estudiantes.
 */
void listar_estudiantes(MySQLConexion &db) {
    Estudiante e(db);
    vector<map<string, string>> regs = e.getAll();
    cout << "\033[1;36m\n--- LISTADO DE ESTUDIANTES ---\033[0m" << endl;
    for (const auto &f : regs) cout << "ID: " << f.at("id") << " | " << f.at("nombre") << " | Carnet: " << f.at("carnet") << endl;
}

/**
 * @brief Menu para gestionar estudiantes.
 */
void gestionar_estudiantes(MySQLConexion &db) {
    int sub = 0;
    while (sub != 5) {
        cout << "\033[1;33m\n--- GESTION DE ESTUDIANTES ---\033[0m" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Crear" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "5. Regresar" << endl;
        cout << "Opcion: "; cin >> sub;

        if (sub == 1) listar_estudiantes(db);
        else if (sub == 2) {
            int id; string n, car, cid;
            cout << "Nuevo ID: "; cin >> id;
            if (id_existe(db, "estudiantes", id)) cout << "\033[1;31mError: ID ya existe.\033[0m" << endl;
            else {
                cout << "Nombre: "; cin.ignore(); getline(cin, n);
                cout << "Carnet: "; getline(cin, car);
                cout << "ID curso: "; getline(cin, cid);
                string q = "INSERT INTO estudiantes (id, nombre, carnet, curso_id) VALUES (" + to_string(id) + ", '" + n + "', '" + car + "', '" + cid + "')";
                if (mysql_query(db.getConnection(), q.c_str()) == 0) cout << "\033[1;32mEstudiante creado con exito.\033[0m" << endl;
            }
        } 
        else if (sub == 3) {
            int id; cout << "ID a actualizar: "; cin >> id;
            Estudiante e(db);
            if (e.find(id)) {
                int n_id; string n, car, cid;
                cout << "Nuevo ID: "; cin >> n_id;
                cout << "Nuevo nombre: "; cin.ignore(); getline(cin, n);
                cout << "Nuevo carnet: "; getline(cin, car);
                cout << "Nuevo ID curso: "; getline(cin, cid);
                string q = "UPDATE estudiantes SET id = " + to_string(n_id) + ", nombre = '" + n + "', carnet = '" + car + "', curso_id = '" + cid + "' WHERE id = " + to_string(id);
                if (mysql_query(db.getConnection(), q.c_str()) == 0) cout << "\033[1;32mEstudiante actualizado con exito.\033[0m" << endl;
            } else cout << "\033[1;31mID no encontrado.\033[0m" << endl;
        } 
        else if (sub == 4) {
            int id; cout << "ID a eliminar: "; cin >> id;
            Estudiante e(db);
            if (e.find(id) && e.remove()) cout << "\033[1;32mEstudiante eliminado.\033[0m" << endl;
        }
    }
}

/**
 * @brief Función principal del programa.
 */
int main() {
    MySQLConexion db("root", "admin", "proyectofinal", "localhost", 3306);
    if (!db.open()) return 1;
    int opcion = 0;
    while (opcion != 3) {
        cout << "\033[1;35m\n--- SISTEMA DE CONTROL ACADEMICO ---\033[0m" << endl;
        cout << "1. Gestionar Cursos" << endl;
        cout << "2. Gestionar Estudiantes" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: "; cin >> opcion;
        switch (opcion) {
            case 1: gestionar_cursos(db); break;
            case 2: gestionar_estudiantes(db); break;
            case 3: cout << "Saliendo..." << endl; break;
        }
    }
    db.close();
    return 0;
}