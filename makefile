# Dossier pour la compilation

DEST_SRC = SRC
DEST_INC = INC
DEST_OBJ = OBJ

# Flags de compilation

CC = gcc
CFLAGS = -W -Wall
ADDFLAGS = -lglut -lGLU -lm

SRC = $(foreach dir, $(DEST_SRC), $(wildcard $(dir)/*.c))
OBJ = $(addsuffix .o, $(basename $(subst ${DEST_SRC}, ${DEST_OBJ}, ${SRC})))

#nom de l'executable
BIN = beta

# Make

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o ${BIN} $(OBJ) $(ADDFLAGS)

$(DEST_OBJ)/%.o: $(DEST_SRC)/%.c
	$(CC) $(CFLAGS) -I$(DEST_INC) -c $< -o $@  $(ADDFLAGS)


# Clean

clean:
	@rm -f $(DEST_OBJ)/*.o
	@rm -f $(BIN)
	@find . -name \*~ -exec rm \-fv {} \;
	@echo  "\t"Le programme $(BIN) a bien été supprimé
