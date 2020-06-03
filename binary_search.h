
// Valores hexadecimales de los caracteres.
// Para UTF los caracteres especiales ocupan 2 bytes.

/*
a 61 -> 97
z 7A -> 122
A 41 -> 65
Z 5A -> 90


a 61 -> 97
e 65 -> 101
i 69 -> 105
o 6F -> 111
u 75 -> 117

A 41 -> 65
E 45 -> 69
I 49 -> 73
O 4F -> 79
U 55 -> 85

n 6E -> 110
N 4E -> 78


UTF
� C3A1 -> 195 161
� C3A9 -> 195 169
� C3AD -> 195 173
� C3B3 -> 195 179
� C3BA -> 195 186
� C381 -> 195 129
� C389 -> 195 137
� C38D -> 195 141
� C393 -> 195 147
� C39A -> 195 154
� C3B1 -> 195 177
� C391 -> 195 145
� C3BC -> 195 188
� C39C -> 195 156

ISO 8859-1
� E1 -> 225
� E9 -> 233
� ED -> 237
� F3 -> 243
� FA -> 250
� C1 -> 193
� C9 -> 201
� CD -> 205
� D3 -> 211
� DA -> 218
� F1 -> 241
� D1 -> 209
� FC -> 252
� DC -> 220

*/
#ifdef _WIN32
#define SEPARADOR "\\"
#endif // _WIN32

#ifdef linux
#define SEPARADOR "/"
#endif // linux


#define MAX_LARGO_PALABRA 50     // M�ximo tama�o de parabra para su busqueda.
#define NUM_CHAR_NORMALIZADO 14  // N�mero de caracteres normalizados.
#define MIN_LARGO_PALABRA 3      // Tama�o m�nimo de la palabra para valirdar.

void limpia_palabra();
int normalizar_caracter_especial(int caracter );
int normalizar_caracter(int caracter);
int normalizar_caracter_especial_iso(int caracter);
void buscar_en_fichero(char* filename);
void recorrer_directorio(DIR *d, char* path);
void imprimir_reg_error (int errcode, size_t length, regex_t *compiled);
void procesa_regex(int *posicion_palabra, char* filename, int *imprimir_nombre, int posicion);
void procesa_caracter(int *posicion_palabra, int caracter);


int caracteres_especiales[NUM_CHAR_NORMALIZADO] = {161,169,173,179,186,129,137,141,147,154,177,145,188,156};                // Caracteres especiales (UTF).
int caracteres_especiales_iso_8859[NUM_CHAR_NORMALIZADO] = {225,233,237,243,250,193,201,205,211,218,241,209,252,220};       // Caracteres especiales ISO-8859-1.
int caracteres_normalizados[NUM_CHAR_NORMALIZADO] = {97,101,105,111,117,65,69,73,79,85,110,78,117,85};                      // Correspondecia de caracteres para su normalizaci�n.
char palabra_actual[MAX_LARGO_PALABRA];  //
