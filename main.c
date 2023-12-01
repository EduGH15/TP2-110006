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

typedef struct datos_comprimidos{
	juego_t* juego;
	adversario_t* adversario;
	lista_t* pokes_usuario;
	lista_t* pokes_adversario;
}datos_comprimidos_t;

void leer_linea(char linea[])
{
	char *leido;
	leido = fgets(linea, 200, stdin);
	if (leido != NULL) {
		linea[strlen(linea) - 1] = 0;
	}
}

char* interpretar_tipo(enum TIPO tipo){
	if(tipo == NORMAL){
		return "NORMAL";
	}else if(tipo == FUEGO){
		return "FUEGO";
	}else if(tipo == AGUA){
		return"AGUA";
	}else if(tipo == PLANTA){
		return "PLANTA";
	}else if(tipo == ELECTRICO){
		return "ELECTRICO";
	}
	return "ROCA";
}

void listar_pokemones(lista_t *lista_juego, lista_t* lista_pokes, char* eleccion1, char* eleccion2, char* eleccion3){

	pokemon_t* pokemon1 = lista_buscar_elemento(lista_juego, comparador_cadenas, eleccion1);
	pokemon_t* pokemon2 = lista_buscar_elemento(lista_juego, comparador_cadenas, eleccion2);
	pokemon_t* pokemon3 = lista_buscar_elemento(lista_juego, comparador_cadenas, eleccion3);

	printf("%s\n", pokemon_nombre(pokemon1));
	printf("%s\n", pokemon_nombre(pokemon2));
	printf("%s\n", pokemon_nombre(pokemon3));

	lista_insertar(lista_pokes, pokemon1);
	lista_insertar(lista_pokes, pokemon2);
	lista_insertar(lista_pokes, pokemon3);
}

/*
Pre:  Recibe dos punteros void no nulos (contexto1 y contexto2).
Post: Devuelve un booleano que indica si se pudo solicitar y almacenar un archivo correctamente:
      - Si la asignación de memoria para el archivo es exitosa, devuelve true.
      - Si la asignación de memoria falla, devuelve false.
      - El contenido ingresado por el usuario se copia en el espacio apuntado por contexto2.
      - La memoria asignada para el archivo se libera adecuadamente.
*/
COMANDO_ESTADO pedir_archivo(void* contexto1, void* juego) {
    char nombre_archivo[50];
    printf("Ingrese un archivo para cargar: ");
	leer_linea(nombre_archivo);
	JUEGO_ESTADO estado = juego_cargar_pokemon(juego, nombre_archivo);
	if(estado == POKEMON_INSUFICIENTES){
		return POKES_INSUFICIENTES;
	}else if(estado == ERROR_GENERAL){
		return ERROR_CARGAR;
	}
    return CARGA_EXITOSA;
}

/*
Pre: ------------------------------------------------
Post:Muestra un mensaje de ayuda por pantalla con los comandos disponibles
*/
COMANDO_ESTADO mostrar_mensaje_ayuda(void *arg1, void *arg2) {
    printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                  LISTA COMANDOS DISPONIBLES                  ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║'a'(Ayuda): muestra los comandos disponibles por consola.     ║\n"
	       "	║'s'(salir): Cierra el programa.                               ║\n"
	       "	║'l'(Limpiar): Borra todas las acciones en la terminal.        ║\n"
		   "	║'e'(Cargar): Empezar el juego.                                ║\n"
		   "	║'p'(Pedir): Pide el archivo y realiza validaciones.           ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
    return ESTADO_OK;
}


/*
Pre:-----------------------------------------------
Post: dado un menu, retorna true si este no es NULL.
*/
COMANDO_ESTADO salir_del_programa(void* menu, void* finalizado){
	if(!menu) return ESTADO_ERROR;
	printf("Saliste del programa\n");
	*(bool*)finalizado = true;
	return ESTADO_OK;
}


