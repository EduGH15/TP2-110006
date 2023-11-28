#include <time.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"

struct adversario {
    lista_t* pokemones;
    abb_t* ataques_disponibles;
    //lista_t* pokemones_jugador;
    int puntaje;
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon) {
        return NULL;
    }

    adversario_t* adversario = calloc(1,sizeof(adversario_t));
    if (!adversario) {
        return NULL;
    }

    adversario->pokemones = lista_crear();
    adversario->ataques_disponibles = abb_crear(comparador_cadenas);
    //adversario->pokemones_jugador = lista_crear();

    if(!adversario->pokemones_disponibles   || !adversario->ataques_disponibles || !adversario->pokemones_jugador) {
        free(adversario);
        return NULL;
    }

    adversario->puntaje = 0;

    return adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{	
	//impelemnto la logica
	//Consejo de ubermasterpro: uso la funcion rand para elegir los 3 pokemones: 2 del adversario y 1 del jugador (No se pueden repetir)
	//Si se repiten, vuelve a ejecutar srand, puedo usar goto
	return false;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{	
	//Se parece a la funcion de juego_seleccionar_pokemon de juego.c pero solo se carga los pokemones del jugador2
	/*
	nombre 1 = pokej1 1
nombre 2 = pokej1 2
nombre 3 = pokeJ2 3
	*/
	return false;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{	
	//Camino Sebas Truco: simple, rapido y supersencillo para regularizar la cursada
	//Puedo reutilizar la funcion de ataque_ya_utilizado para ver si el ataque sigue o no en el abb. Si esta utilizado, vuelve a 
	//Buscar
	//Busco pokemon random y ataque random (Me fijo si esta o no utilizado)
	//Realizo el ataque sacandolo del abb
	jugada_t j = { .ataque = "", .pokemon = "" };
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	//Dejar vacia o cambiar estrategia para hacerlo mas inteligente. 
}

void adversario_destruir(adversario_t *adversario)
{	
 	if (adversario) {
		lista_destruir(adversario->pokemones);
		abb_destruir(adversario->ataques_disponibles);
		free(adversario);

    }
}


// #include "lista.h"
// #include "juego.h"
// #include "adversario.h"
// #include "pokemon.h"

//     

// /*======================================== FUNCIONES PRIVADAS =================================================*/

// // Definición de la función de comparación para cadenas
// int comparador_cadenas(void *elemento1, void *elemento2) {
//     const char *cadena1 = (const char *)elemento1;
//     const char *cadena2 = (const char *)elemento2;

//     return strcmp(cadena1, cadena2);
// }

// // Función comparadora para buscar un pokemon por nombre en la lista
// int comparador_buscar_pokemon(void *elemento, void *contexto) {
//     if(!elemento || !contexto){
//         return 0;
//     }
//     const char *nombre_pokemon = (const char *)contexto;
//     pokemon_t *pokemon = (pokemon_t *)elemento;

//     return strcmp(pokemon_nombre(pokemon), nombre_pokemon);
// }

// /*======================================== FUNCIONES PRINCIPAL =================================================*/


// adversario_t *adversario_crear(lista_t *pokemon)
// {	
/
// }

// bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1, char **nombre2, char **nombre3) {
//     if (!adversario || !adversario->pokemones || !nombre1 || !nombre2 || !nombre3) {
//         return false;
//     }

//     // Obtener los dos primeros pokemones de la lista del adversario
//     pokemon_t *pokemon1 = lista_elemento_en_posicion(adversario->pokemones, 0);
//     pokemon_t *pokemon2 = lista_elemento_en_posicion(adversario->pokemones, 1);

//     // Obtener el tercer pokémon de la lista del jugador (pokémon para el jugador)
//     pokemon_t *pokemon3 = lista_elemento_en_posicion(adversario->pokemones_jugador, 2);

//     // Verificar si los pokemones existen
//     if (!pokemon1 || !pokemon2 || !pokemon3) {
//         return false;
//     }

//     lista_insertar(adversario->pokemones, pokemon1);
//     lista_insertar(adversario->pokemones, pokemon2);
//     lista_insertar(adversario->pokemones_jugador, pokemon3);

//     return true;
// }

// bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1, char *nombre2, char *nombre3) {
//     if (!adversario || !adversario->pokemones || !nombre1 || !nombre2 || !nombre3) {
//         return false;
//     }

//     // Buscar los pokemones por nombre en la lista del adversario
//     pokemon_t *pokemon1 = lista_buscar_elemento(adversario->pokemones_jugador, comparador_buscar_pokemon, nombre1);
//     pokemon_t *pokemon2 = lista_buscar_elemento(adversario->pokemones_jugador, comparador_buscar_pokemon, nombre2);
//     pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemones_jugador, comparador_buscar_pokemon, nombre3);

//     // Verificar si los pokemones existen
//     if (!pokemon1 || !pokemon2 || !pokemon3) {
//         return false;
//     }

//     // Agregar el tercer pokémon a la lista del adversario
//     lista_insertar(adversario->pokemones, pokemon3);

//     return true;
// }

// jugada_t adversario_proxima_jugada(adversario_t *adversario)
// {
// 	jugada_t jugada_adversario;
//     jugada_adversario.pokemon = ATAQUE_ERROR;
//     jugada_adversario.ataque = ATAQUE_ERROR;

//     if (!adversario || !adversario->pokemones || !adversario->ataques_disponibles) {
//         return jugada_adversario;
//     }

//     // Obtener el primer pokemon de la lista del adversario
//     pokemon_t *pokemon_adversario = lista_elemento_en_posicion(adversario->pokemones, 0);
    
//     // Obtener un ataque disponible del adversario
//     //struct ataque *ataque_adversario = abb_buscar(adversario->ataques_disponibles);

//     // Verificar que se haya obtenido un pokemon y un ataque
//     if (!pokemon_adversario || !ataque_adversario) {
//         return jugada_adversario;
//     }

//     // Asignar el pokemon y el ataque a la jugada
//     jugada_adversario.pokemon = pokemon_nombre(pokemon_adversario);
//     jugada_adversario.ataque = ataque_adversario->nombre;

//     // Eliminar el ataque utilizado del adversario
//     abb_quitar(adversario->ataques_disponibles, jugada_adversario.ataque);

//     return jugada_adversario;
// }

// void adversario_informar_jugada(adversario_t *a, jugada_t j)
// {
// }

// void adversario_destruir(adversario_t *adversario)
// {

// }
