#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "abb.h"

typedef struct nodo {
	char* clave;
	void* dato;
    struct nodo* izq;
    struct nodo* der;
} nodo_t;

struct abb {
	nodo_t* raiz;
	size_t cantidad;
    abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
};

// Función para uso interno
nodo_t* __abb_crear_nodo(const char* clave, void* dato);

void* __abb_destruir_nodo(nodo_t* nodo);

nodo_t* __abb_buscar_nodo(const char* clave, nodo_t* nodo, nodo_t** padre, abb_comparar_clave_t cmp);

void __abb_destruir_wrapper(nodo_t* nodo, abb_destruir_dato_t destruir_dato);

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    
    abb_t* arbol = malloc(sizeof(abb_t));

    if(!arbol) return NULL;

    arbol->raiz = NULL;
    arbol->cantidad = 0;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;

    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

    if(!arbol->raiz){
        // Guardar primer nodo sobre árbol nulo
        nodo_t* nodo_nuevo = __abb_crear_nodo(clave, dato);
        if(!nodo_nuevo) return false;

        arbol->raiz = nodo_nuevo;
        arbol->cantidad++;
        return true;
    }
    
    nodo_t* padre = NULL;
    nodo_t* nodo = __abb_buscar_nodo(clave, arbol->raiz, &padre, arbol->cmp);

    // Pisar dato sobre nodo existente
    if(nodo){
        if(arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
        nodo->dato = dato;
        return true;
    }

    // Guardar nodo nuevo a nivel hoja (valido si va a la izq o der del padre)
    nodo_t* nodo_nuevo = __abb_crear_nodo(clave, dato);
    if(!nodo_nuevo) return false;

    if(arbol->cmp(clave, padre->clave) < 0){
        padre->izq = nodo_nuevo;
    } else {        // if(arbol->cmp(clave, padre->clave) > 0)
        padre->der = nodo_nuevo;
    }

    arbol->cantidad++;

    return true;
}

void* abb_borrar(abb_t *arbol, const char *clave){
    
    return NULL;
}

void* abb_obtener(const abb_t *arbol, const char *clave){

    nodo_t* padre = NULL;
    nodo_t* nodo = __abb_buscar_nodo(clave, arbol->raiz, &padre, arbol->cmp);

    if(!nodo) return NULL;

    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){

    nodo_t* padre = NULL;

    return __abb_buscar_nodo(clave, arbol->raiz, &padre, arbol->cmp);
}

size_t abb_cantidad(const abb_t *arbol){
    
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){

    __abb_destruir_wrapper(arbol->raiz, arbol->destruir_dato);

    free(arbol);
}

/* *****************************************************************
 *                   PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// ....

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// ....

 /* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t* __abb_crear_nodo(const char* clave, void* dato){

    nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));

    if(!nodo_nuevo) return NULL;

	char* clave_duplicada = strdup(clave);

	if(!clave_duplicada){
		free(nodo_nuevo);
		return NULL;
	}

	nodo_nuevo->clave = clave_duplicada;
    nodo_nuevo->dato = dato;
    nodo_nuevo->izq = NULL;
    nodo_nuevo->der = NULL;

    return nodo_nuevo;
}

void* __abb_destruir_nodo(nodo_t* nodo){

    void* dato = nodo->dato;
	
    free(nodo->clave);		// Liberar clave duplicada con strdup(...)
	free(nodo);
    
    return dato;
}

nodo_t* __abb_buscar_nodo(const char* clave, nodo_t* nodo, nodo_t** padre, abb_comparar_clave_t cmp){

    // Búsqueda PRE-ORDER

    if(!nodo) return NULL;

    if(!cmp(clave, nodo->clave)) return nodo;

    *padre = nodo;

    if(cmp(clave, nodo->clave) < 0){
        return __abb_buscar_nodo(clave, nodo->izq, padre, cmp);
    }

    // if(cmp(clave, nodo->clave) > 0)
    return __abb_buscar_nodo(clave, nodo->der, padre, cmp); 
}

void __abb_destruir_wrapper(nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    
    // Wrapper para cambiar la firma a nivel nodo en vez de nivel árbol
    // Destrucción POST-ORDER

    if(!nodo) return;

    __abb_destruir_wrapper(nodo->izq, destruir_dato);
 
    __abb_destruir_wrapper(nodo->der, destruir_dato);

    void* dato = __abb_destruir_nodo(nodo);

    if(destruir_dato) destruir_dato(dato);
}