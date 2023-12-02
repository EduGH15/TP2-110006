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
comando_t *comando_crear(const char *nombre, const char *instruccion,
			 const char *descripcion,
			 COMANDO_ESTADO (*funcion)(void *, void* contexto), void* contexto)
```

Para la función de menu_crear se considera que la asignación de memoria para el menu se realiza en tiempo constante. Luego la función hash_crear tiene una complejidad O(n) con respecto al tamaño del hash aunque, dado que el tamaño del hash para este caso es una constante, entonces se toma que menu_crear tiene complejidad O(1).

```c
menu_t *menu_crear()

```

Dado que la función menu_agregar_comando depende de gran medida de la funcion hash_insertar que en el peor caso sería O(n) debido a un posible rehash.
```c
menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     COMANDO_ESTADO (*funcion)(void *, void *), void* contexto);
```

Para la funcion de menu_eliminar_comando, su complejidad será O(n) debido a que su operación principal se basa en hash_quitar,
La búsqueda y eliminación del nodo enlazado tiene una complejidad O(n), donde 'n' es la cantidad de elementos que hay en la posición.

```c
menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion)
```

Para la funcion menu_ejecutar_comando la complejidad es O(1) considerando que la operación principal es hash obtener, cuya complejidad es O(1).

```c
COMANDO_ESTADO menu_ejecutar_comando(menu_t *menu, const char *instruccion);
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

Para crear un juego se pensó en las siguientes estructuras. Primero, un struct juego que tiene por campos dos campos jugador_t*, un informacion_pokemon_t* info_pokemon (cuya estructura está definida en el tp1), una cantidad de rondas que se inicializa en 8 y un booleano que se inicializa en false.

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

Para analizar la complejidad de juego_seleccionar_pokemon pensemos en una de sus operaciones principales, pokemon_buscar tiene una complejidad de O(n) con respecto a la cantidad de pokemones en informacion_pokemon_t, Luego agregar_pokemon_jugador contiene un lista_insertar cuya complejidad es O(n) con respecto a la cantidad de pokemones en la lista y por cada pokemon se realiza un abb_insertar(abb, ataque) cuya inserción es O(log(n)) donde n es la cantidad de elementos presentes en el arbol (esto, suponiendo que está balanceado). Por lo tanto, como ambas funciones (tanto lista_insertar como abb_insertar) no se encuentran al mismo nivel, puedo tomar en cuenta la que mayor tasa de crecimiento tiene y decir que la complejidad de agregar_pokemon_jugador es O(n). Finalmente digo que juego_seleccionar_pokemon tiene una complejidad de O(n).
```c
	JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
                                       const char *nombre1, const char *nombre2,
                                       const char *nombre3) 
```

Para analizar la complejidad de juego_cargar_pokemon pensemos en una de las operaciones principales, la cual es pokemon_cargar_archivo cuya complejidad es O(n) respecto de la cantidad de lineas que procesa. Luego tenemos la funcion agregar_pokemon_a_lista cuya complejidad es O(n) respecto a la cantidad de pokemones ya que depende de lista_insertar. Asi que juego_cargar_pokemon tiene una complejidad de O(n).
```c
    JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
```

La función juego_jugar_turno cuenta con lista_buscar_elemento cuya complejidad es O(n) respecto a la cantidad de pokemones presentes en la lista, luego pokemon_buscar_ataque cuya complejidad es O(n) respecto de la cantidad de ataques presentes en el pokemon. calcular_efectividad_y_puntaje tiene una complejidad de O(1), y por ultimo la función eliminar_ataque_ya_utilizado también es O(n). Asi que en definitiva la función juego_jugar_turno tiene una complejidad de O(n).
```c
    resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
                                     jugada_t jugada_jugador2) 
```

Por ultimo, la complejidad de juego_destruir es O(n) ya que involucra destruir un jugador, que implica destruir una lista cuya complejidad es O(n) con respecto a la cantidad de pokemones, también se destruye un arbol que eso también es O(n) con respecto a la cantidad de ataques. luego se destruye la lista de pokemones totales, que eso es O(n) respecto a la cantidad de pokemones, y luego se destruye info_pokemon. 

```c
    void juego_destruir(juego_t* juego) 

```

## Funcionamiento del main

Para el flujo del juego se optó por crear una estructura de datos de la siguiente forma:

```c
    typedef struct datos_comprimidos {
        juego_t *juego;
        adversario_t *adversario;
        lista_t *pokes_usuario;
        jugada_t jugadas_usuario[9];
        size_t cantidad_jugadas;
        lista_t *pokes_adversario;
    } datos_comprimidos_t;
```

Esta estructura me permite poder pasar por parámetro a ciertas funciones, por ejemplo: que el usuario pueda elegir los pokemones que quiere usar, al hacer esto podemos modificar la lista de pokes_usuarios elegidos por el jugador. 

Al iniciar el main, comenzamos definiendo variables y estructuras necesarias para el flujo del juego

```c
    menu_t *menu = menu_crear();
	juego_t *juego = juego_crear();
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(juego));
	datos_comprimidos_t datos;
	datos.juego = juego;
	datos.adversario = adversario;
	lista_t *pokes_usuario_ = lista_crear();
	if (!pokes_usuario_) {
		return -1;
	}
	lista_t *pokes_adversario_ = lista_crear();
	if (!pokes_adversario_) {
		return -1;
	}
	datos.pokes_usuario = pokes_usuario_;
	datos.pokes_adversario = pokes_adversario_;
	bool salir = false;
```

