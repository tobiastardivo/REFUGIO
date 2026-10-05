Algoritmo SistemaAdopcionRefugio
	// ===================================================================
	// TRABAJO PRACTICO INTEGRADOR - Programacion Orientada a Objetos 2026
	// Sistema de Gestion de un Refugio de Animales
	//
	// Pseudocodigo del menu principal y del proceso de adopcion (Fase 1).
	// Refleja el menu final del programa en C++ (14 opciones).
	//
	// EL USUARIO NO ESCRIBE NINGUN ID. Los numeros existen (son la clave
	// de cada animal, adoptante y solicitud), pero los pone el sistema:
	//   - en las altas, con ProximoIdAnimal y ProximoIdAdoptante;
	//   - para elegir algo que ya existe, con las funciones Elegir...,
	//     que muestran la lista numerada de 1 a N y traducen el numero
	//     de lista al ID interno.
	//
	// Las funciones que acceden a los datos estan simuladas: en el
	// programa real son metodos de la clase Refugio, que administra el
	// catalogo con memoria dinamica.
	// ===================================================================
	Definir opcion Como Entero
	// --- Bucle principal: el menu sigue activo hasta que se elige Salir ---
	Repetir
		Limpiar Pantalla
		Escribir '============================================'
		Escribir '   SISTEMA DE GESTION DE REFUGIO - 2026     '
		Escribir '============================================'
		Escribir ' 1. Registrar perro'
		Escribir ' 2. Registrar gato'
		Escribir ' 3. Listar todos los animales'
		Escribir ' 4. Listar animales disponibles'
		Escribir ' 5. Registrar adoptante'
		Escribir ' 6. Ver ficha de un animal'
		Escribir ' 7. Crear solicitud de adopcion'
		Escribir ' 8. Confirmar solicitud'
		Escribir ' 9. Cancelar solicitud'
		Escribir '10. Devolver animal al refugio'
		Escribir '11. Ver adoptantes y solicitudes'
		Escribir '12. Historial de adopciones'
		Escribir '13. Informes y estadisticas'
		Escribir '14. Pruebas (copia profunda, operadores y excepciones)'
		Escribir ' 0. Salir'
		Escribir '--------------------------------------------'
		// Lectura validada entre 0 y 14
		Repetir
			Escribir 'Ingrese una opcion (0-14):'
			Leer opcion
			Si opcion<0 O opcion>14 Entonces
				Escribir 'Error: El valor debe estar entre 0 y 14.'
			FinSi
		Hasta Que opcion>=0 Y opcion<=14
		Escribir ''
		// Seleccion de la operacion.
		// En C++ todo este bloque va dentro de un try: si una operacion
		// lanza una excepcion se muestra el mensaje y el menu sigue vivo.
		Según opcion Hacer
			1:
				RegistrarPerro()
			2:
				RegistrarGato()
			3:
				ListarAnimales(Falso)
			4:
				ListarAnimales(Verdadero)
			5:
				RegistrarAdoptante()
			6:
				VerFichaDeAnimal()
			7:
				CrearSolicitudDeAdopcion()
			8:
				ConfirmarSolicitud()
			9:
				CancelarSolicitud()
			10:
				DevolverAnimal()
			11:
				VerAdoptantesYSolicitudes()
			12:
				HistorialDeAdopciones()
			13:
				InformesYEstadisticas()
			14:
				PruebasDeIntegridad()
			0:
				Escribir 'Cerrando el sistema...'
		FinSegún
		Si opcion<>0 Entonces
			Escribir ''
			Escribir 'Presione ENTER para continuar...'
			Esperar Tecla
		FinSi
	Hasta Que opcion=0
	// --- Cierre: se destruye el refugio y se libera toda la memoria ---
	Escribir ''
	Escribir 'Animales en memoria ANTES de destruir el refugio: ', ContarAnimales
	LiberarMemoriaDelRefugio()
	Escribir 'Animales en memoria DESPUES de destruir el refugio: 0'
	Escribir 'Memoria liberada correctamente. Hasta luego.'
FinAlgoritmo

// ===================================================================
// OPCIONES 1 Y 2 - ALTAS DE ANIMALES
// El numero de cada animal lo pone el sistema, no el usuario.
// ===================================================================

