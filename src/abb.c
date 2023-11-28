#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL)
		return NULL;

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (abb == NULL)
		return NULL;

	abb->comparador = comparador;
	return abb;
}

nodo_abb_t *nodo_insertar(nodo_abb_t *nodo, void *elemento,
			  abb_comparador comparador)
{
	if (nodo == NULL) {
		nodo_abb_t *nodo_nuevo = calloc(1, sizeof(nodo_abb_t));
		if (nodo_nuevo == NULL) {
			return NULL;
		}
		nodo_nuevo->elemento = elemento;
		return nodo_nuevo;
	}

	int comparacion = comparador(elemento, nodo->elemento);

	if (comparacion <= 0)
		nodo->izquierda =
			nodo_insertar(nodo->izquierda, elemento, comparador);
	else
		nodo->derecha =
			nodo_insertar(nodo->derecha, elemento, comparador);

	return nodo;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL)
		return NULL;

	arbol->nodo_raiz =
		nodo_insertar(arbol->nodo_raiz, elemento, arbol->comparador);
	if (arbol->nodo_raiz != NULL)
		(arbol->tamanio)++;

	return arbol;
}

nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *nodo,
				       void **elemento_eliminado)
{
	if (nodo->derecha == NULL) {
		*elemento_eliminado = nodo->elemento;
		nodo_abb_t *izquierda = nodo->izquierda;
		free(nodo);
		return izquierda;
	}
	nodo->derecha =
		extraer_predecesor_inorden(nodo->derecha, elemento_eliminado);
	return nodo;
}

void *nodo_quitar(nodo_abb_t *nodo, void *elemento, abb_comparador comparador,
		  void **elemento_eliminado)
{
	if (nodo == NULL || comparador == NULL) {
		return NULL;
	}

	int comparacion = comparador(elemento, nodo->elemento);

	if (comparacion == 0) {
		nodo_abb_t *izquierda = nodo->izquierda;
		nodo_abb_t *derecha = nodo->derecha;
		*elemento_eliminado = nodo->elemento;

		if (izquierda != NULL && derecha != NULL) {
			void *predecesor = NULL;
			nodo->izquierda = extraer_predecesor_inorden(
				nodo->izquierda, &predecesor);
			nodo->elemento = predecesor;
		} else {
			free(nodo);
			if (izquierda == NULL)
				return derecha;
			return izquierda;
		}
	} else if (comparacion < 0)
		nodo->izquierda = nodo_quitar(nodo->izquierda, elemento,
					      comparador, elemento_eliminado);
	else
		nodo->derecha = nodo_quitar(nodo->derecha, elemento, comparador,
					    elemento_eliminado);

	return nodo;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || abb_vacio(arbol))
		return NULL;

	void *elemento_eliminado = NULL;
	arbol->nodo_raiz = nodo_quitar(arbol->nodo_raiz, elemento,
				       arbol->comparador, &elemento_eliminado);

	if (elemento_eliminado != NULL)
		(arbol->tamanio)--;

	if (arbol->nodo_raiz == NULL)
		arbol->tamanio = 0;

	return elemento_eliminado;
}

nodo_abb_t *nodo_buscar(nodo_abb_t *nodo, void *elemento,
			abb_comparador comparador)
{
	if (nodo == NULL)
		return NULL;

	int comparacion = comparador(elemento, nodo->elemento);

	if (comparacion == 0)
		return nodo;
	if (comparacion < 0)
		return nodo_buscar(nodo->izquierda, elemento, comparador);
	else
		return nodo_buscar(nodo->derecha, elemento, comparador);
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL)
		return NULL;

	nodo_abb_t *nodo_buscado = NULL;
	nodo_buscado =
		nodo_buscar(arbol->nodo_raiz, elemento, arbol->comparador);

	if (nodo_buscado == NULL)
		return NULL;
	return nodo_buscado->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	if (arbol == NULL)
		return true;
	return arbol->tamanio == 0;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (arbol == NULL)
		return 0;
	return arbol->tamanio;
}

