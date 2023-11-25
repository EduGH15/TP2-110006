#include <stdbool.h>
#include <stdlib.h>
#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include "pokemon.h"
#include "ataque.h"

typedef struct {
    lista_t* pokemones;
    lista_t* ataques_disponibles;
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
        free(juego->jugador_1);
        free(juego);
        return NULL;
    }

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

/*------------------------------------------------------------------------------------------------*/

// Pre: ----------------------------------------------------------------
//Post: Devuelve el puntaje de haber realizado un ataque a cierto tipo de pokemon.
int calcular_puntaje(ataque_t* ataque, pokemon_t* pokemon_atacado) {
    enum TIPO tipo_ataque = ataque->tipo;
    enum TIPO tipo_pokemon_atacado = pokemon_tipo(pokemon_atacado);

    if (tipo_ataque == tipo_pokemon_atacado) {
        return ataque->poder;
    }

    switch (tipo_ataque) {
        case NORMAL:
            return ataque->poder;
        case FUEGO:
            return (tipo_pokemon_atacado == PLANTA) ? 3 * ataque->poder : (tipo_pokemon_atacado == AGUA) ? ataque->poder / 2 : ataque->poder;
        case PLANTA:
            return (tipo_pokemon_atacado == ROCA) ? 3 * ataque->poder : (tipo_pokemon_atacado == FUEGO) ? ataque->poder / 2 : ataque->poder;
        case ROCA:
            return (tipo_pokemon_atacado == ELECTRICO) ? 3 * ataque->poder : (tipo_pokemon_atacado == PLANTA) ? ataque->poder / 2 : ataque->poder;
        case ELECTRICO:
            return (tipo_pokemon_atacado == AGUA) ? 3 * ataque->poder : (tipo_pokemon_atacado == ROCA) ? ataque->poder / 2 : ataque->poder;
        case AGUA:
            return (tipo_pokemon_atacado == FUEGO) ? 3 * ataque->poder : (tipo_pokemon_atacado == ELECTRICO) ? ataque->poder / 2 : ataque->poder;
    }

    return ataque->poder;
}

void calcular_efectividad(ataque_t *ataque, pokemon_t *pokemon_atacado, resultado_jugada_t *resultado) {
    if(!ataque  || !pokemon_atacado || !resultado){
        return;
    } 
    enum TIPO tipo_ataque = ataque->tipo;
    enum TIPO tipo_pokemon = pokemon_tipo(pokemon_atacado);

    if (tipo_ataque == tipo_pokemon) {
        resultado->jugador1 = ATAQUE_REGULAR;
        resultado->jugador2 = ATAQUE_REGULAR;
    } else if (
        (tipo_ataque == FUEGO && tipo_pokemon == PLANTA) ||
        (tipo_ataque == PLANTA && tipo_pokemon == ROCA) ||
        (tipo_ataque == ROCA && tipo_pokemon == ELECTRICO) ||
        (tipo_ataque == ELECTRICO && tipo_pokemon == AGUA) ||
        (tipo_ataque == AGUA && tipo_pokemon == FUEGO)
    ) {
        resultado->jugador1 = ATAQUE_EFECTIVO;
        resultado->jugador2 = ATAQUE_INEFECTIVO;
    } else if (
        (tipo_pokemon == FUEGO && tipo_ataque == PLANTA) ||
        (tipo_pokemon == PLANTA && tipo_ataque == FUEGO) ||
        (tipo_pokemon == ROCA && tipo_ataque == PLANTA) ||
        (tipo_pokemon == ELECTRICO && tipo_ataque == ROCA) ||
        (tipo_pokemon == AGUA && tipo_ataque == ELECTRICO)
    ) {
        resultado->jugador1 = ATAQUE_INEFECTIVO;
        resultado->jugador2 = ATAQUE_EFECTIVO;
    } else {
        resultado->jugador1 = ATAQUE_REGULAR;
        resultado->jugador2 = ATAQUE_REGULAR;
    }
}

