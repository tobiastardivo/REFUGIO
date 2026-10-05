# Trabajo Práctico Integrador — Sistema de Gestión de un Refugio de Animales

**Programación Orientada a Objetos — 2026 — Lic. Diego L. Mattana**
Lenguaje: C++ (estándar C++17) — Aplicación de consola.

## Integrantes

| Nombre y apellido | Clave UCC |
|---|---|
| Tobias Tardivo | 2620549 |
| Santiago Molina | 2620770 |
| Lucas Bernardini | 2610136 |

---

## 1. Descripción del sistema

El programa administra un refugio ficticio de animales. Permite registrar perros y
gatos, registrar adoptantes, generar solicitudes de adopción, confirmarlas o
cancelarlas, devolver un animal al refugio y consultar listados, informes e
historial de adopciones.

Todo el sistema funciona con un menú de consola que se mantiene activo hasta que el
usuario elige *Salir*, y que sigue funcionando aunque se produzca un error, porque
las operaciones se ejecutan dentro de bloques `try` / `catch`.

## 2. ¿Por qué el problema se adapta al paradigma orientado a objetos? (Fase 1)

El dominio del problema está formado por **entidades con identidad propia, datos y
comportamiento**: un animal, un adoptante, una solicitud, el refugio. Eso se traduce
casi directamente en clases.

- **Abstracción:** cada clase modela un concepto real del refugio y expone sólo las
  operaciones que tienen sentido para ese concepto (`confirmar()` una solicitud,
  `mostrarInfo()` de un animal), ocultando cómo están guardados los datos.
- **Encapsulamiento:** los atributos son `private` o `protected` y se acceden por
  métodos. Así el estado de adopción de un animal nunca se puede cambiar "a mano"
  desde afuera salteando las reglas del negocio.
- **Herencia:** perros y gatos comparten casi todos sus datos (ID, nombre, edad,
  salud, estado) y se diferencian en unos pocos atributos. Con programación
  estructurada habría que repetir ese código o usar un `struct` con campos que
  sobran según el caso; con herencia se escribe una sola vez en `Animal`.
- **Polimorfismo:** el refugio guarda un único catálogo de `Animal*` y lo recorre
  llamando a `mostrarInfo()` sin preguntar de qué tipo es cada animal. Agregar un
  tercer tipo (por ejemplo `Conejo`) no obliga a tocar el código del refugio.
- **Composición:** el refugio *tiene* animales, adoptantes y solicitudes, y es el
  responsable de su ciclo de vida.

Un enfoque puramente estructurado (arreglos paralelos y funciones sueltas) obligaría
a mantener sincronizados varios arreglos, repetiría validaciones y sería mucho más
frágil ante cualquier cambio.

## 3. Archivos del proyecto

| Archivo | Contenido |
|---|---|
| `main.cpp` | **Sólo el punto de entrada:** crea el menú y lo ejecuta. Sin lógica. |
| `MenuConsola.h/.cpp` | Interfaz de consola: bucle del menú, despacho de opciones, captura de excepciones, lectura de datos y **selección por lista** (para que no haya que escribir IDs). |
| `Animal.h/.cpp` | Clase **abstracta** base. Estado de adopción, operadores `==`, `!=`, `!`. |
| `Perro.h/.cpp` | Hereda de `Animal`. Agrega raza y tamaño. |
| `Gato.h/.cpp` | Hereda de `Animal`. Agrega color de pelaje e interior/exterior. |
| `Adoptante.h/.cpp` | Datos del adoptante (ID, nombre, contacto, vivienda). |
| `SolicitudAdopcion.h/.cpp` | Relaciona adoptante y animal; estados PENDIENTE / CONFIRMADA / CANCELADA. |
| `Refugio.h/.cpp` | Clase central: catálogo dinámico, altas, búsquedas, proceso de adopción e informes. |
| `Coleccion.h` | **Clase genérica** `Coleccion<T>` con arreglo dinámico y miembro estático. |
| `Excepciones.h` | Excepciones personalizadas derivadas de `std::exception`, incluido el aviso `NadaParaElegirException`. |
| `Utilidades.h/.cpp` | Lectura validada por teclado y la **función genérica** `mostrarColeccion<T>()`. |
| `Makefile` | Compilación alternativa por línea de comandos. |
| `DIAGRAMAS.md` | Diagramas de flujo de cada opción del menú (fuente en Mermaid). |
| `DIAGRAMAS.html` | Los mismos diagramas ya dibujados: se abre en el navegador sin internet. |
| `SistemaAdopcionRefugio.psc` | Pseudocódigo del sistema completo en PSeInt: menú de 14 opciones y proceso de adopción. |
| `.vscode/` | Tareas de compilación y depuración para Visual Studio Code. |

