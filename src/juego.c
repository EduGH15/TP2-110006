#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include "pokemon.h"
#include "ataque.h"
#include "abb.h"
#include "funciones_varias.h"

juego_t* juego_crear() {
    juego_t* juego = calloc(1, sizeof(juego_t));
    if (!juego) {
        return NULL;
    }

    juego->jugador_1 = jugador_crear();
    juego->jugador_2 = jugador_crear();

    if (!juego->jugador_1 || !juego->jugador_2) {
        jugador_destruir(juego->jugador_1);
        jugador_destruir(juego->jugador_2);
        free(juego);
        return NULL;
    }

    juego->pokemones_totales = lista_crear();
    if(!juego->pokemones_totales){
        jugador_destruir(juego->jugador_1);
        jugador_destruir(juego->jugador_2);
        free(juego);
        return NULL;
    }
    juego->cantidad_rondas = 9;
    juego->finalizado = false;

    return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo) {
    if (!juego || !archivo) {
        return ERROR_GENERAL;
    }

    informacion_pokemon_t *info_pokemon = pokemon_cargar_archivo(archivo);
    if (!info_pokemon) {
        return ERROR_GENERAL;
    }

    if(pokemon_cantidad(info_pokemon) < 6){
        pokemon_destruir_todo(info_pokemon);
        return POKEMON_INSUFICIENTES;
    }

    juego->info_pokemon = info_pokemon;
    con_cada_pokemon(juego->info_pokemon, agregar_pokemon_a_lista, juego->pokemones_totales);
    return TODO_OK;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador) {
    if (juego == NULL) {
        return 0;
    }

    return (jugador == JUGADOR1) ? juego->jugador_1->puntaje : juego->jugador_2->puntaje;
}

bool juego_finalizado(juego_t *juego) {
    if (!juego) {
        return false;
    }
    return juego->finalizado;
}

lista_t *juego_listar_pokemon(juego_t *juego) {
    if (!juego) {
        return NULL;
    }

    return juego->pokemones_totales;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
                                       const char *nombre1, const char *nombre2,
                                       const char *nombre3) {
    if (!juego || !nombre1 || !nombre2 || !nombre3) {
        return ERROR_GENERAL;
    }

    pokemon_t *pokemon1 = pokemon_buscar(juego->info_pokemon, nombre1);
    pokemon_t *pokemon2 = pokemon_buscar(juego->info_pokemon, nombre2);
    pokemon_t *pokemon3 = pokemon_buscar(juego->info_pokemon, nombre3);

    if (!pokemon1 || !pokemon2 || !pokemon3) {
        return POKEMON_INEXISTENTE;
    }

    if (pokemon1 == pokemon2 || pokemon1 == pokemon3 || pokemon2 == pokemon3) {
        return POKEMON_REPETIDO;
    }

    if(jugador == JUGADOR1){
        agregar_pokemon_a_jugador(juego->jugador_1, pokemon1, pokemon2, NULL);
        agregar_pokemon_a_jugador(juego->jugador_2, NULL, NULL, pokemon3);
    }else{
        agregar_pokemon_a_jugador(juego->jugador_1, NULL, NULL, pokemon3);
        agregar_pokemon_a_jugador(juego->jugador_2, pokemon1, pokemon2, NULL);
    }
    return TODO_OK;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
                                     jugada_t jugada_jugador2) {

    resultado_jugada_t resultado_ataque;
    resultado_ataque.jugador1 = ATAQUE_ERROR;
    resultado_ataque.jugador2 = ATAQUE_ERROR;

    if (!juego) {
        return resultado_ataque;
    }
    
    pokemon_t *pokemon_jugador1 = lista_buscar_elemento(juego->jugador_1->pokemones, comparador_buscar_pokemon, jugada_jugador1.pokemon);
    pokemon_t *pokemon_jugador2 = lista_buscar_elemento(juego->jugador_2->pokemones, comparador_buscar_pokemon, jugada_jugador2.pokemon);

    if (!pokemon_jugador1 || !pokemon_jugador2){
        return resultado_ataque;
    }
    
    const struct ataque *ataque_jugador1 = pokemon_buscar_ataque(pokemon_jugador1, jugada_jugador1.ataque);
    const struct ataque *ataque_jugador2 = pokemon_buscar_ataque(pokemon_jugador2, jugada_jugador2.ataque);

    if (!ataque_jugador1 || !ataque_jugador2) {
        return resultado_ataque;
    }

    if (ataque_ya_utilizado(juego->jugador_1, ataque_jugador1) || ataque_ya_utilizado(juego->jugador_2, ataque_jugador2)) {
        return resultado_ataque;
    }
    
    int puntaje_jugador1 = calcular_efectividad_y_puntaje(ataque_jugador1, pokemon_jugador2, &resultado_ataque, juego, juego->jugador_1);
    int puntaje_jugador2 = calcular_efectividad_y_puntaje(ataque_jugador2, pokemon_jugador1, &resultado_ataque, juego, juego->jugador_2);

    juego->jugador_1->puntaje += puntaje_jugador1;
    juego->jugador_2->puntaje += puntaje_jugador2;

    eliminar_ataque_utilizado(juego->jugador_1, ataque_jugador1); 
    eliminar_ataque_utilizado(juego->jugador_2, ataque_jugador2);

    if (juego->cantidad_rondas == 0) {
         juego->finalizado = true; 
    }

    juego->cantidad_rondas--;

    return resultado_ataque;
}

void juego_destruir(juego_t* juego) {
    if (juego) {
        jugador_destruir(juego->jugador_1);
        jugador_destruir(juego->jugador_2);
        lista_destruir(juego->pokemones_totales);
        pokemon_destruir_todo(juego->info_pokemon);
        free(juego);
    }
}