// Función comparadora para buscar un pokemon por nombre en la lista
int comparador_buscar_pokemon(void *elemento, void *contexto) {
    const char *nombre_pokemon = (const char *)contexto;
    pokemon_t *pokemon = (pokemon_t *)elemento;

    return strcmp(pokemon_nombre(pokemon), nombre_pokemon);
}

bool ataque_ya_utilizado(pokemon_t* pokemon, ataque_t* ataque) {
    lista_iterador_t *iterador = lista_iterador_crear(NULL);
    while (lista_iterador_tiene_siguiente(iterador)) {
        const struct ataque *ataque_actual = lista_iterador_elemento_actual(iterador);
        if (ataque_actual == ataque) {
            lista_iterador_destruir(iterador);
            return true; 
        }
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
    return false;
}

// Actualiza la lista de ataques del pokemon después de utilizar un ataque
void marcar_ataque_utilizado(pokemon_t *pokemon, const struct ataque *ataque) {
    lista_iterador_t *iterador = lista_iterador_crear(NULL);
    while (lista_iterador_tiene_siguiente(iterador)) {
        const struct ataque *ataque_actual = lista_iterador_elemento_actual(iterador);
        if (ataque_actual == ataque) {
            lista_iterador_quitar(iterador);
            break;
        }
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
}

// función principal

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
                                     jugada_t jugada_jugador2) {

    resultado_jugada_t resultado_ataque;
    resultado_ataque.jugador1 = ATAQUE_ERROR;
    resultado_ataque.jugador2 = ATAQUE_ERROR;

    if (!juego) {
        return resultado_ataque;
    }

    pokemon_t *pokemon_jugador1 = lista_buscar_elemento(juego->jugador_1->pokemones, comparador_buscar_pokemon, jugada_jugador1.pokemon);
    pokemon_t *pokemon_jugador2 = lista_buscar_elemento(juego->jugador_2->pokemones, comparador_buscar_pokemon, jugada_jugador1.pokemon);

    if (!pokemon_jugador1 || !pokemon_jugador2) {
        return resultado_ataque;
    }

    const struct ataque *ataque_jugador1 = pokemon_buscar_ataque(pokemon_jugador1, jugada_jugador1.ataque);
    const struct ataque *ataque_jugador2 = pokemon_buscar_ataque(pokemon_jugador2, jugada_jugador2.ataque);

    if (!ataque_jugador1 || !ataque_jugador2) {
        return resultado_ataque;
    }

    if (ataque_ya_utilizado(pokemon_jugador1, ataque_jugador1) || ataque_ya_utilizado(pokemon_jugador2, ataque_jugador2)) {
        return resultado_ataque;
    }

    int puntaje_jugador1 = calcular_puntaje(ataque_jugador1, pokemon_jugador2);
    int puntaje_jugador2 = calcular_puntaje(ataque_jugador2, pokemon_jugador1);

    juego->jugador_1->puntaje += puntaje_jugador1;
    juego->jugador_2->puntaje += puntaje_jugador2;

    marcar_ataque_utilizado(pokemon_jugador1, ataque_jugador1);
    marcar_ataque_utilizado(pokemon_jugador2, ataque_jugador2);

    if (lista_tamanio(pokemon_jugador1->ataques) == 0 || lista_tamanio(pokemon_jugador2->ataques) == 0) {
        juego->finalizado = true;
    }

    resultado->jugador1 = juego->jugador_1->puntaje;
    resultado->jugador1 = juego->jugador_3->puntaje;
    return resultado;
}

/*------------------------------------------------------------------------------------------------*/

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador) {
    if (!juego) {
        return 0;  
    }

    jugador_t *jugador_actual = (jugador == JUGADOR1) ? juego->jugador_1 : juego->jugador_2;

    if (!jugador_actual) {
        return 0;  
    }

    return jugador_actual->puntaje;
}


bool juego_finalizado(juego_t *juego){
    if(!juego){
        return false;
    }
    return juego->finalizado;
}


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