Función RegistrarPerro
	Definir id, edad, t Como Entero
	Definir nombre, salud, raza, tamanio Como Cadena
	Escribir '--- Registrar perro ---'
	// El ID no se pide por teclado: es el proximo numero libre del
	// refugio. En C++ esta linea es  refugio.nuevoIdAnimal().
	id <- ProximoIdAnimal
	Escribir 'Nombre:'
	Leer nombre
	edad <- LeerEnteroValidado('Edad (anios):',0,30)
	Escribir 'Estado de salud:'
	Leer salud
	Escribir 'Raza:'
	Leer raza
	Escribir 'Tamanio -> 1) Pequenio  2) Mediano  3) Grande'
	t <- LeerEnteroValidado('Opcion:',1,3)
	Si t=1 Entonces
		tamanio <- 'Pequenio'
	SiNo
		Si t=2 Entonces
			tamanio <- 'Mediano'
		SiNo
			tamanio <- 'Grande'
		FinSi
	FinSi
	// En C++ el objeto se crea con new y la memoria pasa a ser del
	// Refugio. Ya no hay que controlar el ID repetido antes del alta:
	// al ponerlo el sistema nunca puede chocar con uno que ya exista
	// (el control sigue estando dentro de agregarAnimal, como red).
	AgregarAnimal(id,nombre,'Perro')
	Escribir 'Perro registrado correctamente (quedo con el numero ', id, ').'
FinFunción

Función RegistrarGato
	Definir id, edad Como Entero
	Definir nombre, salud, color, interior Como Cadena
	Escribir '--- Registrar gato ---'
	id <- ProximoIdAnimal
	Escribir 'Nombre:'
	Leer nombre
	edad <- LeerEnteroValidado('Edad (anios):',0,30)
	Escribir 'Estado de salud:'
	Leer salud
	Escribir 'Color de pelaje:'
	Leer color
	Escribir 'Vive en interior? (SI/NO):'
	Leer interior
	AgregarAnimal(id,nombre,'Gato')
	Escribir 'Gato registrado correctamente (quedo con el numero ', id, ').'
FinFunción

// ===================================================================
// OPCIONES 3 Y 4 - LISTADOS
// ===================================================================

// En C++ este recorrido es POLIMORFICO: se llama siempre a mostrarInfo()
// y la ligadura dinamica ejecuta la version de Perro o la de Gato.
Función ListarAnimales(soloDisponibles)
	Definir i, cant, mostrados Como Entero
	Si soloDisponibles Entonces
		Escribir '--- Animales DISPONIBLES ---'
	SiNo
		Escribir '--- Todos los animales del refugio ---'
	FinSi
	cant <- ContarAnimales
	mostrados <- 0
	Si cant=0 Entonces
		Escribir '(El refugio no tiene animales cargados)'
	SiNo
		Para i<-0 Hasta cant-1 Con Paso 1 Hacer
			// Se saltea el animal que no cumple el filtro
			Si NO (soloDisponibles Y ObtenerEstadoAnimal(i)<>'DISPONIBLE') Entonces
				MostrarAnimal(i)
				mostrados <- mostrados+1
			FinSi
		FinPara
		Si mostrados=0 Entonces
			Escribir '(No hay animales que cumplan el filtro)'
		SiNo
			Escribir 'Total listado: ', mostrados, ' animal(es).'
		FinSi
	FinSi
FinFunción

// ===================================================================
// OPCION 5 - ALTA DE ADOPTANTE
// ===================================================================

Función RegistrarAdoptante
	Definir id Como Entero
	Definir nombre, contacto, vivienda Como Cadena
	Escribir '--- Registrar adoptante ---'
	// Igual que en las altas de animales: el numero lo pone el sistema.
	// En C++ es  refugio.nuevoIdAdoptante().
	id <- ProximoIdAdoptante
	Escribir 'Nombre y apellido:'
	Leer nombre
	Escribir 'Contacto (telefono o mail):'
	Leer contacto
	Escribir 'Tipo de vivienda:'
	Leer vivienda
	AgregarAdoptante(id,nombre)
	Escribir 'Adoptante registrado correctamente (quedo con el numero ', id, ').'
FinFunción

// ===================================================================
// OPCION 6 - FICHA DE UN ANIMAL
// ===================================================================

