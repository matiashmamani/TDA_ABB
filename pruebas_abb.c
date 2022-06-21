#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"
#include "testing.h"
#include <unistd.h>  // For ssize_t in Linux.

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_crear_abb_vacio()
{
    printf("- PRUEBAS CREAR ABB VACIO\n");

    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    printf("- PRUEBAS ABB INSERTAR\n");

    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar()
{
    printf("- PRUEBAS ABB REEMPLAZAR\n");

    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    printf("- PRUEBAS ABB REEMPLAZAR CON DESTRUIR\n");

    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_iterar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    printf("- PRUEBAS ABB ITERAR\n");

    /* Inserta 3 valores */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);

    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];
    
    printf("- PRUEBAS ABB ITERAR EN VOLUMEN\n");

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        valores[i] = i;
        ok = abb_guardar(abb, claves[i], &valores[i]);
        if (!ok) break;
    }

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

bool concatenar_claves(const char* clave, void* elemento, void* extra) {

    const char* cadena = clave;

    strcat((char*)extra, cadena);
    return true;
}

static void prueba_abb_iterador_interno(){

	int tamanio = 15;
	char* abc_desord[] = {"m","g","t","d","k","q","w","a","e","h","l","p","s","u","z"};
	char cadena[16] = "";
	char* cadena_inorder = "adeghklmpqstuwz";

	printf("- PRUEBAS ABB ITERADOR INTERNO\n");
	abb_t* abb = abb_crear(strcmp, NULL);
	
	bool ok = true;
	size_t i;
	for(i = 0; i < tamanio; i++){
        ok &= abb_guardar(abb, abc_desord[i], &i);
    }
    
    print_test("Prueba abb se guardaron todos los elementos desordenados correctamente", ok);

    // Pruebo iterar con el iterador interno.
    abb_in_order(abb, concatenar_claves, cadena);
    print_test("Prueba abb cadena concatenada igual a cadena inorder 'adeghklmpqstuwz'", !strcmp(cadena, cadena_inorder));

    abb_destruir(abb); 
}  

