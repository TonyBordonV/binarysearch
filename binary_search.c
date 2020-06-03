// Prueba de conceto de un buscardor de expresiones regulares en ficheros binarios
// El programa buscará en todos los ficheros de una carpeta dada y en todos sus directorios
// Mostrará por consola las coincidencias en cada fichero y su posición.

// Ejemplo: .\buscador.exe "c:\users\user1\documents" '^[0-9]{8}[a-zA-Z]$'

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include "binary_search.h"




// Argv 1 -> Path al directorio.
// argv 2 -> Expresión regular

regex_t regex;

int main(int argc, char* argv[])
{

    if (argc > 2)
    {
        int result;
        result = regcomp(&regex, argv[2], REG_EXTENDED);
        if (result)
        {
            if (result == REG_ESPACE)
                fprintf (stderr, "%s\n", strerror(ENOMEM));
            else
                fputs ("Error de sintaxis en la expresion regular.\n", stderr);
            return EXIT_FAILURE;
        }
        //printf("Expresion regular: /%s/\n ",argv[2]);
        DIR *d = opendir(argv[1]);
        if(d!=NULL)
        {
            recorrer_directorio(d, argv[1]);
        }
        else
        {
            printf("No existe el directorio: %s\n", argv[1]);
        }
    }
    else
    {
        printf("Faltan parametros:  ./buscadorPalabras Path Regex\n");
    }

    printf("\n");
    return 0;
}


void recorrer_directorio(DIR *d, char* current_path)
{
    struct dirent *actual = readdir(d);

    while(actual != NULL)
    {
        if((strcmp(actual->d_name, ".") != 0) && (strcmp(actual->d_name, "..") != 0))
        {
            // printf("\n%s\n", actual->d_name);
            char new_path[PATH_MAX + 1];
            strcpy(new_path, current_path);
            strcat(new_path,  SEPARADOR);
            strcat(new_path, actual->d_name);
            //  printf("El nuevo path %s\n", new_path);

            // Intentar abrirlo como un directorio
            DIR *siguiente = opendir(new_path);
            if(siguiente != NULL)
            {
                // printf("Buscando en el directorio %s\n", actual->d_name);
                recorrer_directorio(siguiente, new_path);
            }
            else
            {
                // Es archivo
                //   printf("Buscando en el archivo %s\n", new_path);
                buscar_en_fichero(new_path);
                //   printf("Fin de la busqueda\n ");
            }
        }
        actual = readdir(d);
    }
    closedir(d);
}
void buscar_en_fichero(char* filename)
{
    int c;
    int posicion = 0;
    int imprimir_nombre = 1;
    FILE *f;
    f = fopen(filename, "rb");
    if (f)
    {
        limpia_palabra();
        int posicion_palabra = 0;

        do
        {
            c = fgetc(f);
            if (EOF != c)
            {
                // Si la palabra ha superado el límite de tamaño verificamos si cumple la expresión regular.
                if(posicion_palabra > MAX_LARGO_PALABRA)
                {
                    procesa_regex(&posicion_palabra, filename, &imprimir_nombre, posicion);
                }
                // Verficamos si el caracter actual está dentro de los rangos a-z o A-Z o 0-9
                if((c>=97 && c<=122) || (c>=65 && c<=90) || (c>=48 && c<=57))
                {
                    procesa_caracter(&posicion_palabra, c);
                }
                // Verificamos si el texto pudiera estar codificado en UTC. Si fuera así necesitamos leer otro byte.
                else if(c == 195)
                {
                    c = fgetc(f);
                    if (EOF != c)
                    {
                        int normalizado = normalizar_caracter_especial(c);
                        if(normalizado!=0)
                        {
                            procesa_caracter(&posicion_palabra, normalizado);
                        }
                        posicion++;
                    }
                }
                else
                {
                    // Verificamos si el caracter es un caracter "especial" en formato ISO
                    int num = normalizar_caracter_especial_iso(c);
                    if (num!=0)
                    {
                        procesa_caracter(&posicion_palabra, num);
                    }
                    else
                    {
                        if(posicion_palabra >0 )
                        {
                            procesa_regex(&posicion_palabra, filename, &imprimir_nombre, posicion);
                        }
                    }
                }
                posicion++;
            }
        }
        while (EOF != c);
        fclose(f);
    }
    else
    {
        printf("Fichero no encontrado: %s", filename);
    }
}


/*
*  Limpia el array con la palabra rellenadolo de 0s
*/

void limpia_palabra()
{
    int i = 0;
    while(palabra_actual[i] != 0 && i < MAX_LARGO_PALABRA)
    {
        palabra_actual[i]=0;
        i++;
    }
}

/*
* Verifica si el carácter esta dentro de los carateres especiales para UTF
* Si está dentro de la lista lo susituye por el caracter normalizado
*/
int normalizar_caracter_especial(int caracter )
{
    int i = 0;
    int result = 0;
    while(caracteres_especiales[i] != caracter  && i < NUM_CHAR_NORMALIZADO)
    {
        i++;
    }
    if( i<NUM_CHAR_NORMALIZADO )
    {
        result = caracteres_normalizados[i];
    }
    return result;
}

/*
* Para normalizar se convierten las mayúsculas a minúsculas.
*/

int normalizar_caracter(int caracter)
{
    if(caracter >=65 && caracter<=90)
    {
        return caracter + 32;
    }
    return caracter;
}

/*
* Verifica si el carácter esta dentro de los carateres especiales para ISO 8859
* Si está dentro de la lista lo susituye por el caracter normalizado
*/

int normalizar_caracter_especial_iso(int caracter)
{

    int i = 0;
    int result = 0;
    while(caracteres_especiales_iso_8859[i] != caracter  && i < NUM_CHAR_NORMALIZADO)
    {
        i++;
    }
    if( i<NUM_CHAR_NORMALIZADO )
    {
        result = caracteres_normalizados[i];
    }
    return result;
}


void procesa_caracter(int *posicion_palabra, int caracter)
{

    palabra_actual[*posicion_palabra] = normalizar_caracter(caracter);
    *posicion_palabra = *posicion_palabra + 1;

}

void procesa_regex(int *posicion_palabra, char* filename, int *imprimir_nombre, int posicion)
{
    if(*posicion_palabra>MIN_LARGO_PALABRA)
    {
        // printf ("Palabra  validar: '%s'  \n", palabra_actual);
        int  result = regexec (&regex, palabra_actual, 0, NULL, 0);
        if (!result)
        {
            if(*imprimir_nombre)
            {
                printf("%s\n", filename);
                *imprimir_nombre = 0;
            }
            printf("%08X - %s\n", posicion, palabra_actual);
        }
        else if (result != REG_NOMATCH)
        {
            size_t length = regerror (result, &regex, NULL, 0);
            imprimir_reg_error (result, length, &regex);
            printf("Error al validar el patron");
        }
    }
    limpia_palabra();
    *posicion_palabra = 0;
}


void imprimir_reg_error (int errcode, size_t length, regex_t *compiled)
{
    char buffer[length];
    (void) regerror (errcode, compiled, buffer, length);
    fprintf(stderr, "Fallo al comprobar el patrón: %s\n", buffer);
}

