#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
// #include <algorithm>

using namespace std;

// se declaran los Structs
struct Cliente
{
    string nombre;
    string clave;
    bool al_corriente;
};

struct Entrega
{
    string clave_entrega;
    string clave_cliente;
    string direccion;
};

// se declaran los vectores
vector<Cliente> lista_clientes;
vector<Entrega> lista_entregas;

// se declaran las funciones
void agregar_cliente(string nombre, string clave, bool al_corriente);
void agregar_entrega(string clave_entrega, string clave_cliente, string direccion);
void auditar();
void leer_archivos();
void obtener_datos_cliente();
void obtener_datos_entrega();
void app_cliente();
void app_admin();
void pagar_servicio(string clave_cliente);
void mostrar_entregas(string nombre_cliente);
bool encontrar_cliente(string clave_cliente);
bool encontrar_entrega(string nombre_entrega);
bool cliente_activo(string clave_cliente);
bool inicio_admin(string usuario, string contrasena);
bool stob(string str);
string input(string str);
string print_menu();
string print_menu_cliente();
string print_menu_admin();
string obtener_usuario();
string get_clave(string usuario);

// crea un nuevo cliente y lo agrega al vector de clientes
void agregar_cliente(string nombre, string clave, bool al_corriente)
{
    // Verificar si el cliente ya existe en el archivo CSV
    if (encontrar_cliente(clave))
    {
        cout << "El cliente ya existe." << endl;
        return;
    }

    // Agregar el cliente al archivo clientes.csv
    ofstream archivo("clientes.csv", std::ios::app);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    archivo << nombre << "," << clave << "," << al_corriente << endl;
    archivo.close();

    // Agregar el cliente a la lista en memoria
    Cliente cl;
    cl.nombre = nombre;
    cl.clave = clave;
    cl.al_corriente = al_corriente;
    lista_clientes.push_back(cl);
}

// crea una nueva entrega y la agrega al vector de entregas
void agregar_entrega(string clave_entrega, string clave_cliente, string direccion)
{
    // Verificar si la entrega ya existe en el archivo entregas.csv
    if (encontrar_entrega(clave_entrega))
    {
        cout << "La entrega ya existe." << endl;
        return;
    }

    // Verificar si el cliente existe
    if (!encontrar_cliente(clave_cliente))
    {
        cout << "Cliente no encontrado" << endl;
        return;
    }

    // Agregar la entrega al archivo entregas.csv
    ofstream archivo("entregas.csv", std::ios::app);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    archivo << clave_entrega << "," << clave_cliente << "," << direccion << endl;
    archivo.close();

    // Agregar la entrega al vector de entregas en memoria
    Entrega en;
    en.clave_entrega = clave_entrega;
    en.clave_cliente = clave_cliente;
    en.direccion = direccion;
    lista_entregas.push_back(en);
}

// imprime los valores de las listas
void auditar()
{
    // imprime los valores de la lista de entregas
    cout << "Entregas: " << endl;
    for (Entrega en : lista_entregas)
    {
        cout << "Clave de la entrega: " << en.clave_entrega << endl;
        cout << "Clave del cliente: " << en.clave_cliente << endl;
        cout << "Dirección de la entrega: " << en.direccion << endl;
        cout << "_________________________________________" << endl;
    }

    // imprime los valores de la lista de clientes
    cout << "Clientes: " << endl;
    for (Cliente cl : lista_clientes)
    {
        cout << "Nombre del cliente: " << cl.nombre << endl;
        cout << "Clave del cliente: " << cl.clave << endl;
        if (cl.al_corriente)
            cout << "El cliente esta al corriente" << endl;
        else
            cout << "El cliente no esta al corriente" << endl;
        cout << "_________________________________________" << endl;
    }
}

