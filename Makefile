CC = gcc
CFlags = -c -Wall

SourcesDir = src/
IncludesDir = include/
ObjectsDir = tmp/
BinDir = bin/

ServerDir = server/
ServerSources = server.c
ServerObjects = $(ServerSources:.c=.o)
ServerExecutable = server
_ServerCSources = $(addprefix $(SourcesDir), $(ServerSources))
ServerCSources = $(addprefix $(ServerDir), $(_ServerCSources))
ServerCObjects = $(addprefix $(ObjectsDir), $(ServerObjects))
ServerCExecutable = $(addprefix $(BinDir), $(ServerExecutable))

ClientDir = client/
ClientSources = client.c
ClientObjects = $(ClientSources:.c=.o)
ClientExecutable = client
_ClientCSources = $(addprefix $(SourcesDir), $(ClientSources))
ClientCSources = $(addprefix $(ClientDir), $(_ClientCSources))
ClientCObjects = $(addprefix $(ObjectsDir), $(ClientObjects))
ClientCExecutable = $(addprefix $(BinDir), $(ClientExecutable))

all: $(ServerExecutable) $(ClientExecutable)

$(ServerExecutable): $(ServerCExecutable)

$(ServerCExecutable): $(ServerCObjects) .bin_dir
	$(CC) $(ServerCObjects) -o $@

$(ServerCObjects): $(ServerCSources) .object_dir
	$(CC) $(CFlags) $< -o $@

$(ClientExecutable): $(ClientCExecutable)

$(ClientCExecutable): $(ClientCObjects) .bin_dir
	$(CC) $(ClientCObjects) -o $@

$(ClientCObjects): $(ClientCSources) .object_dir
	$(CC) $(CFlags) $< -o $@

.bin_dir:
	mkdir -p $(BinDir)

.object_dir:
	mkdir -p $(ObjectsDir)

clean:
	rm -rf $(BinDir) $(ObjectsDir)
