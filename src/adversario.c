#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include "abb.h"
#include "funciones_varias.h"

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon) {
		return NULL;
	}

	adversario_t *adversario = calloc(1, sizeof(adversario_t));
	if (!adversario) {
		return NULL;
	}

	adversario->pokemon = pokemon;
	adversario->pokemones_disponibles = lista_crear();
	if (!adversario->pokemones_disponibles) {
		free(adversario);
		return NULL;
	}

	adversario->pokemones_jugador = lista_crear();

	if (!adversario->pokemones_jugador) {
		lista_destruir(adversario->pokemones_disponibles);
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

	while (indice_pokemon1 == indice_pokemon2 ||
	       indice_pokemon1 == indice_pokemon3 ||
	       indice_pokemon2 == indice_pokemon3) {
		indice_pokemon1 = rand() % cantidad_pokemon;
		indice_pokemon1 = rand() % cantidad_pokemon;
		indice_pokemon1 = rand() % cantidad_pokemon;
	}

	pokemon_t *pokemon1 = lista_elemento_en_posicion(
		adversario->pokemon, (size_t)indice_pokemon1);
	pokemon_t *pokemon2 = lista_elemento_en_posicion(
		adversario->pokemon, (size_t)indice_pokemon2);
	pokemon_t *pokemon3 = lista_elemento_en_posicion(
		adversario->pokemon, (size_t)indice_pokemon3);

	if (!pokemon1 || !pokemon2 || !pokemon3) {
		return false;
	}
	*nombre1 = (char *)pokemon_nombre(pokemon1);
	*nombre2 = (char *)pokemon_nombre(pokemon2);
	*nombre3 = (char *)pokemon_nombre(pokemon3);

	lista_insertar(adversario->pokemones_disponibles, pokemon1);
	lista_insertar(adversario->pokemones_disponibles, pokemon2);
	lista_insertar(adversario->pokemones_jugador, pokemon3);

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	if (!adversario || !nombre1 || !nombre2 || !nombre3) {
		return false;
	}

	pokemon_t *pokemon1 = lista_buscar_elemento(
		adversario->pokemon, comparador_buscar_pokemon, nombre1);
	pokemon_t *pokemon2 = lista_buscar_elemento(
		adversario->pokemon, comparador_buscar_pokemon, nombre2);
	pokemon_t *pokemon3 = lista_buscar_elemento(
		adversario->pokemon, comparador_buscar_pokemon, nombre3);

	if (!pokemon1 || !pokemon2 || !pokemon3) {
		return false;
	}

	if (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 ||
	    strcmp(nombre2, nombre3) == 0) {
		return false;
	}

	lista_insertar(adversario->pokemones_jugador, pokemon1);
	lista_insertar(adversario->pokemones_jugador, pokemon2);
	lista_insertar(adversario->pokemones_disponibles, pokemon3);

	dato_t datos;
	datos.cantidad_nombres = 0;
	lista_con_cada_elemento(adversario->pokemones_disponibles,
				guardar_nombres, &datos);
	size_t j = 0;
	for (size_t i = 0; i < datos.cantidad_nombres; i++) {
		pokemon_t *pokemon = lista_elemento_en_posicion(
			adversario->pokemones_disponibles, i);
		strcpy(adversario->posibles_jugadas[j].pokemon,
		       datos.nombres[i]);
		j++;
		strcpy(adversario->posibles_jugadas[j].pokemon,
		       datos.nombres[i]);
		j++;
		strcpy(adversario->posibles_jugadas[j].pokemon,
		       datos.nombres[i]);
		j++;
		con_cada_ataque(pokemon, agregar_ataque_a_vector, adversario);
	}
	return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };

	if (lista_vacia(adversario->pokemones_disponibles)) {
		return j;
	}

	int indice_jugada = rand() % (int)adversario->cantidad_jugadas;
	strcpy(j.ataque, adversario->posibles_jugadas[indice_jugada].ataque);
	strcpy(j.pokemon, adversario->posibles_jugadas[indice_jugada].pokemon);
	adversario->posibles_jugadas[indice_jugada] =
		adversario->posibles_jugadas[adversario->cantidad_jugadas - 1];
	(adversario->cantidad_jugadas)--;
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	return;
}

void adversario_destruir(adversario_t *adversario)
{
	if (adversario) {
		lista_destruir(adversario->pokemones_disponibles);
		lista_destruir(adversario->pokemones_jugador);
		free(adversario);
	}
}