// lee los archivos .csv y agrega los valores a la lista correspondiente
void leer_archivos()
{
    string linea;

    // Lectura del archivo clientes.csv
    ifstream archivo_clientes("clientes.csv");

    if (!archivo_clientes.is_open())
    {
        cout << "No se pudo abrir el archivo de clientes." << endl;
        return;
    }

    while (getline(archivo_clientes, linea))
    {
        istringstream ss(linea);
        string nombre, clave, al_corriente_str;

        if (getline(ss, nombre, ',') && getline(ss, clave, ',') && getline(ss, al_corriente_str))
        {
            bool al_corriente = (al_corriente_str == "true" || al_corriente_str == "1");
            Cliente cl;
            cl.nombre = nombre;
            cl.clave = clave;
            cl.al_corriente = al_corriente;

            lista_clientes.push_back(cl);
        }
        else
        {
            cout << "Formato de línea incorrecto: " << linea << endl;
        }
    }

    archivo_clientes.close();

    // Lectura del archivo entregas.csv
    ifstream archivo_entregas("entregas.csv");

    if (!archivo_entregas.is_open())
    {
        cout << "No se pudo abrir el archivo de entregas." << endl;
        return;
    }

    while (getline(archivo_entregas, linea))
    {
        istringstream ss(linea);
        string clave_entrega, clave_cliente, direccion;

        if (getline(ss, clave_entrega, ',') && getline(ss, clave_cliente, ',') && getline(ss, direccion))
        {
            Entrega en;
            en.clave_entrega = clave_entrega;
            en.clave_cliente = clave_cliente;
            en.direccion = direccion;

            lista_entregas.push_back(en);
        }
        else
        {
            cout << "Formato de línea incorrecto en entregas.csv: " << linea << endl;
        }
    }

    archivo_entregas.close();
}

// obtiene los datos del cliente y los agrega a la lista correspondiente
void obtener_datos_cliente()
{
    string nombre = input("ingrese el nombre del cliente");
    string clave = input("ingrese la clave del cliente");
    bool al_corriente = stob(input("al corriente con los pagos? [si/no]"));
    agregar_cliente(nombre, clave, al_corriente);
}

// obtiene los datos de la entrega y los agrega a la lista correspondiente
void obtener_datos_entrega()
{
    string clave_entrega = input("ingrese la clave de la entrega");
    string clave_cliente = input("ingrese la clave del cliente");
    string direccion = input("ingrese la direccion de la entrega");
    agregar_entrega(clave_entrega, clave_cliente, direccion);
}

// interface gráfica del lado del cliente
void app_cliente()
{
    string usuario;
    bool running = true;

    do
    {
        int op = stoi(print_menu_cliente());

        switch (op)
        {
        case 1:
            do
            {
                usuario = obtener_usuario();
            } while (usuario == "");
            break;

        case 2:
            if (usuario != "")
            {
                pagar_servicio(get_clave(usuario));
            }
            else
                cout << "Es necesario que se Identifique primero" << endl;
            break;

        case 3:
            if (usuario != "")
            {
                mostrar_entregas(get_clave(usuario));
            }
            else
                cout << "Es necesario que se Identifique primero" << endl;
            break;

        case 4:
            running = false;
            break;

        default:
            cout << "ERROR, opción invalida" << endl;
            running = false;
            break;
        }
    } while (running);
}

// interface gráfica del lado del administrador
void app_admin()
{
    string usuario = input("ingresa tu usuario");
    string contrasena = input("ingresa tu contraseña");
    if (inicio_admin(usuario, contrasena))
    {
        bool running = true;
        do
        {
            int op = stoi(print_menu_admin());

            switch (op)
            {
            case 1:
                obtener_datos_cliente();
                break;

            case 2:
                obtener_datos_entrega();
                break;

            case 3:
                auditar();
                break;

            case 4:
                running = false;
                break;

            default:
                cout << "ERROR, opción invalida" << endl;
                running = false;
                break;
            }
        } while (running);
    }
    else
        cout << "usuario o contraseña incorrectos";
}

void pagar_servicio(string clave_cliente)
{
    ifstream archivo_lectura("clientes.csv");
    ofstream archivo_temporal("temp.csv");

    if (!archivo_lectura.is_open() || !archivo_temporal.is_open())
    {
        cout << "Error al abrir archivos." << endl;
        return;
    }

    string linea;
    bool cliente_actualizado = false;

    while (getline(archivo_lectura, linea))
    {
        istringstream ss(linea);
        string token;
        vector<string> campos;
        while (getline(ss, token, ','))
        {
            campos.push_back(token);
        }

        if (campos.size() == 3 && campos[1] == clave_cliente)
        {
            campos[2] = "1"; // Actualiza el estado de pago a "true"
            cliente_actualizado = true;
        }

        archivo_temporal << campos[0] << "," << campos[1] << "," << campos[2] << endl;
    }

    archivo_lectura.close();
    archivo_temporal.close();

    remove("clientes.csv");
    rename("temp.csv", "clientes.csv");

    // Actualizar la lista en memoria también si se encontró el cliente y se actualizó
    if (cliente_actualizado)
    {
        for (Cliente &cl : lista_clientes)
        {
            if (cl.clave == clave_cliente)
            {
                cl.al_corriente = true;                                            // Actualiza el estado de pago en la lista en memoria
                cout << "Cliente encontrado y actualizado: " << cl.nombre << endl; // Mensaje de depuración
                break;                                                             // Terminar después de encontrar al cliente
            }
        }
    }
    else
    {
        cout << "No se encontró al cliente para actualizar" << endl; // Mensaje de depuración
    }

    if (cliente_actualizado)
    {
        cout << "Pago al corriente" << endl;
    }
    else
    {
        cout << "No se pudo realizar el pago" << endl;
    }
}