// En C++ usa dos operadores sobrecargados: () para buscar por ID
// y el operador unario ! para saber si el animal NO esta disponible.
// El ID que recibe el operador () sale de la lista, no del teclado.
Función VerFichaDeAnimal
	Definir id, idx Como Entero
	Escribir '--- Ver ficha de un animal ---'
	id <- ElegirAnimal('Animales del refugio:','TODOS')
	Si id<>0 Entonces
		idx <- BuscarIndiceAnimal(id)
		Escribir ''
		Escribir 'Ficha del animal elegido:'
		MostrarAnimal(idx)
		Si ObtenerEstadoAnimal(idx)<>'DISPONIBLE' Entonces
			Escribir 'Atencion: este animal NO esta disponible para adopcion.'
		SiNo
			Escribir 'Este animal esta disponible para adopcion.'
		FinSi
	FinSi
FinFunción

// ===================================================================
// OPCION 7 - PROCESO DE ADOPCION (proceso central del sistema)
// ===================================================================

Función CrearSolicitudDeAdopcion
	Definir idAdoptante, idAnimal, idxAnimal, nro Como Entero
	Definir deseaConfirmar Como Cadena
	Escribir '--- Nueva solicitud de adopcion ---'
	// Las funciones Elegir... avisan solas cuando no hay nada para
	// elegir y en ese caso devuelven 0. Por eso aca ya no hacen falta
	// las verificaciones previas ni los controles de "existe el ID",
	// que es lo que antes hacia larga esta operacion.
	idAdoptante <- ElegirAdoptante('Quien quiere adoptar?')
	Si idAdoptante<>0 Entonces
		Escribir ''
		idAnimal <- ElegirAnimal('A que animal?','DISPONIBLES')
		Si idAnimal<>0 Entonces
			idxAnimal <- BuscarIndiceAnimal(idAnimal)
			// --- Alta de la solicitud y reserva del animal ---
			nro <- CrearSolicitud(idAdoptante,idAnimal)
			CambiarEstadoAnimal(idxAnimal,'EN_PROCESO')
			Escribir ''
			Escribir 'Solicitud N ', nro, ' creada en estado PENDIENTE.'
			Escribir 'El animal quedo reservado (EN_PROCESO).'
			// --- Confirmacion inmediata (opcional) ---
			Escribir 'Desea confirmar la adopcion ahora? (SI/NO):'
			Leer deseaConfirmar
			Si Mayusculas(deseaConfirmar)='SI' O Mayusculas(deseaConfirmar)='S' Entonces
				ActualizarEstadoSolicitud(nro,'CONFIRMADA')
				CambiarEstadoAnimal(idxAnimal,'ADOPTADO')
				Escribir 'Adopcion confirmada. El animal pasa a ADOPTADO.'
			SiNo
				Escribir 'La solicitud queda PENDIENTE y el animal sigue reservado.'
			FinSi
		FinSi
	FinSi
FinFunción

// ===================================================================
// OPCIONES 8 Y 9 - CONFIRMAR Y CANCELAR SOLICITUDES
// Las dos eligen de la lista de solicitudes PENDIENTES, que son las
// unicas que se pueden confirmar o cancelar.
// ===================================================================

Función ConfirmarSolicitud
	Definir nro, idx, idxAnimal Como Entero
	Escribir '--- Confirmar solicitud ---'
	nro <- ElegirSolicitudPendiente('Solicitudes pendientes:')
	Si nro<>0 Entonces
		idx <- BuscarIndiceSolicitud(nro)
		idxAnimal <- BuscarIndiceAnimal(ObtenerAnimalDeSolicitud(idx))
		ActualizarEstadoSolicitud(nro,'CONFIRMADA')
		CambiarEstadoAnimal(idxAnimal,'ADOPTADO')
		Escribir ''
		Escribir 'Solicitud CONFIRMADA. El animal quedo ADOPTADO.'
	FinSi
FinFunción

Función CancelarSolicitud
	Definir nro, idx, idxAnimal Como Entero
	Escribir '--- Cancelar solicitud ---'
	nro <- ElegirSolicitudPendiente('Solicitudes pendientes:')
	Si nro<>0 Entonces
		idx <- BuscarIndiceSolicitud(nro)
		idxAnimal <- BuscarIndiceAnimal(ObtenerAnimalDeSolicitud(idx))
		ActualizarEstadoSolicitud(nro,'CANCELADA')
		// Al cancelarse el tramite el animal vuelve a estar disponible
		CambiarEstadoAnimal(idxAnimal,'DISPONIBLE')
		Escribir ''
		Escribir 'Solicitud CANCELADA. El animal vuelve a estar DISPONIBLE.'
	FinSi
