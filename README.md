<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Eduardo González - 110006 - eegonzalez@fi.uba.ar

- Para compilar:

```bash
make pruebas_chanutron
```

- Para ejecutar:

```bash
./pruebas_chanutron
```

- Para ejecutar con valgrind:

```bash
make valgrind_chanutron
```
---
##  Funcionamiento:

Para la creación del TDA Menú se decidió usar el TDA Hash ya que su estructura es conveniente para almacenar pares clave-valor, donde cada comando funciona como clave y cada clave es única. Esto nos permite tener acceso a las claves en tiempo constante.

## Análisis del TDA Menu

La funcion comando_crear tiene una complejidad de O(1) ya que se supone que la asignación de memoria se realiza en tiempo constante, al igual que la asignación de valores para cada campo del struct comando.

```c
comando_t *comando_crear(const char *nombre, const char *instruccion, const char *descripcion, bool (*funcion)(void *, void *))
```

Para la función de menu_crear se considera que la asignación de memoria para el menu se realiza en tiempo constante. Luego la función hash_crear tiene una complejidad O(n) con respecto al tamaño del hash aunque, dado que el tamaño del hash para este caso es una constante, entonces se toma que menu_crear tiene complejidad O(1).

```c
menu_t *menu_crear()

```

Dado que la función menu_agregar_comando depende de gran medida de la funcion hash_insertar que en el peor caso sería O(n) debido a un posible rehash.
```c
menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto)
```

Para la funcion de menu_eliminar_comando, su complejidad será O(n) debido a que su operación principal se basa en hash_quitar,
La búsqueda y eliminación del nodo enlazado tiene una complejidad O(n), donde 'n' es la cantidad de elementos que hay en la posición.

```c
menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion)
```

Para la funcion menu_ejecutar_comando la complejidad es O(1) considerando que la operación principal es hash obtener, cuya complejidad es O(1).

```c
menu_t *menu_ejecutar_comando(menu_t *menu, const char *instruccion)
```

La funcion menu_contiene_comando es de tiempo constante.

```c
bool menu_contiene_comando(menu_t *menu, const char *instruccion)
```

La funcion menu_obtener_comando, al igual que la funcion menu_ejecutar_comando también cuenta con una operación principal que es hash_obtener que se realiza en tiempo constante, asi que esta función es O(1) también.

```c
comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion)
```

La función menu_vacio es de tiempo constante ya que su operación principal es hash_cantidad la cual es de tiempo constante. Lo mismo pasa con la funcion menu_cantidad_comandos.

```c
bool menu_vacio(menu_t *menu)

size_t menu_cantidad_comandos(menu_t *menu)
```

la funcion menu_destruir es O(n) donde n es la cantidad de comandos del menú. Esto es debido a que la funcion hash_destruir_todo es de tiempo lineal con respecto a los elementos del hash.

```c
void menu_destruir(menu_t *menu)
```

# Estructura del juego y del Jugador en Juego.c:

Para crear un juego se pensó en las siguientes estructuras. Primero, un struct juego que tiene por campos dos campos jugador_t*, un informacion_pokemon_t* info_pokemon (cuya estructura está definida en el tp1), una cantidad de rondas que se inicializa en 9 y un booleano que se inicializa en false.

El struct de jugador cuenta con una lista de pokemones que son elegidos por el jugador, y un abb de ataques disponibles. Se utilizó un abb de ataques para que, cuando el jugador realice un movimiento, dicho movimiento pueda ser buscado y eliminado del arbol fácilmente, cosa que se complica si hubiera usado una lista. Y también cuenta con un puntaje que se va actualizando conforme avance el juego. 

```c
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
```

## Lógica de juego_jugar_turno

Inicializamos de antemano el resultado_ataque en ATAQUE_ERROR por si se presenta algun error a lo largo de la función.

```c
	resultado_jugada_t resultado_ataque;
    resultado_ataque.jugador1 = ATAQUE_ERROR;
    resultado_ataque.jugador2 = ATAQUE_ERROR;
```

En base a la selección del jugador, buscamos en la lista de pokemones disponibles los pokemones elegidos.

```c
    pokemon_t *pokemon_jugador1 = lista_buscar_elemento(juego->jugador_1->pokemones, comparador_buscar_pokemon, jugada_jugador1.pokemon);
    pokemon_t *pokemon_jugador2 = lista_buscar_elemento(juego->jugador_2->pokemones, comparador_buscar_pokemon, jugada_jugador2.pokemon);
```

Lo mismo hacemos para los ataques, buscamos en el abb de ataques, los ataques o movimientos realizados por el jugador

```c
    const struct ataque *ataque_jugador1 = pokemon_buscar_ataque(pokemon_jugador1, jugada_jugador1.ataque);
    const struct ataque *ataque_jugador2 = pokemon_buscar_ataque(pokemon_jugador2, jugada_jugador2.ataque);
```

Validamos si el ataque fue utilizado o no, esto se hace verificando si el ataque se encuentra o no en el abb de ataques. Si se encuentra en el arbol es porque no ha sido utilizado. Si no está, es porque ya fue utilizado.

```c
    if (ataque_ya_utilizado(juego->jugador_1, ataque_jugador1) || ataque_ya_utilizado(juego->jugador_2, ataque_jugador2))
```

Luego calculamos la efectividad y el puntaje del ataque realizado ya sea por el jugador 1 o jugador 2. Actualizamos los puntajes de los jugadores y eliminamos el ataque recién utilizado.


## Complejidad del Juego.c:

La complejidad de jeugo_crear es O(1) ya que la asignación de memoria para juego es O(1), la creación de jugadores (juego->jugador_1 y juego->jugador_2) dependen de jugador_crear, que es O(1).Y la creación de la lista de pokemones totales (juego->pokemones_totales) también es O(1). Encima la configuración de campos y verificación de errores es O(1).

```c
	juego_t* juego_crear()
```

La complejidad de juego_obtener_puntaje es de tiempo constante ya que sus operaciones se ejecutan en tiempo constante. Lo mismo ocurre con juego_finalizado y juego_listar_pokemon.

```c
	int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)

	bool juego_finalizado(juego_t *juego)

	lista_t *juego_listar_pokemon(juego_t *juego)


```


## Complejidad de funciones_varias.h

 la complejidad de jugador_crear es O(1) en el peor caso, ya que cada operación individual dentro de la función implica un número constante de pasos.

```c
	jugador_t* jugador_crear()
```

La funcion jugador_destruir realiza la destrucción de la lista de pokemones (jugador->pokemones) en donde la complejidad depende de la cantidad de pokemones en la lista, siendo O(n), donde "n" es el número de pokemones en la lista. Luego realiza la destrucción del árbol binario de búsqueda (jugador->ataques_disponibles) en donde la complejidad depende del tamaño del árbol, siendo O(m), donde "m" es la cantidad de elementos en el árbol. Y la liberación de la estructura es O(1).
Por lo que puedo decir que la complejidad de jugador_destruir es O(m+n).

```c
	void jugador_destruir(jugador_t* jugador) {
```