/*
Pre:  Recibe un puntero no nulo a una estructura de tipo juego_t.
Post: Muestra la lista de Pokémon disponibles utilizando la función mostrar_pokemon
      en cada elemento de la lista obtenida mediante juego_listar_pokemon.
      Si el puntero al juego es nulo, la función no realiza ninguna acción.
*/
// void mostrar_pokemon_disponibles(juego_t* juego) {
//     if (!juego) {
//         return;
//     }
// 	printf("\n");
//     printf("Lista de Pokémon Disponibles:\n");
//     lista_con_cada_elemento(juego_listar_pokemon(juego), mostrar_pokemon, NULL);
// }

void mostrar_pokemon_disponibles_jugador(juego_t* juego, char* eleccionJugador1, char* eleccionJugador2, char* eleccionAdversario3) {
    if (!juego) {
        return;
    }
    printf("Tus pokemones son: %s\n", eleccionJugador1);
	printf("Tus pokemones son: %s\n", eleccionJugador2);
    printf("Tus pokemones son: %s\n", eleccionAdversario3);

}

// void jugador_seleccionar_pokemon(char eleccionJugador1[50], char eleccionJugador2[50], char eleccionJugador3[50]){
// 	printf("\n");
// 	printf("Ingresa un pokemon para ti: ");
// 	scanf("%s", eleccionJugador1);
// 	printf("Ingresa un pokemon para ti: ");
// 	scanf("%s", eleccionJugador2);
// 	printf("Ingresa un pokemon para el adversario: ");
// 	scanf("%s", eleccionJugador3);
// }

jugada_t jugador_pedir_nombre_y_ataque(){
	jugada_t j;
	char nombre_poke[20];
	char poke_ataque[20];
	printf("Ingrese el nombre del pokemon: ");
	scanf("%s", nombre_poke);
	printf("Ingrese el nombre del ataque: ");
	scanf("%s", poke_ataque);
	strcpy(j.pokemon, nombre_poke);
	strcpy(j.ataque, poke_ataque);
	return j;
}

// char* registrar_acciones(char* entrada, char* instruccion){
// 	printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
// 	fgets(entrada, MAX_STRING, stdin);
// 	sscanf(entrada,"%s", instruccion);
// 	for(int i = 0; instruccion[i]; i++){
// 		instruccion[i] = (char)tolower(instruccion[i]);
// 	}
// 	return instruccion;
// } 

COMANDO_ESTADO elegir_pokemones(void* contexto, void* datos){
	datos_comprimidos_t* datos_ = datos;
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
	estado = juego_seleccionar_pokemon(datos_->juego,JUGADOR1, eleccionJugador1, eleccionJugador2, eleccionJugador3);
	if(estado == POKEMON_REPETIDO){
		return POKES_REPETIDOS;
	}else if(estado == POKEMON_INEXISTENTE){
		return POKES_INEXISTENTES;
	}
	char *eleccionAdversario1 = NULL;
	char *eleccionAdversario2 = NULL;
	char *eleccionAdversario3 = NULL;

	adversario_seleccionar_pokemon(datos_->adversario, &eleccionAdversario1, &eleccionAdversario2, &eleccionAdversario3);
	juego_seleccionar_pokemon(datos_->juego, JUGADOR2, eleccionAdversario1, eleccionAdversario2, eleccionAdversario3);
	adversario_pokemon_seleccionado(datos_->adversario, eleccionJugador1, eleccionJugador2, eleccionJugador3);

	listar_pokemones(juego_listar_pokemon(datos_->juego),datos_->pokes_usuario, eleccionJugador1, eleccionJugador2, eleccionAdversario3);

	listar_pokemones(juego_listar_pokemon(datos_->juego),datos_->pokes_adversario, eleccionAdversario1, eleccionAdversario2, eleccionJugador3);
	return CARGA_POKES_EXITOSA;
}

/*
Pre:  Recibe dos punteros no nulos, uno a una estructura de tipo pokemon_t (elemento) y otro a un contexto (contexto).
Post: Muestra la información del pokemon (nombre y tipo) utilizando la función mostrar_informacion_pokemon.
      Devuelve un booleano indicando si se debe continuar iterando (true) o detener la iteración (false).
      Si alguno de los punteros es nulo, la función devuelve false para detener la iteración.
*/
bool mostrar_pokemon(void* pokemon, void* contexto) {
    pokemon_t* pokemon_ = pokemon;
	printf("%s; TIPO %s\n", pokemon_nombre(pokemon_), interpretar_tipo(pokemon_tipo(pokemon_)));
    return true;
}

COMANDO_ESTADO mostrar_pokemones_cargados(void* contexto, void* juego){
	juego_t* juego_ = juego;
    printf("Lista de Pokémon Disponibles:\n");
    lista_con_cada_elemento(juego_listar_pokemon(juego_), mostrar_pokemon, NULL);
	return ESTADO_OK;
}

COMANDO_ESTADO mostrar_equipo_pokemon(void* contexto, void* lista_pokemones_usuario){
	lista_t* lista_pokemones = lista_pokemones_usuario;
	lista_con_cada_elemento(lista_pokemones, mostrar_pokemon, NULL);
	return ESTADO_OK;
}

int main(int argc, char *argv[])
{	
	menu_t* menu = menu_crear();
	juego_t *juego = juego_crear();
	adversario_t *adversario = adversario_crear(juego_listar_pokemon(juego));
	datos_comprimidos_t datos;
	datos.juego = juego;
	datos.adversario = adversario;
	lista_t* pokes_usuario_ = lista_crear();
	if(!pokes_usuario_){
		return -1;
	}
	lista_t* pokes_adversario_ = lista_crear();
	if(!pokes_adversario_){
		return -1;
	}
	datos.pokes_usuario = pokes_usuario_;
	datos.pokes_adversario = pokes_adversario_;
	bool finalizado = false;

	menu_agregar_comando(menu, "lista_crearrchivo", "p", "Pide un archivo txt", pedir_archivo, juego);
	menu_agregar_comando(menu, "Mostrar Ayuda", "a", "Muestra un mensaje de ayuda sobre las acciones disponibles", 
	mostrar_mensaje_ayuda, NULL);
	menu_agregar_comando(menu, "Salir del programa", "s", "Sale del programa", salir_del_programa, &finalizado);
	menu_agregar_comando(menu, "Elegir pokemones", "e", "Elige los pokemones presentes en el juego", elegir_pokemones, &datos);
	menu_agregar_comando(menu, "Mostrar equipo pokemon usuario", "u", "Carga los pokemones del usuario y los muestra", mostrar_equipo_pokemon, datos.pokes_usuario);
	menu_agregar_comando(menu, "Mostrar todos los pokemones", "m", "Mostrar todos los pokemones del archivo", mostrar_pokemones_cargados, datos.juego);

	char linea[120];

	while (!finalizado && !juego_finalizado(juego)) {
		printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
		leer_linea(linea);
		COMANDO_ESTADO estado = menu_ejecutar_comando(menu, linea);

		if(estado == POKES_INSUFICIENTES){
			printf("POKEMONE INSUFICIENTESSSSS\n");
		}else if(estado == COMANDO_INEXISTENTE){
			printf("EL COMANDO NO EXISTE\n");
		}else if(estado == CARGA_EXITOSA){
			printf("Todo ok\n");
		}else if(estado == ERROR_CARGAR){
			printf("La direccion no es valida o el archivo no existe\n");
		}else if(estado == POKES_REPETIDOS){
			printf("No puedes elegir pokemones repetidos\n");
		}else if(estado == POKES_INEXISTENTES){
			printf("Error, no puedes elegir pokemones inexistentes\n");
		}else if(estado == POKES_REPETIDOS){
			printf("Error, no puedes elegir pokemones repetidos\n");
		}else if(estado == CARGA_POKES_EXITOSA){
			printf("La elección y carga de pokemones fue exitosa.\n");
		}
	}

	//VERIFICAR LA JUGADA DEL USUARIO ANTES DE QUE ENTRE EN EL ADVERSARIO_PROXIMA_JUGADA. 
	
	lista_destruir(pokes_usuario_);
	lista_destruir(pokes_adversario_);
	menu_destruir(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);
	return 0;
}

// int main(int argc, char *argv[])		
// {	
// 	menu_t* menu = menu_crear();
// 	juego_t *juego = juego_crear();
// 	adversario_t *adversario = adversario_crear(juego_listar_pokemon(juego));

// 	char archivo[100];

// 	menu_agregar_comando(menu, "Pedir Archivo", "p", "Pide un archivo txt", pedir_archivo, archivo);
// 	menu_agregar_comando(menu, "Mostrar Ayuda", "a", "Muestra un mensaje de ayuda sobre las acciones disponibles", mostrar_mensaje_ayuda, NULL);
// 	/*
// 	listar --- l
// 	lanzar ugada --- j
// 	mostrar pokemones disponibles --- m
// 	mostrar pokemones adversario --- d
// 	salir ---- s
// 	*/

// 	menu_ejecutar_comando(menu, "p");
// 	JUEGO_ESTADO estado = juego_cargar_pokemon(juego, archivo);
// 	if(estado == ERROR_GENERAL){
// 		printf("Error General\n");
// 	}else if(estado == POKEMON_INSUFICIENTES){
// 		printf("El archivo no contiene la cantidad de pokemones suficientes para jugar");
// 	}else{
// 		mostrar_pokemon_disponibles(juego);
// 	}

// 	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
// 	char eleccionJugador1[50];
// 	char eleccionJugador2[50];
// 	char eleccionJugador3[50];

// 	jugador_seleccionar_pokemon(eleccionJugador1, eleccionJugador2, eleccionJugador3);
// 	juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1, eleccionJugador2, eleccionJugador3);

// 	//pedirle al adversario que indique los 3 pokemon que quiere usar
// 	char *eleccionAdversario1 = NULL;
// 	char *eleccionAdversario2 = NULL;
// 	char *eleccionAdversario3 = NULL;

// 	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1, &eleccionAdversario2, &eleccionAdversario3);
// 	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1, eleccionAdversario2, eleccionAdversario3);

// 	//informarle al adversario cuales son los pokemon del jugador
// 	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
// 					eleccionJugador2, eleccionJugador3);

// 	while (!juego_finalizado(juego)) {
// 		resultado_jugada_t resultado_ronda;
		
// 		//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
// 		mostrar_pokemon_disponibles_jugador(juego, eleccionJugador1, eleccionJugador2, eleccionAdversario3);

// 		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
// 		jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();
		
// 		//Hacer validación

// 		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
// 		jugada_t jugada_adversario =
// 			adversario_proxima_jugada(adversario);

// 		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
// 		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
// 						    jugada_adversario);

// 		if (resultado_ronda.jugador1 == ATAQUE_ERROR){
// 			printf("Hubo un error.\n");
// 			break;
// 		}

// 		printf("Tus puntos son: %i\n", juego_obtener_puntaje(juego, JUGADOR1));
// 		printf("Los puntos del adversario son: %i\n", juego_obtener_puntaje(juego, JUGADOR2));
// 	}

// 	if(juego_obtener_puntaje(juego, JUGADOR1) > juego_obtener_puntaje(juego, JUGADOR2)){
// 		printf("GANASTE!!!!!!!");
// 	}else if(juego_obtener_puntaje(juego, JUGADOR1) == juego_obtener_puntaje(juego, JUGADOR2)){
// 		printf("EMPATE!!!!!!!");
// 	}else{
// 		printf("PERDISTE!!!");
// 	}


// 	menu_destruir(menu);
// 	juego_destruir(juego);
// 	adversario_destruir(adversario);
// 	return 0;
// }