Gracias al TDA Menu podemos crear diversos comandos que pueden ser utilizados por el usuario, entre ellos se encuentra:

1.- "p": permite pedirle un archivo al usuario. realizamos validaciones para que el usuario no pueda ingresar nombres incorrectos, archivos con pokemones insuficientes o para que solo cargue una vez un archivo.

2.- "a": permite mostrarle al usuario los comandos disponibles para utilizar. Lo ideal seria usar un menu_con_cada_comando que permita mostrar la descripción de cada comando, pero por falta de tiempo no se pudo.

3.- "s": permite que el usuario pueda cortar el flujo del programa.

4.- "e": permite que el usuario pueda elegir entre los pokemones presentes en el archivo después de cargarlo. Se realizan validaciones para que el usuario no pueda cargar pokemones en nuestra lista de pokes_usuario ubicado en la estructura de datos_comprimidos_t, entre las validaciones se encuentra que no exista el pokemon (que este caso se da si escribe mal el pokemon o si quiere elegir un pokemon antes de cargar el archivo).

5.- "u": permite que el usuario pueda ver los pokemones que puede usar y los ataques que puede usar cada pokemon. También se realizan validaciones para que el usuario no pueda ver pokemones si ejecuta este comando antes de cargar el archivo. Así que no le muestra nada.

6.- "m": permite que el usuario pueda ver todos los pokemones disponibles para armar su equipo pokemon. Se realizan validaciones para que no pueda elegir pokemones inexistentes o repetidos.

7.- "d": permite que el usuario pueda ver todos los pokemones que tiene el adversario. 

8.- "r": permite realizar una jugada, el usuario podrá elegir un pokemon y un ataque. Del mismo modo se realizan validaciones para que el usuario no pueda elegir los mismos pokemones (si es que no cuenta con otro pokemon igual) más de 3 veces o un ataque más de una vez. En caso de que suceda esto, se produce un error en la jugada, el adversario no realiza ningun movimiento y no se modifica el puntaje.

Para validar las jugadas, se decidió alterar el vector de jugadas_usuario que se encuentra en el struct de datos_comprimidos_t, el cual contiene todas las opciones posibles, el nombre del pokemon, y cada ataque. Por cada vez que se realiza una validación, se recorre dicho vector. En caso de que el nombre y el ataque elegidos por el usuario se encuentren en este vector, significa que la jugada es válida así que podemos reescribir el pokemon y el ataque como usados, esto no permite que el usuario realice jugadas repetidas.

```c
    bool verificar_datos(jugada_t jugada, datos_comprimidos_t *datos)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(datos->jugadas_usuario[i].pokemon, jugada.pokemon) ==
			    0 &&
		    strcmp(datos->jugadas_usuario[i].ataque, jugada.ataque) ==
			    0) {
			strcpy(datos->jugadas_usuario[i].ataque, "usado");
			strcpy(datos->jugadas_usuario[i].pokemon, "usado");
			return true;
		}
	}
	return false;
}
```

Siguiendo con el flujo del juego, cada vez que ejecutamos un comando, se imprime un mensaje dependiendo del estado, tenemos mensajes distintos para errores o casos de éxitos distintos, lo que permite orientar de mejor forma al usuario. Durante el curso del juego, por cada ataque realizado, mostramos en todo momento los puntos que posee el usuario y el adversario. Las únicas formas de salir deberían de ser si el usuario cierra el programa con "s" o si el juego está finalizado.

```c
    while (!salir && !juego_finalizado(juego)) {
		printf("Ingresa un comando para realizar una acción. Presiona 'a' para acceder a la ayuda: ");
		leer_linea(linea);
		COMANDO_ESTADO estado = menu_ejecutar_comando(menu, linea);
		imprimir_mensaje(estado);
		if (estado == ATAQUE_REALIZADO) {
			printf("Tus puntos son: %i\n",
			       juego_obtener_puntaje(juego, JUGADOR1));
			printf("Los puntos del adversario son: %i\n",
			       juego_obtener_puntaje(juego, JUGADOR2));
		}
	}
```

Si el usuario decidió salir del programa, se decidió no mostrar si ganó, perdió o empató pues, nunca terminó de jugar.

```c
    if (!salir) {
		mostrar_resultado_juego(juego);
	}
```

Al final, destruimos toda la memoria almacenada.

```c
    lista_destruir(pokes_usuario_);
	lista_destruir(pokes_adversario_);
	menu_destruir(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);
	return 0;
```

Con respecto al cálculo de puntos, este se encuentra en funciones_varias.c y se optó por una matriz cuyas filas contienen el tipo del ataque y columnas contiene el tipo del pokemon. Esto nos permite que dependiendo de la posición, la efectividad sea 1, 0.5 o 3. 

```c
    int calcular_efectividad_y_puntaje(const struct ataque *ataque,
				   pokemon_t *pokemon_atacado,
				   resultado_jugada_t *resultado,
				   juego_t *juego, jugador_t *jugador)
{
	if (!ataque || !pokemon_atacado || !resultado || !juego || !jugador) {
		return 0;
	}
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
```