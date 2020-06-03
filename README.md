## Buscador binario de cadenas en ficheros mediante expresiones regulares
La siguiente prueba de concepto muestra el código fuente de una herramienta escrita en C para la búsqueda de cadenas en ficheros.  La herramienta es capaz de buscar de forma recursiva en todas las carpetas contenidas en la carpeta inicial de búsqueda.

### Compilación

``` gcc  binary_search.c  -o bsearch ```

#### Nota
En sistemas **Windows** si utilizas MinGW y al compilar no localiza el fichero cabecera  *regex.h* debes realizar los siguientes pasos:
-  Ejecutar el comando ```setx C_INCLUDE_PATH the_folder_to_regex.h ``` . El ficheo regex.h está localizado en la carpeta MinGW\opt\include
-  Deberás incluir la libreria libregex.a en la compilación:
 ```gcc binary_search.c -L"[PATH]\MinGW\opt\lib\libregex.a" -o bsearch.exe ```

### Ejecución
``` ./bsearch  ruta expresión_reguar ```

*Ejemplo:*

``` ./bsearch c:\users\user1\documents  ^[a-z]{6}[0-9]$ ```







