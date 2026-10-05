# Diagramas de flujo del sistema

**Trabajo Práctico Integrador — Programación Orientada a Objetos 2026 — Lic. Diego L. Mattana**
Sistema de Gestión de un Refugio de Animales (C++17)

Este documento contiene **un diagrama de flujo por cada opción del menú**, como pide la
consigna en los requisitos de entrega, más el diagrama del programa principal y los de las
dos rutinas auxiliares que usan todas las opciones: la lectura validada por teclado y la
selección de un elemento de una lista.

> **El usuario nunca escribe un ID.** Los números internos (ID de animal, de adoptante y
> de solicitud) existen porque son la clave del modelo, pero no se piden por teclado:
> en las altas los pone el refugio y, cuando hay que elegir algo que ya existe, se muestra
> la lista numerada de 1 a N y se elige por posición. Por eso en los diagramas no aparece
> ningún "leer el ID": aparece *pedir el próximo número al refugio* o *elegir de la lista*.

Los diagramas están escritos en **Mermaid**. Se pueden ver de tres formas:

1. Abriendo `DIAGRAMAS.html` en cualquier navegador (no necesita internet).
2. En Visual Studio Code, con la vista previa de Markdown (`Ctrl + Shift + V`) y la
   extensión *Markdown Preview Mermaid Support*.
3. Pegando el contenido de cualquier bloque en <https://mermaid.live>.

**Convención de figuras usada en todos los diagramas:**

| Figura | Significado |
|---|---|
| Óvalo | Inicio / Fin del proceso |
| Paralelogramo | Entrada o salida por consola (`cin` / `cout`) |
| Rectángulo | Proceso: llamada a un método o cálculo |
| Rombo | Decisión |
| Rectángulo con borde doble | Lanzamiento de una excepción (`throw`) |

---

## Programa principal — Menú del sistema

El menú es un `do...while` que se repite hasta que el usuario elige `0`. Toda la operación
elegida se ejecuta dentro de un bloque `try`, por eso **un error nunca corta el programa**:
se muestra el mensaje y el menú vuelve a aparecer.

```mermaid
flowchart TD
    INI(["Inicio del programa"])
    CREA["Crear el objeto Refugio<br/>reserva el arreglo dinamico de punteros a Animal"]
    DATOS["cargarDatosDePrueba<br/>2 perros, 2 gatos y 2 adoptantes"]
    MENU[/"Limpiar pantalla y mostrar el menu<br/>con los contadores del refugio"/]
    LEE[/"Leer opcion validada entre 0 y 14"/]
    TRY["Abrir bloque try"]
    SW{"Segun el valor de opcion"}
    OP["Ejecutar la operacion elegida<br/>ver el diagrama de cada opcion"]
    SALIR[/"Mostrar: Cerrando el sistema"/]
    EXC{"La operacion lanzo<br/>una excepcion?"}
    CATCH[/"Mostrar el mensaje del catch<br/>que corresponde al tipo de error"/]
    ES0{"opcion es igual a 0?"}
    PAUSA[/"Pausar hasta que el usuario<br/>presione ENTER"/]
    ANTES[/"Mostrar animalesVivos<br/>ANTES de destruir el refugio"/]
    DEST["Fin del bloque: se ejecuta el destructor<br/>delete de cada animal y del arreglo"]
    DESPUES[/"Mostrar animalesVivos<br/>DESPUES de destruir el refugio: debe dar 0"/]
    FIN(["Fin del programa"])

    INI --> CREA --> DATOS --> MENU --> LEE --> TRY --> SW
    SW -->|"1 a 14"| OP
    SW -->|"0"| SALIR
    OP --> EXC
    SALIR --> EXC
    EXC -->|"Si"| CATCH --> ES0
    EXC -->|"No"| ES0
    ES0 -->|"No"| PAUSA --> MENU
    ES0 -->|"Si"| ANTES --> DEST --> DESPUES --> FIN
```

