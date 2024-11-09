#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <windows.h> // Para CreateDirectory en Windows
#else
#include <sys/stat.h> // Para mkdir en sistemas POSIX
#endif

// Funcion para crear un directorio compatible con Windows y Linux/Unix
void crearDirectorio(const std::string& ruta) {
#ifdef _WIN32
    CreateDirectoryA(ruta.c_str(), NULL); // Windows
#else
    mkdir(ruta.c_str(), 0777); // Linux/Unix
#endif
}

// Funcion para aplicar un tipo de cifrado (ejemplo: inversion de bits)
void cifrarArchivo(const std::string& rutaOriginal, const std::string& rutaCifrada, int tipo) {
    std::ifstream archivoOriginal(rutaOriginal, std::ios::binary);
    std::ofstream archivoCifrado(rutaCifrada, std::ios::binary);

    if (!archivoOriginal || !archivoCifrado) {
        std::cerr << "Error al abrir los archivos para el cifrado.\n";
        return;
    }

    unsigned char byte;
    while (archivoOriginal.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        switch (tipo) {
        case 1:
            // Tipo 1: Inversion de bits (cambia 0s a 1s y 1s a 0s)
            byte = ~byte;
            break;
        case 2:
            // Tipo 2: Inversion de bits en posiciones especificas
            byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1); // Ejemplo de inversion en posiciones
            break;
        case 3:
            // Tipo 3: Aplicacion de una mascara
            byte = byte ^ 0b10101010; // Aplicamos una mascara XOR para alternar bits
            break;
        default:
            std::cerr << "Tipo de cifrado no valido.\n";
            return;
        }
        archivoCifrado.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    archivoOriginal.close();
    archivoCifrado.close();
    std::cout << "Archivo cifrado exitosamente en " << rutaCifrada << "\n";
}

// Funcion para aplicar el descifrado segun el tipo
void descifrarArchivo(const std::string& rutaCifrada, const std::string& rutaDescifrada, int tipo) {
    std::ifstream archivoCifrado(rutaCifrada, std::ios::binary);
    std::ofstream archivoDescifrado(rutaDescifrada, std::ios::binary);

    if (!archivoCifrado || !archivoDescifrado) {
        std::cerr << "Error al abrir los archivos para el descifrado.\n";
        return;
    }

    unsigned char byte;
    while (archivoCifrado.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        switch (tipo) {
        case 1:
            // Tipo 1: Inversion de bits (cambia 0s a 1s y 1s a 0s)
            byte = ~byte;
            break;
        case 2:
            // Tipo 2: Inversion de bits en posiciones especificas
            byte = ((byte & 0xAA) >> 1) | ((byte & 0x55) << 1); // Inversion en posiciones especificas
            break;
        case 3:
            // Tipo 3: Aplicacion de una mascara
            byte = byte ^ 0b10101010; // Aplicacion de la misma mascara XOR usada en el cifrado
            break;
        default:
            std::cerr << "Tipo de descifrado no valido.\n";
            return;
        }
        archivoDescifrado.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    archivoCifrado.close();
    archivoDescifrado.close();
    std::cout << "Archivo descifrado exitosamente en " << rutaDescifrada << "\n";
}

int main() {
    int opcion;
    std::string nombreArchivo;
    int tipoCifrado;

    // Rutas relativas para carpetas de respaldo y restauracion
    std::string carpetaBak = "bak";
    std::string carpetaRestore = "Restore";

    // Crear las carpetas de respaldo y restauracion si no existen
    crearDirectorio(carpetaBak);
    crearDirectorio(carpetaRestore);

    do {
        // Menu principal para elegir cifrar o descifrar
        std::cout << "\nSeleccione una opcion:\n";
        std::cout << "1. Cifrar archivo\n";
        std::cout << "2. Descifrar archivo\n";
        std::cout << "3. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        if (opcion == 1 || opcion == 2) {
            std::cout << "Ingrese el nombre completo del archivo (incluyendo extension): ";
            std::cin >> nombreArchivo;

            std::cout << "Seleccione el tipo de cifrado (1: Inversion de bits, 2: Inversion en posiciones, 3: Mascara XOR): ";
            std::cin >> tipoCifrado;
        }

        if (opcion == 1) {
            std::string rutaOriginal = nombreArchivo;
            std::string rutaCifrada = carpetaBak + "/" + nombreArchivo + ".enc";

            // Cifrar el archivo y guardarlo en la carpeta de respaldo
            cifrarArchivo(rutaOriginal, rutaCifrada, tipoCifrado);
        }
        else if (opcion == 2) {
            std::string rutaCifrada = carpetaBak + "/" + nombreArchivo;
            std::string rutaDescifrada = carpetaRestore + "/" + nombreArchivo.substr(0, nombreArchivo.find(".enc"));

            // Descifrar el archivo y guardarlo en la carpeta de restauracion
            descifrarArchivo(rutaCifrada, rutaDescifrada, tipoCifrado);
        }
        else if (opcion == 3) {
            std::cout << "Saliendo del programa...\n";
        }
        else {
            std::cout << "Opcion no valida, intente nuevamente.\n";
        }

    } while (opcion != 3);

    return 0;
}
