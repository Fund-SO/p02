TENNIS1_SRC	=	./task1
TENNIS1_FILES	=	${TENNIS1_SRC}/tennis1.c ${TENNIS1_SRC}/th_paleta_ordenador.c ${TENNIS1_SRC}/th_paleta_usuario.c ${TENNIS1_SRC}/th_pelota.c
TENNIS1_HEADER = ${TENNIS1_SRC}/tennis1.h

TENNIS2_SRC	=	./task2
TENNIS2_FILES	=	${TENNIS2_SRC}/tennis2.c ${TENNIS2_SRC}/th_paleta_ordenador.c ${TENNIS2_SRC}/th_paleta_usuario.c ${TENNIS2_SRC}/th_pelota.c ${TENNIS2_SRC}/th_reloj.c
TENNIS2_HEADER = ${TENNIS2_SRC}/tennis2.h

TENNIS3_SRC	=	./task3
TENNIS3_FILES	=	${TENNIS3_SRC}/tennis3.c ${TENNIS3_SRC}/th_paleta_usuario.c ${TENNIS3_SRC}/th_pelota.c ${TENNIS3_SRC}/th_reloj.c
TENNIS3_HEADER = ${TENNIS3_SRC}/tennis3.h

TENNIS4_SRC	=	./task4
TENNIS4_FILES	=	${TENNIS4_SRC}/tennis4.c ${TENNIS4_SRC}/th_paleta_usuario.c ${TENNIS4_SRC}/th_pelota.c ${TENNIS4_SRC}/th_reloj.c
TENNIS4_HEADER = ${TENNIS4_SRC}/tennis4.h

all : winsuport.o winsuport2.o semafor.o memoria.o tennis0 tennis1 tennis2 tennis3

winsuport.o : winsuport.c winsuport.h
	gcc -Wall -c winsuport.c -o winsuport.o

winsuport2.o : winsuport2.c winsuport2.h
	gcc -Wall -c winsuport2.c -o winsuport2.o

semafor.o : semafor.c semafor.h
	gcc -Wall -c semafor.c -o semafor.o

missatge.o : missatge.c missatge.h
	gcc -Wall -c missatge.c -o missatge.o

memoria.o : memoria.c memoria.h
	gcc -Wall -c memoria.c -o memoria.o

tennis0 : task0/tennis0.c winsuport.o winsuport.h
	gcc -Wall task0/tennis0.c winsuport.o -I. -lcurses -o tennis0

tennis1 : ${TENNIS1_FILES} winsuport.o winsuport.h ${TENNIS1_HEADER}
	gcc -Wall ${TENNIS1_FILES} winsuport.o -I. -I${TENNIS1_SRC} -lcurses -lpthread -o tennis1

tennis2 : ${TENNIS2_FILES} winsuport.o winsuport.h ${TENNIS2_HEADER}
	gcc -Wall ${TENNIS2_FILES} winsuport.o -I. -I${TENNIS2_SRC} -lcurses -lpthread -o tennis2

tennis3: ${TENNIS3_FILES} ./task3/pal_ord3.c winsuport2.o winsuport2.h semafor.o semafor.h memoria.o memoria.h ${TENNIS3_HEADER}
	gcc -Wall -Wextra -Werror winsuport2.o semafor.o memoria.o task3/pal_ord3.c -I. -I${TENNIS3_SRC} -o pal_ord3 -lcurses
	gcc -Wall ${TENNIS3_FILES} winsuport2.o semafor.o memoria.o -I. -I${TENNIS3_SRC} -lcurses -o tennis3

tennis4: ${TENNIS4_FILES} ./task4/pal_ord4.c winsuport2.o winsuport2.h semafor.o semafor.h memoria.o memoria.h ${TENNIS4_HEADER}
	gcc -Wall -Wextra -Werror winsuport2.o semafor.o memoria.o task4/pal_ord4.c -I. -I${TENNIS4_SRC} -o pal_ord4 -lcurses
	gcc -Wall ${TENNIS4_FILES} winsuport2.o semafor.o memoria.o -I. -I${TENNIS4_SRC} -lcurses -o tennis3

remove: clean all

clean: 
	rm winsuport.o semafor.o memoria.o winsuport2.o tennis0 tennis1 tennis2 tennis3 tennis4