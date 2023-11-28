#include <time.h>
#include <string.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"
#include "funciones_varias.h"

struct adversario {
    lista_t* pokemon;
	lista_t* pokemones_disponibles;
    abb_t* ataques_disponibles;
	lista_t* pokemones_jugador;
	abb_t* ataques_jugador;

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

    adversario->pokemon = pokemon;
	adversario->pokemones_disponibles = lista_crear();
	if(!adversario->pokemones_disponibles){
		free(adversario);
		return NULL;
	}

    adversario->ataques_disponibles = abb_crear(comparador_cadenas);
	if(!adversario->ataques_disponibles){
		lista_destruir(adversario->pokemones_disponibles);
		free(adversario);
		return NULL;
	}
	adversario->pokemones_jugador = lista_crear();

    if(!adversario->pokemones_jugador) {
		lista_destruir(adversario->pokemones_disponibles);
		abb_destruir(adversario->ataques_disponibles);
        free(adversario);
        return NULL;
    }
    return adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{	
    srand((unsigned int)time(NULL));


    int cantidad_pokemon = (int)lista_tamanio(adversario->pokemon);

    if (cantidad_pokemon < 3) {
        return false;
    }

	int indice_pokemon1 = rand() % cantidad_pokemon;
	int indice_pokemon2;
	int indice_pokemon3;

seleccionar_pokemon2:
	indice_pokemon2 = rand() % cantidad_pokemon;
	if (indice_pokemon2 == indice_pokemon1) {
    	goto seleccionar_pokemon2;
	}

seleccionar_pokemon3:
	indice_pokemon3 = rand() % cantidad_pokemon;
	if (indice_pokemon3 == indice_pokemon1 || indice_pokemon3 == indice_pokemon2) {
	 	goto seleccionar_pokemon3;
	}

	pokemon_t* pokemon1 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon1);
	pokemon_t* pokemon2 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon2);
	pokemon_t* pokemon3 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon3);

	if(!pokemon1 || !pokemon2 || !pokemon3){
		return false;
	}

	lista_insertar(adversario->pokemon, pokemon1);
	lista_insertar(adversario->pokemon, pokemon2);
	lista_insertar(adversario->pokemones_jugador, pokemon3);
	
    con_cada_ataque(pokemon1, agregar_ataque_a_abb, adversario->ataques_disponibles);
	con_cada_ataque(pokemon2, agregar_ataque_a_abb, adversario->ataques_disponibles);
    con_cada_ataque(pokemon3, agregar_ataque_a_abb, adversario->ataques_jugador);

    return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{	
	if (!adversario || !nombre1 || !nombre2 || !nombre3) {
        return false;
    }

    // Buscar los pokémon por nombre en la información disponible
    pokemon_t *pokemon1 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre1);
    pokemon_t *pokemon2 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre2);
    pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre3);

    // Verificar que se encontraron los pokémon
    if (!pokemon1 || !pokemon2 || !pokemon3) {
        return false;
    }

    // Verificar que los nombres no se repiten
    if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0) {
        return false;
    }

    // Modificar los pokémon del adversario
    lista_insertar(adversario->pokemones_jugador,pokemon1);
    lista_insertar(adversario->pokemones_jugador, pokemon2);
    lista_insertar(adversario->pokemones_disponibles, pokemon3);


    con_cada_ataque(pokemon1, agregar_ataque_a_abb, adversario->ataques_jugador);
	con_cada_ataque(pokemon2, agregar_ataque_a_abb, adversario->ataques_jugador);
    con_cada_ataque(pokemon3, agregar_ataque_a_abb, adversario->ataques_disponibles);

    return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{	
    jugada_t j = { .ataque = "", .pokemon = "" };

    // Verificar que hay pokemones disponibles para el jugador
    if (lista_vacia(adversario->pokemones_disponibles)) {
        return j;
    }

    // Obtener un pokemon aleatorio de la lista de pokemones disponibles
    int indice_pokemon = rand() % (int)lista_tamanio(adversario->pokemones_disponibles);
    pokemon_t *pokemon = lista_elemento_en_posicion(adversario->pokemones_disponibles, (size_t)indice_pokemon);

    // Verificar que hay ataques disponibles para el jugador
    if (!pokemon || abb_vacio(adversario->ataques_disponibles)) {
        return j;
    }

    // Obtener el primer ataque del abb
    struct ataque *ataque;
    abb_con_cada_elemento(adversario->ataques_disponibles, INORDEN, hay_ataque, &ataque);

    // Verificar si el ataque ya fue utilizado
    if (ataque_ya_utilizado((jugador_t*)adversario, ataque)) {
        return j;
    }

    // Realizar la jugada
    strcpy(j.pokemon, pokemon_nombre(pokemon));
	strcpy(j.ataque, ataque->nombre);

    // Marcar el ataque como utilizado
    eliminar_ataque_utilizado((jugador_t*)adversario, ataque);

    return j;
}


void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	//Dejar vacia o cambiar estrategia para hacerlo mas inteligente. 
}

void adversario_destruir(adversario_t *adversario)
{	
 	if (adversario) {
		lista_destruir(adversario->pokemones_disponibles);
		abb_destruir(adversario->ataques_disponibles);
		lista_destruir(adversario->pokemones_jugador);
		free(adversario);

    }
}