> **Orden de los `catch`:** van del más específico al más general
> (`IdRepetidoException`, `IndiceInvalidoException`, `AnimalNoDisponibleException`,
> `NoEncontradoException`, `RefugioException`, `std::exception`). Si `std::exception`
> estuviera primero atraparía todo y los demás nunca se ejecutarían. Todos capturan
> **por referencia constante**.

---

## Subrutina auxiliar — Lectura validada por teclado

`leerEntero()` es la función que usan casi todas las opciones para pedir números. Aparece
en los demás diagramas como un solo paralelogramo de "Leer ... validado", pero por dentro
hace esto:

```mermaid
flowchart TD
    INI(["leerEntero con mensaje, minimo y maximo"])
    PIDE[/"Mostrar el mensaje y leer el valor"/]
    FALLO{"Fallo la lectura?<br/>se escribio texto en vez de numero"}
    LIMPIA["Limpiar el estado de cin<br/>y descartar la linea escrita"]
    ERR1[/"Mostrar: debe ingresar un numero"/]
    RANGO{"El valor esta entre<br/>minimo y maximo?"}
    ERR2[/"Mostrar: el valor debe estar<br/>entre minimo y maximo"/]
    OK(["Devolver el valor leido"])

    INI --> PIDE --> FALLO
    FALLO -->|"Si"| LIMPIA --> ERR1 --> PIDE
    FALLO -->|"No"| RANGO
    RANGO -->|"No"| ERR2 --> PIDE
    RANGO -->|"Si"| OK
```

> Por eso en los diagramas de las opciones no se repite la validación de tipo y de rango:
> ya está resuelta acá. Lo mismo hacen `leerTexto()` y `leerSiNo()`.

---

## Subrutina auxiliar — Elegir un elemento de una lista

Es la rutina que reemplazó a "escribir el ID". Las opciones 6 a 10 la usan a través de
`elegirAnimal()`, `elegirAdoptante()` y `elegirSolicitudPendiente()`, que trabajan todas
igual: muestran la lista numerada, leen **un número de esa lista** y devuelven el ID
interno del elemento elegido. En los diagramas de esas opciones aparece como un solo
paralelogramo de "Elegir ... de la lista".

```mermaid
flowchart TD
    INI(["Elegir de la lista: recibe el titulo<br/>y el filtro que se quiere ofrecer"])
    CUENTA["Contar cuantos elementos<br/>pasan el filtro"]
    HAY{"Hay al menos uno?"}
    THROW[["throw DatoInvalidoException<br/>con el motivo: no hay animales<br/>disponibles, no hay adoptantes, etc."]]
    LISTA[/"Mostrar la lista numerada de 1 a N<br/>con el nombre y el estado de cada uno"/]
    LEE[/"Leer un numero validado entre 1 y N<br/>con leerEntero: no se puede elegir<br/>nada que no este en la lista"/]
    TRAD["Recorrer otra vez contando los que pasan<br/>el filtro hasta llegar al elegido"]
    OK(["Devolver el ID interno de ese elemento"])

    INI --> CUENTA --> HAY
    HAY -->|"No"| THROW
    HAY -->|"Si"| LISTA --> LEE --> TRAD --> OK
```

> **Por qué hace falta el último paso:** el número que ve el usuario **no** es la posición
> en el catálogo. Si se están ofreciendo sólo los animales disponibles, el `1` de la lista
> puede ser el animal que está guardado en la posición 3. Por eso se vuelve a recorrer
> aplicando el mismo filtro hasta llegar al que eligió. En `elegirAdoptante()` ese paso no
> existe, porque ahí no hay filtro y el número de lista coincide con la posición.
>
> Como `leerEntero()` ya acota la respuesta al rango `1..N`, **ninguna de estas rutinas
> necesita validar la elección**: no se puede elegir algo que no exista.

---

## Opción 1 — Registrar perro

