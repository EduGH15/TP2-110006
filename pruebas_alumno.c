#include <stdio.h>
#include <string.h>

#include "src/tipo.h"
#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"

#include "src/menu_comandos.h"
#include "pa2m.h"

#define ARCHIVO_OK "ejemplos/correcto.txt"
#define ARCHIVO_CORTO "ejemplos/corto.txt"
#define ARCHIVO_INEXISTENTE "/AsddA/ASD/ASD/ASDasD/ASD/ASD7aSD/ASD/a"

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

void jugar()
{
	juego_t *j = juego_crear();

	pa2m_afirmar(juego_cargar_pokemon(j, ARCHIVO_OK) == TODO_OK,
		     "Cargar un archivo existente resulta en TODO_OK");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 6,
		     "El listado de pokemones tiene el tamaño correcto");
	pa2m_afirmar(
		juego_seleccionar_pokemon(j, JUGADOR1, "Pikachu", "Charmander",
					  "Cacnea") == TODO_OK,
		"Los pokemon del jugador 1 se pueden seleccionar sin problema");
	pa2m_afirmar(
		juego_seleccionar_pokemon(j, JUGADOR2, "Floatzel", "Togepi",
					  "Larvitar") == TODO_OK,
		"Los pokemon del jugador 2 se pueden seleccionar sin problema");

	jugada_t invalida1 = { .pokemon = "Pochita", .ataque = "nada" };
	jugada_t invalida2 = { .pokemon = "Pikachu", .ataque = "nada" };

	jugada_t validaj11 = { .pokemon = "Pikachu", .ataque = "Latigo" };
	jugada_t validaj21 = { .pokemon = "Floatzel", .ataque = "Retribucion" };

	jugada_t validaj12 = { .pokemon = "Pikachu", .ataque = "Chispa" };
	jugada_t validaj22 = { .pokemon = "Floatzel", .ataque = "Buceo" };

	jugada_t validaj13 = { .pokemon = "Charmander",
			       .ataque = "Lanzallamas" };
	jugada_t validaj23 = { .pokemon = "Cacnea", .ataque = "Hojas" };

	resultado_jugada_t obtenido;

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, invalida1, invalida1))
				.jugador1 == ATAQUE_ERROR,
		"Realizar una jugada inválida con ambos jugadores devuelve ATAQUE_ERROR");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, invalida1, validaj21))
				.jugador1 == ATAQUE_ERROR,
		"Realizar una jugada inválida con el primer jugador devuelve ATAQUE_ERROR para el jugador");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, invalida2))
				.jugador2 == ATAQUE_ERROR,
		"Realizar una jugada inválida con el segundo jugador devuelve ATAQUE_ERROR para el segundo jugador");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, validaj21))
					.jugador1 == ATAQUE_REGULAR &&
			obtenido.jugador2 == ATAQUE_REGULAR,
		"Realizar una jugada valida de tipo normal devuelve ATAQUE_REGULAR");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, validaj21))
				.jugador1 == ATAQUE_ERROR,
		"No se puede volver a utilizar el mismo ataque/pokemon dos veces");
	
	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 1,
		     "El puntaje del jugador 1 es 1");
	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR2) == 2,
		     "El puntaje del jugador 2 es 2");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj12, validaj22))
				.jugador1 == ATAQUE_EFECTIVO,
		"Realizar una jugada valida de tipo ELECTRICO contra un pokemon AGUA resulta en ataque efectivo");
	pa2m_afirmar(
		obtenido.jugador2 == ATAQUE_INEFECTIVO,
		"Realizar una jugada valida de tipo AGUA contra un pokemon ELECTRICO resulta en ataque inefectivo");

	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 4,
		     "El puntaje del jugador 1 es 4 (1 + 1*3)");
	pa2m_afirmar(
		juego_obtener_puntaje(j, JUGADOR2) == 4,
		"El puntaje del jugador 2 es 4 (2 + 3/2 redondeando hacia arriba)");

	pa2m_afirmar(juego_finalizado(j) == false,
		     "El juego no está finalizado");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj13, validaj23))
				.jugador1 == ATAQUE_EFECTIVO,
		"Realizar una jugada valida de tipo FUEGO contra un pokemon PLANTA resulta en ataque efectivo");
	pa2m_afirmar(
		obtenido.jugador2 == ATAQUE_INEFECTIVO,
		"Realizar una jugada valida de tipo PLANTA contra un pokemon FUEGO resulta en ataque inefectivo");

	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 16,
		     "El puntaje del jugador 1 es 16 (4 + 4*3)");
	pa2m_afirmar(
		juego_obtener_puntaje(j, JUGADOR2) == 5,
		"El puntaje del jugador 2 es 5 (4 + 2/2 redondeando hacia arriba)");

	pa2m_afirmar(juego_finalizado(j) == false,
		     "El juego no está finalizado");

	juego_destruir(j);
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



    pa2m_nuevo_grupo("------------ PRUEBAS DEL TP1 ------------");

	pa2m_nuevo_grupo("JUGAR");
	jugar();

	return pa2m_mostrar_reporte();
}