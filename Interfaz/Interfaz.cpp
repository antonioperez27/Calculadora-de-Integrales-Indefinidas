#include <windows.h>
#include <string>
#include <sstream>
#include <wctype.h>
#include "C:\Users\51993\Desktop\C++POO\poo\Maxima\Maxima.cpp"
#include "C:\Users\51993\Desktop\C++POO\poo\ListaDeIntegrales\ListaDeIntegrales.cpp" // Lista de integrales

// Prototipos
LRESULT CALLBACK ProcedimientoVentana(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::string ResolverIntegral(const std::string& input);

HINSTANCE hInstanciaGlobal; // Instancia global para controles

// Identificadores para controles
#define ID_INPUT 101
#define ID_OUTPUT 102
#define ID_CALCULAR 103
#define ID_RAIZ 104
#define ID_POTENCIA 105
#define ID_PI 106
#define ID_LIMPIAR 107
#define ID_RESULTADOS 1001

// Crear Lista
List resultList; 

// Función principal
int main()
{
    // Llama a WinMain como el punto de entrada principal
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}

// Manejar el evento de agregar un resultado
void addResultToList(HWND hwnd, const std::wstring& result)
{
    // Obtener el control LISTBOX
    HWND hresultadosPrevios = GetDlgItem(hwnd, ID_RESULTADOS);
    if (hresultadosPrevios == NULL)
    {
        MessageBox(hwnd, L"No se encontró el control de la lista", L"Error", MB_ICONERROR);
        return;
    }

    int count = SendMessage(hresultadosPrevios, LB_GETCOUNT, 0, 0);
    for (int i = 0; i < count; i++)
    {
        wchar_t buffer[256];
        SendMessage(hresultadosPrevios, LB_GETTEXT, i, (LPARAM)buffer);
        if (result == buffer)
        {
            MessageBox(hwnd, L"El resultado ya existe en la lista.", L"Advertencia", MB_ICONWARNING);
            return; // Salir sin agregar el resultado
        }
    }

    // Agregar el resultado a la lista
    SendMessage(hresultadosPrevios, LB_ADDSTRING, 0, (LPARAM)result.c_str());
}

int WINAPI WinMain(HINSTANCE hInstanciaActual, HINSTANCE hInstanciaPrevia, LPSTR lpCmdLinea, int nCmdShow)
{
    HWND ventana;     // Manejador de ventana
    MSG mensaje;      // Mensajes de la aplicación
    WNDCLASSEX clase; // Clase de ventana

    clase.cbSize = sizeof(WNDCLASSEX);
    clase.style = CS_HREDRAW | CS_VREDRAW;
    clase.lpfnWndProc = ProcedimientoVentana;
    clase.hInstance = hInstanciaActual;
    clase.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    clase.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    clase.hCursor = LoadCursor(NULL, IDC_ARROW);
    clase.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
    clase.lpszClassName = L"CalculadoraIntegrales";
    clase.lpszMenuName = NULL;
    clase.cbClsExtra = 0;
    clase.cbWndExtra = 0;

    if (!RegisterClassEx(&clase))
    {
        MessageBox(NULL, L"No se pudo registrar la clase de ventana", L"Error", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    ventana = CreateWindowEx(
        0,
        L"CalculadoraIntegrales",
        L"Calculadora de Integrales",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // Ventana sin maximizar
        400, 80, 700, 530,                   // Posición y tamaño
        HWND_DESKTOP,
        NULL,
        hInstanciaActual,
        NULL);

        HWND hStaticText = CreateWindowEx(
        0, L"STATIC", L"CALCULADORA DE INTEGRALES INDEFINIDAS",  // Texto del control
        WS_CHILD | WS_VISIBLE | SS_LEFT,         // Estilo del control
        190, 20, 300, 20,                         // Posición y tamaño del texto
        ventana, NULL, hInstanciaActual, NULL);  // Ventana padre, sin menú

    ShowWindow(ventana, nCmdShow);
    UpdateWindow(ventana);

    // Bucle de mensajes
    while (GetMessage(&mensaje, NULL, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

    return mensaje.wParam;
}

LRESULT CALLBACK ProcedimientoVentana(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hIntegralSimbolo, hInput, hDxSimbolo, hOutput, hBoton, hRaizBoton, hPotenciaBoton, hPiBoton, hLimpiarBoton, hAgregarResultado, hResultadosPrevios;
    static HFONT hFuenteGrande;
    bool flag = false; // Bandera 
    std::string resultado; // Almacenar resultado

    switch (msg)
    {
    case WM_CREATE:
    {
        // Crear una fuente personalizada para agrandar el símbolo de la integral
        hFuenteGrande = CreateFont(
            65,                        // Altura de la fuente
            0,                         // Ancho
            0, 0,                      
            FW_BOLD,                   // Peso de la fuente
            FALSE,                     // Cursiva
            FALSE,                     // Subrayado
            FALSE,                     // Tachado
            DEFAULT_CHARSET,           // Conjunto de caracteres
            OUT_DEFAULT_PRECIS,        
            CLIP_DEFAULT_PRECIS,       // Precisión de recorte
            DEFAULT_QUALITY,           // Calidad
            DEFAULT_PITCH | FF_SWISS,  // Familia de fuente
            L"Times New Roman");       // Nombre de la fuente

        HFONT hFuenteRaiz = CreateFont(
            20,                        
            0,                         // Ancho
            0, 0,                      // Ángulo de escapamiento y orientación
            FW_BOLD,                   // Peso de la fuente
            FALSE,                     // Cursiva
            FALSE,                     // Subrayado
            FALSE,                     // Tachado
            DEFAULT_CHARSET,           // Conjunto de caracteres
            OUT_DEFAULT_PRECIS,        // Precisión de salida
            CLIP_DEFAULT_PRECIS,       // Precisión de recorte
            DEFAULT_QUALITY,           // Calidad
            DEFAULT_PITCH | FF_SWISS,  // Familia de fuente
            L"Times New Roman");       // Nombre de la fuente
        hIntegralSimbolo = CreateWindowEx(
            0, L"STATIC", L"\u222b", // Código Unicode para el símbolo de integral
            WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOPREFIX, // SS_NOPREFIX elimina el fondo
            120, 60, 40, 60,     // Ajustar tamaño para la nueva fuente
            hwnd, NULL, hInstanciaGlobal, NULL);
        SendMessage(hIntegralSimbolo, WM_SETFONT, (WPARAM)hFuenteGrande, TRUE);

        // Crear cuadro de texto para ingresar la función
        hInput = CreateWindowEx(
            0, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            170, 80, 330, 25,
            hwnd, (HMENU)ID_INPUT, hInstanciaGlobal, NULL);

        // Crear el símbolo "dx" (derecha del cuadro de texto)
        hDxSimbolo = CreateWindowEx(
            0, L"STATIC", L"dx",
            WS_CHILD | WS_VISIBLE,
            510, 80, 20, 25,
            hwnd, NULL, hInstanciaGlobal, NULL);

        // Crear botón para calcular la integral
        hBoton = CreateWindowEx(0, L"BUTTON", L"Calcular", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 120, 100, 30, hwnd, (HMENU)ID_CALCULAR, hInstanciaGlobal, NULL);

        // Crear botón para la raíz
        hRaizBoton = CreateWindowEx(
            0, L"BUTTON", L"\u221a(x)",         // Símbolo de la raíz cuadrada
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            250, 170, 50, 30,
            hwnd, (HMENU)ID_RAIZ, hInstanciaGlobal, NULL);

        // Aplicar la fuente personalizada al símbolo de la raíz cuadrada
        SendMessage(hRaizBoton, WM_SETFONT, (WPARAM)hFuenteRaiz, TRUE);

        // Crear área de salida para mostrar el resultado
        hOutput = CreateWindowEx(
            0, L"STATIC", L"Resultado:",
            WS_CHILD | WS_VISIBLE,
            90, 220, 500, 30,
            hwnd, (HMENU)ID_OUTPUT, hInstanciaGlobal, NULL);

        // Crear botón (simbolo de potencia) 
        hPotenciaBoton = CreateWindowEx(
            0, L"BUTTON", L"x^y",         // Símbolo de la raíz cuadrada
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            310, 170, 50, 30,
            hwnd, (HMENU)ID_POTENCIA, hInstanciaGlobal, NULL);

        // Aplicar la fuente personalizada al símbolo de la raíz cuadrada
        SendMessage(hPotenciaBoton, WM_SETFONT, (WPARAM)hFuenteRaiz, TRUE);

        // Crear botón (simbolo )
        hPotenciaBoton = CreateWindowEx(
            0, L"BUTTON", L"sin(x)",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            370, 170, 50, 30,
            hwnd, (HMENU)ID_PI, hInstanciaGlobal, NULL);

        // Crear botón de "Limpiar"
        hLimpiarBoton = CreateWindowEx(
            0, L"BUTTON", L"Limpiar", // Texto del botón
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // Estilo del botón
            340, 120, 100, 30, // Posición y tamaño
            hwnd, (HMENU)ID_LIMPIAR, hInstanciaGlobal, NULL); // Identificador de control

        hResultadosPrevios = CreateWindowEx(
            0, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | LBS_MULTIPLESEL,
            90, 280, 500, 150,
            hwnd, (HMENU)ID_RESULTADOS, hInstanciaGlobal, NULL);  
        break;
    }

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_CALCULAR)
        {
            wchar_t buffer[256];
            GetWindowText(hInput, buffer, sizeof(buffer) / sizeof(buffer[0])); // Leer entrada del usuario
            std::wstring input(buffer);

            // Resolver integral
            resultado = ResolverIntegral(std::string(input.begin(), input.end())); // Resolver Integral
            addResultToList(hwnd, std::wstring(resultado.begin(), resultado.end())); // Agregar Integral 
            flag = true; 

            // Mostrar el resultado
            std::wstring textoResultado = L"Resultado: " + std::wstring(resultado.begin(), resultado.end());
            SetWindowText(hOutput, textoResultado.c_str());
        }
        // Responder a los botones adicionales
        else if (LOWORD(wParam) == ID_RAIZ)
        {
            // Obtener el texto actual del input
            wchar_t buffer[256];
            GetWindowText(hInput, buffer, sizeof(buffer) / sizeof(buffer[0]));
            std::wstring input(buffer);

            
            if (input.empty() || input.back() != ')') {
                input += L"\u221a(x)";
            }
            else {
                input += L"\u221a(x)";
            }

            SetWindowText(hInput, input.c_str()); // Mostrar el nuevo texto en el input
        }
        // Responder a los botones de potenciación 
        else if (LOWORD(wParam) == ID_POTENCIA)
        {
            // Obtener el texto actual del input
            wchar_t buffer[256];
            GetWindowText(hInput, buffer, sizeof(buffer) / sizeof(buffer[0]));
            std::wstring input(buffer);

            // Agregar "sqrt()" al final del texto si no hay nada dentro de los paréntesis
            if (!input.empty()) {
                input += L"^";
            }

            SetWindowText(hInput, input.c_str()); // Mostrar el nuevo texto en el input
        }
        // Respoonder a los botones (pi)
        else if (LOWORD(wParam) == ID_PI)
        {
            // Obtener el texto actual del input
            wchar_t buffer[256];
            GetWindowText(hInput, buffer, sizeof(buffer) / sizeof(buffer[0]));
            std::wstring input(buffer);
            input += L"sin(x)";

            SetWindowText(hInput, input.c_str()); // Mostrar el nuevo texto en el input
        }
        else if (LOWORD(wParam) == ID_LIMPIAR)
        {
            // Limpiar el campo de entrada
            SetWindowText(hInput, L"");
        }
        break;
    }

    case WM_DESTROY:
        DeleteObject(hFuenteGrande); // Liberar recursos de la fuente
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// resolver integrales 
std::string ResolverIntegral(const std::string& input)
{
    return getExpression(input);
}