```mermaid
flowchart TD
    INI(["Opcion 1: Registrar perro"])
    ID["Pedir al refugio el proximo numero libre<br/>nuevoIdAnimal: el usuario no lo escribe"]
    NOM[/"Leer nombre"/]
    EDAD[/"Leer edad validada entre 0 y 30"/]
    SALUD[/"Leer estado de salud"/]
    RAZA[/"Leer raza"/]
    TAM[/"Leer tamanio: 1 Pequenio, 2 Mediano, 3 Grande"/]
    NEW["Crear el objeto con new Perro<br/>la memoria pasa a ser del Refugio"]
    AGR["Llamar a agregarAnimal con ese puntero"]
    REP{"Ya existe un animal<br/>con ese ID?"}
    DEL["delete del objeto recien creado<br/>para no perder memoria"]
    THROW[["throw IdRepetidoException"]]
    LLENO{"El catalogo esta lleno?<br/>cantAnimales igual a capacidad"}
    RESIZE["redimensionarCatalogo al doble:<br/>arreglo nuevo, copiar punteros,<br/>liberar el arreglo viejo"]
    GUARDA["Guardar el puntero en el catalogo,<br/>aumentar cantAnimales y correr<br/>el numerador al siguiente numero"]
    OK[/"Mostrar: Perro registrado correctamente<br/>y con que numero quedo"/]
    CATCH[/"El catch del menu muestra el error<br/>y el menu sigue funcionando"/]
    FIN(["Volver al menu"])

    INI --> ID --> NOM --> EDAD --> SALUD --> RAZA --> TAM --> NEW --> AGR --> REP
    REP -->|"Si"| DEL --> THROW --> CATCH --> FIN
    REP -->|"No"| LLENO
    LLENO -->|"Si"| RESIZE --> GUARDA
    LLENO -->|"No"| GUARDA
    GUARDA --> OK --> FIN
```

> **Memoria (Fase 2):** el `new` se hace en `MenuConsola.cpp` y la propiedad de ese bloque pasa al
> `Refugio` apenas se llama a `agregarAnimal()`. Si el alta se rechaza por ID repetido, el
> propio `agregarAnimal()` hace el `delete` **antes** de lanzar la excepción.
>
> **El control de ID repetido ya no se puede disparar desde el menú**, porque el número lo
> pone el propio refugio. Queda igual dentro de `agregarAnimal()` como red de seguridad del
> modelo, y es la que se prueba a propósito en la opción 14.

---

## Opción 2 — Registrar gato

Mismo circuito que la opción 1, pero con los atributos propios de `Gato`.

```mermaid
flowchart TD
    INI(["Opcion 2: Registrar gato"])
    ID["Pedir al refugio el proximo numero libre<br/>nuevoIdAnimal: el usuario no lo escribe"]
    NOM[/"Leer nombre"/]
    EDAD[/"Leer edad validada entre 0 y 30"/]
    SALUD[/"Leer estado de salud"/]
    COLOR[/"Leer color de pelaje"/]
    INT[/"Leer si vive en interior: SI o NO"/]
    NEW["Crear el objeto con new Gato"]
    AGR["Llamar a agregarAnimal con ese puntero"]
    REP{"Ya existe un animal<br/>con ese ID?"}
    DEL["delete del objeto y<br/>throw IdRepetidoException"]
    GUARDA["Redimensionar si hace falta<br/>y guardar el puntero en el catalogo"]
    OK[/"Mostrar: Gato registrado correctamente<br/>y con que numero quedo"/]
    CATCH[/"El catch del menu muestra el error"/]
    FIN(["Volver al menu"])

    INI --> ID --> NOM --> EDAD --> SALUD --> COLOR --> INT --> NEW --> AGR --> REP
    REP -->|"Si"| DEL --> CATCH --> FIN
    REP -->|"No"| GUARDA --> OK --> FIN
```

---

## Opción 3 — Listar todos los animales

Recorrido **polimórfico**: se llama siempre a `mostrarInfo()` y la ligadura dinámica decide
si ejecuta la versión de `Perro` o la de `Gato`.