void mostrar_entregas(string clave_cliente)
{
    bool entregas_encontradas = false;

    cout << "Entregas para el cliente: " << clave_cliente << endl;

    for (const Entrega &en : lista_entregas)
    {
        if (en.clave_cliente == clave_cliente)
        {
            entregas_encontradas = true;
            cout << "Clave de la entrega: " << en.clave_entrega << endl;
            cout << "Dirección de la entrega: " << en.direccion << endl;
            cout << "__________________________" << endl;
        }
    }

    if (!entregas_encontradas)
    {
        cout << "No se encontraron entregas para el cliente" << endl;
    }
}

// encuentra al cliente dentro de la lista de clientes
bool encontrar_cliente(string clave_cliente)
{
    for (Cliente cl : lista_clientes)
    {
        if (cl.clave == clave_cliente)
            return true;
    }
    return false;
}

// encuentra la entrega dentro de la lista de entregas
bool encontrar_entrega(string clave_entrega)
{
    for (Entrega en : lista_entregas)
    {
        if (en.clave_entrega == clave_entrega)
            return true;
    }
    return false;
}

// verifica que el usuario y contraseña del admin sean correctos
bool inicio_admin(string usuario, string contrasena)
{
    if (contrasena == "123" && usuario == "admin")
        return true;
    return false;
}

// encuentra el cliente y verifica que este al corriente
bool cliente_activo(string clave_cliente)
{
    for (const Cliente &cl : lista_clientes)
    {
        if (cl.clave == clave_cliente)
        {
            return cl.al_corriente;
        }
    }
    return false;
}

// convierte de string a booleano
bool stob(string str)
{
    return (str == "true" || str == "1" || str == "si");
}

// obtiene el input del usuario
string input(string str)
{
    cout << str << endl;
    string out;
    getline(cin, out);
    return out;
}

// imprime el menu y obtiene la opción seleccionada por el usuario
string print_menu()
{
    cout << "\t1)\tIngresar como cliente" << endl;
    cout << "\t2)\tIngresar como administrador" << endl;
    cout << "\t3)\tSalir" << endl;

    return input("Escoge una opción: ");
}

// imprime el menu para el cliente
string print_menu_cliente()
{
    cout << "\t1)\tIdentificarse" << endl;
    cout << "\t2)\tPagar servicio" << endl;
    cout << "\t3)\tRevisar entregas a tu nombre" << endl;
    cout << "\t4)\tSalir" << endl;

    return input("Escoge una opción: ");
}

// imprime el menu para el administrador
string print_menu_admin()
{
    cout << "\t1)\tRegistro de Cliente" << endl;
    cout << "\t2)\tRegistro de Entrega" << endl;
    cout << "\t3)\tAuditar" << endl;
    cout << "\t4)\tSalir" << endl;

    return input("Escoge una opción: ");
}

// obtiene y verifica al usuario
string obtener_usuario()
{
    string nombre = input("Ingresa tu nombre: ");

    for (const Cliente &cl : lista_clientes)
    {
        if (cl.nombre == nombre)
        {
            cout << "Cliente encontrado: " << cl.nombre << endl;
            return cl.clave;
        }
    }

    cout << "Cliente no encontrado o inexistente" << endl;
    return "";
}

string get_clave(string usuario)
{
    for (Cliente cl : lista_clientes)
    {
        if (cl.nombre == usuario)
        {
            return cl.clave;
        }
    }
    return "";
}

int main()
{
    leer_archivos();

    int op = stoi(print_menu());

    switch (op)
    {
    case 1:
        app_cliente();
        break;

    case 2:
        app_admin();
        break;

    case 3:
        break;

    default:
        cout << "ERROR, opción invalida" << endl;
        break;
    }
    return 0;
}