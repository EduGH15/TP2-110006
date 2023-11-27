#include <stdbool.h>
#include <stdlib.h>
#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include "pokemon.h"
#include "ataque.h"
#include <string.h>
#include <stdio.h>

// Estructura para almacenar el contexto de búsqueda de un ataque
struct contexto_buscar_ataque {
    const struct ataque *ataque_a_eliminar;
    size_t posicion_encontrada;
    bool encontrado;
};


typedef struct {
    lista_t* pokemones;
    lista_t* ataques_disponibles;
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

// Función para crear un jugador con sus listas
jugador_t* jugador_crear() {
    jugador_t* jugador = calloc(1, sizeof(jugador_t));
    if (!jugador) {
        return NULL;
    }

    jugador->pokemones = lista_crear();
    jugador->ataques_disponibles = lista_crear();

    if (!jugador->pokemones || !jugador->ataques_disponibles) {
        lista_destruir(jugador->pokemones);
        lista_destruir(jugador->ataques_disponibles);
        free(jugador);
        return NULL;
    }

    jugador->puntaje = 0;

    return jugador;
}

// Función para destruir un jugador y sus listas
void jugador_destruir(jugador_t* jugador) {
    if (jugador) {
        lista_destruir(jugador->pokemones);
        lista_destruir(jugador->ataques_disponibles);
        free(jugador);
    }
}

// Función para crear un juego con jugadores
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

// Función auxiliar para agregar pokemon a la lista
void agregar_pokemon_a_lista(pokemon_t *pokemon, void *contexto) {
    if(!pokemon || !contexto) {
        return;
    }
    lista_t *lista_pokemon = (lista_t *)contexto;
    lista_insertar(lista_pokemon, pokemon);
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
    if (!juego || !juego->info_pokemon) {
        return NULL;
    }

    return juego->pokemones_totales;
}

void agregar_ataque_a_lista(const struct ataque *ataque, void *lista)
{   
    if(!lista || !ataque) {
        return;
    }
    lista_insertar(lista, (void*) ataque);
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

    con_cada_ataque(pokemon1, agregar_ataque_a_lista, jugador->ataques_disponibles);

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

// Función para calcular la efectividad de un ataque y el puntaje simultáneamente
int calcular_efectividad_y_puntaje(const struct ataque* ataque, pokemon_t* pokemon_atacado, resultado_jugada_t* resultado, juego_t* juego, jugador_t* jugador) {
    if(!ataque || !pokemon_atacado  || !resultado || !juego || !jugador){
        return 0;
    }
    // Definir la matriz de efectividades localmente
    const float efectividades[6][6] = {
    // NORMAL, FUEGO, AGUA, PLANTA, ELECTRICO, ROCA
        {1, 1, 1, 1, 1, 1},                 // NORMAL
        {1, 1, 0.5, 3, 1, 1},               // FUEGO
        {1, 3, 1, 1, 0.5, 1},               // AGUA
        {1, 0.5, 1, 1, 1, 3},               // PLANTA
        {1, 1, 3, 1, 1, 0.5},               // ELECTRICO
        {1, 1, 1, 0.5, 3, 1}                // ROCA
};


    enum TIPO tipo_ataque = ataque->tipo;
    enum TIPO tipo_pokemon_atacado = pokemon_tipo(pokemon_atacado);

    int poder = (int)ataque->poder;
    float efectividad = efectividades[tipo_ataque][tipo_pokemon_atacado];    

    if (efectividad == 1) {
        if(jugador == juego->jugador_1){
            resultado->jugador1 = ATAQUE_REGULAR;
        }else{
            resultado->jugador2 = ATAQUE_REGULAR;
        }
    } else if (efectividad > 1) {
        if(jugador == juego->jugador_1){
            resultado->jugador1 = ATAQUE_EFECTIVO;
        }else{
            resultado->jugador2 = ATAQUE_EFECTIVO;
        }
    } else if (efectividad < 1) {
        if(jugador == juego->jugador_1){
            resultado->jugador1 = ATAQUE_INEFECTIVO;
        }else{
            resultado->jugador2 = ATAQUE_INEFECTIVO;
        }
    } else {
        if(jugador == juego->jugador_1){
            resultado->jugador1 = ATAQUE_ERROR;
        }else{
            resultado->jugador2 = ATAQUE_ERROR;
        }
        return 0;  // Retorna 0 en caso de error
    }

    return poder * (int)(efectividad);
}

// Función comparadora para buscar un pokemon por nombre en la lista
int comparador_buscar_pokemon(void *elemento, void *contexto) {
    if(!elemento || !contexto){
        return 0;
    }
    const char *nombre_pokemon = (const char *)contexto;
    pokemon_t *pokemon = (pokemon_t *)elemento;

    return strcmp(pokemon_nombre(pokemon), nombre_pokemon);
}


bool comparador_buscar_ataque(void *elemento, void *contexto) {
    if (!elemento || !contexto) {
        return false;
    }

    const struct ataque *ataque = (const struct ataque *)elemento;
    const char *nombre_a_eliminar = (const char *)contexto;

    // Comparar el nombre del ataque
    return strcmp(ataque->nombre, nombre_a_eliminar) == 0;
}



int buscar_ataque(void* p1, void *p2){
    if(!p1 || !p2){
        return 0;
    }
	return p1==p2;
}

bool ataque_ya_utilizado(jugador_t* jugador, const struct ataque* ataque) {

    if(!jugador || !ataque){
        return false;
    } 
	return lista_buscar_elemento(jugador->ataques_disponibles, buscar_ataque, (void*) ataque) == NULL;
}

// Actualiza la lista de ataques del pokemon después de utilizar un ataque
void eliminar_ataque_utilizado(jugador_t *jugador, const struct ataque *ataque) {
    if (!jugador || !ataque) {
        return;
    }

    const char *nombre_ataque_a_eliminar = ataque->nombre;

    lista_iterador_t *iterador = lista_iterador_crear(jugador->ataques_disponibles);
    if (iterador == NULL) {
        return;
    }

    size_t posicion = 0;

    while (lista_iterador_tiene_siguiente(iterador)) {
        void *elemento_actual = lista_iterador_elemento_actual(iterador);
        if (comparador_buscar_ataque(elemento_actual, (void *)nombre_ataque_a_eliminar)) {
            lista_quitar_de_posicion(jugador->ataques_disponibles, posicion);
            break;
        }
        lista_iterador_avanzar(iterador);
        posicion++;
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
    pokemon_t *pokemon_jugador2 = lista_buscar_elemento(juego->jugador_2->pokemones, comparador_buscar_pokemon, jugada_jugador2.pokemon);

    if (!pokemon_jugador1 || !pokemon_jugador2) {
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

// Función para destruir un juego y sus jugadores
void juego_destruir(juego_t* juego) {
    if (juego) {
        jugador_destruir(juego->jugador_1);
        jugador_destruir(juego->jugador_2);
        lista_destruir(juego->pokemones_totales);
        pokemon_destruir_todo(juego->info_pokemon);
        free(juego);
    }
}