FinFunción

// ===================================================================
// OPCION 10 - DEVOLUCION DE UN ANIMAL
// ===================================================================

Función DevolverAnimal
	Definir id, idx, i, cantSol Como Entero
	Escribir '--- Devolver animal al refugio ---'
	// Solo se ofrecen los que estan reservados o adoptados: de los que
	// figuran DISPONIBLE no hay nada que devolver.
	id <- ElegirAnimal('Animales reservados o adoptados:','NO_DISPONIBLES')
	Si id<>0 Entonces
		idx <- BuscarIndiceAnimal(id)
		// Si tenia una solicitud pendiente se cancela para no dejarla colgada
		cantSol <- ContarSolicitudes
		Para i<-0 Hasta cantSol-1 Con Paso 1 Hacer
			Si ObtenerAnimalDeSolicitud(i)=id Y ObtenerEstadoSolicitud(i)='PENDIENTE' Entonces
				ActualizarEstadoSolicitud(ObtenerNumeroDeSolicitud(i),'CANCELADA')
			FinSi
		FinPara
		CambiarEstadoAnimal(idx,'DISPONIBLE')
		Escribir ''
		Escribir 'El animal volvio al estado DISPONIBLE.'
	FinSi
FinFunción

// ===================================================================
// OPCIONES 11, 12 Y 13 - CONSULTAS E INFORMES
// ===================================================================

// En C++ las dos listas se muestran con la MISMA funcion generica
// mostrarColeccion<T>(), instanciada con Adoptante y con SolicitudAdopcion.
Función VerAdoptantesYSolicitudes
	Escribir '--- Adoptantes registrados ---'
	MostrarAdoptantes()
	Escribir ''
	Escribir '--- Solicitudes de adopcion ---'
	MostrarSolicitudes()
FinFunción

Función HistorialDeAdopciones
	Definir i, cantSol, cont Como Entero
	Escribir '--- Historial de adopciones concretadas ---'
	cantSol <- ContarSolicitudes
	cont <- 0
	Para i<-0 Hasta cantSol-1 Con Paso 1 Hacer
		Si ObtenerEstadoSolicitud(i)='CONFIRMADA' Entonces
			MostrarSolicitud(i)
			cont <- cont+1
		FinSi
	FinPara
	Si cont=0 Entonces
		Escribir '(Todavia no hay adopciones concretadas)'
	SiNo
		Escribir 'Total de adopciones: ', cont
	FinSi
FinFunción

Función InformesYEstadisticas
	Definir i, cant, disponibles, enProceso, adoptados Como Entero
	Definir estadoActual Como Cadena
	Escribir '--- Informes y estadisticas ---'
	cant <- ContarAnimales
	disponibles <- 0
	enProceso <- 0
	adoptados <- 0
	Para i<-0 Hasta cant-1 Con Paso 1 Hacer
		estadoActual <- ObtenerEstadoAnimal(i)
		Si estadoActual='DISPONIBLE' Entonces
			disponibles <- disponibles+1
		SiNo
			Si estadoActual='EN_PROCESO' Entonces
				enProceso <- enProceso+1
			SiNo
				adoptados <- adoptados+1
			FinSi
		FinSi
	FinPara
	Escribir 'Animales: ', cant
	Escribir '  Disponibles: ', disponibles, ' | En proceso: ', enProceso, ' | Adoptados: ', adoptados
	Escribir 'Adoptantes registrados: ', ContarAdoptantes
	Escribir 'Solicitudes: ', ContarSolicitudes
	// En C++ aca se muestran ademas los MIEMBROS ESTATICOS:
	// Animal::animalesVivos y Coleccion<T>::coleccionesCreadas, que son
	// contadores independientes para cada instanciacion de la plantilla.
	Escribir '[Miembros estaticos] Animales vivos en memoria: ', ContarAnimales
FinFunción

// ===================================================================
// OPCION 14 - PRUEBAS DE INTEGRIDAD
// ===================================================================

