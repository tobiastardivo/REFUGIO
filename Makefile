# Makefile del Trabajo Practico Integrador - POO 2026
# Sistema de Gestion de un Refugio de Animales
#
# Uso:
#   make          -> compila y genera refugio.exe
#   make clean    -> borra los .o y el ejecutable
#
# Si g++ no esta en el PATH (caso tipico en Windows con el MinGW de CLion),
# se puede indicar asi:
#   make CXX="C:/Program Files/JetBrains/CLion 2026.1/bin/mingw/bin/g++.exe"

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
OBJS     = main.o MenuConsola.o Animal.o Perro.o Gato.o Adoptante.o SolicitudAdopcion.o Refugio.o Utilidades.o
TARGET   = refugio.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Dependencias de cada unidad de compilacion respecto de los .h
main.o:              main.cpp MenuConsola.h
MenuConsola.o:       MenuConsola.cpp MenuConsola.h Refugio.h Perro.h Gato.h Adoptante.h Utilidades.h Excepciones.h
Animal.o:            Animal.cpp Animal.h
Perro.o:             Perro.cpp Perro.h Animal.h
Gato.o:              Gato.cpp Gato.h Animal.h
Adoptante.o:         Adoptante.cpp Adoptante.h
SolicitudAdopcion.o: SolicitudAdopcion.cpp SolicitudAdopcion.h
Refugio.o:           Refugio.cpp Refugio.h Animal.h Perro.h Gato.h Coleccion.h Excepciones.h Utilidades.h
Utilidades.o:        Utilidades.cpp Utilidades.h Coleccion.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