## 4. Cómo compilar y ejecutar

### Opción A — Visual Studio Code (recomendada)

1. Abrir la carpeta del proyecto (`Archivo → Abrir carpeta...`).
2. Compilar con **Ctrl + Shift + B** (tarea *Compilar refugio (g++)*).
3. Ejecutar en la terminal integrada: `.\refugio.exe`
   (o `Terminal → Ejecutar tarea → Compilar y ejecutar`).
4. Para depurar paso a paso: **F5**.

> **Importante:** las tareas usan el compilador MinGW que viene con CLion
> (`C:\Program Files\JetBrains\CLion 2026.1\bin\mingw\bin\g++.exe`) y agregan esa
> carpeta al `PATH`, porque `cc1plus.exe` necesita las DLL que están ahí
> (`libssp-0.dll`). Si se usa otro compilador, hay que cambiar la ruta en
> `.vscode/tasks.json`, `.vscode/launch.json` y `.vscode/c_cpp_properties.json`.

### Opción B — Línea de comandos

```bash
g++ -std=c++17 -Wall -Wextra -o refugio.exe main.cpp MenuConsola.cpp Animal.cpp Perro.cpp Gato.cpp Adoptante.cpp SolicitudAdopcion.cpp Refugio.cpp Utilidades.cpp
./refugio.exe
```

### Opción C — Makefile

```bash
make          # genera refugio.exe
make clean    # limpia los .o y el ejecutable
```

El proyecto compila **sin warnings** con `-Wall -Wextra`.

## 5. Menú del sistema

```
 1. Registrar perro                  8. Confirmar solicitud
 2. Registrar gato                   9. Cancelar solicitud
 3. Listar todos los animales       10. Devolver animal al refugio
 4. Listar animales disponibles     11. Ver adoptantes y solicitudes
 5. Registrar adoptante             12. Historial de adopciones
 6. Ver ficha de un animal          13. Informes y estadísticas
 7. Crear solicitud de adopción     14. Pruebas (copia profunda, operadores, excepciones)
                                     0. Salir
```

El programa arranca con datos de prueba cargados (2 perros, 2 gatos y 2 adoptantes)
para poder probar el circuito completo sin tener que cargar todo a mano.

### El usuario nunca escribe un ID

Los números internos (ID de animal, de adoptante y número de solicitud) siguen siendo la
clave del modelo, porque son lo que usan `Refugio::operator()`, los `operator==` de cada
clase y las solicitudes. Pero **no se piden nunca por teclado**: quien usa el programa no
tiene por qué conocerlos ni acordarse de ellos. Se resuelve de dos maneras:

| Situación | Cómo se resuelve |
|---|---|
| **Altas** (opciones 1, 2 y 5) | El número lo pone el refugio con `nuevoIdAnimal()` / `nuevoIdAdoptante()`. Al terminar se avisa con qué número quedó registrado. Los animales se numeran desde 1 y los adoptantes desde 100, para no confundirlos en pantalla. |
| **Elegir algo que ya existe** (opciones 6 a 10) | Se muestra la **lista numerada de 1 a N** y se elige por posición. `MenuConsola::elegirAnimal()`, `elegirAdoptante()` y `elegirSolicitudPendiente()` traducen ese número de lista al ID interno antes de llamar al `Refugio`. |

Cada lista ofrece **sólo lo que tiene sentido elegir** en ese momento: para adoptar
aparecen únicamente los animales `DISPONIBLE`, para devolver sólo los que están reservados
o adoptados, y para confirmar o cancelar sólo las solicitudes `PENDIENTE`. Si no hay nada
que ofrecer, la lista avisa el motivo antes de pedir cualquier otro dato, y lo hace como un
**aviso y no como un error**: se lee *"No hay solicitudes pendientes. Se crean con la
opción 7."*, sin ningún cartel de `[ERROR]` adelante, porque el usuario no hizo nada mal.
Para eso existe `NadaParaElegirException`, que es la única excepción que el menú captura
aparte para imprimir el mensaje tal cual.

### Las personas y los animales se identifican por su nombre

Lo mismo vale para lo que el programa **muestra**: en ningún listado aparece el ID de un
adoptante. Las personas se listan por nombre, y las solicitudes, que por dentro guardan
dos IDs, se escriben traducidas: *"Solicitud N 1 | Ana Perez quiere adoptar a Rocky
(Perro)"*. El historial de adopciones (opción 12) hace lo mismo cruzando los dos IDs contra
la colección de personas y el catálogo.