```mermaid
flowchart TD
    INI(["Opcion 3: Listar todos los animales"])
    VACIO{"cantAnimales es 0?"}
    MSG[/"Mostrar: el refugio no tiene<br/>animales cargados"/]
    I["i igual a 0<br/>mostrados igual a 0"]
    COND{"i menor que cantAnimales?"}
    SHOW[/"Llamar a mostrarInfo del animal i<br/>llamada polimorfica"/]
    INC["mostrados mas 1<br/>i mas 1"]
    TOT[/"Mostrar el total listado"/]
    FIN(["Volver al menu"])

    INI --> VACIO
    VACIO -->|"Si"| MSG --> FIN
    VACIO -->|"No"| I --> COND
    COND -->|"Si"| SHOW --> INC --> COND
    COND -->|"No"| TOT --> FIN
```

---

## Opción 4 — Listar animales disponibles

Es el mismo recorrido de la opción 3 pero con un filtro: se saltea todo animal que no esté
en estado `DISPONIBLE`.

```mermaid
flowchart TD
    INI(["Opcion 4: Listar animales disponibles"])
    VACIO{"cantAnimales es 0?"}
    MSG[/"Mostrar: el refugio no tiene<br/>animales cargados"/]
    I["i igual a 0<br/>mostrados igual a 0"]
    COND{"i menor que cantAnimales?"}
    DISP{"El animal i esta DISPONIBLE?"}
    SHOW[/"Mostrar los datos del animal i"/]
    MAS["mostrados mas 1"]
    INC["i mas 1"]
    NING{"mostrados es 0?"}
    MSG2[/"Mostrar: no hay animales<br/>que cumplan el filtro"/]
    TOT[/"Mostrar el total listado"/]
    FIN(["Volver al menu"])

    INI --> VACIO
    VACIO -->|"Si"| MSG --> FIN
    VACIO -->|"No"| I --> COND
    COND -->|"Si"| DISP
    DISP -->|"Si"| SHOW --> MAS --> INC
    DISP -->|"No"| INC
    INC --> COND
    COND -->|"No"| NING
    NING -->|"Si"| MSG2 --> FIN
    NING -->|"No"| TOT --> FIN
```

---

## Opción 5 — Registrar adoptante

```mermaid
flowchart TD
    INI(["Opcion 5: Registrar adoptante"])
    ID["Pedir al refugio el proximo numero libre<br/>nuevoIdAdoptante: arranca en 100"]
    NOM[/"Leer nombre y apellido"/]
    CONT[/"Leer contacto"/]
    VIV[/"Leer tipo de vivienda"/]
    OBJ["Construir el objeto Adoptante"]
    REP{"Ya existe un adoptante<br/>con ese ID?"}
    THROW[["throw IdRepetidoException"]]
    AGR["Agregar a la coleccion de adoptantes:<br/>guarda una COPIA y redimensiona<br/>sola si esta llena"]
    OK[/"Mostrar: Adoptante registrado correctamente<br/>y con que numero quedo"/]
    CATCH[/"El catch del menu muestra el error"/]
    FIN(["Volver al menu"])

    INI --> ID --> NOM --> CONT --> VIV --> OBJ --> REP
    REP -->|"Si"| THROW --> CATCH --> FIN
    REP -->|"No"| AGR --> OK --> FIN
```

> A diferencia de los animales, los adoptantes se guardan **por valor** dentro de
> `Coleccion<Adoptante>`, que administra su propia memoria dinámica.
>
> Los números de adoptante arrancan en **100** y los de animal en **1**, así en pantalla
> nunca se confunde uno con el otro.

---

## Opción 6 — Ver ficha de un animal

Usa dos operadores sobrecargados de la Fase 3: `operator()` para buscar por ID y el
operador unario `operator!` para preguntar si el animal **no** está disponible. El ID que
recibe el `operator()` sale de la lista, no del teclado.