static void prueba_abb_borrar_dos_hijos(){
 
    int tamanio = 15;
    char* abc_desord[] = {"m","g","t","d","k","q","w","a","e","h","l","p","s","u","z"};
    char cadena[16] = "";
    char cadena2[16] = "";
    char cadena3[16] = "";
    char cadena4[16] = "";
    char cadena5[16] = "";

    printf("- PRUEBAS ABB BORRAR CON DOS HIJOS\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    bool ok = true;
    size_t i;
    for(i = 0; i < tamanio; i++) {
        ok &= abb_guardar(abb, abc_desord[i], &i);
    }

    print_test("Prueba abb se guardaron todos los elementos desordenados correctamente", ok);
    print_test("Prueba abb la cantidad de elementos es 15", abb_cantidad(abb) == 15);

    print_test("Prueba abb borrar elemento 'g' con dos hijos", abb_borrar(abb, "g") == &i);
    print_test("Prueba abb la cantidad de elementos es 14", abb_cantidad(abb) == 14);
    abb_in_order(abb, concatenar_claves, cadena);
    print_test("Prueba abb cadena concatenada inorder es 'adehklmpqstuwz'", !strcmp(cadena, "adehklmpqstuwz"));

    print_test("Prueba abb borrar elemento 'p' sin hijos", abb_borrar(abb, "p") == &i);
    print_test("Prueba abb la cantidad de elementos es 13", abb_cantidad(abb) == 13);
    abb_in_order(abb, concatenar_claves, cadena2);
    print_test("Prueba abb cadena concatenada inorder es 'adehklmqstuwz'", !strcmp(cadena2, "adehklmqstuwz"));

    print_test("Prueba abb borrar elemento 'm' con dos hijos", abb_borrar(abb, "m") == &i);
    print_test("Prueba abb la cantidad de elementos es 12", abb_cantidad(abb) == 12);
    abb_in_order(abb, concatenar_claves, cadena3);
    print_test("Prueba abb cadena concatenada inorder es 'adehklqstuwz'", !strcmp(cadena3, "adehklqstuwz"));

    print_test("Prueba abb insertar elemento 'v'", abb_guardar(abb, "v", &i));
    print_test("Prueba abb la cantidad de elementos es 13", abb_cantidad(abb) == 13);
    abb_in_order(abb, concatenar_claves, cadena4);
    print_test("Prueba abb cadena concatenada inorder es 'adehklqstuvwz'", !strcmp(cadena4, "adehklqstuvwz"));

    print_test("Prueba abb borrar elemento 't' con dos hijos", abb_borrar(abb, "t") == &i);
    print_test("Prueba abb la cantidad de elementos es 12", abb_cantidad(abb) == 12);
    abb_in_order(abb, concatenar_claves, cadena5);
    print_test("Prueba abb cadena concatenada inorder es 'adehklqsuvwz'", !strcmp(cadena5, "adehklqsuvwz"));

    abb_destruir(abb);    
}

static void prueba_abb_valor_null(){

    printf("- PRUEBAS ABB VALOR NULL\n");
    abb_t* abb = abb_crear(strcmp, NULL);
    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}


static void prueba_abb_volumen(size_t largo){

    printf("- PRUEBAS ABB VOLUMEN\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for(unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = abb_guardar(abb, claves[i], valores[i]);
        if(!ok) break;
    }

    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que devuelva los valores correctos */
    for(size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if(!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if(!ok) break;
    }

    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for(size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if(!ok) break;
    }

    print_test("Prueba abb borrar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Destruye el abb y crea uno nuevo que sí libera */
    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    /* Inserta 'largo' parejas en el abb */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb);
}

void pila_destruir_wrapper(void *pila){
    pila_destruir((pila_t*)pila);
}

static void prueba_abb_funcion_destruir(void){

    printf("- PRUEBAS ABB FUNCION DESTRUIR (PILA_DESTRUIR)\n");
    abb_t* abb = abb_crear(strcmp, pila_destruir_wrapper);
    size_t largo = 10;

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    pila_t *pila = NULL;

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for(unsigned i = 0; i < largo; i++) {
        pila = pila_crear();
        sprintf(claves[i], "%08d", i);
        ok = abb_guardar(abb, claves[i], pila);
        if(!ok) break;
    }

    print_test("Prueba abb almacenar varias pilas", ok);
    print_test("Prueba abb la cantidad de pilas almacenadas es correcta", abb_cantidad(abb) == largo);
    print_test("Prueba abb destruir con funcion de destruccion pila_destruir", true);

    /* Destruye el abb con funcion pila_destruir */
    abb_destruir(abb);
    free(claves);
}

static void prueba_abb_borrar_sin_hijos(){
 
    char *letra_a = "a";
    char *letra_b = "b";
    char *letra_c = "c";

    printf("- PRUEBAS ABB BORRAR SIN HIJOS\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    bool ok = true;
    ok &= abb_guardar(abb, letra_b, NULL);
    ok &= abb_guardar(abb, letra_a, NULL);
    ok &= abb_guardar(abb, letra_c, NULL);

    print_test("Prueba abb se guardaron todos los elementos correctamente", ok);
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);
    print_test("Prueba abb borrar elemento 'a' sin hijos", !abb_borrar(abb, "a"));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb borrar elemento 'c' sin hijos", !abb_borrar(abb, "c"));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    abb_destruir(abb);    
}

static void prueba_abb_borrar_un_hijo(){
 
    char *letra_d = "d";
    char *letra_b = "b";
    char *letra_f = "f";
    char *letra_c = "c";
    char *letra_e = "e";

    printf("- PRUEBAS ABB BORRAR CON UN HIJO\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    bool ok = true;
    ok &= abb_guardar(abb, letra_d, NULL);
    ok &= abb_guardar(abb, letra_b, NULL);
    ok &= abb_guardar(abb, letra_f, NULL);
    ok &= abb_guardar(abb, letra_c, NULL);
    ok &= abb_guardar(abb, letra_e, NULL);

    print_test("Prueba abb se guardaron todos los elementos correctamente", ok);
    print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);
    print_test("Prueba abb borrar elemento 'b' con un hijo", !abb_borrar(abb, "b"));
    print_test("Prueba abb la cantidad de elementos es 4", abb_cantidad(abb) == 4);
    print_test("Prueba abb borrar elemento 'f' con un hijo", !abb_borrar(abb, "f"));
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);

    abb_destruir(abb);    
}

void pruebas_abb_estudiante() {
    
    prueba_crear_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_funcion_destruir();
    prueba_abb_borrar_sin_hijos();
    prueba_abb_borrar_un_hijo();
    prueba_abb_borrar_dos_hijos();
    prueba_abb_valor_null();
    prueba_abb_volumen(5000);
    prueba_abb_iterador_interno();
    prueba_abb_iterar();
    prueba_abb_iterar_volumen(5000);
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif