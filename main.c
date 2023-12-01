#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "src/menu_comandos.h"
#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"

#define MAX_LENGTH 120
#define MAX_STRING 100

/*
Pre:  Recibe dos punteros void no nulos (contexto1 y contexto2).
Post: Devuelve un booleano que indica si se pudo solicitar y almacenar un archivo correctamente:
      - Si la asignación de memoria para el archivo es exitosa, devuelve true.
      - Si la asignación de memoria falla, devuelve false.
      - El contenido ingresado por el usuario se copia en el espacio apuntado por contexto2.
      - La memoria asignada para el archivo se libera adecuadamente.
*/
bool pedir_archivo(void* contexto1, void* archivo) {
    char* nombre_archivo = calloc(100, sizeof(char));  // Asigna memoria para el nombre del archivo
    if (nombre_archivo == NULL) {
        return false;
    }

    printf("Ingrese un archivo para cargar: ");
    scanf("%s", nombre_archivo);
    strcpy((char*)archivo, nombre_archivo);
    free(nombre_archivo);  

    return true;
}

/*
Pre: ------------------------------------------------
Post:Muestra un mensaje de ayuda por pantalla con los comandos disponibles
*/
bool mostrar_mensaje_ayuda(void *arg1, void *arg2) {
    printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                  LISTA COMANDOS DISPONIBLES                  ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║'a'(Ayuda): muestra los comandos disponibles por consola.     ║\n"
	       "	║'s'(salir): Cierra el programa.                               ║\n"
	       "	║'l'(Limpiar): Borra todas las acciones en la terminal.        ║\n"
		   "	║'e'(Cargar): Empezar el juego.                                ║\n"
		   "	║'p'(Pedir): Pide el archivo y realiza validaciones.           ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
    return true;
}

/*
Pre: ---------------------------------------------------
Post: Limpia la terminal
*/
bool limpiar_terminal(void *_menu, void *_contexto)
{
    printf("\033[2J\033[H"); // Secuencia de escape ANSI para borrar la pantalla
	return true;
}

/*
Pre:-----------------------------------------------
Post: dado un menu, retorna true si este no es NULL.
*/
bool salir_del_programa(void* menu, void* finalizado){
	if(!menu) return false;
	printf("Saliste del programa\n");
	*(bool*)finalizado = true;
	return true;
}

/*
Pre:  Recibe dos punteros no nulos a cadenas de caracteres (entrada e instruccion). 
      Se asume que instruccion tiene suficiente espacio asignado para almacenar la instrucción.
Post: Lee una línea de entrada desde stdin y almacena la instrucción resultante en instruccion.
      Convierte la instrucción a minúsculas.
      Devuelve un puntero a la cadena de instruccion.
*/
void registrar_acciones(char* instruccion){
	printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
	scanf("%s", instruccion);
	for(int i = 0; instruccion[i]; i++){
		instruccion[i] = (char)tolower(instruccion[i]);
	}
} 


/*
Pre:  Recibe un puntero no nulo a una estructura de tipo pokemon_t correctamente inicializada.
Post: Muestra por stdout la información del pokemon, incluyendo su nombre y tipo.
      Si el puntero al pokemon es nulo, la función no realiza ninguna acción.
*/
void mostrar_informacion_pokemon(pokemon_t* pokemon) {
    if (!pokemon) {
        return;
    }

    printf("%s; Tipo: %i", pokemon_nombre(pokemon), pokemon_tipo(pokemon));
    printf("\n");
}

/*
Pre:  Recibe dos punteros no nulos, uno a una estructura de tipo pokemon_t (elemento) y otro a un contexto (contexto).
Post: Muestra la información del pokemon (nombre y tipo) utilizando la función mostrar_informacion_pokemon.
      Devuelve un booleano indicando si se debe continuar iterando (true) o detener la iteración (false).
      Si alguno de los punteros es nulo, la función devuelve false para detener la iteración.
*/
bool mostrar_pokemon(void* elemento, void* contexto) {
	if(!elemento) return false;
    pokemon_t* pokemon = (pokemon_t*)elemento;
    mostrar_informacion_pokemon(pokemon);
    return true;
}