Los IDs crudos quedan a la vista en un solo lugar a propósito: la **opción 14**, que es la
pantalla de datos internos del sistema, donde se demuestran los operadores, la copia
profunda y la función genérica.

Como `leerEntero()` ya acota la respuesta al rango `1..N`, el usuario **no puede elegir
algo que no exista**. Las validaciones del modelo (`IdRepetidoException`,
`NoEncontradoException`, `AnimalNoDisponibleException`) siguen estando dentro de `Refugio`
como reglas del negocio, pero la interfaz ya no las puede provocar: ahora sólo se ven en
las pruebas de la opción 14, que las fuerzan a propósito.

### Circuito de una adopción

1. El animal nace **DISPONIBLE**.
2. Al crear una solicitud (opción 7) la solicitud queda **PENDIENTE** y el animal
   pasa a **EN_PROCESO** (queda reservado, nadie más puede solicitarlo).
3. Si se confirma (opción 8): solicitud **CONFIRMADA** y animal **ADOPTADO**.
4. Si se cancela (opción 9): solicitud **CANCELADA** y el animal vuelve a
   **DISPONIBLE**.
5. La devolución (opción 10) deja al animal **DISPONIBLE** otra vez y cancela la
   solicitud pendiente que pudiera tener.

## 6. Decisiones de diseño

- **`main()` no tiene lógica.** Son seis líneas: crea un `MenuConsola` y lo ejecuta.
  Toda la interfaz (bucle del menú, `switch` de opciones, `try` / `catch` y lectura de
  datos) vive en `MenuConsola`, y las reglas del negocio en `Refugio`. Cada archivo
  tiene una sola responsabilidad y el punto de entrada no depende de cómo funciona el
  sistema por dentro.
- **`MenuConsola` contiene al `Refugio` por composición.** Al destruirse el menú se
  destruye el refugio, y con él todo el catálogo. Por eso `main()` crea el menú dentro
  de un bloque: al cerrarlo, ya se puede verificar que la memoria quedó liberada.
- **`Animal` es abstracta.** No existe "un animal genérico" en el refugio: existe un
  perro o un gato. `mostrarInfo()`, `tipo()` y `clonar()` son virtuales puras.
- **Destructor virtual en `Animal`.** El catálogo guarda `Animal*` que en realidad
  apuntan a `Perro` o `Gato`; sin destructor virtual, `delete` no llamaría al
  destructor de la clase derivada.
- **`clonar()`** resuelve la copia profunda polimórfica: cada objeto sabe crear una
  copia exacta de sí mismo, así el constructor de copia de `Refugio` no necesita
  preguntar de qué tipo es cada animal.
- **Las solicitudes guardan IDs, no punteros.** Evita punteros colgados si un
  animal se elimina y mantiene las clases desacopladas.
- **El catálogo usa `Animal**` (memoria dinámica manual)** en lugar de `vector`,
  porque la consigna pide administrar punteros, `new` / `delete` y recorrer con
  aritmética de punteros.
- **Los adoptantes y solicitudes se guardan por valor** dentro de `Coleccion<T>`:
  son objetos chicos, sin herencia, y así la propia colección administra su memoria.
- **Las validaciones de teclado no usan excepciones.** Que alguien escriba "hola"
  donde va un número es un error esperable de la interfaz: se vuelve a pedir el dato.
  Las excepciones se reservan para errores del modelo (ID repetido, índice fuera de
  rango, animal no disponible, no encontrado).
- **Los IDs son un dato interno, no una pregunta al usuario.** La interfaz elige por
  lista y el modelo sigue trabajando con IDs (ver *El usuario nunca escribe un ID* en la
  sección 5). La ventaja de haberlo hecho así, y no cambiando el modelo, es que `Refugio`
  no se enteró del cambio: se le agregaron dos numeradores y sigue recibiendo IDs en todos
  sus métodos, que son los mismos de antes. Toda la traducción "número de lista → ID" vive
  en `MenuConsola`, que es la clase que habla con el usuario.

## 7. Documentación de la memoria dinámica (quién reserva y quién libera)

