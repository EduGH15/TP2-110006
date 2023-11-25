#ifndef __MENU_COMANDOS_H__
#define __MENU_COMANDOS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

#define ERROR -1

typedef struct comando {
	const char* nombre;
	const char* instruccion;
    const char* descripcion;
	bool (*funcion)(void *, void *);
	void*contexto;
} comando_t;

typedef struct menu {
	hash_t *comandos;
} menu_t;

/* 
	Crea un menu, Devuelve NULL en caso de error.
*/
menu_t *menu_crear();

/* 
	Agrega un comando y devuelve el menu. Devuelve NULL en caso de error (menu, nombre, instrucción, descripción o función NULL)
	o de no poder agregarlo,
*/
menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto);

/* 
    Elimina el comando y devuelve el menu. Devuelve NULL en caso de error o de no poder eliminarlo, 
*/
menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion);


/* 
	Ejecuta el comando y devuelve el menu. Devuelve NULL en caso de error o de no poder ejecutarlo, 
*/
menu_t *menu_ejecutar_comando(menu_t *menu, const char *instruccion);

/* 
	Devuelve true si el menú contiene el comando pasado por parámetro, en caso contrario devuelve false.
 */
bool menu_contiene_comando(menu_t *menu, const char *instruccion);

/* 
	Busca el comando en el menú y si lo encuentra devuelve el comando. Devuelve NULL en caso de error o de no encontrarlo,
*/
comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion);

/* 
	Devuelve true si el menú no tiene comandos o no existe y false en caso contrario.
*/
bool menu_vacio(menu_t *menu);

/* 
	Devuelve la cantidad de comandos del menú. devuelve 0 si la cantidad de comandos es 0 o si no existe el menu.
*/
size_t menu_cantidad_comandos(menu_t *menu);

/* 
	* Libera la memoria reservada para el menu
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_COMANDOS_H__