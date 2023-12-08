#include <string.h>
#include <stdlib.h>
#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MIN 3

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	nodo_t **vector;
	size_t capacidad;
	size_t cantidad;
};

nodo_t *nodo_crear(const char *clave, void *elemento)
{
	if (clave == NULL) {
		return NULL;
	}

	char *copia_clave = malloc(strlen(clave) + 1);
	if (copia_clave == NULL) {
		return NULL;
	}
	strcpy(copia_clave, clave);

	nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL) {
		free(copia_clave);
		return NULL;
	}

	nodo_nuevo->clave = copia_clave;
	nodo_nuevo->valor = elemento;
	nodo_nuevo->siguiente = NULL;

	return nodo_nuevo;
}

unsigned long funcion_hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) +
		       (unsigned long)c; /* hash * 33 + c */
	}

	return hash;
}

hash_t *reshash(hash_t *hash)
{
	if (hash == NULL) {
		return NULL;
	}

	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
	if (nuevo_hash == NULL)
		return NULL;

	for (size_t i = 0; i < hash->capacidad; i++) {
		nodo_t *nodo_actual = hash->vector[i];
		while (nodo_actual != NULL) {
			hash_insertar(nuevo_hash, nodo_actual->clave,
				      nodo_actual->valor, NULL);
			nodo_actual = nodo_actual->siguiente;
		}
	}

	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;

	hash_destruir(nuevo_hash);

	return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < CAPACIDAD_MIN) {
		capacidad = CAPACIDAD_MIN;
	}
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (hash == NULL)
		return NULL;
	hash->capacidad = capacidad;

	hash->vector = calloc(1, sizeof(nodo_t) * capacidad);
	if (hash->vector == NULL) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (hash == NULL || clave == NULL) {
		return NULL;
	}

	float carga = (float)(hash->cantidad) / (float)hash->capacidad;
	if (carga > FACTOR_CARGA_MAXIMO) {
		if (reshash(hash) == NULL) {
			return NULL;
		}
	}

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	if (hash->vector[posicion] == NULL) {
		nodo_t *nodo = nodo_crear(clave, elemento);
		if (nodo == NULL) {
			return NULL;
		}

		hash->vector[posicion] = nodo;
		if (anterior != NULL) {
			*anterior = NULL;
		}
		hash->cantidad++;
		return hash;
	}

	nodo_t *nodo_actual = hash->vector[posicion];
	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (anterior != NULL) {
				*anterior = nodo_actual->valor;
			}
			nodo_actual->valor = elemento;
			return hash;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	nodo_t *nodo_nuevo = nodo_crear(clave, elemento);
	nodo_nuevo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nodo_nuevo;
	hash->cantidad++;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL) {
		return NULL;
	}

	if (!hash_contiene(hash, clave)) {
		return NULL;
	}

	size_t posicion = funcion_hash(clave) % hash->capacidad;

	nodo_t *nodo_actual = hash->vector[posicion];
	nodo_t *nodo_anterior = NULL;

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (nodo_anterior == NULL) {
				hash->vector[posicion] = nodo_actual->siguiente;
			} else {
				nodo_anterior->siguiente =
					nodo_actual->siguiente;
			}

			void *valor_eliminado = nodo_actual->valor;
			free(nodo_actual->clave);
			free(nodo_actual);
			hash->cantidad--;

			return valor_eliminado;
		}

		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			return nodo_actual->valor;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL)
		return false;

	return hash_obtener(hash, clave) != NULL;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void destruir_nodos(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL) {
		return;
	}

	destruir_nodos(nodo->siguiente, destructor);

	if (destructor != NULL) {
		destructor(nodo->valor);
	}
	free(nodo->clave);
	free(nodo);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL) {
		return;
	}

	for (size_t i = 0; i < hash->capacidad; i++) {
		destruir_nodos(hash->vector[i], destructor);
	}

	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (hash == NULL || f == NULL) {
		return n;
	}
	bool seguir_recorriendo = true;
	for (size_t posicion = 0; posicion < hash->capacidad; posicion++) {
		nodo_t *nodo_actual = hash->vector[posicion];
		while (nodo_actual != NULL && seguir_recorriendo) {
			if (!f(nodo_actual->clave, nodo_actual->valor, aux)) {
				seguir_recorriendo = false;
			}
			nodo_actual = nodo_actual->siguiente;
			n++;
		}
	}
	return n;
}
