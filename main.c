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
bool pedir_archivo(void* contexto1, void* contexto2) {
    char* archivo = calloc(20, sizeof(char));
    if (archivo == NULL) {
        return false;
    }

    printf("Ingrese un archivo para cargar: ");
    scanf("%s", archivo);

    strcpy((char*)contexto2, archivo);

    free(archivo);

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
bool salir_del_programa(void* menu, void* instruccion){
	if(!menu) return false;
	return true;
}

/*
Pre:  Recibe dos punteros no nulos a cadenas de caracteres (entrada e instruccion). 
      Se asume que instruccion tiene suficiente espacio asignado para almacenar la instrucción.
Post: Lee una línea de entrada desde stdin y almacena la instrucción resultante en instruccion.
      Convierte la instrucción a minúsculas.
      Devuelve un puntero a la cadena de instruccion.
*/
char* registrar_acciones(char* entrada, char* instruccion){
	printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
	fgets(entrada, MAX_STRING, stdin);
	sscanf(entrada,"%s", instruccion);
	for(int i = 0; instruccion[i]; i++){
		instruccion[i] = (char)tolower(instruccion[i]);
	}
	return instruccion;
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
	if(!elemento || !contexto) return false;
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

int main(int argc, char *argv[])
{	
	menu_t* menu = menu_crear();
	juego_t *juego = juego_crear();
	adversario_t *adversario = adversario_crear(juego_listar_pokemon(juego));

	char nombre_archivo[20];
	bool finalizado = false;

	menu_agregar_comando(menu, "Pedir Archivo", "p", "Pide un archivo txt", pedir_archivo, nombre_archivo);
	menu_agregar_comando(menu, "Mostrar Ayuda", "a", "Muestra un mensaje de ayuda sobre las acciones disponibles", mostrar_mensaje_ayuda, NULL);
	menu_agregar_comando(menu, "Limpiar terminal", "l", "Borra todo lo que esté en la terminal", limpiar_terminal, NULL);
	menu_agregar_comando(menu, "Salir del programa", "s", "Sale del programa y destruye el menu", salir_del_programa, NULL);
	char entrada[MAX_STRING];
	char instruccion[MAX_STRING];

	seccion_1:
	while (!finalizado) {
		const char * comando = registrar_acciones(entrada, instruccion);
		if (strcmp(comando, "s") == 0){
			finalizado = true;
		}else if(strcmp(comando, "e") == 0){
			juego_cargar_pokemon(juego, nombre_archivo);
		}else if (!menu_ejecutar_comando(menu, instruccion)){
			printf("El comando no es válido o hubo un error de ejecución.\n");
			goto seccion_1;
		}		
	}		

	menu_destruir(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);
	return 0;
}