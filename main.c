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

bool pedir_archivo(void *menu, void *contexto) {
    char nombre_ingresado[MAX_LENGTH];

    printf("Ingrese el nombre del archivo: ");
    fgets(nombre_ingresado, MAX_LENGTH, stdin);

    size_t len = strlen(nombre_ingresado);
    if (len > 0 && nombre_ingresado[len - 1] == '\n') {
        nombre_ingresado[len - 1] = '\0';
    }

    FILE *archivo = fopen(nombre_ingresado, "r");

    if (archivo) {
        fclose(archivo);
        printf("Archivo cargado exitosamente: %s\n", nombre_ingresado);
		contexto = nombre_ingresado;
        return true;
    } else {
        printf("Error: El archivo %s no existe o no se pudo abrir.\n", nombre_ingresado);
        return false;
    }
}

bool mostrar_mensaje_ayuda(void *arg1, void *arg2) {
    printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                  LISTA COMANDOS DISPONIBLES                  ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║'a'(Ayuda): muestra los comandos disponibles por consola.     ║\n"
	       "	║'s'(salir): Cierra el programa.                               ║\n"
	       "	║'l'(Limpiar): Borra todas las acciones en la terminal.        ║\n"
		   "	║'c'(Cargar): Carga el archivo de pokemones.                   ║\n"
		   "	║'p'(Pedir): Pide el archivo y realiza validaciones.           ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
    return true;
}

bool limpiar_terminal(void *_menu, void *_contexto)
{
    printf("\033[2J\033[H"); // Secuencia de escape ANSI para borrar la pantalla
	return true;
}

bool salir_del_programa(void* menu, void* instruccion){
	if(!menu) return false;
	return true;
}

// char* analizar_comando(void* instruccion){
// 	if (!instruccion)
// 		return NULL;
// 	if (strcmp(instruccion, "clear") == 0 || strcmp(instruccion, "limpiar") == 0) {
// 		strcpy(instruccion, "l");
// 	} else if (strcmp(instruccion, "help") == 0 || strcmp(instruccion, "ayuda") == 0) {
// 		strcpy(instruccion, "a");
// 	} else if (strcmp(instruccion, "salir") == 0 || strcmp(instruccion, "exit") == 0 || strcmp(instruccion, "quit") == 0 || strcmp(instruccion, "q") == 0) {
// 		strcpy(instruccion, "s");
// 	} else if (strcmp(instruccion, "cargar") == 0) {
// 		strcpy(instruccion, "c");
// 	}
// 	return instruccion;
// }

char* registrar_acciones(char* entrada, char* instruccion){
	printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
	fgets(entrada, MAX_STRING, stdin);
	sscanf(entrada,"%s", instruccion);
	for(int i = 0; instruccion[i]; i++){
		instruccion[i] = (char)tolower(instruccion[i]);
	}
	return instruccion;
} 


/**
* Este main debe ser modificado para que el usuario pueda jugar el juego. Las
* instrucciones existentes son solamente a modo ilustrativo del funcionamiento
* muy alto nivel del juego.
*
* Las interacciones deben realizarse por entrada/salida estandar y estar validadas.
*
* Se aconseja en todo momento mostrar información relevante para el jugador (por
* ejemplo, mostrar puntaje actual y movimientos disponibles) para hacer que el
* juego sea facil de utilizar.
*/

int main(int argc, char *argv[])
{	
	menu_t* menu = menu_crear();
	juego_t *juego = juego_crear();
	char* nombre_archivo = NULL;
	bool finalizado = false;

	menu_agregar_comando(menu, "Pedir Archivo", "p", "Pide un archivo txt", pedir_archivo, (void*)nombre_archivo);
	menu_agregar_comando(menu, "Mostrar Ayuda", "a", "Muestra un mensaje de ayuda sobre las acciones disponibles", mostrar_mensaje_ayuda, NULL);
	menu_agregar_comando(menu, "Limpiar terminal", "l", "Borra todo lo que esté en la terminal", limpiar_terminal, NULL);
	menu_agregar_comando(menu, "Salir del programa", "s", "Sale del programa y destruye el menu", salir_del_programa, NULL);

	char entrada[MAX_STRING];
	char instruccion[MAX_STRING];
	while (!finalizado) {
		if (strcmp(registrar_acciones(entrada, instruccion), "s") == 0)
			finalizado = true;
		if (!menu_ejecutar_comando(menu, instruccion)){
			printf("El comando no es válido.\n");
		}
	}

	menu_destruir(menu);
	juego_destruir(juego);
	return 0;
}


/*

	//Crea un adversario que será utilizado como jugador 2
	//adversario_t *adversario =
	// 	adversario_crear(juego_listar_pokemon(juego));

	// //Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
	// mostrar_pokemon_disponibles(juego);

		// //Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
		// char *eleccionJugador1, *eleccionJugador2, *eleccionJugador3;
		// jugador_seleccionar_pokemon(juego, &eleccionJugador1,
		// 			    &eleccionJugador2,
		// 			    &eleccionJugador3);

		// //pedirle al adversario que indique los 3 pokemon que quiere usar
		// // char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
		// // adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
	// // 			       &eleccionAdversario2,
	// // 			       &eleccionAdversario3);

	// //Seleccionar los pokemon de los jugadores
	// juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
	// 			  eleccionJugador2, eleccionJugador3);
	// // juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1,
	// // 			  eleccionAdversario2, eleccionAdversario3);

	// //informarle al adversario cuales son los pokemon del jugador
	// // adversario_pokemon_seleccionado(adversario, eleccionJugador1,
	// // 				eleccionJugador2, eleccionJugador3);

	// while (!juego_finalizado(juego)) {
	// 	resultado_jugada_t resultado_ronda;

	// 	//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
	// 	// jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();

	// 	//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
	// 	// jugada_t jugada_adversario =
	// 	// 	adversario_proxima_jugada(adversario);

	// 	//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
	// 	resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
	// 					    jugada_adversario);

	// 	//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
	// 	//adversario_informar_jugada(adversario, jugada_jugador);
	// }

	//juego_destruir(juego);
	//adversario_destruir(adversario);
*/