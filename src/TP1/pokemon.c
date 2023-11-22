#include "pokemon.h"
#include "ataque.h"
#include "tipo.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char TIPO_NORMAL = 'N';
const char TIPO_FUEGO = 'F';
const char TIPO_AGUA = 'A';
const char TIPO_PLANTA = 'P';
const char TIPO_ELECTRICO = 'E';
const char TIPO_ROCA = 'R';
const int SIN_ATAQUES = 0;
const int SIN_PODER = 0;
const int SIN_POKEMON = 0;
const int SIN_CAMPOS_INVALIDOS = 0;
const int SIN_DELIMITADORES = 0;

#define MAX_NOMBRE 20
#define MAX_ATAQUES 3
#define MAX_CARACTERES 100

typedef struct ataque ataque_t;

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	ataque_t ataques[MAX_ATAQUES];
	int cantidad_ataques;
};

struct info_pokemon {
	pokemon_t *pokemones;
	int cantidad;
};

// Pre: El parámetro cantidad_ataques debe ser igual a cero.
// Post: Inicializa los valores del vector de ataques.
void inicializar_ataque(ataque_t ataques[MAX_ATAQUES], int cantidad_ataques)
{
	strcpy(ataques[cantidad_ataques].nombre, "");
	ataques[cantidad_ataques].tipo = NORMAL;
	ataques[cantidad_ataques].poder = SIN_PODER;
}

// Pre: El parámetro cantidad debe ser igual a cero.
// Post: Inicializa los valores del struct de pokemones.
void inicializar_pokemon(pokemon_t *pokemones, int cantidad)
{
	strcpy(pokemones[cantidad].nombre, "");
	pokemones[cantidad].tipo = NORMAL;
	pokemones[cantidad].cantidad_ataques = SIN_ATAQUES;
	inicializar_ataque(pokemones[cantidad].ataques,
			   pokemones[cantidad].cantidad_ataques);
}

