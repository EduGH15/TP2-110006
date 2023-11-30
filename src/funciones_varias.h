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

/*
Pre:  Recibe dos punteros void (elemento1 y elemento2), que se asumen apuntan a cadenas de caracteres.
Post: Devuelve un entero que indica la relación lexicográfica entre las dos cadenas:
      - Si cadena1 < cadena2, devuelve un número negativo.
      - Si cadena1 > cadena2, devuelve un número positivo.
      - Si cadena1 es igual a cadena2, devuelve 0.
*/
int comparador_cadenas(void *elemento1, void *elemento2);

/*
Pre:  -------------------------------------------------------------------
Post: Devuelve un entero que representa el redondeo hacia arriba del número dado:
      - Si el número tiene parte decimal, redondea hacia arriba.
      - Si el número es un entero, devuelve el mismo entero.
*/
int redondear_hacia_arriba(float numero);

/*
Pre:  --------------------------------------------------------------------------
Post: Devuelve un puntero a una estructura de tipo jugador_t correctamente inicializada, con
      una lista de pokemones y un árbol de ataques disponibles. El puntaje se inicializa en 0.
      En caso de error, devuelve NULL.
*/
jugador_t* jugador_crear();

/*
Pre:  Recibe un puntero a una estructura de tipo jugador_t que debe estar inicializada.
Post: Libera los recursos asociados al jugador, incluyendo la destrucción de la lista de pokemones,
      el árbol de ataques disponibles y la liberación de la memoria del propio jugador.
      Si el puntero a jugador es NULL, la función no realiza ninguna acción.
*/
void jugador_destruir(jugador_t* jugador);

/*
Pre:  Recibe un puntero a una estructura de tipo pokemon_t (pokemon) y un puntero void (contexto),
      el cual se asume que apunta a una lista_t.
Post: Agrega el pokemon dado a la lista apuntada por el contexto. Si el pokemon o el contexto son NULL,
      la función no realiza ninguna acción.
*/
void agregar_pokemon_a_lista(pokemon_t *pokemon, void *contexto);


/*
Pre:  Recibe un puntero a una estructura de tipo ataque (ataque) y un puntero void (abb),
      el cual se asume que apunta a un árbol binario de búsqueda (abb_t).
Post: Agrega el ataque dado al árbol binario de búsqueda apuntado por abb.
      Si el ataque o el abb son NULL, la función no realiza ninguna acción.
*/
void agregar_ataque_a_abb(const struct ataque *ataque, void *abb);

/*
Pre:  Recibe un puntero a una estructura de tipo jugador_t (jugador) y hasta tres punteros a estructuras de tipo pokemon_t
      (pokemon1, pokemon2, pokemon3). Se asume que el jugador existe.
Post: Agrega los pokemones dados a la lista de pokemones del jugador. Además, agrega los ataques de cada pokemon al árbol
      de ataques disponibles del jugador. Si algún pokemon es NULL, no se realiza ninguna acción para ese pokemon.
*/

void agregar_pokemon_a_jugador(jugador_t *jugador, pokemon_t *pokemon1, pokemon_t *pokemon2, pokemon_t *pokemon3);

/*
Pre:  Recibe un puntero a una estructura de tipo ataque (ataque), un puntero a una estructura de tipo pokemon_t
      (pokemon_atacado), un puntero a una estructura de tipo resultado_jugada_t (resultado), un puntero a una estructura
      de tipo juego_t (juego), y un puntero a una estructura de tipo jugador_t (jugador).
      Se asume que los punteros a ataque, pokemon_atacado, resultado, juego y jugador no son NULL.
Post: Calcula la efectividad del ataque sobre el pokemon atacado utilizando una matriz, dodne las filas representan
      el tipo de ataque y las columnas representan el tipo del pokemon atacado,actualiza el resultado de la jugada y 
      devuelve el puntaje obtenido, redondeado hacia arriba, teniendo en cuenta el poder del ataque y la efectividad contra el tipo del pokemon.
*/
int calcular_efectividad_y_puntaje(const struct ataque* ataque, pokemon_t* pokemon_atacado, resultado_jugada_t* resultado, juego_t* juego, jugador_t* jugador);

/*
Pre:  Recibe dos punteros void (elemento y contexto). Se asume que elemento apunta a una estructura de tipo pokemon_t
      y contexto apunta a una cadena de caracteres (nombre del pokemon).
Post: Devuelve un entero que indica la relación lexicográfica entre el nombre del pokemon (elemento) y el nombre dado
      como contexto. Devuelve 0 si los nombres son iguales, un número negativo si el nombre del pokemon es menor al
      nombre de contexto, y un número positivo si es mayor. Si elemento o contexto son NULL, devuelve 0.
*/
int comparador_buscar_pokemon(void *elemento, void *contexto);

/*
Pre:  Recibe un puntero a una estructura de tipo jugador_t (jugador) y un puntero a una estructura de tipo ataque (ataque).
      Se asume que ambos punteros no son NULL.
Post: Devuelve true si el ataque ya ha sido utilizado por el jugador (se encuentra en el árbol de ataques disponibles),
      y false en caso contrario. Si jugador o ataque son NULL, devuelve false.
*/
bool ataque_ya_utilizado(jugador_t* jugador, const struct ataque* ataque);

/*
Pre:  Recibe un puntero a una estructura de tipo jugador_t (jugador) y un puntero a una estructura de tipo ataque (ataque).
      Se asume que ambos punteros no son NULL.
Post: Elimina el ataque utilizado del árbol de ataques disponibles del jugador.
      Si jugador o ataque son NULL, la función no realiza ninguna acción.
*/
void eliminar_ataque_utilizado(jugador_t *jugador, const struct ataque *ataque);

/*
Pre:  Recibe dos punteros void (elemento y aux). Se asume que elemento apunta a una estructura de tipo ataque_t y
      aux apunta a una estructura de tipo ataque_t o es NULL.
Post: Si aux es NULL, copia la información del ataque (elemento) en aux y devuelve true. Si aux no es NULL, devuelve false.
*/
bool hay_ataque(void *elemento, void *aux);


#endif // FUNCIONES_VARIAS_H_
