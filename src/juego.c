#include <stdbool.h>
#include <stdlib.h>
#include "juego.h"
#include "TDA/lista.h"
#include "TP1/tipo.h"
#include "TP1/pokemon.h"
#include "TP1/ataque.h"

typedef struct {
    lista_t* pokemones; 
    int puntaje;
} jugador_t;

struct juego {
    jugador_t* jugador_1;   
    jugador_t* jugador_2; 
    informacion_pokemon_t* info_pokemon;
    bool finalizado;
};

juego_t *juego_crear()
{	
	juego_t *juego = calloc(1, sizeof(juego_t));
    if (!juego) {
        return NULL;
    }

    juego->jugador_1 = calloc(1, sizeof(jugador_t));
    if (!juego->jugador_1) {
        free(juego);
        return NULL;
    }

    juego->jugador_2 = calloc(1, sizeof(jugador_t));
    if (!juego->jugador_2) {
        free(juego);
        return NULL;
    }

    juego->info_pokemon == NULL;
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
        return POKEMON_INSUFICIENTES;
    }

    juego->info_pokemon = info_pokemon;
    return TODO_OK;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador) {
    if (juego == NULL) {
        return 0;
    }

    return (jugador == JUGADOR1) ? juego->jugador_1->puntaje : juego->jugador_2->puntaje;
}

bool juego_finalizado(juego_t *juego) {
    if (juego == NULL) {
        return false;
    }
    return juego->finalizado;
}

// Función auxiliar para agregar pokemon a la lista
void agregar_pokemon_a_lista(pokemon_t *pokemon, void *contexto) {
    if(!pokemon || !contexto) {
        return;
    }
    lista_t *lista_pokemon = (lista_t *)contexto;
    lista_insertar(lista_pokemon, pokemon);
}

lista_t *juego_listar_pokemon(juego_t *juego) {
    if (!juego || !juego->info_pokemon) {
        return NULL;
    }

    lista_t *lista_pokemon = lista_crear();
    if (!lista_pokemon) {
        return NULL;
    }

    con_cada_pokemon(juego->info_pokemon, agregar_pokemon_a_lista, lista_pokemon);

    return lista_pokemon;
}

//Funcion auxiliar para agregar pokemones a la lista de pokemones del jugador
void agregar_pokemon_a_jugador(jugador_t *jugador, pokemon_t *pokemon1, pokemon_t *pokemon2, pokemon_t *pokemon3) 
{
    if(!jugador || !pokemon1 || !pokemon2 || !pokemon3){
        return;
    }

    lista_insertar(jugador->pokemones, pokemon1);
    lista_insertar(jugador->pokemones, pokemon2);
    lista_insertar(jugador->pokemones, pokemon3);
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
        agregar_pokemon_a_jugador(juego->jugador_1, pokemon1, pokemon2, pokemon3);
    }else{
        agregar_pokemon_a_jugador(juego->jugador_2, pokemon1, pokemon2, pokemon3);

    }
    return TODO_OK;
}