Función PruebasDeIntegridad
	Definir vivosAntes Como Entero
	Si ContarAnimales=0 Entonces
		Escribir 'Error: cargue al menos un animal antes de probar.'
	SiNo
		Escribir 'PRUEBA 1: recorrido del catalogo con aritmetica de punteros.'
		ListarAnimales(Falso)
		Escribir 'PRUEBA 2: operadores [] , () , == y ! sobre el catalogo.'
		Escribir 'PRUEBA 3: copia profunda y liberacion de memoria.'
		vivosAntes <- ContarAnimales
		Escribir '  Animales vivos ANTES de copiar: ', vivosAntes
		Escribir '  Animales vivos DESPUES de copiar: ', vivosAntes*2, ' (se duplicaron)'
		Escribir '  Las direcciones son DISTINTAS: la copia es profunda.'
		Escribir '  Se modifica la copia y el original queda intacto.'
		Escribir '  Animales vivos DESPUES de destruir la copia: ', vivosAntes
		Escribir 'PRUEBA 4: excepciones controladas.'
		Escribir '  a) Indice invalido  -> IndiceInvalidoException capturada.'
		Escribir '  b) ID inexistente   -> NoEncontradoException capturada.'
		Escribir '  c) ID repetido      -> IdRepetidoException capturada.'
		Escribir 'Fin de las pruebas. El menu sigue funcionando normalmente.'
	FinSi
FinFunción

// ===================================================================
// SELECCION POR LISTA
// Son las funciones que hacen que el usuario no tenga que escribir
// ningun ID. Todas trabajan igual: muestran la lista numerada de 1 a N,
// leen UN numero de esa lista y devuelven el ID interno del elemento
// elegido. Si no hay nada para elegir avisan el motivo y devuelven 0.
// En C++ son MenuConsola::elegirAnimal(), elegirAdoptante() y
// elegirSolicitudPendiente().
// ===================================================================

Función id <- ElegirAnimal(titulo,filtro)
	Definir i, cant, enLista, elegido Como Entero
	Dimension idsDeLaLista[20]
	cant <- ContarAnimales
	enLista <- 0
	Escribir titulo
	// Primera pasada: mostrar la lista y guardar a que ID corresponde
	// cada numero, porque el numero de lista NO es la posicion en el
	// catalogo cuando hay un filtro aplicado.
	Para i<-0 Hasta cant-1 Con Paso 1 Hacer
		Si PasaElFiltro(i,filtro) Entonces
			enLista <- enLista+1
			idsDeLaLista[enLista] <- IdDelAnimal(i)
			Escribir '  ', enLista, ') ', NombreDelAnimal(i), ' - ', ObtenerEstadoAnimal(i)
		FinSi
	FinPara
	Si enLista=0 Entonces
		Si filtro='DISPONIBLES' Entonces
			Escribir 'Error: no hay animales disponibles para adoptar.'
		SiNo
			Si filtro='NO_DISPONIBLES' Entonces
				Escribir 'Error: no hay animales reservados ni adoptados: no hay nada para devolver.'
			SiNo
				Escribir 'Error: el refugio no tiene animales cargados (opciones 1 y 2).'
			FinSi
		FinSi
		id <- 0
	SiNo
		elegido <- LeerEnteroValidado('Elija un numero de la lista:',1,enLista)
		id <- idsDeLaLista[elegido]
	FinSi
FinFunción

Función id <- ElegirAdoptante(titulo)
	Definir i, cant, elegido Como Entero
	cant <- ContarAdoptantes
	Si cant=0 Entonces
		Escribir 'Error: no hay adoptantes registrados (opcion 5).'
		id <- 0
	SiNo
		Escribir titulo
		// Aca no hay filtro, asi que el numero de la lista coincide
		// siempre con la posicion: alcanza con restarle 1.
		Para i<-0 Hasta cant-1 Con Paso 1 Hacer
			Escribir '  ', i+1, ') ', NombreDelAdoptante(i)
		FinPara
		elegido <- LeerEnteroValidado('Elija un numero de la lista:',1,cant)
		id <- IdDelAdoptante(elegido-1)
	FinSi
FinFunción

