Algoritmo SistemaAdopcionRefugio
	// ===================================================================
	// TRABAJO PRACTICO INTEGRADOR - Programacion Orientada a Objetos 2026
	// Sistema de Gestion de un Refugio de Animales
	//
	// Pseudocodigo del menu principal y del proceso de adopcion (Fase 1).
	// Refleja el menu final del programa en C++ (14 opciones).
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
		Escribir ' 6. Buscar animal por ID'
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
				BuscarAnimalPorId()
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
// ===================================================================

Función RegistrarPerro
	Definir id, edad, t Como Entero
	Definir nombre, salud, raza, tamanio Como Cadena
	Escribir '--- Registrar perro ---'
	id <- LeerEnteroValidado('ID:',1,99999)
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
	// En C++ el objeto se crea con new y la memoria pasa a ser del Refugio
	Si BuscarIndiceAnimal(id)<>-1 Entonces
		// El Refugio libera el objeto antes de lanzar IdRepetidoException
		Escribir 'Error: El ID ', id, ' ya esta registrado en el sistema.'
	SiNo
		AgregarAnimal(id,nombre,'Perro')
		Escribir 'Perro registrado correctamente.'
	FinSi
FinFunción

Función RegistrarGato
	Definir id, edad Como Entero
	Definir nombre, salud, color, interior Como Cadena
	Escribir '--- Registrar gato ---'
	id <- LeerEnteroValidado('ID:',1,99999)
	Escribir 'Nombre:'
	Leer nombre
	edad <- LeerEnteroValidado('Edad (anios):',0,30)
	Escribir 'Estado de salud:'
	Leer salud
	Escribir 'Color de pelaje:'
	Leer color
	Escribir 'Vive en interior? (SI/NO):'
	Leer interior
	Si BuscarIndiceAnimal(id)<>-1 Entonces
		Escribir 'Error: El ID ', id, ' ya esta registrado en el sistema.'
	SiNo
		AgregarAnimal(id,nombre,'Gato')
		Escribir 'Gato registrado correctamente.'
	FinSi
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
	id <- LeerEnteroValidado('ID:',1,99999)
	Escribir 'Nombre y apellido:'
	Leer nombre
	Escribir 'Contacto (telefono o mail):'
	Leer contacto
	Escribir 'Tipo de vivienda:'
	Leer vivienda
	Si BuscarIndiceAdoptante(id)<>-1 Entonces
		Escribir 'Error: El ID ', id, ' ya esta registrado en el sistema.'
	SiNo
		AgregarAdoptante(id,nombre)
		Escribir 'Adoptante registrado correctamente.'
	FinSi
FinFunción

// ===================================================================
// OPCION 6 - BUSQUEDA POR ID
// ===================================================================

// En C++ usa dos operadores sobrecargados: () para buscar por ID
// y el operador unario ! para saber si el animal NO esta disponible.
Función BuscarAnimalPorId
	Definir id, idx Como Entero
	Escribir '--- Buscar animal por ID ---'
	id <- LeerEnteroValidado('ID a buscar:',1,99999)
	idx <- BuscarIndiceAnimal(id)
	Si idx=-1 Entonces
		Escribir 'Error: No existe un animal con ID ', id, '.'
	SiNo
		Escribir 'Animal encontrado:'
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
	Definir idAdoptante, idAnimal Como Entero
	Definir idxAdoptante, idxAnimal, nro Como Entero
	Definir estadoAnimal, deseaConfirmar Como Cadena
	Escribir '--- Nueva solicitud de adopcion ---'
	// --- Verificaciones previas ---
	Si ContarAnimalesDisponibles=0 Entonces
		Escribir 'Error: no hay animales disponibles en este momento.'
	SiNo
		Si ContarAdoptantes=0 Entonces
			Escribir 'Error: no hay adoptantes registrados (opcion 5).'
		SiNo
			// --- Datos de la solicitud ---
			idAdoptante <- LeerEnteroValidado('ID del adoptante:',1,99999)
			idAnimal <- LeerEnteroValidado('ID del animal:',1,99999)
			idxAdoptante <- BuscarIndiceAdoptante(idAdoptante)
			idxAnimal <- BuscarIndiceAnimal(idAnimal)
			Si idxAdoptante=-1 Entonces
				Escribir 'Error: No existe un adoptante con ID ', idAdoptante, '.'
			SiNo
				Si idxAnimal=-1 Entonces
					Escribir 'Error: No existe un animal con ID ', idAnimal, '.'
				SiNo
					estadoAnimal <- ObtenerEstadoAnimal(idxAnimal)
					Si estadoAnimal<>'DISPONIBLE' Entonces
						Escribir 'Error: El animal con ID ', idAnimal, ' no esta disponible (', estadoAnimal, ').'
					SiNo
						// --- Alta de la solicitud y reserva del animal ---
						nro <- CrearSolicitud(idAdoptante,idAnimal)
						CambiarEstadoAnimal(idxAnimal,'EN_PROCESO')
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
			FinSi
		FinSi
	FinSi