```mermaid
flowchart TD
    INI(["Opcion 6: Ver ficha de un animal"])
    ELIGE[/"Elegir un animal de la lista<br/>filtro TODOS: ver la subrutina"/]
    BUSCA["Buscar ese ID con el operador parentesis<br/>sobrecargado del Refugio"]
    SHOW[/"Mostrar los datos con mostrarInfo"/]
    NOT{"El operador unario ! da verdadero?<br/>es decir: no esta disponible"}
    MSG1[/"Mostrar: este animal NO esta<br/>disponible para adopcion"/]
    MSG2[/"Mostrar: este animal esta<br/>disponible para adopcion"/]
    CATCH[/"El catch del menu muestra el error<br/>si el refugio esta vacio"/]
    FIN(["Volver al menu"])

    INI --> ELIGE
    ELIGE -->|"no hay animales cargados"| CATCH --> FIN
    ELIGE -->|"eligio uno"| BUSCA --> SHOW --> NOT
    NOT -->|"Si"| MSG1 --> FIN
    NOT -->|"No"| MSG2 --> FIN
```

> Ya no hace falta el rombo de "¿se encontró el animal?": el ID viene de la lista que
> armó el propio sistema, así que el animal existe seguro. El `NoEncontradoException` del
> `operator()` sigue estando, pero ahora es una red del modelo y no un error esperable de
> la interfaz.

---

## Opción 7 — Crear solicitud de adopción

Es el proceso central del sistema. Reserva el animal (`EN_PROCESO`) y opcionalmente
confirma la adopción en el momento.

```mermaid
flowchart TD
    INI(["Opcion 7: Crear solicitud de adopcion"])
    ELAD[/"Elegir el adoptante de la lista<br/>Quien quiere adoptar?"/]
    ELAN[/"Elegir el animal de la lista<br/>filtro DISPONIBLES: A que animal?"/]
    CREA["Crear la solicitud en estado PENDIENTE<br/>y guardarla en la coleccion"]
    RESERVA["Cambiar el estado del animal a EN_PROCESO"]
    MSG[/"Mostrar el numero de solicitud creada"/]
    CONF{"Desea confirmar<br/>la adopcion ahora?"}
    CONFIRMA["Confirmar la solicitud:<br/>solicitud CONFIRMADA y animal ADOPTADO"]
    MSG1[/"Mostrar: adopcion confirmada"/]
    MSG2[/"Mostrar: la solicitud queda PENDIENTE<br/>y el animal sigue reservado"/]
    CATCH[/"El catch del menu muestra el motivo:<br/>no hay adoptantes o no hay<br/>animales disponibles"/]
    FIN(["Volver al menu"])

    INI --> ELAD
    ELAD -->|"no hay adoptantes"| CATCH --> FIN
    ELAD -->|"eligio uno"| ELAN
    ELAN -->|"no hay disponibles"| CATCH
    ELAN -->|"eligio uno"| CREA --> RESERVA --> MSG --> CONF
    CONF -->|"SI"| CONFIRMA --> MSG1 --> FIN
    CONF -->|"NO"| MSG2 --> FIN
```

> **Comparado con la versión anterior este diagrama tiene cinco rombos menos.** Antes había
> que verificar que hubiera animales y adoptantes, leer los dos IDs y después comprobar uno
> por uno que existieran y que el animal estuviera disponible. Ahora:
>
> - los dos avisos de "no hay nada para elegir" los da la propia subrutina de selección;
> - "¿existe el adoptante?" y "¿existe el animal?" desaparecieron: los IDs salen de la lista;
> - el filtro `DISPONIBLES` hace que un animal reservado o adoptado **ni siquiera aparezca**,
>   así que tampoco hay que preguntar si está disponible.
>
> Las validaciones siguen existiendo dentro de `Refugio::crearSolicitud()` (incluido el
> `AnimalNoDisponibleException`, que usa el operador `!`), pero como reglas del modelo: la
> interfaz ya no puede provocarlas.

---

## Opción 8 — Confirmar solicitud

