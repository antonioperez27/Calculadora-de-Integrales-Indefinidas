#include "C:\Users\51993\Desktop\C++POO\poo\ValidarExpresiones\ValidarExpresiones.cpp"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <sstream>
#include <stdexcept>

using namespace std;

// Definir tipos de nodos
enum NodeType
{
    NUMBER,
    OPERATOR,
    FUNCTION,
    VARIABLE
};

// Estructura de nodo para el árbol de expresión
struct ExpressionNode
{
    string data;
    NodeType type;
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(const string& d, NodeType t) : data(d), type(t), left(nullptr), right(nullptr) {}
    ~ExpressionNode()
    {
        delete left;
        delete right;
    }
};

// Función para verificar si un carácter es un operador
bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Función para obtener la precedencia de los operadores
int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

// Función para verificar si una cadena es una función matemática
bool isFunction(const string& str)
{
    return str == "sin" || str == "cos" || str == "tan" || str == "ln" || str == "sqrt";
}

// Función para verificar si un carácter es una letra (una variable)
bool isVariable(char c)
{
    return isalpha(c) && !isdigit(c); // solo letras, excluyendo números
}

// Función para crear un árbol de expresión a partir de una cadena en notación infija
ExpressionNode* parseExpression(const string& expression, size_t& i)
{
    stack<ExpressionNode*> values;
    stack<char> operators;

    while (i < expression.size())
    {
        if (isspace(expression[i]))
        {
            i++;
            continue;
        }

        if (isalpha(expression[i]))
        {
            string token;
            while (i < expression.size() && isalpha(expression[i]))
            {
                token += expression[i++];
            }

            if (isFunction(token))
            {
                if (i < expression.size() && expression[i] == '(')
                {
                    i++; // Saltar '('
                    ExpressionNode* arg = parseExpression(expression, i);
                    if (i >= expression.size() || expression[i] != ')')
                    {
                        throw runtime_error("Falta un paréntesis de cierre para la función.");
                    }
                    i++; // Saltar ')'
                    ExpressionNode* funcNode = new ExpressionNode(token, FUNCTION);
                    funcNode->right = arg;
                    values.push(funcNode);
                }
                else
                {
                    throw runtime_error("Paréntesis esperado después de la función: " + token);
                }
            }
            else
            {
                values.push(new ExpressionNode(token, VARIABLE));
            }
        }
        else if (isdigit(expression[i]) || expression[i] == '.')
        {
            string num;
            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.'))
            {
                num += expression[i++];
            }

            // Verificamos si es seguido de una variable o paréntesis, lo que indica multiplicación implícita
            if (i < expression.size() && (isVariable(expression[i]) || expression[i] == '('))
            {
                string var_or_paren(1, expression[i]);
                i++;                                       // Saltamos el carácter que es una variable o paréntesis
                string num_var = num + "*" + var_or_paren; // Multiplicación implícita
                values.push(new ExpressionNode(num_var, NUMBER));
            }
            else
            {
                values.push(new ExpressionNode(num, NUMBER));
            }
        }
        else if (isOperator(expression[i]))
        {
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i]))
            {
                char op = operators.top();
                operators.pop();
                ExpressionNode* right = values.top();
                values.pop();
                ExpressionNode* left = values.top();
                values.pop();
                ExpressionNode* opNode = new ExpressionNode(string(1, op), OPERATOR);
                opNode->left = left;
                opNode->right = right;
                values.push(opNode);
            }
            operators.push(expression[i]);
            i++;
        }
        else if (expression[i] == '(')
        {
            i++; // Saltar '('
            values.push(parseExpression(expression, i));
        }
        else if (expression[i] == ')')
        {
            break;
        }
        else
        {
            throw runtime_error("Carácter inesperado: " + string(1, expression[i]));
        }
    }

    while (!operators.empty())
    {
        char op = operators.top();
        operators.pop();
        ExpressionNode* right = values.top();
        values.pop();
        ExpressionNode* left = values.top();
        values.pop();
        ExpressionNode* opNode = new ExpressionNode(string(1, op), OPERATOR);
        opNode->left = left;
        opNode->right = right;
        values.push(opNode);
    }

    return values.empty() ? nullptr : values.top();
}

// Función para convertir el árbol de expresión a formato Maxima
string toMaximaString(ExpressionNode* node, bool isParent = false)
{
    if (!node)
        return "";

    if (node->type == FUNCTION)
    {
        return node->data + "(" + toMaximaString(node->right, false) + ")";
    }
    else if (node->type == OPERATOR)
    {
        string left = toMaximaString(node->left, true);
        string right = toMaximaString(node->right, true);

        string expr = left + " " + node->data + " " + right;
        if (isParent)
        {
            return "(" + expr + ")";
        }
        else
        {
            return expr;
        }
    }
    else if (node->type == NUMBER || node->type == VARIABLE)
    {
        return node->data;
    }

    return "";
}

// Función principal para convertir una expresión infija al formato de Maxima
string convertToMaximaFormat(const string& expression)
{
    size_t i = 0;
    ExpressionNode* root = parseExpression(expression, i);
    if (i != expression.size())
    {
        throw runtime_error("Error al analizar la expresión. Carácter inesperado en la posición " + to_string(i));
    }
    string result = toMaximaString(root);
    delete root;
    return result;
}

string chain(const string& expression)
{
    string convertToMaxima = convertToMaximaFormat(expression);

    if (findOut(convertToMaxima))
    {
        return convertToMaxima;
    }

    return "";
}