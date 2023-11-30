#include "menu_comandos.h"

/*
Pre:  Recibe un puntero a una cadena de caracteres no nula (nombre), un puntero a una cadena de caracteres no nula
      (instruccion), un puntero a una cadena de caracteres no nula (descripcion), y un puntero a una función booleana
      (funcion). Todos los punteros deben ser válidos.
Post: Devuelve un puntero a una estructura de tipo comando_t correctamente inicializada con los datos proporcionados.
      Si hay algún error en la asignación de memoria o algún puntero es nulo, devuelve NULL.
*/
comando_t *comando_crear(const char *nombre, const char *instruccion,
			 const char *descripcion,
			 bool (*funcion)(void *, void *))
{
	if (!nombre || !instruccion || !descripcion || !funcion){
		return NULL;
	}

	comando_t *comando = malloc(sizeof(comando_t));
	if (comando == NULL){
		return NULL;
	}

	comando->nombre = nombre;
	comando->instruccion = instruccion;
	comando->descripcion = descripcion;
	comando->funcion = funcion;
	return comando;
}

menu_t *menu_crear(){
    menu_t *menu = calloc(1, sizeof(menu_t));
	if(!menu){
		return NULL;
	}
	menu->comandos = hash_crear(10);
	if(!menu->comandos){
		free(menu);
		return NULL;
	}
	return menu;
}

menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto)
{
	if(!menu || !nombre || !instruccion || !descripcion || !funcion || hash_contiene(menu->comandos, instruccion) == true){
		return NULL;
	}

    comando_t *comando = comando_crear(nombre, instruccion, descripcion, funcion);
    if (comando == NULL)
    {
        return NULL;
    }

    void *anterior;
    if (hash_insertar(menu->comandos, instruccion, comando, &anterior) == NULL)
    {
        free(comando);
        return NULL;
    }
    return menu;
}

menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion)
{
	if (!menu || !instruccion || hash_contiene(menu->comandos, instruccion) == false)
    {
        return NULL;
    }

    comando_t *comando = hash_quitar(menu->comandos, instruccion);
    free(comando);

    return menu;
}

menu_t *menu_ejecutar_comando(menu_t *menu, const char *instruccion){
	if(menu == NULL || instruccion == NULL){
        return NULL;
    }

    comando_t *comando = hash_obtener(menu->comandos, instruccion);
    if(comando == NULL || comando->funcion(menu, comando->contexto) == false){
        return NULL;
    }

    return menu;
}

bool menu_contiene_comando(menu_t *menu, const char *instruccion){
	if(menu == NULL ||	instruccion == NULL || hash_obtener(menu->comandos, instruccion) == NULL){
		return false;
	}
	return true;
}

comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion){
	if(menu == NULL ||   instruccion == NULL){
        return NULL;
    }
    return hash_obtener(menu->comandos, instruccion);
}

bool menu_vacio(menu_t *menu){
	if(menu == NULL){
        return true;
    }
    return hash_cantidad(menu->comandos) == 0;
}

size_t menu_cantidad_comandos(menu_t *menu){
	if(menu == NULL){
		return 0;
	}
	return hash_cantidad(menu->comandos);
}

void menu_destruir(menu_t *menu){
	if(menu == NULL){
		return;
	}
	hash_destruir_todo(menu->comandos, free);
	free(menu);
}
