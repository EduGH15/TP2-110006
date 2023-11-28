#include <time.h>
#include <string.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"
#include "funciones_varias.h"

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

    if(!adversario->pokemones || !adversario->ataques_disponibles) {
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