void nodo_destruir(nodo_abb_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL)
		return;

	if (destructor != NULL)
		destructor(nodo->elemento);

	nodo_destruir(nodo->izquierda, destructor);
	nodo_destruir(nodo->derecha, destructor);
	free(nodo);
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL)
		return;
	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL)
		return;

	nodo_destruir(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento_inorden(nodo_abb_t *nodo,
				     bool (*funcion)(void *, void *), void *aux,
				     bool *seguir_leyendo,
				     size_t *cantidad_elementos)
{
	if (!nodo || *seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->izquierda) {
		*cantidad_elementos = abb_con_cada_elemento_inorden(
			nodo->izquierda, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	*seguir_leyendo = funcion(nodo->elemento, aux);
	(*cantidad_elementos)++;

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->derecha) {
		*cantidad_elementos = abb_con_cada_elemento_inorden(
			nodo->derecha, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	return *cantidad_elementos;
}

size_t abb_con_cada_elemento_preorden(nodo_abb_t *nodo,
				      bool (*funcion)(void *, void *),
				      void *aux, bool *seguir_leyendo,
				      size_t *cantidad_elementos)
{
	if (!nodo || *seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	*seguir_leyendo = funcion(nodo->elemento, aux);
	(*cantidad_elementos)++;

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->izquierda) {
		*cantidad_elementos = abb_con_cada_elemento_preorden(
			nodo->izquierda, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->derecha) {
		*cantidad_elementos = abb_con_cada_elemento_preorden(
			nodo->derecha, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	return *cantidad_elementos;
}

size_t abb_con_cada_elemento_postorden(nodo_abb_t *nodo,
				       bool (*funcion)(void *, void *),
				       void *aux, bool *seguir_leyendo,
				       size_t *cantidad_elementos)
{
	if (!nodo || *seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->izquierda) {
		*cantidad_elementos = abb_con_cada_elemento_postorden(
			nodo->izquierda, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	if (nodo->derecha) {
		*cantidad_elementos = abb_con_cada_elemento_postorden(
			nodo->derecha, funcion, aux, seguir_leyendo,
			cantidad_elementos);
	}

	if (*seguir_leyendo == false) {
		return *cantidad_elementos;
	}

	*seguir_leyendo = funcion(nodo->elemento, aux);
	(*cantidad_elementos)++;

	return *cantidad_elementos;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || funcion == NULL)
		return 0;

	size_t cantidad_elementos = 0;
	bool seguir_recorriendo = true;
	switch (recorrido) {
	case INORDEN:
		return abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion,
						     aux, &seguir_recorriendo,
						     &cantidad_elementos);
	case PREORDEN:
		return abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion,
						      aux, &seguir_recorriendo,
						      &cantidad_elementos);
	case POSTORDEN:
		return abb_con_cada_elemento_postorden(arbol->nodo_raiz,
						       funcion, aux,
						       &seguir_recorriendo,
						       &cantidad_elementos);
	default:
		return 0;
	}
}

size_t abb_recorrer_inorden(nodo_abb_t *nodo, void **array,
			    size_t tamanio_array, size_t *indice)
{
	if (nodo == NULL || *indice >= tamanio_array) {
		return *indice;
	}

	*indice = abb_recorrer_inorden(nodo->izquierda, array, tamanio_array,
				       indice);

	if (*indice < tamanio_array) {
		array[*indice] = nodo->elemento;
		(*indice)++;
	}

	*indice = abb_recorrer_inorden(nodo->derecha, array, tamanio_array,
				       indice);

	return *indice;
}

size_t abb_recorrer_preorden(nodo_abb_t *nodo, void **array,
			     size_t tamanio_array, size_t *indice)
{
	if (nodo == NULL || *indice >= tamanio_array)
		return *indice;

	if (*indice < tamanio_array) {
		array[*indice] = nodo->elemento;
		(*indice)++;
	}

	*indice = abb_recorrer_preorden(nodo->izquierda, array, tamanio_array,
					indice);
	*indice = abb_recorrer_preorden(nodo->derecha, array, tamanio_array,
					indice);
	return *indice;
}

size_t abb_recorrer_postorden(nodo_abb_t *nodo, void **array,
			      size_t tamanio_array, size_t *indice)
{
	if (nodo == NULL || *indice >= tamanio_array)
		return *indice;

	*indice = abb_recorrer_postorden(nodo->izquierda, array, tamanio_array,
					 indice);
	*indice = abb_recorrer_postorden(nodo->derecha, array, tamanio_array,
					 indice);

	if (*indice < tamanio_array) {
		array[*indice] = nodo->elemento;
		(*indice)++;
	}

	return *indice;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || array == NULL || tamanio_array == 0)
		return 0;

	size_t indice = 0;

	switch (recorrido) {
	case INORDEN:
		return abb_recorrer_inorden(arbol->nodo_raiz, array,
					    tamanio_array, &indice);
	case PREORDEN:
		return abb_recorrer_preorden(arbol->nodo_raiz, array,
					     tamanio_array, &indice);
	case POSTORDEN:
		return abb_recorrer_postorden(arbol->nodo_raiz, array,
					      tamanio_array, &indice);
	default:
		return 0;
	}
}