/*
Pre:  Recibe un puntero no nulo a una estructura de tipo juego_t.
Post: Muestra la lista de Pokémon disponibles utilizando la función mostrar_pokemon
      en cada elemento de la lista obtenida mediante juego_listar_pokemon.
      Si el puntero al juego es nulo, la función no realiza ninguna acción.
*/
void mostrar_pokemon_disponibles(juego_t* juego) {
    if (!juego) {
        return;
    }

    printf("Lista de Pokémon Disponibles:\n");
    lista_con_cada_elemento(juego_listar_pokemon(juego), mostrar_pokemon, NULL);
}

void mostrar_pokemon_disponibles_jugador(juego_t* juego, char* eleccionJugador1, char* eleccionJugador2, char* eleccionAdversario3) {
    if (!juego) {
        return;
    }
    printf("Tus pokemones son: %s\n", eleccionJugador1);
	printf("Tus pokemones son: %s\n", eleccionJugador2);
    printf("Tus pokemones son: %s\n", eleccionAdversario3);

}

void jugador_seleccionar_pokemon(char eleccionJugador1[50], char eleccionJugador2[50], char eleccionJugador3[50]){
	printf("Ingresa un pokemon para ti: ");
	scanf("%s", eleccionJugador1);
	printf("Ingresa un pokemon para ti: ");
	scanf("%s", eleccionJugador2);
	printf("Ingresa un pokemon para el adversario: ");
	scanf("%s", eleccionJugador3);
}

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

int main(int argc, char *argv[])		
{	
	menu_t* menu = menu_crear();
	juego_t *juego = juego_crear();
	adversario_t *adversario = adversario_crear(juego_listar_pokemon(juego));

	char archivo[100];

	menu_agregar_comando(menu, "Pedir Archivo", "p", "Pide un archivo txt", pedir_archivo, archivo);
	menu_agregar_comando(menu, "Mostrar Ayuda", "a", "Muestra un mensaje de ayuda sobre las acciones disponibles", mostrar_mensaje_ayuda, NULL);

	menu_ejecutar_comando(menu, "p");
	JUEGO_ESTADO estado = juego_cargar_pokemon(juego, archivo);
	if(estado == ERROR_GENERAL){
		printf("Error General\n");
	}else if(estado == POKEMON_INSUFICIENTES){
		printf("El archivo no contiene la cantidad de pokemones suficientes para jugar");
	}else{
		mostrar_pokemon_disponibles(juego);
	}

	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
	char eleccionJugador1[50];
	char eleccionJugador2[50];
	char eleccionJugador3[50];

	jugador_seleccionar_pokemon(eleccionJugador1, eleccionJugador2, eleccionJugador3);
	juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1, eleccionJugador2, eleccionJugador3);

	//pedirle al adversario que indique los 3 pokemon que quiere usar
	char *eleccionAdversario1 = NULL;
	char *eleccionAdversario2 = NULL;
	char *eleccionAdversario3 = NULL;

	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1, &eleccionAdversario2, &eleccionAdversario3);
	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1, eleccionAdversario2, eleccionAdversario3);

	//informarle al adversario cuales son los pokemon del jugador
	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
					eleccionJugador2, eleccionJugador3);

	while (!juego_finalizado(juego)) {
		resultado_jugada_t resultado_ronda;
		
		//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
		mostrar_pokemon_disponibles_jugador(juego, eleccionJugador1, eleccionJugador2, eleccionAdversario3);

		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
		jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();

		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
		jugada_t jugada_adversario =
			adversario_proxima_jugada(adversario);

		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
						    jugada_adversario);

		if (resultado_ronda.jugador1 == ATAQUE_ERROR){
			printf("Hubo un error.\n");
			break;
		}

		printf("Tus puntos son: %i\n", juego_obtener_puntaje(juego, JUGADOR1));
		printf("Los puntos del adversario son: %i\n", juego_obtener_puntaje(juego, JUGADOR2));

	}
	
	if(juego_obtener_puntaje(juego, JUGADOR1) > juego_obtener_puntaje(juego, JUGADOR2)){
		printf("GANASTE!!!!!!!");
	}else if(juego_obtener_puntaje(juego, JUGADOR1) == juego_obtener_puntaje(juego, JUGADOR2)){
		printf("EMPATE!!!!!!!");
	}else{
		printf("PERDISTE!!!");
	}


	menu_destruir(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);
	return 0;
}