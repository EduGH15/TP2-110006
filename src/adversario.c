#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"
#include "funciones_varias.h"

struct ContextoIterador {
    size_t contador;
    struct ataque* ataque_aleatorio;
};

struct adversario {
    lista_t* pokemon;
	lista_t* pokemones_disponibles;
    abb_t* ataques_disponibles;
	lista_t* pokemones_jugador;
	abb_t* ataques_jugador;

};

/*
Pre:  Recibe un puntero a un elemento (elemento) y un puntero a un contexto (contexto).
      El contexto debe ser una estructura de tipo struct ContextoIterador con al menos
      un campo llamado contador de tipo size_t y un campo llamado ataque_aleatorio de tipo
      struct ataque*.
Post: Disminuye el valor del campo contador en el contexto en 1. Si el valor del campo contador
      en el contexto llega a 0, asigna el puntero al elemento (casteado a struct ataque*) al
      campo ataque_aleatorio en el contexto y devuelve false. En caso contrario, devuelve true.
*/
bool obtener_ataque_aleatorio(void* elemento, void* contexto) {
    struct ContextoIterador* ctx = (struct ContextoIterador*)contexto;

    ctx->contador--;
    if (ctx->contador == 0) {
        ctx->ataque_aleatorio = (struct ataque*)elemento;
        return false;
    }
    return true;
}

/*
Pre:  Recibe un puntero a una estructura de tipo abb_t (abb). Se asume que el abb ha sido 
      creado y contiene elementos válidos.
Post: Devuelve un puntero a una estructura de tipo struct ataque que representa un ataque
      aleatorio dentro del abb proporcionado. Si el abb es nulo o está vacío, devuelve NULL.
      El ataque aleatorio se selecciona utilizando un contador aleatorio y un iterador interno.
*/
struct ataque* obtener_ataque_aleatorio_en_abb(abb_t* abb) {
    if (!abb || abb_vacio(abb)) {
        return NULL;
    }

    size_t cantidad_elementos = abb_tamanio(abb);

    size_t contador_aleatorio = (size_t)(rand() % (int)cantidad_elementos);

    struct ContextoIterador ctx = { .contador = contador_aleatorio, .ataque_aleatorio = NULL };

    abb_con_cada_elemento(abb, INORDEN, obtener_ataque_aleatorio, &ctx);

    return ctx.ataque_aleatorio;
}

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

    pokemon_t *pokemon1 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre1);
    pokemon_t *pokemon2 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre2);
    pokemon_t *pokemon3 = lista_buscar_elemento(adversario->pokemon, comparador_buscar_pokemon, nombre3);

    if (!pokemon1 || !pokemon2 || !pokemon3) {
        return false;
    }

    if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0) {
        return false;
    }

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

    if (lista_vacia(adversario->pokemones_disponibles)) {
        return j;
    }

    int indice_pokemon = rand() % (int)lista_tamanio(adversario->pokemones_disponibles);
    pokemon_t *pokemon = lista_elemento_en_posicion(adversario->pokemones_disponibles, (size_t)indice_pokemon);

    if (!pokemon || abb_vacio(adversario->ataques_disponibles)) {
        return j;
    }

    struct ataque *ataque = obtener_ataque_aleatorio_en_abb(adversario->ataques_disponibles);

    if (ataque_ya_utilizado((jugador_t*)adversario, ataque)) {
        return j;
    }

    strcpy(j.pokemon, pokemon_nombre(pokemon));
	strcpy(j.ataque, ataque->nombre);

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