CC = gcc
CFlags = -Wall -lpthread

SourcesDir = src/
IncludesDir = include/
ObjectsDir = obj/
BinDir = bin/
TmpDir = tmp/

ServerDir = server/
ServerSourcesDir = $(addprefix $(ServerDir), $(SourcesDir))
ServerObjectsDir = $(addprefix $(ServerDir), $(ObjectsDir))
ServerTmpDir = $(addprefix $(ServerDir), $(TmpDir))
ServerSources = server.c controller.c communicator.c
ServerObjects = $(ServerSources:.c=.o)
ServerCObjects = $(addprefix $(ServerObjectsDir), $(ServerObjects))
ServerExecutable = server
ServerCExecutable = $(addprefix $(BinDir), $(ServerExecutable))

ClientDir = client/
ClientSourcesDir = $(addprefix $(ClientDir), $(SourcesDir))
ClientObjectsDir = $(addprefix $(ClientDir), $(ObjectsDir))
ClientTmpDir = $(addprefix $(ClientDir), $(TmpDir))
ClientSources = client.c
ClientObjects = $(ClientSources:.c=.o)
ClientCObjects = $(addprefix $(ClientObjectsDir), $(ClientObjects))
ClientExecutable = client
ClientCExecutable = $(addprefix $(BinDir), $(ClientExecutable))

all: $(ServerExecutable) $(ClientExecutable)

$(ServerExecutable): $(ServerCExecutable) .server_tmp_dir .tmp_dir

$(ServerCExecutable): $(ServerCObjects) .bin_dir
	$(CC) $(CFlags) $(ServerCObjects) -o $@

$(ServerObjectsDir)%.o: $(ServerSourcesDir)%.c .server_object_dir
	$(CC) $(CFlags) $< -c -o $@

.server_tmp_dir:
	mkdir -p $(ServerTmpDir)

.server_object_dir:
	mkdir -p $(ServerObjectsDir)

$(ClientExecutable): $(ClientCExecutable) .client_tmp_dir .tmp_dir

$(ClientCExecutable): $(ClientCObjects) .bin_dir
	$(CC) $(CFlags) $(ClientCObjects) -o $@

$(ClientObjectsDir)%.o: $(ClientSourcesDir)%.c .client_object_dir
	$(CC) $(CFlags) $< -c -o $@

.client_tmp_dir:
	mkdir -p $(ClientTmpDir)

.client_object_dir:
	mkdir -p $(ClientObjectsDir)

.tmp_dir:
	mkdir -p $(TmpDir)

.bin_dir:
	mkdir -p $(BinDir)

clean:
	rm -rf $(BinDir) $(ServerObjectsDir) $(ClientObjectsDir) $(TmpDir) $(ServerTmpDir) $(ClientTmpDir)
