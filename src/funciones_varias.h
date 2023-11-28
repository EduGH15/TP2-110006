#ifndef FUNCIONES_VARIAS_H_
#define FUNCIONES_VARIAS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "juego.h"
#include "pokemon.h"
#include "ataque.h"
#include "abb.h"

typedef struct {
    lista_t* pokemones;
    abb_t* ataques_disponibles;
    int puntaje;
} jugador_t;

struct juego {
    jugador_t* jugador_1;   
    jugador_t* jugador_2; 
    informacion_pokemon_t* info_pokemon;
    lista_t* pokemones_totales;
    int cantidad_rondas;
    bool finalizado;
};

// Definición de la función de comparación para cadenas
int comparador_cadenas(void *elemento1, void *elemento2);

// Función para redondear hacia arriba
int redondear_hacia_arriba(float numero);

// Función para crear un jugador con sus listas
jugador_t* jugador_crear();

// Función para destruir un jugador y sus listas
void jugador_destruir(jugador_t* jugador);

// Función auxiliar para agregar pokemon a la lista
void agregar_pokemon_a_lista(pokemon_t *pokemon, void *contexto);

void agregar_ataque_a_abb(const struct ataque *ataque, void *abb);

//Funcion auxiliar para agregar pokemones a la lista de pokemones del jugador
void agregar_pokemon_a_jugador(jugador_t *jugador, pokemon_t *pokemon1, pokemon_t *pokemon2, pokemon_t *pokemon3);

// Función para calcular la efectividad de un ataque y el puntaje simultáneamente
int calcular_efectividad_y_puntaje(const struct ataque* ataque, pokemon_t* pokemon_atacado, resultado_jugada_t* resultado, juego_t* juego, jugador_t* jugador);

// Función comparadora para buscar un pokemon por nombre en la lista
int comparador_buscar_pokemon(void *elemento, void *contexto);

bool ataque_ya_utilizado(jugador_t* jugador, const struct ataque* ataque);

// Función para eliminar un ataque utilizado del ABB de ataques
void eliminar_ataque_utilizado(jugador_t *jugador, const struct ataque *ataque);

bool hay_ataque(void *elemento, void *aux);


#endif // FUNCIONES_VARIAS_H_
