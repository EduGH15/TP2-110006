#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "src/menu_comandos.h"
#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include "src/funciones_varias.h"

#define MAX_LENGTH 120
#define MAX_STRING 100

typedef struct datos_comprimidos {
	juego_t *juego;
	adversario_t *adversario;
	lista_t *pokes_usuario;
	jugada_t jugadas_usuario[9];
	size_t cantidad_jugadas;
	lista_t *pokes_adversario;
} datos_comprimidos_t;

/**
 * Completa las jugadas del usuario con los nombres de los pokemones y sus ataques.
 *
 * Pre: datos no es NULL y contiene punteros válidos a juego, adversario, pokes_usuario,
 *      pokes_adversario, y jugadas_usuario con tamaños suficientes.
 * Post: jugadas_usuario se completa con los nombres y ataques de los pokemones del usuario.
 */

void completar_jugadas(const struct ataque *ataque, void *datos)
{
	datos_comprimidos_t *datos_ = datos;
	strcpy(datos_->jugadas_usuario[datos_->cantidad_jugadas].ataque,
	       ataque->nombre);
	datos_->cantidad_jugadas++;
}

/**
 * Completa una jugada del usuario con el nombre y el ataque del pokemon.
 *
 * Pre: datos contiene punteros válidos a juego, adversario,
 *      pokes_usuario, pokes_adversario, y jugadas_usuario con tamaños suficientes.
 * Post: Se agrega una jugada al arreglo jugadas_usuario en datos con el nombre y el ataque del pokemon.
 */
void cargar_jugadas(datos_comprimidos_t *datos)
{
	int pos_pokemon = 0;
	datos->cantidad_jugadas = 0;
	for (size_t i = 0; i < 3; i++) {
		pokemon_t *poke =
			lista_elemento_en_posicion(datos->pokes_usuario, i);
		for (int j = 0; j < 3; j++) {
			strcpy(datos->jugadas_usuario[pos_pokemon].pokemon,
			       pokemon_nombre(poke));
			pos_pokemon++;
		}
		con_cada_ataque(poke, completar_jugadas, datos);
	}
}

/**
 * Verifica si la jugada dada se encuentra en las jugadas del usuario en datos.
 *
 * Pre: datos contiene punteros válidos a jugadas_usuario con tamaño suficiente.
 * Post: Si la jugada está en jugadas_usuario en datos, se marca como "usado" y devuelve true. 
 *       Si no se encuentra, devuelve false.
 */
bool verificar_datos(jugada_t jugada, datos_comprimidos_t *datos)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(datos->jugadas_usuario[i].pokemon, jugada.pokemon) ==
			    0 &&
		    strcmp(datos->jugadas_usuario[i].ataque, jugada.ataque) ==
			    0) {
			strcpy(datos->jugadas_usuario[i].ataque, "usado");
			strcpy(datos->jugadas_usuario[i].pokemon, "usado");
			return true;
		}
	}
	return false;
}

/**
 * Lee una línea de hasta 199 caracteres desde la entrada estándar.
 * 
 * Pre: linea es un array de caracteres de al menos tamaño 100.
 * Post: Se lee una línea de stdin y se almacena en linea. Si se lee correctamente,
 *       se elimina el salto de línea del final si existe.
 */
void leer_linea(char linea[])
{
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido != NULL) {
		linea[strlen(linea) - 1] = 0;
	}
}

/**
 * Interpreta y devuelve una cadena de caracteres que representa el tipo de Pokémon.
 * 
 * Pre: tipo es un valor válido de la enumeración TIPO.
 * Post: Devuelve una cadena de caracteres con el nombre del tipo correspondiente.
 */
char *interpretar_tipo(enum TIPO tipo)
{
	if (tipo == NORMAL) {
		return "NORMAL";
	} else if (tipo == FUEGO) {
		return "FUEGO";
	} else if (tipo == AGUA) {
		return "AGUA";
	} else if (tipo == PLANTA) {
		return "PLANTA";
	} else if (tipo == ELECTRICO) {
		return "ELECTRICO";
	}
	return "ROCA";
}

/** 
 * Pre: lista_juego y lista_pokes son listas válidas. eleccion1, eleccion2 y eleccion3
 *      son nombres válidos de pokemones.
 * Post: Agrega los pokemones seleccionados a la lista_pokes.
 */