FinFunción

// ===================================================================
// OPCIONES 8 Y 9 - CONFIRMAR Y CANCELAR SOLICITUDES
// ===================================================================

Función ConfirmarSolicitud
	Definir nro, idx, idxAnimal Como Entero
	Escribir '--- Confirmar solicitud ---'
	MostrarSolicitudes()
	nro <- LeerEnteroValidado('N de solicitud a confirmar:',1,99999)
	idx <- BuscarIndiceSolicitud(nro)
	Si idx=-1 Entonces
		Escribir 'Error: No existe una solicitud con N ', nro, '.'
	SiNo
		Si ObtenerEstadoSolicitud(idx)<>'PENDIENTE' Entonces
			Escribir 'Error: la solicitud N ', nro, ' ya fue ', ObtenerEstadoSolicitud(idx), '.'
		SiNo
			idxAnimal <- BuscarIndiceAnimal(ObtenerAnimalDeSolicitud(idx))
			ActualizarEstadoSolicitud(nro,'CONFIRMADA')
			CambiarEstadoAnimal(idxAnimal,'ADOPTADO')
			Escribir 'Solicitud CONFIRMADA. El animal quedo ADOPTADO.'
		FinSi
	FinSi
FinFunción

Función CancelarSolicitud
	Definir nro, idx, idxAnimal Como Entero
	Escribir '--- Cancelar solicitud ---'
	MostrarSolicitudes()
	nro <- LeerEnteroValidado('N de solicitud a cancelar:',1,99999)
	idx <- BuscarIndiceSolicitud(nro)
	Si idx=-1 Entonces
		Escribir 'Error: No existe una solicitud con N ', nro, '.'
	SiNo
		Si ObtenerEstadoSolicitud(idx)<>'PENDIENTE' Entonces
			Escribir 'Error: la solicitud N ', nro, ' ya fue ', ObtenerEstadoSolicitud(idx), '.'
		SiNo
			idxAnimal <- BuscarIndiceAnimal(ObtenerAnimalDeSolicitud(idx))
			ActualizarEstadoSolicitud(nro,'CANCELADA')
			// Al cancelarse el tramite el animal vuelve a estar disponible
			CambiarEstadoAnimal(idxAnimal,'DISPONIBLE')
			Escribir 'Solicitud CANCELADA. El animal vuelve a estar DISPONIBLE.'
		FinSi
	FinSi
FinFunción

// ===================================================================
// OPCION 10 - DEVOLUCION DE UN ANIMAL
// ===================================================================

Función DevolverAnimal
	Definir id, idx, i, cantSol Como Entero
	Escribir '--- Devolver animal al refugio ---'
	id <- LeerEnteroValidado('ID del animal:',1,99999)
	idx <- BuscarIndiceAnimal(id)
	Si idx=-1 Entonces
		Escribir 'Error: No existe un animal con ID ', id, '.'
	SiNo
		Si ObtenerEstadoAnimal(idx)='DISPONIBLE' Entonces
			Escribir 'Error: el animal con ID ', id, ' ya figura como DISPONIBLE.'
		SiNo
			// Si tenia una solicitud pendiente se cancela para no dejarla colgada
			cantSol <- ContarSolicitudes
			Para i<-0 Hasta cantSol-1 Con Paso 1 Hacer
				Si ObtenerAnimalDeSolicitud(i)=id Y ObtenerEstadoSolicitud(i)='PENDIENTE' Entonces
					ActualizarEstadoSolicitud(ObtenerNumeroDeSolicitud(i),'CANCELADA')
				FinSi
			FinPara
			CambiarEstadoAnimal(idx,'DISPONIBLE')
			Escribir 'El animal volvio al estado DISPONIBLE.'
		FinSi
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
	cant <- 2
FinFunción

Función cant <- ContarAdoptantes
	cant <- 2
FinFunción

Función cant <- ContarSolicitudes
	cant <- 1
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
	est <- 'DISPONIBLE'
FinFunción

Función est <- ObtenerEstadoSolicitud(idx)
	// PENDIENTE | CONFIRMADA | CANCELADA
	est <- 'PENDIENTE'
FinFunción

Función idAn <- ObtenerAnimalDeSolicitud(idx)
	idAn <- 1
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
	Escribir '  [Animal en la posicion ', idx, ' del catalogo]'
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
