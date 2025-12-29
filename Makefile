BIN := dooz
ODIN_FLAGS += -thread-count:$(shell nproc) -out:$(BIN) -debug

ifeq ($(release), 1)
	ODIN_FLAGS += -o:speed
else
	ODIN_FLAGS += -o:minimal
endif

.PHONY: all build run clean

all: $(BIN)

$(BIN): build

build:
	odin build . $(ODIN_FLAGS)

run:
	odin run . $(ODIN_FLAGS)

clean:
	rm -f $(BIN)