void listar_pokemones(lista_t *lista_juego, lista_t *lista_pokes,
		      char *eleccion1, char *eleccion2, char *eleccion3)
{
	pokemon_t *pokemon1 = lista_buscar_elemento(
		lista_juego, comparador_cadenas, eleccion1);
	pokemon_t *pokemon2 = lista_buscar_elemento(
		lista_juego, comparador_cadenas, eleccion2);
	pokemon_t *pokemon3 = lista_buscar_elemento(
		lista_juego, comparador_cadenas, eleccion3);

	lista_insertar(lista_pokes, pokemon1);
	lista_insertar(lista_pokes, pokemon2);
	lista_insertar(lista_pokes, pokemon3);
}

/**
 * Solicita al usuario el nombre de un archivo para cargar pokemones en el juego.
 * 
 * Pre: contexto1 y juego son punteros válidos.
 * Post: Retorna el estado correspondiente según la carga del archivo.
 */
COMANDO_ESTADO pedir_archivo(void *contexto1, void *juego)
{
	char nombre_archivo[50];
	printf("Ingrese un archivo para cargar: ");
	leer_linea(nombre_archivo);
	if (lista_tamanio(juego_listar_pokemon(juego)) != 0) {
		return ARCHIVO_YA_CARGADO;
	}
	JUEGO_ESTADO estado = juego_cargar_pokemon(juego, nombre_archivo);
	if (estado == POKEMON_INSUFICIENTES) {
		return POKES_INSUFICIENTES;
	} else if (estado == ERROR_GENERAL) {
		return ERROR_CARGAR;
	}
	return CARGA_EXITOSA;
}

/**
 * Muestra los comandos disponibles por consola.
 * 
 * Pre: ---------------------------------------------------------------
 * Post: Imprime en consola los comandos disponibles y retorna ESTADO_OK.
 */
COMANDO_ESTADO mostrar_mensaje_ayuda(void *arg1, void *arg2)
{
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                  LISTA COMANDOS DISPONIBLES                  ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║'a'(Ayuda): muestra los comandos disponibles por consola.     ║\n"
	       "	║'s'(salir): Cierra el programa.                               ║\n"
	       "	║'m'(mostrar): Muestra todos los pokemones del archivo.        ║\n"
	       "	║'u'(mostrar pokes user): Muestra los pokemones del usuario.   ║\n"
	       "	║'d'(mostrar pokes ad): Muestra los pokemones del adversario.  ║\n"
	       "	║'e'(elegir): Elige los pokemones que usará el usuario.        ║\n"
	       "	║'r'(Realizar): Realiza una jugada de parte del usuario.       ║\n"
	       "	║'p'(Pedir): Pide el archivo y realiza validaciones.           ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	return ESTADO_OK;
}

/**
 * Sale del programa.
 * 
 * Pre: menu y salir son un puntero válido.
 * Post: Imprime un mensaje de salida en consola, actualiza el valor de salir a true y retorna ESTADO_OK.
 */

COMANDO_ESTADO salir_del_programa(void *menu, void *salir)
{
	if (!menu)
		return ESTADO_ERROR;
	printf("Saliste del programa\n");
	*(bool *)salir = true;
	return ESTADO_OK;
}

/**
 * Muestra en consola los pokemones disponibles del jugador.
 * 
 * Pre: juego es un puntero válido.
 *      eleccionJugador1, eleccionJugador2 y eleccionAdversario3 son punteros a cadenas de caracteres válidos.
 * Post: Imprime en consola los pokemones disponibles del jugador con los nombres proporcionados.
 */

void mostrar_pokemon_disponibles_jugador(juego_t *juego, char *eleccionJugador1,
					 char *eleccionJugador2,
					 char *eleccionAdversario3)
{
	if (!juego) {
		return;
	}
	printf("Tus pokemones son: %s\n", eleccionJugador1);
	printf("Tus pokemones son: %s\n", eleccionJugador2);
	printf("Tus pokemones son: %s\n", eleccionAdversario3);
}

/**
 * Ejecuta el comando de elegir pokemones en el juego.
 * 
 * Pre: contexto y datos son punteros válidos a estructuras de datos necesarias para la ejecución del comando.
 * Post: Realiza la selección de pokemones para los jugadores y adversario en el juego.
 *       Devuelve el estado correspondiente al resultado de la ejecución del comando.
 */
