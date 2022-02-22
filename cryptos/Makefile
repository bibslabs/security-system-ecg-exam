MKDIR   := md
RMDIR   := rd /S /Q
CC      := g++ -O2
BIN     := ./bin
OBJ     := ./obj
INCLUDE := ./include	-I./libs/simdjson/include	-I./libs/base64/include
SRC     := ./src ./libs/simdjson/src ./libs/base64/src
SRCS    := $(foreach d, $(SRC), $(wildcard $(d)/*.cpp))
OBJS    := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
EXE     := $(BIN)/main
CFLAGS  := -I$(INCLUDE) -O2 -Wall -Wextra -Werror -c
LDLIBS  := -lm -lstdc++

#$(foreach d, $(INC), -I$d)
#$(foreach d, $(SRCS), $(wildcard $(d)/*.cpp))

.PHONY: all run clean

all: $(EXE)


$(EXE):	$(OBJS)	|	$(BIN)
	$(CC) $(LDFLAGS) -I$(INCLUDE) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o:	$(SRC)/%.cpp	|	$(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)	$(OBJ):
	$(MKDIR) $@

run: $(EXE)
	$<

clean:
	$(RMDIR) $(OBJ) $(BIN)