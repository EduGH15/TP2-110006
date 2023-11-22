#include <stdio.h>
#include <string.h>
/*
#include "src/TP1/tipo.h"
#include "src/TP1/pokemon.h"
#include "src/TP1/ataque.h"
#include "src/juego.h"
#include "src/TDA/lista.h"
*/
#include "src/TDA/menu_comandos.h"
#include "pa2m.h"

bool mostrar_ayuda(void *param1, void *param2) {
    printf("Mostrando mensaje de ayuda...\n");
    return true;
}

bool mostrar_instrucciones(void *param1, void *param2) {
    printf("Mostrando mensaje de instrucciones...\n");
    return true;
}

void se_puede_crear_menu(){
	menu_t* menu = menu_crear();
	pa2m_afirmar(menu != NULL,"Se pudo crear un menu");
    pa2m_afirmar(menu_cantidad_comandos(menu) == 0, "Un menu recién creado tiene 0 comandos");
    pa2m_afirmar(menu_vacio(menu) == true, "El menu se encuentra vacío.");
    menu_destruir(menu);
}

void se_puede_agregar_comandos(){
    menu_t* menu = menu_crear();

    pa2m_afirmar(menu_cantidad_comandos(menu) == 0, "Un menu recién creado tiene 0 comandos");
    pa2m_afirmar(menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", NULL, NULL) == NULL, "No se pudo agregar un comando si la funcion es NULL");
    pa2m_afirmar(menu_cantidad_comandos(menu) == 0, "La cantidad de comandos sigue siendo 0.");
    pa2m_afirmar(menu_vacio(menu) == true, "El menu se encuentra vacío.");

    pa2m_afirmar(menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", mostrar_ayuda, NULL) != NULL, "Se pudo agregar un comando con contexto NULL");
    pa2m_afirmar(menu_cantidad_comandos(menu) == 1, "La cantidad de comandos ahora es 1.");
    pa2m_afirmar(menu_vacio(menu) == false, "El menu ya no se encuentra vacío.");

    comando_t *comando = menu_obtener_comando(menu, "h");
    pa2m_afirmar(comando != NULL && strcmp(comando->instruccion, "h") == 0, "El comando existe y tiene la instrucción esperada");

    menu_destruir(menu);
}

void se_puede_quitar_comandos(){
    menu_t* menu = menu_crear();

    pa2m_afirmar(menu_eliminar_comando(menu, NULL)== NULL, "No se puede eliminar un comando NULL");
    pa2m_afirmar(menu_eliminar_comando(menu, "h") == NULL, "No se puede eliminar un comando que no existe.");

    menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", mostrar_ayuda, NULL);
    pa2m_afirmar(menu_cantidad_comandos(menu) == 1, "Se agregó un comando y la cantidad de comandos es 1.");
    pa2m_afirmar(menu_eliminar_comando(menu, "h") == menu, "Se puede eliminar un comando recién agregado");
    pa2m_afirmar(menu_eliminar_comando(menu, "h") == NULL, "No se puede eliminar un mismo comando dos veces");
    pa2m_afirmar(menu_cantidad_comandos(menu) == 0, "La cantidad de comandos es 0.");

    menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", mostrar_ayuda, NULL);
    menu_agregar_comando(menu, "instrucciones", "i", "Muestra un mensaje con las instrucciones del juego", mostrar_instrucciones, NULL);
    pa2m_afirmar(menu_cantidad_comandos(menu) == 2, "Se agregan dos comandos y ahora la cantidad de comandos es 2.");

    menu_eliminar_comando(menu, "h") == menu;
    pa2m_afirmar(menu_cantidad_comandos(menu) == 1, "Se elimina un comando y ahora la cantidad de comandos es 1.");
    pa2m_afirmar(menu_obtener_comando(menu, "h") == NULL, "El comando eliminado fue el esperado");

    menu_destruir(menu);
}

void se_puede_obtener_comandos(){
    menu_t* menu = menu_crear();

    pa2m_afirmar(menu_obtener_comando(NULL, NULL) == NULL, "Un menu NULL no contiene comandos" );
    pa2m_afirmar(menu_obtener_comando(menu, NULL) == NULL, "Un menu valido con comando NULL, no se encuentra en el menu" );
    pa2m_afirmar(menu_obtener_comando(menu, "h") == NULL, "Un menu valido recién creado no contiene comandos" );

    menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", mostrar_ayuda, NULL);
    menu_agregar_comando(menu, "instrucciones", "i", "Muestra un mensaje con las instrucciones del juego", mostrar_instrucciones, NULL);

    comando_t *comando_ayuda = menu_obtener_comando(menu, "h");
    pa2m_afirmar(comando_ayuda != NULL && strcmp(comando_ayuda->instruccion, "h") == 0, "Se puede obtener el comando 'h'");

    comando_t *comando_instrucciones = menu_obtener_comando(menu, "i");
    pa2m_afirmar(comando_instrucciones != NULL && strcmp(comando_instrucciones->instruccion, "i") == 0, "Se puede obtener el comando 'i'");

    pa2m_afirmar(menu_obtener_comando(menu, "xyz") == NULL, "No se puede obtener un comando no agregado");

    menu_destruir(menu);
}

void se_puede_ejecutar_comandos() {
    menu_t* menu = menu_crear();

    pa2m_afirmar(menu_ejecutar_comando(NULL, "h") == NULL, "No se puede ejecutar un comando en un menú NULL");
    pa2m_afirmar(menu_ejecutar_comando(menu, NULL) == NULL, "No se puede ejecutar un comando NULL en un menú válido");
    pa2m_afirmar(menu_ejecutar_comando(menu, "h") == NULL, "No se puede ejecutar un comando que no existe");

    menu_agregar_comando(menu, "ayuda", "h", "Muestra un mensaje de ayuda con los comandos disponibles", mostrar_ayuda, NULL);
    menu_agregar_comando(menu, "instrucciones", "i", "Muestra un mensaje con las instrucciones del juego", mostrar_instrucciones, NULL);

    menu_t *menu_despues_ayuda = menu_ejecutar_comando(menu, "h");
    pa2m_afirmar(menu_despues_ayuda != NULL && menu_obtener_comando(menu_despues_ayuda, "h") != NULL, "Se ejecutó el comando 'h'");

    menu_t *menu_despues_instrucciones = menu_ejecutar_comando(menu, "i");
    pa2m_afirmar(menu_despues_instrucciones != NULL && menu_obtener_comando(menu_despues_instrucciones, "i") != NULL, "Se ejecutó el comando 'i' y se eliminó del menú");

    pa2m_afirmar(menu_ejecutar_comando(menu, "xyz") == NULL, "No se puede ejecutar un comando no existente");

    menu_destruir(menu);
}

int main()
{
	pa2m_nuevo_grupo("============ Pruebas de creación de menu =================");
    se_puede_crear_menu();
	
    pa2m_nuevo_grupo("============ Pruebas de agregar comandos =================");
    se_puede_agregar_comandos();

    pa2m_nuevo_grupo("============ Pruebas de quitar comandos =================");
    se_puede_quitar_comandos();

    pa2m_nuevo_grupo("============ Pruebas de obtención de comandos =================");
    se_puede_obtener_comandos();

    pa2m_nuevo_grupo("============ Pruebas de ejecución de comandos =================");
    se_puede_ejecutar_comandos();

	return pa2m_mostrar_reporte();
}