```mermaid
flowchart TD
    INI(["Opcion 8: Confirmar solicitud"])
    ELIGE[/"Elegir una solicitud de la lista de<br/>PENDIENTES, que se leen en castellano:<br/>Ana Perez quiere adoptar a Rocky"/]
    BUSCA["Buscar el animal de la solicitud<br/>con el operador parentesis"]
    CONF["Marcar la solicitud como CONFIRMADA"]
    ADOPT["Poner el animal en estado ADOPTADO"]
    OK[/"Mostrar: solicitud CONFIRMADA<br/>y animal ADOPTADO"/]
    CATCH[/"El catch del menu avisa que no hay<br/>solicitudes pendientes"/]
    FIN(["Volver al menu"])

    INI --> ELIGE
    ELIGE -->|"no hay pendientes"| CATCH --> FIN
    ELIGE -->|"eligio una"| BUSCA --> CONF --> ADOPT --> OK --> FIN
```

> **La lista sólo ofrece las PENDIENTES**, que son las únicas que se pueden confirmar.
> Antes se listaban todas y el usuario podía elegir una ya cerrada, para recién entonces
> recibir el error. Los dos rombos que verificaban eso siguen dentro de
> `Refugio::confirmarSolicitud()`, pero la interfaz ya no los puede disparar.

---

## Opción 9 — Cancelar solicitud

Igual que la opción 8, pero el animal **vuelve a estar disponible** en lugar de quedar
adoptado.

```mermaid
flowchart TD
    INI(["Opcion 9: Cancelar solicitud"])
    ELIGE[/"Elegir una solicitud de la lista<br/>de PENDIENTES"/]
    BUSCA["Buscar el animal de la solicitud"]
    CANC["Marcar la solicitud como CANCELADA"]
    DISP["El animal vuelve al estado DISPONIBLE"]
    OK[/"Mostrar: solicitud CANCELADA<br/>y animal DISPONIBLE"/]
    CATCH[/"El catch del menu avisa que no hay<br/>solicitudes pendientes"/]
    FIN(["Volver al menu"])

    INI --> ELIGE
    ELIGE -->|"no hay pendientes"| CATCH --> FIN
    ELIGE -->|"eligio una"| BUSCA --> CANC --> DISP --> OK --> FIN
```

---

## Opción 10 — Devolver animal al refugio

```mermaid
flowchart TD
    INI(["Opcion 10: Devolver animal al refugio"])
    ELIGE[/"Elegir el animal de la lista<br/>filtro NO_DISPONIBLES: solo los que<br/>estan reservados o adoptados"/]
    I["i igual a 0"]
    COND{"i menor que la cantidad<br/>de solicitudes?"}
    MATCH{"La solicitud i es de este animal<br/>y esta PENDIENTE?"}
    CANC["Cancelar esa solicitud<br/>para no dejarla colgada"]
    INC["i mas 1"]
    DISP["Poner el animal en estado DISPONIBLE"]
    OK[/"Mostrar: el animal volvio<br/>al estado DISPONIBLE"/]
    CATCH[/"El catch del menu avisa que no hay<br/>nada para devolver"/]
    FIN(["Volver al menu"])

    INI --> ELIGE
    ELIGE -->|"no hay reservados<br/>ni adoptados"| CATCH --> FIN
    ELIGE -->|"eligio uno"| I --> COND
    COND -->|"Si"| MATCH
    MATCH -->|"Si"| CANC --> INC
    MATCH -->|"No"| INC
    INC --> COND
    COND -->|"No"| DISP --> OK --> FIN
```

> El filtro `NO_DISPONIBLES` reemplazó al rombo de "¿el animal ya figura como
> DISPONIBLE?": un animal que nunca salió del refugio **no aparece en la lista**, así que
> no se puede elegir para devolver. El bucle que cancela la solicitud pendiente que pudiera
> tener ese animal sigue igual, dentro de `Refugio::devolverAnimal()`.

