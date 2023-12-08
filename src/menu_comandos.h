#ifndef __MENU_COMANDOS_H__
#define __MENU_COMANDOS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

#define ERROR -1

typedef enum {
	ERROR_CARGAR,
	COMANDO_INEXISTENTE,
	CARGA_EXITOSA,
	ARCHIVO_YA_CARGADO,
	ESTADO_OK,
	ESTADO_ERROR,
	POKES_INSUFICIENTES,
	POKES_REPETIDOS,
	POKES_INEXISTENTES,
	SIN_POKES,
	CARGA_POKES_EXITOSA,
	ATAQUE_REALIZADO,
	JUGADA_ERROR
} COMANDO_ESTADO;

typedef struct comando {
	const char *nombre;
	const char *instruccion;
	const char *descripcion;
	COMANDO_ESTADO (*funcion)(void *, void *);
	void *contexto;
} comando_t;

typedef struct menu {
	hash_t *comandos;
} menu_t;

/*
Pre:  -
Post: Devuelve un puntero a una estructura de tipo menu_t correctamente inicializada.
      Si hay algún error en la asignación de memoria, devuelve NULL.
*/
menu_t *menu_crear();

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu), una cadena de caracteres no nula (nombre), una cadena de
      caracteres no nula (instruccion), una cadena de caracteres no nula (descripcion), un puntero a una función COMANDO_ESTADO
      no nula, y un puntero a contexto. El puntero a menu debe ser válido y estar correctamente inicializado.
      Además, la instrucción del comando no debe existir previamente en el menú.
Post: Agrega un nuevo comando al menú con la información proporcionada. Devuelve un puntero al menú modificado.
      Si hay algún error en la asignación de memoria, si algún puntero es nulo o si la instrucción del comando ya existe
      en el menú, devuelve NULL.
*/
menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     COMANDO_ESTADO (*funcion)(void *, void *),
			     void *contexto);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu) y una cadena de caracteres no nula (instruccion).
      El puntero a menu debe ser válido y estar correctamente inicializado. Además, la instrucción del comando debe
      existir previamente en el menú.
Post: Elimina el comando asociado a la instrucción dada del menú y libera la memoria asociada al comando. Devuelve un
      puntero al menú modificado.
      Si hay algún error (puntero nulo o la instrucción no existe en el menú), devuelve NULL.
*/
menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu) y una cadena de caracteres no nula (instruccion).
      El puntero a menu debe ser válido y estar correctamente inicializado. La instrucción del comando debe existir
      previamente en el menú y el contexto del comando no puede ser nulo.
Post: Ejecuta la función asociada a la instrucción dada en el menú, pasando como argumentos el propio menú y el contexto
      del comando. Devuelve un COMANDO_ESTADO.
      Si hay algún error (puntero nulo, la instrucción no existe en el menú o la ejecución de la función falla),
      devuelve COMANDO_INEXISTENTE.
*/
COMANDO_ESTADO menu_ejecutar_comando(menu_t *menu, const char *instruccion);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu) y una cadena de caracteres no nula (instruccion).
      El puntero a menu debe ser válido y estar correctamente inicializado.
Post: Devuelve true si el menú contiene un comando con la instrucción dada, y false en caso contrario.
      Si hay algún error (puntero nulo o instrucción nula), devuelve false.
*/
bool menu_contiene_comando(menu_t *menu, const char *instruccion);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu) y una cadena de caracteres no nula (instruccion).
      El puntero a menu debe ser válido y estar correctamente inicializado.
Post: Devuelve un puntero al comando asociado a la instrucción dada en el menú.
      Si la instrucción no existe en el menú o hay algún error (puntero nulo o instrucción nula), devuelve NULL.
*/
comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu).
      El puntero a menu debe ser válido y estar correctamente inicializado.
Post: Devuelve true si el menú está vacío (no contiene comandos), y false en caso contrario.
      Si hay algún error (puntero nulo), devuelve true.
*/
bool menu_vacio(menu_t *menu);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu).
      El puntero a menu debe ser válido y estar correctamente inicializado.
Post: Devuelve la cantidad de comandos contenidos en el menú.
      Si hay algún error (puntero nulo), devuelve 0.
*/
size_t menu_cantidad_comandos(menu_t *menu);

/*
Pre:  Recibe un puntero a una estructura de tipo menu_t (menu).
      El puntero a menu debe ser válido y estar correctamente inicializado.
Post: Libera la memoria asociada al menú y a todos sus comandos.
      Si el menú es nulo, la función no realiza ninguna acción.
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_COMANDOS_H__