Función nro <- ElegirSolicitudPendiente(titulo)
	Definir i, cant, enLista, elegido Como Entero
	Dimension nrosDeLaLista[20]
	cant <- ContarSolicitudes
	enLista <- 0
	Escribir titulo
	Para i<-0 Hasta cant-1 Con Paso 1 Hacer
		Si ObtenerEstadoSolicitud(i)='PENDIENTE' Entonces
			enLista <- enLista+1
			nrosDeLaLista[enLista] <- ObtenerNumeroDeSolicitud(i)
			MostrarSolicitudEnLista(i,enLista)
		FinSi
	FinPara
	Si enLista=0 Entonces
		Escribir 'Error: no hay solicitudes pendientes (opcion 7).'
		nro <- 0
	SiNo
		elegido <- LeerEnteroValidado('Elija un numero de la lista:',1,enLista)
		nro <- nrosDeLaLista[elegido]
	FinSi
FinFunción

// Un renglon de la lista de solicitudes, escrito en castellano: en vez de
// los dos IDs se muestran el nombre del adoptante y el del animal.
Función MostrarSolicitudEnLista(idx,numeroDeLista)
	Definir idxAnimal Como Entero
	idxAnimal <- BuscarIndiceAnimal(ObtenerAnimalDeSolicitud(idx))
	Escribir '  ', numeroDeLista, ') Solicitud N ', ObtenerNumeroDeSolicitud(idx), ' | ', NombreDelAdoptanteDeSolicitud(idx), ' quiere adoptar a ', NombreDelAnimal(idxAnimal)
FinFunción

// ===================================================================
// FUNCION AUXILIAR DE LECTURA VALIDADA
// Todas las opciones piden los numeros con esta funcion: no se sale
// de aca hasta que el valor sea correcto y este dentro del rango.
// ===================================================================

Función valor <- LeerEnteroValidado(mensaje,minimo,maximo)
	Repetir
		Escribir mensaje
		Leer valor
		Si valor<minimo O valor>maximo Entonces
			Escribir 'Error: el valor debe estar entre ', minimo, ' y ', maximo, '.'
		FinSi
	Hasta Que valor>=minimo Y valor<=maximo
FinFunción

// ===================================================================
// FUNCIONES DE ACCESO A LOS DATOS (SIMULADAS)
// En el programa en C++ son metodos de la clase Refugio, que guarda el
// catalogo en un arreglo dinamico de punteros a Animal.
// ===================================================================

Función cant <- ContarAnimales
	cant <- 4
FinFunción

Función cant <- ContarAnimalesDisponibles
	cant <- 3
FinFunción

Función cant <- ContarAdoptantes
	cant <- 2
FinFunción

Función cant <- ContarSolicitudes
	cant <- 1
FinFunción

// --- Numeradores automaticos ---
// Guardan cual es el proximo numero libre. En C++ son los atributos
// proximoIdAnimal y proximoIdAdoptante del Refugio, que arrancan en 1 y
// en 100 y se corren solos en cada alta.
Función id <- ProximoIdAnimal
	// Hay 4 animales cargados (IDs 1 a 4), asi que el proximo es el 5
	id <- ContarAnimales+1
FinFunción

Función id <- ProximoIdAdoptante
	// Los adoptantes arrancan en 100 para no confundirlos con un animal
	id <- 100+ContarAdoptantes
FinFunción

// --- Filtro de los listados de animales ---
// TODOS | DISPONIBLES | NO_DISPONIBLES
Función pasa <- PasaElFiltro(idx,filtro)
	Si filtro='DISPONIBLES' Entonces
		pasa <- ObtenerEstadoAnimal(idx)='DISPONIBLE'
	SiNo
		Si filtro='NO_DISPONIBLES' Entonces
			pasa <- ObtenerEstadoAnimal(idx)<>'DISPONIBLE'
		SiNo
			pasa <- Verdadero
		FinSi
	FinSi
FinFunción

Función idAn <- IdDelAnimal(idx)
	// Los 4 animales de prueba tienen los IDs 1 a 4
	idAn <- idx+1
FinFunción

Función nom <- NombreDelAnimal(idx)
	// Los nombres de los animales que carga cargarDatosDePrueba
	Si idx=0 Entonces
		nom <- 'Rocky (Perro)'
	SiNo
		Si idx=1 Entonces
			nom <- 'Luna (Perro)'
		SiNo
			Si idx=2 Entonces
				nom <- 'Michi (Gato)'
			SiNo
				Si idx=3 Entonces
					nom <- 'Nube (Gato)'
				SiNo
					nom <- '(animal dado de baja)'
				FinSi
			FinSi
		FinSi
	FinSi
FinFunción

Función idAd <- IdDelAdoptante(idx)
	// Los 2 adoptantes de prueba tienen los IDs 100 y 101
	idAd <- 100+idx