---

## Opción 11 — Ver adoptantes y solicitudes

Esta opción existe para mostrar la **función genérica** de la Fase 5: la misma función
`mostrarColeccion<T>()` se usa con dos tipos distintos.

```mermaid
flowchart TD
    INI(["Opcion 11: Ver adoptantes y solicitudes"])
    T1["Llamar a mostrarColeccion con T igual a Adoptante"]
    V1{"La coleccion esta vacia?"}
    M1[/"Mostrar: la coleccion esta vacia"/]
    L1[/"Recorrer y mostrar cada adoptante"/]
    T2["Llamar a mostrarColeccion con T igual a SolicitudAdopcion"]
    V2{"La coleccion esta vacia?"}
    M2[/"Mostrar: la coleccion esta vacia"/]
    L2[/"Recorrer y mostrar cada solicitud"/]
    FIN(["Volver al menu"])

    INI --> T1 --> V1
    V1 -->|"Si"| M1 --> T2
    V1 -->|"No"| L1 --> T2
    T2 --> V2
    V2 -->|"Si"| M2 --> FIN
    V2 -->|"No"| L2 --> FIN
```

---

## Opción 12 — Historial de adopciones

```mermaid
flowchart TD
    INI(["Opcion 12: Historial de adopciones"])
    I["i igual a 0<br/>contador igual a 0"]
    COND{"i menor que la cantidad<br/>de solicitudes?"}
    CONF{"La solicitud i esta<br/>CONFIRMADA?"}
    SHOW[/"Mostrar los datos de la solicitud"/]
    BUSCA["Buscar el indice del animal"]
    EXISTE{"Se encontro el animal<br/>en el catalogo?"}
    NOM[/"Mostrar el tipo y el nombre del animal"/]
    MAS["contador mas 1"]
    INC["i mas 1"]
    VACIO{"contador es 0?"}
    MSG[/"Mostrar: todavia no hay<br/>adopciones concretadas"/]
    TOT[/"Mostrar el total de adopciones"/]
    FIN(["Volver al menu"])

    INI --> I --> COND
    COND -->|"Si"| CONF
    CONF -->|"Si"| SHOW --> BUSCA --> EXISTE
    EXISTE -->|"Si"| NOM --> MAS
    EXISTE -->|"No"| MAS
    MAS --> INC
    CONF -->|"No"| INC
    INC --> COND
    COND -->|"No"| VACIO
    VACIO -->|"Si"| MSG --> FIN
    VACIO -->|"No"| TOT --> FIN
```

---

## Opción 13 — Informes y estadísticas

Recorre el catálogo y las solicitudes contando por estado, y después muestra los
**miembros estáticos**, que sirven para verificar que no queden fugas de memoria.

```mermaid
flowchart TD
    INI(["Opcion 13: Informes y estadisticas"])
    CERO["Poner en 0 los contadores:<br/>perros, gatos, disponibles,<br/>en proceso y adoptados"]
    C1{"i menor que cantAnimales?"}
    TIPO{"El animal i es un Perro?"}
    P["perros mas 1"]
    G["gatos mas 1"]
    EST{"Segun el estado<br/>del animal"}
    SUMA["Aumentar el contador<br/>que corresponde al estado"]
    I1["i mas 1"]
    CERO2["Poner en 0 pendientes,<br/>confirmadas y canceladas"]
    C2{"j menor que la cantidad<br/>de solicitudes?"}
    EST2{"Segun el estado<br/>de la solicitud"}
    SUMA2["Aumentar el contador<br/>que corresponde"]
    J1["j mas 1"]
    REP[/"Mostrar el informe completo:<br/>animales por tipo y estado,<br/>adoptantes y solicitudes"/]
    ESTAT[/"Mostrar los miembros estaticos:<br/>animalesVivos y coleccionesCreadas<br/>de cada instanciacion"/]
    FIN(["Volver al menu"])

    INI --> CERO --> C1
    C1 -->|"Si"| TIPO
    TIPO -->|"Si"| P --> EST
    TIPO -->|"No"| G --> EST
    EST --> SUMA --> I1 --> C1
    C1 -->|"No"| CERO2 --> C2
    C2 -->|"Si"| EST2 --> SUMA2 --> J1 --> C2
    C2 -->|"No"| REP --> ESTAT --> FIN
```