COMANDO_ESTADO elegir_pokemones(void *contexto, void *datos)
{
	datos_comprimidos_t *datos_ = datos;
	JUEGO_ESTADO estado;
	char eleccionJugador1[50];
	char eleccionJugador2[50];
	char eleccionJugador3[50];
	printf("Ingresa un pokemon para ti: ");
	leer_linea(eleccionJugador1);
	printf("Ingresa un pokemon para ti: ");
	leer_linea(eleccionJugador2);
	printf("Ingresa un pokemon para el adversario: ");
	leer_linea(eleccionJugador3);
	estado = juego_seleccionar_pokemon(datos_->juego, JUGADOR1,
					   eleccionJugador1, eleccionJugador2,
					   eleccionJugador3);
	if (estado == POKEMON_REPETIDO) {
		return POKES_REPETIDOS;
	} else if (estado == POKEMON_INEXISTENTE) {
		return POKES_INEXISTENTES;
	}
	char *eleccionAdversario1 = NULL;
	char *eleccionAdversario2 = NULL;
	char *eleccionAdversario3 = NULL;

	adversario_seleccionar_pokemon(datos_->adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
	juego_seleccionar_pokemon(datos_->juego, JUGADOR2, eleccionAdversario1,
				  eleccionAdversario2, eleccionAdversario3);
	adversario_pokemon_seleccionado(datos_->adversario, eleccionJugador1,
					eleccionJugador2, eleccionJugador3);

	listar_pokemones(juego_listar_pokemon(datos_->juego),
			 datos_->pokes_usuario, eleccionJugador1,
			 eleccionJugador2, eleccionAdversario3);
	listar_pokemones(juego_listar_pokemon(datos_->juego),
			 datos_->pokes_adversario, eleccionAdversario1,
			 eleccionAdversario2, eleccionJugador3);
	cargar_jugadas(datos_);
	return CARGA_POKES_EXITOSA;
}

/**
 * Muestra por consola la información de un ataque.
 *
 * Pre: ataque_ es un puntero válido a una estructura de ataque.
 *      aux es un puntero auxiliar (no utilizado en esta función).
 * Post: Imprime por consola el nombre y tipo del ataque.
 */
void mostrar_ataque(const struct ataque *ataque_, void *aux)
{
	(printf("ataque: %s.  Tipo: %s\n", ataque_->nombre,
		interpretar_tipo(ataque_->tipo)));
}

/**
 * Muestra por consola la información de un Pokémon.
 *
 * Pre: pokemon es un puntero válido a una estructura de Pokémon.
 *      contexto es un puntero auxiliar (no utilizado en esta función).
 * Post: Imprime por consola el nombre y tipo del Pokémon,
 *       y muestra la información de sus ataques llamando a la función mostrar_ataque.
 */

bool mostrar_pokemon(void *pokemon, void *contexto)
{
	pokemon_t *pokemon_ = pokemon;
	printf("%s; TIPO %s\n", pokemon_nombre(pokemon_),
	       interpretar_tipo(pokemon_tipo(pokemon_)));
	con_cada_ataque(pokemon, mostrar_ataque, NULL);
	return true;
}

/**
 * Muestra por consola la información de un Pokémon adversario.
 *
 * Pre: pokemon es un puntero válido a una estructura de Pokémon.
 *      contexto es un puntero auxiliar (no utilizado en esta función).
 * Post: Imprime por consola el nombre y tipo del Pokémon adversario.
 */

bool mostrar_pokemon_adversario(void *pokemon, void *contexto)
{
	pokemon_t *pokemon_ = pokemon;
	printf("%s; TIPO %s\n", pokemon_nombre(pokemon_),
	       interpretar_tipo(pokemon_tipo(pokemon_)));
	return true;
}

/**
 * Muestra por consola la lista de Pokémon cargados en el juego.
 *
 * Pre: contexto y juego son punteros válidos a datos de contexto y juego,
 *      respectivamente.
 * Post: Imprime por consola la información de cada Pokémon cargado en el juego.
 *       Retorna ESTADO_OK.
 */
COMANDO_ESTADO mostrar_pokemones_cargados(void *contexto, void *juego)
{
	juego_t *juego_ = juego;
	printf("Lista de Pokémon Disponibles:\n");
	lista_con_cada_elemento(juego_listar_pokemon(juego_), mostrar_pokemon,
				NULL);
	return ESTADO_OK;
}

/**
 * Muestra por consola el equipo de Pokémon de un jugador.
 *
 * Pre: contexto y lista_pokemones_usuario son punteros válidos a datos de contexto
 *      y a una lista de pokémones del usuario, respectivamente.
 * Post: Imprime por consola la información de cada Pokémon en la lista del usuario.
 *       Retorna ESTADO_OK si la operación se realiza correctamente.
 *       En caso de no tener pokémones en la lista, retorna SIN_POKES.
 */
COMANDO_ESTADO mostrar_equipo_pokemon(void *contexto,
				      void *lista_pokemones_usuario)
{
	lista_t *lista_pokemones = lista_pokemones_usuario;
	size_t cantidad =
		lista_con_cada_elemento(lista_pokemones, mostrar_pokemon, NULL);
	if (cantidad == 0) {
		return SIN_POKES;
	}
	return ESTADO_OK;
}

/**
 * Muestra por consola el equipo de Pokémon de un adversario.
 *
 * Pre: contexto y lista_pokemones_adversario son punteros válidos a datos de contexto
 *      y a una lista de pokémones del adversario, respectivamente.
 * Post: Imprime por consola la información de cada Pokémon en la lista del adversario.
 *       Retorna ESTADO_OK si la operación se realiza correctamente.
 *       En caso de no tener pokémones en la lista, retorna SIN_POKES.
 */
COMANDO_ESTADO
mostrar_equipo_pokemon_adversario(void *contexto,
				  void *lista_pokemones_adversario)
{
	lista_t *lista_pokemones = lista_pokemones_adversario;
	size_t cantidad = lista_con_cada_elemento(
		lista_pokemones, mostrar_pokemon_adversario, NULL);
	if (cantidad == 0) {
		return SIN_POKES;
	}
	return ESTADO_OK;
}

/**
 * Solicita al jugador el nombre de un Pokémon y el nombre de un ataque.
 *
 * Pre: -----------------------------------------------------------------
 * Post: Imprime por consola mensajes solicitando el nombre del Pokémon y del ataque,
 *       y retorna una estructura jugada_t con los datos ingresados por el jugador.
 */
jugada_t jugador_pedir_nombre_y_ataque()
{
	jugada_t j;
	char nombre_poke[20];
	char poke_ataque[20];
	printf("Ingrese el nombre del pokemon: ");
	leer_linea(nombre_poke);
	printf("Ingrese el nombre del ataque: ");
	leer_linea(poke_ataque);
	strcpy(j.pokemon, nombre_poke);
	strcpy(j.ataque, poke_ataque);
	return j;
}

/**
 * Realiza una jugada en el juego, solicitando al jugador el nombre de un Pokémon
 * y el nombre de un ataque, y ejecutando la jugada en el juego.
 *
 * Pre: El puntero contexto1 y datos no son nulo.
 * Post: Imprime por consola el resultado de la jugada y retorna el estado de la misma. Si la jugada del usuario no es válida
 * retornar JUGADA_ERROR, en caso contrario retorna ATAQUE_REALIZADO
 */
COMANDO_ESTADO realizar_jugada(void *contexto1, void *datos)
{
	datos_comprimidos_t *datos_ = datos;
	jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();
	if (verificar_datos(jugada_jugador, datos_)) {
		jugada_t jugada_adversario =
			adversario_proxima_jugada(datos_->adversario);
		printf("El adversario usó a: %s. %s usó %s\n",
		       jugada_adversario.pokemon, jugada_adversario.pokemon,
		       jugada_adversario.ataque);
		juego_jugar_turno(datos_->juego, jugada_jugador,
				  jugada_adversario);
		return ATAQUE_REALIZADO;
	}
	return JUGADA_ERROR;
}

/**
 * Imprime un mensaje asociado a un estado de comando en el juego.
 *
 * Pre: El estado pasado como argumento es válido.
 * Post: Imprime por consola un mensaje relacionado con el estado pasado.
 */
void imprimir_mensaje(COMANDO_ESTADO estado)
{
	switch (estado) {
	case COMANDO_INEXISTENTE:
		printf("Error, el comando no existe.\n");
		break;
	case POKES_INSUFICIENTES:
		printf("Error, el archivo no cuenta con pokemones suficientes\n");
		break;
	case ARCHIVO_YA_CARGADO:
		printf("Error, ya cargaste un archivo\n");
		break;
	case CARGA_EXITOSA:
		printf("El archivo se cargó exitosamente\n");
		break;
	case ERROR_CARGAR:
		printf("Error, la dirección no es válida o el archivo no existe\n");
		break;
	case POKES_REPETIDOS:
		printf("Error, no puedes elegir pokemones repetidos\n");
		break;
	case POKES_INEXISTENTES:
		printf("Error, no puedes elegir pokemones inexistentes\n");
		break;
	case SIN_POKES:
		printf("No hay pokemones para mostrar\n");
		break;
	case CARGA_POKES_EXITOSA:
		printf("La elección y carga de pokemones fue exitosa.\n");
		break;
	case ATAQUE_REALIZADO:
		printf("Ronda realizada.\n");
		break;
	case JUGADA_ERROR:
		printf("Jugada incorrecta.\n");
		break;
	case ESTADO_ERROR:
		printf("Se produjo un error general.\n");
		break;
	default:
		printf("Todo Ok\n");
		break;
	}
}

/**
 * Muestra el resultado del juego indicando si el jugador ganó, empató o perdió.
 *
 * Pre: El juego es válido.
 * Post: Imprime por consola el resultado del juego.
 */
void mostrar_resultado_juego(juego_t *juego)
{
	if (juego_obtener_puntaje(juego, JUGADOR1) >
	    juego_obtener_puntaje(juego, JUGADOR2)) {
		printf("GANASTE!\n");
	} else if (juego_obtener_puntaje(juego, JUGADOR1) ==
		   juego_obtener_puntaje(juego, JUGADOR2)) {
		printf("EMPATE!\n");
	} else {
		printf("PERDISTE!\n");
	}
}

int main(int argc, char *argv[])
{
	menu_t *menu = menu_crear();
	juego_t *juego = juego_crear();
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(juego));
	datos_comprimidos_t datos;
	datos.juego = juego;
	datos.adversario = adversario;
	lista_t *pokes_usuario_ = lista_crear();
	if (!pokes_usuario_) {
		return -1;
	}
	lista_t *pokes_adversario_ = lista_crear();
	if (!pokes_adversario_) {
		return -1;
	}
	datos.pokes_usuario = pokes_usuario_;
	datos.pokes_adversario = pokes_adversario_;
	bool salir = false;

	menu_agregar_comando(menu, "lista_crearrchivo", "p",
			     "Pide un archivo txt", pedir_archivo, juego);
	menu_agregar_comando(
		menu, "Mostrar Ayuda", "a",
		"Muestra un mensaje de ayuda sobre las acciones disponibles",
		mostrar_mensaje_ayuda, NULL);
	menu_agregar_comando(menu, "Salir del programa", "s",
			     "Sale del programa", salir_del_programa, &salir);
	menu_agregar_comando(menu, "Elegir pokemones", "e",
			     "Elige los pokemones presentes en el juego",
			     elegir_pokemones, &datos);
	menu_agregar_comando(menu, "Mostrar equipo pokemon usuario", "u",
			     "Muestra los pokemones elegidos por el usuario",
			     mostrar_equipo_pokemon, datos.pokes_usuario);
	menu_agregar_comando(menu, "Mostrar todos los pokemones", "m",
			     "Mostrar todos los pokemones del archivo",
			     mostrar_pokemones_cargados, datos.juego);
	menu_agregar_comando(menu, "Mostrar equipo pokemon adversario", "d",
			     "Muestra los pokemones del adversario",
			     mostrar_equipo_pokemon_adversario,
			     datos.pokes_adversario);
	menu_agregar_comando(menu, "Realizar jugada", "r",
			     "Escoge un pokemon y realiza un ataque",
			     realizar_jugada, &datos);

	char linea[120];

	while (!salir && !juego_finalizado(juego)) {
		printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
		leer_linea(linea);
		COMANDO_ESTADO estado = menu_ejecutar_comando(menu, linea);
		imprimir_mensaje(estado);
		if (estado == ATAQUE_REALIZADO) {
			printf("Tus puntos son: %i\n",
			       juego_obtener_puntaje(juego, JUGADOR1));
			printf("Los puntos del adversario son: %i\n",
			       juego_obtener_puntaje(juego, JUGADOR2));
		}
	}

	if (!salir) {
		mostrar_resultado_juego(juego);
	}

	lista_destruir(pokes_usuario_);
	lista_destruir(pokes_adversario_);
	menu_destruir(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);
	return 0;
}