FinFunción

Función nom <- NombreDelAdoptante(idx)
	Si idx=0 Entonces
		nom <- 'Ana Perez'
	SiNo
		nom <- 'Juan Gomez'
	FinSi
FinFunción

Función nom <- NombreDelAdoptanteDeSolicitud(idx)
	// Traduce el ID guardado en la solicitud al nombre de la persona
	nom <- NombreDelAdoptante(BuscarIndiceAdoptante(ObtenerAdoptanteDeSolicitud(idx)))
FinFunción

Función idx <- BuscarIndiceAnimal(idAn)
	// Simula los 4 animales que carga cargarDatosDePrueba (IDs 1 a 4).
	// Devuelve -1 cuando el ID no existe en el catalogo.
	Si idAn>=1 Y idAn<=4 Entonces
		idx <- idAn-1
	SiNo
		idx <- -1
	FinSi
FinFunción

Función idx <- BuscarIndiceAdoptante(idAd)
	// Simula los 2 adoptantes de prueba (IDs 100 y 101)
	Si idAd=100 Entonces
		idx <- 0
	SiNo
		Si idAd=101 Entonces
			idx <- 1
		SiNo
			idx <- -1
		FinSi
	FinSi
FinFunción

Función idx <- BuscarIndiceSolicitud(nro)
	// Simula una unica solicitud cargada, la numero 1
	Si nro=1 Entonces
		idx <- 0
	SiNo
		idx <- -1
	FinSi
FinFunción

Función est <- ObtenerEstadoAnimal(idx)
	// DISPONIBLE | EN_PROCESO | ADOPTADO
	// Se simula el ultimo animal reservado, asi se pueden probar tanto
	// la adopcion (opcion 7) como la devolucion (opcion 10).
	Si idx=3 Entonces
		est <- 'EN_PROCESO'
	SiNo
		est <- 'DISPONIBLE'
	FinSi
FinFunción

Función est <- ObtenerEstadoSolicitud(idx)
	// PENDIENTE | CONFIRMADA | CANCELADA
	est <- 'PENDIENTE'
FinFunción

Función idAn <- ObtenerAnimalDeSolicitud(idx)
	idAn <- 4
FinFunción

Función idAd <- ObtenerAdoptanteDeSolicitud(idx)
	idAd <- 100
FinFunción

Función nro <- ObtenerNumeroDeSolicitud(idx)
	nro <- 1
FinFunción

Función nro <- CrearSolicitud(idAd,idAn)
	Escribir '[Info]: Solicitud creada para el adoptante ', idAd, ' y el animal ', idAn, '.'
	nro <- 1
FinFunción

Función AgregarAnimal(id,nombre,tipo)
	// En C++: refugio.agregarAnimal(new Perro(...)) -> la memoria pasa al Refugio
	Escribir '[Info]: ', tipo, ' ', nombre, ' agregado al catalogo con ID ', id, '.'
FinFunción

Función AgregarAdoptante(id,nombre)
	Escribir '[Info]: Adoptante ', nombre, ' agregado con ID ', id, '.'
FinFunción

Función CambiarEstadoAnimal(idx,nuevoEstado)
	Escribir '[Info]: Estado del animal modificado a ', nuevoEstado, '.'
FinFunción

Función ActualizarEstadoSolicitud(nro,nuevoEstado)
	Escribir '[Info]: Solicitud N ', nro, ' modificada a ', nuevoEstado, '.'
FinFunción

Función MostrarAnimal(idx)
	Escribir '  ', NombreDelAnimal(idx), ' - ', ObtenerEstadoAnimal(idx), ' [posicion ', idx, ' del catalogo]'
FinFunción

Función MostrarSolicitud(idx)
	Escribir '  [Solicitud en la posicion ', idx, ' de la coleccion]'
FinFunción

Función MostrarAdoptantes
	Escribir '  [Listado de adoptantes con la funcion generica]'
FinFunción

Función MostrarSolicitudes
	Escribir '  [Listado de solicitudes con la funcion generica]'
FinFunción

Función LiberarMemoriaDelRefugio
	// En C++ lo hace el destructor ~Refugio(): delete de cada animal
	// y despues delete[] del arreglo de punteros.
	Escribir '[Info]: Se destruye el refugio y se libera toda la memoria.'
FinFunción
