#include "funciones_varias.h"

bool guardar_nombres(void *pokemon, void *datos)
{
	pokemon_t *pokemon_nuevo = pokemon;
	dato_t *datos_nuevos = datos;
	datos_nuevos->nombres[datos_nuevos->cantidad_nombres] =
		(char *)pokemon_nombre(pokemon_nuevo);
	datos_nuevos->cantidad_nombres++;
	return true;
}

void agregar_ataque_a_vector(const struct ataque *ataque, void *adversario)
{
	adversario_t *adversario_nuevo = adversario;
	strcpy(adversario_nuevo
		       ->posibles_jugadas[adversario_nuevo->cantidad_jugadas]
		       .ataque,
	       ataque->nombre);
	adversario_nuevo->cantidad_jugadas++;
}

int comparador_cadenas(void *elemento1, void *elemento2)
{
	const char *cadena1 = (const char *)elemento1;
	const char *cadena2 = (const char *)elemento2;
	return strcmp(cadena1, cadena2);
}

int redondear_hacia_arriba(float numero)
{
	int entero = (int)numero;
	if (numero > entero) {
		return entero + 1;
	} else {
		return entero;
	}
}

jugador_t *jugador_crear()
{
	jugador_t *jugador = calloc(1, sizeof(jugador_t));
	if (!jugador) {
		return NULL;
	}

	jugador->pokemones = lista_crear();
	jugador->ataques_disponibles = abb_crear(comparador_cadenas);

	if (!jugador->pokemones || !jugador->ataques_disponibles) {
		lista_destruir(jugador->pokemones);
		abb_destruir(jugador->ataques_disponibles);
		free(jugador);
		return NULL;
	}

	jugador->puntaje = 0;

	return jugador;
}

void jugador_destruir(jugador_t *jugador)
{
	if (jugador) {
		lista_destruir(jugador->pokemones);
		abb_destruir(jugador->ataques_disponibles);
		free(jugador);
	}
}

void agregar_pokemon_a_lista(pokemon_t *pokemon, void *contexto)
{
	if (!pokemon || !contexto) {
		return;
	}
	lista_t *lista_pokemon = (lista_t *)contexto;
	lista_insertar(lista_pokemon, pokemon);
}

void agregar_ataque_a_abb(const struct ataque *ataque, void *abb)
{
	if (!abb || !ataque) {
		return;
	}
	abb_insertar(abb, (void *)ataque);
}

void agregar_pokemon_a_jugador(jugador_t *jugador, pokemon_t *pokemon1,
			       pokemon_t *pokemon2, pokemon_t *pokemon3)
{
	if (!jugador) {
		return;
	}
	if (pokemon1)
		lista_insertar(jugador->pokemones, pokemon1);
	if (pokemon2)
		lista_insertar(jugador->pokemones, pokemon2);
	if (pokemon3)
		lista_insertar(jugador->pokemones, pokemon3);

	con_cada_ataque(pokemon1, agregar_ataque_a_abb,
			jugador->ataques_disponibles);
	con_cada_ataque(pokemon2, agregar_ataque_a_abb,
			jugador->ataques_disponibles);
	con_cada_ataque(pokemon3, agregar_ataque_a_abb,
			jugador->ataques_disponibles);
}

int calcular_efectividad_y_puntaje(const struct ataque *ataque,
				   pokemon_t *pokemon_atacado,
				   resultado_jugada_t *resultado,
				   juego_t *juego, jugador_t *jugador)
{
	if (!ataque || !pokemon_atacado || !resultado || !juego || !jugador) {
		return 0;
	}
	// Definir la matriz de efectividades localmente
	const float efectividades[6][6] = {
		// NORMAL, FUEGO, AGUA, PLANTA, ELECTRICO, ROCA
		{ 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // ATAQUE_NORMAL
		{ 1.0, 1.0, 0.5, 3.0, 1.0, 1.0 }, // ATAQUE_FUEGO
		{ 1.0, 3.0, 1.0, 1.0, 0.5, 1.0 }, // ATAQUE_AGUA
		{ 1.0, 0.5, 1.0, 1.0, 1.0, 3.0 }, // ATAQUE_PLANTA
		{ 1.0, 1.0, 3.0, 1.0, 1.0, 0.5 }, // ATAQUE_ELECTRICO
		{ 1.0, 1.0, 1.0, 0.5, 3.0, 1.0 } // ATAQUE_ROCA
	};

	enum TIPO tipo_ataque = ataque->tipo;
	enum TIPO tipo_pokemon_atacado = pokemon_tipo(pokemon_atacado);

	int poder = (int)ataque->poder;
	float efectividad = efectividades[tipo_ataque][tipo_pokemon_atacado];

	if (efectividad == 1.0) {
		if (jugador == juego->jugador_1) {
			resultado->jugador1 = ATAQUE_REGULAR;
		} else {
			resultado->jugador2 = ATAQUE_REGULAR;
		}
	} else if (efectividad > 1.0) {
		if (jugador == juego->jugador_1) {
			resultado->jugador1 = ATAQUE_EFECTIVO;
		} else {
			resultado->jugador2 = ATAQUE_EFECTIVO;
		}
	} else if (efectividad < 1.0) {
		if (jugador == juego->jugador_1) {
			resultado->jugador1 = ATAQUE_INEFECTIVO;
		} else {
			resultado->jugador2 = ATAQUE_INEFECTIVO;
		}
	}

	float res = (float)poder * efectividad;
	return redondear_hacia_arriba(res);
}

int comparador_buscar_pokemon(void *elemento, void *contexto)
{
	if (!elemento || !contexto) {
		return 0;
	}
	const char *nombre_pokemon = (const char *)contexto;
	pokemon_t *pokemon = (pokemon_t *)elemento;

	return strcmp(pokemon_nombre(pokemon), nombre_pokemon);
}

bool ataque_ya_utilizado(jugador_t *jugador, const struct ataque *ataque)
{
	if (!jugador || !ataque) {
		return false;
	}
	return abb_buscar(jugador->ataques_disponibles, (void *)ataque) == NULL;
}

void eliminar_ataque_utilizado(jugador_t *jugador, const struct ataque *ataque)
{
	if (!jugador || !ataque) {
		return;
	}

	const char *nombre_ataque_a_eliminar = ataque->nombre;
	abb_quitar(jugador->ataques_disponibles,
		   (void *)nombre_ataque_a_eliminar);
}

bool hay_ataque(void *elemento, void *aux)
{
	struct ataque *ataque_abb = (struct ataque *)elemento;
	struct ataque *auxiliar = (struct ataque *)aux;

	if (!auxiliar) {
		strncpy(auxiliar->nombre, ataque_abb->nombre,
			sizeof(auxiliar->nombre) - 1);
		auxiliar->nombre[sizeof(auxiliar->nombre) - 1] = '\0';
		auxiliar->poder = ataque_abb->poder;
		auxiliar->tipo = ataque_abb->tipo;
		return true;
	}

	return false;
}