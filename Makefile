CC = cc

CFLAGS = -03 -ffast-math -Wall -Wextra
LIBS = -lXft -lX11 -lXcursor -lXft -lfontconfig -lXcomposite
FREETYPEINC = /usr/include/freetype2
INCS = -I${FREETYPEINC}

SRC = wm.cpp
OBJ = ${SRC:.c=.o}

all: neonwm print_options

print_options:
	@echo neonwm build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS = ${LIBS}"
	@echo "INCS = ${INCS}"
	@echo "CC = ${CC}"

.cpp.o:
	${CC} -c ${CFLAGS} ${LIBS} ${INCS} $<

# ${OBJ}: /core/types/definitions.hpp

neonwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LIBS} ${INCS}

install:
	cp -f neonwm /usr/bin
	cp -f neonwm.desktop /usr/share/xsessions

clean:
	rm -f neonwm ${OBJ}

uninstall:
	rm -f /usr/bin/neonwm
	rm -f /usr/share/xsessions/neonwm.desktop

.PHONY: all print_options clean install uninstall freetype