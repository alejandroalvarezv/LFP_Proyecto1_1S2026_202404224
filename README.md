MedLexer - Sistema de Analisis Hospitalario
Este proyecto es un analizador lexico desarrollado en C++ utilizando el framework Qt. Permite cargar archivos con extension .med, analizar su contenido para identificar tokens y reportar errores lexicos en una interfaz grafica.

Instrucciones de Compilacion
Para compilar y ejecutar este proyecto, siga estos pasos:

Requisitos Previos
Qt Creator (Version 6.0 o superior recomendada).

Qt Framework (Version 6.x o 5.15).

Compilador C++ (MinGW para Windows, GCC para Linux o Clang para macOS).

Pasos para Compilar
Abra Qt Creator.

Cargue el proyecto abriendo el archivo con extension .pro.

Seleccione el Kit de compilacion configurado en su sistema.

Limpie el proyecto: Menu Build > Clean All.

Compile y ejecute: Presione el boton de Play (triangulo verde) o use el atajo Ctrl + R.

Instrucciones de Uso
Ejecute la aplicacion.

Haga clic en el boton "Cargar Archivo".

Seleccione un archivo de entrada con extension .med.

El texto se mostrara en el editor principal.

Las tablas "tablaTokens" y "tablaErrores" se llenaran automaticamente con los resultados del analisis.

Estructuras de Datos Utilizadas
El proyecto implementa las siguientes estructuras para el manejo de informacion:

Struct Token: Almacena el tipo de token (enum), el lexema (QString), la linea y la columna donde fue encontrado.

Struct ErrorLexico: Almacena el numero de error, el caracter incorrecto, el tipo de error, una descripcion tecnica, linea y columna.

Struct Paciente y Medico: Estructuras personalizadas para organizar la informacion extraida de los bloques correspondientes en el archivo .med.

QList: Se utiliza esta coleccion de Qt para gestionar listas dinamicas de tokens, errores y objetos, permitiendo un manejo eficiente de la memoria y facilidad para iterar los datos en las tablas.

Datos del Autor
Nombre: Alejandro Emmanuel Alvarez Velasquez
Carnet: 202404224

Curso: Introduccion a la Programacion y Computacion 2

Seccion: P
