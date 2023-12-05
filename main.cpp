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
void mostrar_entregas(string clave_cliente);
bool encontrar_cliente(string nombre_cliente);
bool encontrar_entrega(string nombre_entrega);
bool cliente_activo(string nombre_cliente);
bool inicio_admin(string contrasena);
bool stob(string str);
string input(string str);
string print_menu();
string print_menu_cliente();
string print_menu_admin();
string obtener_usuario();

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

    ifstream Clientes("clientes.csv");
    ifstream Entregas("entregas.csv");

    // verifica que se hayan abierto los archivos
    if (!Clientes.is_open())
    {
        cout << "No se pudo abrir el archivo de clientes." << endl;
        return;
    }

    if (!Entregas.is_open())
    {
        cout << "No se pudo abrir el archivo de entregas." << endl;
        return;
    }

    // agrega los clientes al vector
    while (getline(Clientes, linea))
    {
        istringstream ss(linea);
        string token;
        vector<string> campos;
        while (getline(ss, token, ','))
        {
            campos.push_back(token);
        }
        if (campos.size() == 3)
        {
            Cliente cl;
            cl.nombre = campos[0];
            cl.clave = campos[1];
            cl.al_corriente = stob(campos[2]);

            lista_clientes.push_back(cl);
        }
    }
    // agrega las entregas al vector
    while (getline(Entregas, linea))
    {
        istringstream ss(linea);
        string token;
        vector<string> campos;
        while (getline(ss, token, ','))
        {
            campos.push_back(token);
        }

        if (campos.size() == 3)
        {
            Entrega en;
            en.clave_entrega = campos[0];
            en.clave_cliente = campos[1];
            en.direccion = campos[2];

            lista_entregas.push_back(en);
        }
    }

    // cierra los archivos
    Clientes.close();
    Entregas.close();
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
            if (cliente_activo(usuario))
            {
            }
            break;

        case 2:
            if (usuario != "")
            {
            }
            break;

        case 3:
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
        cout << "usuario o contrasena incorrectos";
}

void pagar_servicio(string nombre_cliente) {}
void mostrar_entregas(string nombre_cliente) {}

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
bool cliente_activo(string nombre_cliente){
    for (Cliente cl : lista_clientes)
    {
        if (cl.nombre == nombre_cliente)
            return cl.al_corriente;
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
    for (Cliente cl : lista_clientes)
    {
        if (cl.nombre == nombre)
        {
            return nombre;
        }
    }
    cout << "cliente no encontrado o inexistente" << endl;
    return "";
}

int main()
{
    leer_archivos();
    bool running = true;
    do
    {
        int op = stoi(print_menu());

        switch (op)
        {
        case 1:
            app_admin();
            break;

        case 2:
            obtener_datos_entrega();
            break;

        case 3:
            running = false;
            break;

        default:
            cout << "ERROR, opción invalida" << endl;
            running = false;
            break;
        }
    } while (running);
    return 0;
}