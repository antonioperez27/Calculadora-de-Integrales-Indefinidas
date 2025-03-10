#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "C:\Users\51993\Desktop\C++POO\poo\ArbolDeExpresion\ArbolDeExpresion.cpp"

void writeMaximaScript(const std::string& expression)
{
    std::ofstream file("script.mac");
    file << "display2d:false;\n";                            // Evita salida gráfica
    file << "string(integrate(" << expression << ", x));\n"; // Genera salida como texto simple
    file << "quit();\n";                                     // Termina Maxima
    file.close();
}

std::string runMaximaScript()
{
    const char* maximaCommand = "\"C:\\maxima-5.47.0\\bin\\maxima.bat\" --very-quiet -b script.mac > output.txt";

    // Ejecutar Maxima redirigiendo la salida al archivo
    int result = std::system(maximaCommand);
    if (result != 0)
    {
        std::cerr << "Error al ejecutar Maxima. Código de salida: " << result << std::endl;
    }

    std::ifstream resultFile("output.txt");
    std::string line, output;
    int lineNumber = 0;

    // Leer el resultado del archivo
    while (std::getline(resultFile, line))
    {
        lineNumber++;
        if (lineNumber == 7)
        {
            output = line; // Devolver la línea 5
        }
    }

    return output;
}

// Obtener la expresión para Maxima
std::string getExpression(const std::string& expression)
{
    std::string str = chain(expression);
    writeMaximaScript(str);

    return runMaximaScript(); 
}