// Pre: ------------------------------------------------------
// Post: Dada una linea, se remueve el salto de linea y se lo cambia por un '\0'
void quitar_salto_linea(char linea[MAX_CARACTERES])
{
	size_t len = strlen(linea);
	if (len > 0 && linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

// Pre: el parámetro tipo debe ser un dato de tipo char.
// Post: Dado un tipo, devuelve True si la letra corresponde a un TIPO_NORMAL,
// TIPO_FUEGO, TIPO_AGUA, TIPO_PLANTA, TIPO_ELECTRICO o TIPO_ROCA. En caso
// contrario, devuelve False.
bool es_tipo_valido(char tipo)
{
	return (tipo == TIPO_NORMAL) || (tipo == TIPO_FUEGO) ||
	       (tipo == TIPO_AGUA) || (tipo == TIPO_PLANTA) ||
	       (tipo == TIPO_ELECTRICO) || (tipo == TIPO_ROCA);
}

// Pre: el parámetro tipo debe ser válido (TIPO_NORMAL, TIPO_FUEGO, TIPO_AGUA,
// TIPO_PLANTA, TIPO_ELECTRICO o TIPO_ROCA). Post: Carga el tipo del struct (Ya
// sea un struct de pokemon o un struct de ataque) con un tipo (NORMAL, FUEGO,
// AGUA, PLANTA, ELECTRICO o ROCA).
void asignar_tipo(void *estructura, int tipo)
{
	if (estructura == NULL) {
		return;
	}
	if (tipo == TIPO_NORMAL) {
		((struct pokemon *)estructura)->tipo = NORMAL;
	} else if (tipo == TIPO_FUEGO) {
		((struct pokemon *)estructura)->tipo = FUEGO;
	} else if (tipo == TIPO_AGUA) {
		((struct pokemon *)estructura)->tipo = AGUA;
	} else if (tipo == TIPO_PLANTA) {
		((struct pokemon *)estructura)->tipo = PLANTA;
	} else if (tipo == TIPO_ELECTRICO) {
		((struct pokemon *)estructura)->tipo = ELECTRICO;
	} else if (tipo == TIPO_ROCA) {
		((struct pokemon *)estructura)->tipo = ROCA;
	}
}

// Pre: el struct de pokemones debe contener datos válidos y la
// cantidad_pokemones debe ser mayor o igual a cero. Post: Dado un vector de
// pokemones, se ordenan en orden alfabético.
void ordenar_pokemones(pokemon_t *pokemones, int cantidad_pokemones)
{
	if (pokemones == NULL) {
		return;
	}

	bool ordenado = false;
	int i = 0;
	pokemon_t aux;
	while (i < cantidad_pokemones - 1 && !ordenado) {
		ordenado = true;
		for (int j = 0; j < cantidad_pokemones - i - 1; j++) {
			if (strcmp(pokemones[j].nombre,
				   pokemones[j + 1].nombre) > 0) {
				aux = pokemones[j];
				pokemones[j] = pokemones[j + 1];
				pokemones[j + 1] = aux;
				ordenado = false;
			}
		}
		i++;
	}
	return;
}

// Pre: el tipo_pokemon tiene que ser un dato de tipo char y el struct de info
// debe tener datos válidos. Post: Carga el struct con los datos pasados por
// parámetro y aumenta la cantidad de pokemones presentes en el struct.
void asignar_pokemon(informacion_pokemon_t *info, char nombre_pokemon[20],
		     char tipo_pokemon)
{
	if (info == NULL) {
		return;
	}
	inicializar_pokemon(info->pokemones, info->cantidad);
	(info->cantidad)++;
	strcpy(info->pokemones[info->cantidad - 1].nombre, nombre_pokemon);
	asignar_tipo(&(info->pokemones[info->cantidad - 1]), tipo_pokemon);
	pokemon_t *aux =
		realloc(info->pokemones,
			sizeof(struct pokemon) *
				(long unsigned int)(info->cantidad + 1));
	if (aux != NULL) {
		info->pokemones = aux;
	}
}

// Pre: el tipo_movimiento tiene que ser un dato de tipo char y el struct de
// info debe tener datos válidos. Post: Carga el struct con los datos pasados
// por parámetro y aumenta la cantidad de ataques presentes en el struct.
void asignar_ataque(informacion_pokemon_t *info, char nombre_ataque[20],
		    char tipo_movimiento, unsigned int poder)
{
	if (info == NULL) {
		return;
	}
	(info->pokemones[info->cantidad - 1].cantidad_ataques)++;
	strcpy(info->pokemones[info->cantidad - 1]
		       .ataques[info->pokemones[info->cantidad - 1]
					.cantidad_ataques -
				1]
		       .nombre,
	       nombre_ataque);
	asignar_tipo(&(info->pokemones[info->cantidad - 1]
			       .ataques[info->pokemones[info->cantidad - 1]
						.cantidad_ataques -
					1]),
		     tipo_movimiento);
	info->pokemones[info->cantidad - 1]
		.ataques[info->pokemones[info->cantidad - 1].cantidad_ataques -
			 1]
		.poder = poder;
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(path, "r");
	if (!archivo) {
		return NULL;
	}

	struct info_pokemon *info = malloc(sizeof(struct info_pokemon));
	if (info == NULL) {
		fclose(archivo);
		return NULL;
	}
	info->cantidad = SIN_POKEMON;

	info->pokemones = malloc(sizeof(struct pokemon));
	if (info->pokemones == NULL) {
		free(info);
		fclose(archivo);
		return NULL;
	}

	info->pokemones[info->cantidad].cantidad_ataques = SIN_ATAQUES;

	char linea[MAX_CARACTERES];
	bool seguir_leyendo = true;
	char nombre_pokemon[MAX_NOMBRE];
	char tipo_pokemon;
	char nombre_ataque[MAX_NOMBRE];
	char tipo_movimiento;
	unsigned int poder;
	int cantidad_pokemones_validos = SIN_POKEMON;
	int linea_datos = 0;

	while (fgets(linea, sizeof(linea), archivo) != NULL && seguir_leyendo) {
		int cantidad_campos_invalidos = SIN_CAMPOS_INVALIDOS;
		int cantidad_delimitadores = SIN_DELIMITADORES;
		quitar_salto_linea(linea);

		for (size_t i = 0; linea[i] != '\0'; i++) {
			if (linea[i] == ':') {
				cantidad_campos_invalidos++;
			} else if (linea[i] == ';') {
				cantidad_delimitadores++;
			}
		}

		if (cantidad_delimitadores == 1 &&
		    cantidad_campos_invalidos == 0) {
			sscanf(linea, "%[^;];%c", nombre_pokemon,
			       &tipo_pokemon);
			if (es_tipo_valido(tipo_pokemon)) {
				asignar_pokemon(info, nombre_pokemon,
						tipo_pokemon);
				cantidad_pokemones_validos++;
			} else {
				cantidad_campos_invalidos++;
			}
		} else if (cantidad_delimitadores == 2) {
			sscanf(linea, "%[^;];%c;%u", nombre_ataque,
			       &tipo_movimiento, &poder);
			if (es_tipo_valido(tipo_movimiento)) {
				asignar_ataque(info, nombre_ataque,
					       tipo_movimiento, poder);
			} else {
				cantidad_campos_invalidos++;
			}
		}

		if (cantidad_campos_invalidos != SIN_CAMPOS_INVALIDOS) {
			if (linea_datos != 0) {
				cantidad_pokemones_validos -= 1;
			}
			info->cantidad = cantidad_pokemones_validos;
			seguir_leyendo = false;
		}

		if (linea_datos == 3) {
			linea_datos = 0;
		} else {
			linea_datos++;
		}
	}

	if (info->cantidad == SIN_POKEMON) {
		free(info->pokemones);
		free(info);
		fclose(archivo);
		return NULL;
	}

	fclose(archivo);
	return info;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL) {
		return NULL;
	}
	int i = 0;
	bool encontrado = false;

	while (i < ip->cantidad && !encontrado) {
		if (strcmp(ip->pokemones[i].nombre, nombre) == 0) {
			encontrado = true;
		}
		i++;
	}

	if (encontrado) {
		return &(ip->pokemones[i - 1]);
	} else {
		return NULL;
	}
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		return ip->cantidad;
	} else {
		return 0;
	}
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon != NULL) {
		return pokemon->nombre;
	} else {
		return NULL;
	}
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NORMAL;
	}
	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL || nombre == NULL) {
		return NULL;
	}

	int i = 0;
	bool encontrado = false;

	while (i < pokemon->cantidad_ataques && !encontrado) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0) {
			encontrado = true;
		}
		i++;
	}

	if (encontrado == true) {
		return &(pokemon->ataques[i - 1]);
	} else {
		return NULL;
	}
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL) {
		return 0;
	}
	ordenar_pokemones(ip->pokemones, ip->cantidad);
	for (int i = 0; i < ip->cantidad; i++) {
		f(&(ip->pokemones[i]), aux);
	}
	return ip->cantidad;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL) {
		return 0;
	}

	for (int i = 0; i < pokemon->cantidad_ataques; i++) {
		f(&(pokemon->ataques[i]), aux);
	}
	return pokemon->cantidad_ataques;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return;
	}
	free(ip->pokemones);
	free(ip);
	return;
}