| Bloque | Reserva | Libera |
|---|---|---|
| Objeto `Perro` / `Gato` | `MenuConsola.cpp` con `new` al registrar (`registrarPerro()` / `registrarGato()`) | `Refugio::liberarCatalogo()`, llamado por `~Refugio()` |
| Objeto `Perro` / `Gato` clonado | `Perro::clonar()` / `Gato::clonar()` (usados por el constructor de copia y el `operator=` de `Refugio`) | `~Refugio()` de la copia |
| Arreglo `Animal** catalogo` | Constructor de `Refugio` y `redimensionarCatalogo()` | `liberarCatalogo()` (`delete[]`) |
| Arreglo interno de `Coleccion<T>` | Constructor, constructor de copia y `redimensionar()` | `~Coleccion()` (`delete[]`) |

Reglas que sigue el proyecto:

- **El `Refugio` es el dueño** de los animales del catálogo. Una vez que se le pasa
  un puntero a `agregarAnimal()`, nadie más debe hacer `delete` sobre ese objeto.
- Si `agregarAnimal()` rechaza el alta por ID repetido, **libera el objeto antes de
  lanzar la excepción** para no perder memoria.
- `redimensionarCatalogo()` hace `delete[]` sólo del **arreglo de punteros**, nunca
  de los animales: los objetos siguen siendo los mismos, cambia el arreglo que los
  referencia.
- Tanto `Refugio` como `Coleccion<T>` cumplen la **Regla de los Tres** (constructor
  de copia + `operator=` + destructor), todos con copia profunda y control de
  autoasignación.
- El contador estático `Animal::animalesVivos` permite verificar en pantalla que no
  queden objetos sin liberar: al cerrar el programa debe llegar a **0**.

## 8. Contenidos de la consigna y dónde están resueltos

| Fase | Requisito | Dónde |
|---|---|---|
| 1 | Menú, estructuras de control, funciones, `cin`/`cout` | `MenuConsola.cpp`, `Utilidades.cpp` |
| 1 | Separación en `.h` y `.cpp` | Todo el proyecto |
| 1 | Pseudocódigo del menú y del proceso de adopción | `SistemaAdopcionRefugio.psc` |
| 1 | Diagrama de flujo de cada opción del sistema | `DIAGRAMAS.md` / `DIAGRAMAS.html` |
| 2 | Clases, encapsulamiento, constructores y destructores | Todas las clases |
| 2 | Colección con memoria dinámica | `Refugio` (`Animal** catalogo`), `Coleccion<T>` |
| 2 | Recorrido con aritmética de punteros | `Refugio::listarConAritmeticaDePunteros()` |
| 2 | Documentar reservas y liberaciones | Sección 7 y comentarios en `Refugio.h` |
| 3 | Constructor de copia, `operator=`, destructor (Regla de los Tres) | `Refugio`, `Coleccion<T>` |
| 3 | `==` comparar por ID | `Animal::operator==` |
| 3 | `[]` acceso por posición | `Refugio::operator[]`, `Coleccion<T>::operator[]` |
| 3 | `()` buscar por ID | `Refugio::operator()` |
| 3 | Operador unario | `Animal::operator!` ("no está disponible") |
| 4 | Clase abstracta con función virtual pura y destructor virtual | `Animal` |
| 4 | Herencia pública y `override` | `Perro`, `Gato` |
| 4 | Catálogo único recorrido polimórficamente | `Refugio::listarAnimales()` |
| 4 | Composición | `Refugio` contiene animales, adoptantes y solicitudes |
| 5 | Clase genérica instanciada con dos tipos | `Coleccion<Adoptante>` y `Coleccion<SolicitudAdopcion>` |
| 5 | Miembro estático en la clase genérica | `Coleccion<T>::coleccionesCreadas` (opción 13) |
| 5 | Función genérica usada con dos tipos | `mostrarColeccion<T>()` (opción 14, PRUEBA 5) |
| 5 | `try` / `throw` / `catch` múltiples, captura por `const&` | `MenuConsola.cpp`, `Excepciones.h` |

### Sobre el miembro estático de la clase genérica

`Coleccion<T>::coleccionesCreadas` **no es un contador único y global**. El
compilador genera una clase distinta por cada tipo con el que se instancia la
plantilla, y cada una tiene su propia copia del miembro estático. Es decir,
`Coleccion<Adoptante>::coleccionesCreadas` y
`Coleccion<SolicitudAdopcion>::coleccionesCreadas` son dos variables
independientes. Esto se ve en pantalla en la opción 13 (*Informes y estadísticas*).

## 9. Casos de prueba

### Pruebas automáticas (opción 14 del menú)

