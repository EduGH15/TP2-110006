#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"
#include "funciones_varias.h"

typedef struct datos{
    char* nombres[3];
    size_t cantidad_nombres;
}dato_t;

struct adversario {
    lista_t* pokemon;
	lista_t* pokemones_disponibles;
    //abb_t* ataques_disponibles;
	lista_t* pokemones_jugador;
	abb_t* ataques_jugador;
    jugada_t posibles_jugadas[9];
    size_t cantidad_jugadas;
};


bool guardar_nombres(void* pokemon, void* datos){
    pokemon_t* pokemon_nuevo = pokemon;
    dato_t* datos_nuevos = datos;
    datos_nuevos->nombres[datos_nuevos->cantidad_nombres] = (char*)pokemon_nombre(pokemon_nuevo);
    printf("Los pokemones del adversario:\n");
    printf("%s\n", datos_nuevos->nombres[datos_nuevos->cantidad_nombres]);
    datos_nuevos->cantidad_nombres++;
    return true;
}

void agregar_ataque_a_vector(const struct ataque* ataque, void* adversario){
    adversario_t* adversario_nuevo = adversario;
    strcpy(adversario_nuevo->posibles_jugadas[adversario_nuevo->cantidad_jugadas].ataque,ataque->nombre);
    // printf("El ataque del pokemon adversario es:\n");
    // printf("%s\n", adversario_nuevo->posibles_jugadas[adversario_nuevo->cantidad_jugadas].ataque);
    adversario_nuevo->cantidad_jugadas++;
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

    // adversario->ataques_disponibles = abb_crear(comparador_cadenas);
	// if(!adversario->ataques_disponibles){
	// 	lista_destruir(adversario->pokemones_disponibles);
	// 	free(adversario);
	// 	return NULL;
	// }
	adversario->pokemones_jugador = lista_crear();

    if(!adversario->pokemones_jugador) {
		lista_destruir(adversario->pokemones_disponibles);
		//abb_destruir(adversario->ataques_disponibles);
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
    int indice_pokemon2 = rand() % cantidad_pokemon;
	int indice_pokemon3 = rand() % cantidad_pokemon;
    
    while(indice_pokemon1 == indice_pokemon2 || indice_pokemon1 == indice_pokemon3 ||  indice_pokemon2 == indice_pokemon3){
        indice_pokemon1 = rand() % cantidad_pokemon;
        indice_pokemon1 = rand() % cantidad_pokemon;
        indice_pokemon1 = rand() % cantidad_pokemon;
    }
    // printf("indice1: %i\n", indice_pokemon1);
    // printf("indice2: %i\n", indice_pokemon2);
    // printf("indice2: %i\n", indice_pokemon3);

	pokemon_t* pokemon1 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon1);
	pokemon_t* pokemon2 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon2);
	pokemon_t* pokemon3 = lista_elemento_en_posicion(adversario->pokemon, (size_t)indice_pokemon3);

	if(!pokemon1 || !pokemon2 || !pokemon3){
		return false;
	}
    *nombre1 = (char*)pokemon_nombre(pokemon1);
    *nombre2 = (char*)pokemon_nombre(pokemon2);
    *nombre3 = (char*)pokemon_nombre(pokemon3);

	lista_insertar(adversario->pokemones_disponibles, pokemon1);
	lista_insertar(adversario->pokemones_disponibles, pokemon2);
	lista_insertar(adversario->pokemones_jugador, pokemon3);
	
    // con_cada_ataque(pokemon1, agregar_ataque_a_abb, adversario->ataques_disponibles);
	// con_cada_ataque(pokemon2, agregar_ataque_a_abb, adversario->ataques_disponibles);
    // con_cada_ataque(pokemon3, agregar_ataque_a_abb, adversario->ataques_jugador);

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
        printf("ALGUN PUNTERO ES NULO EN POKEMON");
        return false;
    }

    if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0) {
        printf("EL ADVERSARIO ESTA REPITIENDO POKEMONES");
        return false;
    }

    lista_insertar(adversario->pokemones_jugador,pokemon1);
    lista_insertar(adversario->pokemones_jugador, pokemon2);
    lista_insertar(adversario->pokemones_disponibles, pokemon3);

    dato_t datos;
    datos.cantidad_nombres = 0;
    lista_con_cada_elemento(adversario->pokemones_disponibles, guardar_nombres, &datos);
    size_t j = 0;
    for(size_t i = 0; i < datos.cantidad_nombres; i++){
        pokemon_t* pokemon = lista_elemento_en_posicion(adversario->pokemones_disponibles,i);
        strcpy(adversario->posibles_jugadas[j].pokemon,datos.nombres[i]);
        j++;
        strcpy(adversario->posibles_jugadas[j].pokemon,datos.nombres[i]);
        j++;
        strcpy(adversario->posibles_jugadas[j].pokemon,datos.nombres[i]);
        j++;
        con_cada_ataque(pokemon, agregar_ataque_a_vector, adversario);
    }
    return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{	
    jugada_t j = { .ataque = "", .pokemon = "" };

    if (lista_vacia(adversario->pokemones_disponibles)) {
        printf("EL ADVERSARIO NO TIENE POKEMONES DISPONIBLES\n");
        return j;
    }

    int indice_jugada = rand() % (int)adversario->cantidad_jugadas;
    strcpy(j.ataque,adversario->posibles_jugadas[indice_jugada].ataque);
    strcpy(j.pokemon,adversario->posibles_jugadas[indice_jugada].pokemon);
    printf("El nombre del pokemon del adversario que va a atacar es: %s\n", j.pokemon);
    printf("El pokemon adversario usó: %s\n", j.ataque);

    adversario->posibles_jugadas[indice_jugada] = adversario->posibles_jugadas[adversario->cantidad_jugadas - 1];
    (adversario->cantidad_jugadas)--;

    
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
		//abb_destruir(adversario->ataques_disponibles);
		lista_destruir(adversario->pokemones_jugador);
		free(adversario);

    }
}