/*
void buscar_pokemon_por_nombre(pokemon_t *pokemon, void *nombre_pokemon, void *pokemon_seleccionado) {
    const char *nombre = *(const char **)nombre_pokemon;

    // Comparar nombres de pokemon
    if (strcmp(pokemon_nombre(pokemon), nombre) == 0) {
        // Pokémon encontrado
        *(pokemon_t **)pokemon_seleccionado = pokemon;
    }
}

void buscar_ataque_por_nombre(const struct ataque *ataque, void *nombre_ataque, void *ataque_seleccionado) {
    const char *nombre = *(const char **)nombre_ataque;

    // Comparar nombres de ataques
    if (strcmp(ataque->nombre, nombre) == 0) {
        // Ataque encontrado
        *(const struct ataque **)ataque_seleccionado = ataque;
    }
}

pokemon_t *buscar_pokemon_seleccionado(lista_t *selecciones, const char *nombre_pokemon) {
    // Buscar el pokémon en la lista de selecciones
    pokemon_t *pokemon_seleccionado = NULL;
    con_cada_pokemon(selecciones, buscar_pokemon_por_nombre, nombre_pokemon, pokemon_seleccionado);
    return pokemon_seleccionado;
}

const struct ataque *buscar_ataque_seleccionado(pokemon_t *pokemon, const char *nombre_ataque) {
    // Buscar el ataque en el pokémon
    const struct ataque *ataque_seleccionado = NULL;
    con_cada_ataque(pokemon, buscar_ataque_por_nombre, &nombre_ataque, ataque_seleccionado);
    return ataque_seleccionado;
}

bool ataque_ya_utilizado(lista_t *ataques_utilizados, const struct ataque *ataque) {
    // Verificar si el ataque ya fue utilizado
    return con_cada_ataque(ataques_utilizados, comparar_ataque, &ataque, NULL) > 0;
}

RESULTADO_ATAQUE calcular_puntaje(juego_t *juego, pokemon_t *atacante, const struct ataque *ataque, pokemon_t *defensor) {
    // Calcular el puntaje según las reglas del juego
    // ...

    return ATAQUE_REGULAR;  // Cambiar según la lógica de puntaje
}


int comparar_ataque(const struct ataque *ataque1, void *ataque2) {
    const struct ataque *ataque = *(const struct ataque **)ataque2;

    // Comparar nombres de ataques
    return strcmp(ataque1->nombre, ataque->nombre);
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	 resultado_jugada_t resultado;
    resultado.jugador1 = ATAQUE_ERROR;
    resultado.jugador2 = ATAQUE_ERROR;

    // Buscar los pokémon seleccionados por ambos jugadores
    pokemon_t *pokemon_jugador1 = buscar_pokemon_seleccionado(juego->selecciones_jugador1, jugada_jugador1.pokemon);
    pokemon_t *pokemon_jugador2 = buscar_pokemon_seleccionado(juego->selecciones_jugador2, jugada_jugador2.pokemon);

    // Verificar si los pokémon existen en las selecciones
    if (!pokemon_jugador1 || !pokemon_jugador2) {
        return resultado;
    }

    // Buscar los ataques seleccionados por ambos jugadores
    const struct ataque *ataque_jugador1 = buscar_ataque_seleccionado(pokemon_jugador1, jugada_jugador1.ataque);
    const struct ataque *ataque_jugador2 = buscar_ataque_seleccionado(pokemon_jugador2, jugada_jugador2.ataque);

    // Verificar si los ataques existen en los pokémon seleccionados
    if (!ataque_jugador1 || !ataque_jugador2) {
        return resultado;
    }

    // Verificar si los ataques ya fueron utilizados en turnos anteriores
    if (ataque_ya_utilizado(juego->ataques_utilizados, ataque_jugador1) || ataque_ya_utilizado(juego->ataques_utilizados, ataque_jugador2)) {
        return resultado;
    }

    // Marcar los ataques como utilizados
    lista_insertar_ultimo(juego->ataques_utilizados, ataque_jugador1);
    lista_insertar_ultimo(juego->ataques_utilizados, ataque_jugador2);

    // Calcular el puntaje para cada jugador
    resultado.jugador1 = calcular_puntaje(juego, pokemon_jugador1, ataque_jugador1, pokemon_jugador2);
    resultado.jugador2 = calcular_puntaje(juego, pokemon_jugador2, ataque_jugador2, pokemon_jugador1);

    return resultado;
}





*/

void juego_destruir(juego_t *juego) {
    if (juego == NULL) {
        return;
    }

    if (juego->jugador_1 != NULL) {
        if (juego->jugador_1->pokemones != NULL) {
            lista_destruir_todo(juego->jugador_1->pokemones, free);
        }
        free(juego->jugador_1);
    }

    if (juego->jugador_2 != NULL) {
        if (juego->jugador_2->pokemones != NULL) {
            lista_destruir_todo(juego->jugador_2->pokemones, free);
        }
        free(juego->jugador_2);
    }

    free(juego);
}