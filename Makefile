CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=ftpServer.c myLib.c
OBJECTS=$(SOURCES:.c=.o)		#sostituisce i file .c con file .o
EXECUTABLE=serverFTP
DEPS=commands.h

#penso linking
$(EXECUTABLE): $(OBJECTS) 			#prende ftpServer.o e commands.o		
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@	
# $@ The file name of the target of the rule -> in questo caso il target è EXECUTABLES che corrisponde a serverFTP -> sarebbe il nome che diamo all'eseguibile
#Questo comando linka tutti i file oggetto in un file esegiibile finale


#penso compilazione
%.o: %.c $(DEPS)		#tutti i file che finiscono in .o dipendono dai rispettivi file che finiscono in .c e dai file .h definiti in DEPS
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -f *.o serverFTP logs.txt

#make senza speciicare il comando esegue il primo della lista
# <nomeComando>: <dependencyFile>			--> la esegue se uno di questi due è cambiatos


#The -I. is included so that gcc will look in the current directory (.) for the include file hellomake.h. 


https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html#:~:text=If%20the%20target%20is%20an,rule's%20recipe%20to%20be%20run.