> `Coleccion<Adoptante>::coleccionesCreadas` y
> `Coleccion<SolicitudAdopcion>::coleccionesCreadas` son **contadores independientes**:
> el compilador genera una clase distinta por cada tipo `T` y cada una tiene su propia
> copia del miembro estático.

---

## Opción 14 — Pruebas de integridad

Esta opción concentra las pruebas que pide la consigna: aritmética de punteros, operadores,
copia profunda, liberación de memoria y excepciones.

```mermaid
flowchart TD
    INI(["Opcion 14: Pruebas"])
    HAY{"Hay al menos un animal cargado?"}
    THROW[["throw DatoInvalidoException:<br/>cargue al menos un animal"]]
    P1[/"PRUEBA 1: recorrer el catalogo con un<br/>puntero que avanza de a una posicion<br/>hasta llegar al final del arreglo"/]
    P2[/"PRUEBA 2: usar los operadores<br/>corchetes, parentesis, igualdad y !"/]
    ANTES[/"Mostrar animales vivos ANTES de copiar"/]
    COPIA["Crear una copia del refugio:<br/>el constructor de copia clona cada animal"]
    DESP[/"Mostrar animales vivos DESPUES de copiar:<br/>se duplicaron"/]
    DIR[/"Mostrar las direcciones del animal 0<br/>en el original y en la copia"/]
    MOD["Modificar el animal 0 de la COPIA"]
    VER[/"Mostrar los dos: el original quedo intacto"/]
    DESTR["Fin del bloque: se destruye la copia<br/>y libera todos sus animales"]
    VUELVE[/"Mostrar animales vivos:<br/>volvio al valor original"/]
    E1["PRUEBA 4a: acceder a la posicion 999"]
    C1[/"Capturar IndiceInvalidoException"/]
    E2["PRUEBA 4b: buscar el ID 99999"]
    C2[/"Capturar NoEncontradoException"/]
    E3["PRUEBA 4c: alta con un ID ya existente"]
    C3[/"Capturar IdRepetidoException"/]
    CATCH[/"El catch del menu muestra el error"/]
    FIN(["Volver al menu"])

    INI --> HAY
    HAY -->|"No"| THROW --> CATCH --> FIN
    HAY -->|"Si"| P1 --> P2 --> ANTES --> COPIA --> DESP --> DIR --> MOD --> VER --> DESTR --> VUELVE
    VUELVE --> E1 --> C1 --> E2 --> C2 --> E3 --> C3 --> FIN
```

> Cada prueba de excepción tiene su **propio `try`** dentro de la función, para que una
> prueba no corte a la siguiente.

---

## Opción 0 — Salir del sistema

```mermaid
flowchart TD
    INI(["Opcion 0: Salir"])
    MSG[/"Mostrar: Cerrando el sistema"/]
    SALE["Salir del bucle do while"]
    ANTES[/"Mostrar animalesVivos<br/>ANTES de destruir el refugio"/]
    BLOQUE["Se cierra el bloque donde vive el Refugio"]
    DEST["Destructor del Refugio: delete de cada animal<br/>y liberacion del arreglo de punteros"]
    COL["Las colecciones liberan su memoria<br/>en su propio destructor"]
    DESPUES[/"Mostrar animalesVivos DESPUES:<br/>debe dar 0"/]
    FIN(["Fin del programa"])

    INI --> MSG --> SALE --> ANTES --> BLOQUE --> DEST --> COL --> DESPUES --> FIN
```

> Si el contador final da **0**, quedó demostrado que no hay fugas de memoria: todos los
> objetos creados con `new` fueron liberados.