| Prueba | Qué verifica | Resultado esperado |
|---|---|---|
| 1 | Recorrido con aritmética de punteros | Lista los 4 animales con su desplazamiento (+0, +1, +2, +3) |
| 2 | Operadores `[]`, `()`, `==`, `!` | `refugio[0] == refugio(1)` da SI; `!refugio[0]` da NO (está disponible) |
| 3 | **Copia profunda** | Los animales vivos pasan de 4 a 8; las direcciones del animal 0 son distintas; modificar la copia no altera el original; al salir del bloque vuelve a 4 |
| 4a | Índice inválido | Captura `IndiceInvalidoException` y el menú sigue |
| 4b | ID inexistente | Captura `NoEncontradoException` |
| 4c | ID repetido | Captura `IdRepetidoException` |
| 5 | **Función genérica** con dos tipos | `mostrarColeccion<T>()` lista los adoptantes y después las solicitudes: una sola función, dos instanciaciones |

### Pruebas manuales sugeridas

| # | Pasos | Resultado esperado |
|---|---|---|
| 1 | Opción 1 → registrar un perro | No pide ningún ID: avisa *quedó con el número 5* |
| 2 | Opción 1 → escribir "abc" en la edad | Error de la interfaz: vuelve a pedir la edad |
| 3 | Opción 7 → elegir el adoptante 1 y el animal 1, responder NO | Solicitud PENDIENTE y animal EN_PROCESO |
| 4 | Opción 7 otra vez | Ese animal **ya no aparece** en la lista: está reservado |
| 5 | Opción 8 → confirmar la solicitud de la lista | Solicitud CONFIRMADA y animal ADOPTADO |
| 6 | Opción 8 otra vez | Aviso (sin cartel de error): *No hay solicitudes pendientes*. La confirmada no se ofrece |
| 7 | Opción 12 | El historial muestra la adopción con el nombre del animal |
| 8 | Opción 10 → elegir el animal adoptado | Vuelve a DISPONIBLE |
| 9 | Opción 10 otra vez | Aviso: *No hay animales reservados ni adoptados* |
| 10 | Opción 6 → elegir cualquier animal | Muestra la ficha y si está disponible o no |
| 11 | Opción 0 | *Animales en memoria DESPUÉS de destruir el refugio: 0* |

## 10. Diagramas de flujo

Los diagramas de flujo están en **`DIAGRAMAS.md`** (fuente) y en **`DIAGRAMAS.html`**
(ya dibujados, se abre con doble clic en cualquier navegador y no necesita internet).

Hay 18 diagramas en total: uno por cada opción del sistema, como pide la consigna, más el
del programa principal y los de las dos subrutinas auxiliares.

| Diagrama | Contenido |
|---|---|
| Programa principal | Bucle del menú, `switch` y bloque `try` / `catch` |
| Subrutina auxiliar | `leerEntero()`: validación de tipo y de rango |
| Subrutina auxiliar | Elegir un elemento de una lista: lo que reemplazó a escribir el ID |
| Opciones 1 y 2 | Registrar perro / registrar gato (numerador automático y liberación de memoria) |
| Opciones 3 y 4 | Listar todos / listar sólo los disponibles |
| Opción 5 | Registrar adoptante |
| Opción 6 | Ver ficha de un animal (operadores `()` y `!`) |
| Opción 7 | Crear solicitud de adopción (proceso completo) |
| Opciones 8, 9 y 10 | Confirmar / cancelar solicitud y devolver animal |
| Opciones 11, 12 y 13 | Listados por nombre, historial e informes |
| Opción 14 | Pruebas de copia profunda, operadores, excepciones y función genérica |
| Opción 0 | Salida del sistema y liberación de la memoria |

El pseudocódigo está en `SistemaAdopcionRefugio.psc` (PSeInt) y acompaña a los diagramas:
tiene el bucle del menú con las 14 opciones, el proceso de adopción completo, la función de
lectura validada y las tres rutinas de selección por lista (`ElegirAnimal`,
`ElegirAdoptante` y `ElegirSolicitudPendiente`), así que tampoco ahí se escribe un ID. Las
funciones que acceden a los datos están simuladas, porque en el programa real son métodos
de la clase `Refugio` que trabajan sobre el catálogo con memoria dinámica, algo que no se
puede representar en pseudocódigo.

## 11. Pendiente de la entrega

- [x] Completar los integrantes en este README.
- [x] Diagramas de flujo de cada opción del sistema (`DIAGRAMAS.md` / `DIAGRAMAS.html`).
- [x] Comprimir la carpeta en un `.zip` para entregar (`TP_Refugio_POO.zip`).
