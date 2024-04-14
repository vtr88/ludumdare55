SOURCES = feam.c screen_ending.c screen_gameplay.c screen_logo.c screen_title.c
NATIVE_OBJS = $(SOURCES:.c=.o)
WEB_OBJS = $(SOURCES:.c=.web.o)

CC = gcc
EMCC = emcc

INCLUDES = -I. -I../../raylib/src -I../../raylib/src/external -I../../raylib/src/extras

COMPILER_FLAGS = -w -std=c99 -g $(INCLUDES) -I/usr/local/include
EM_COMPILER_FLAGS = -std=c99 -Wall -Wno-missing-braces -Wunused-result -D_DEFAULT_SOURCE -Os $(INCLUDES) -DPLATFORM_WEB

LINKER_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
EM_LINKER_FLAGS = -L../../raylib/src -s USE_GLFW=3 -s TOTAL_MEMORY=134217728 -s FORCE_FILESYSTEM=1 --preload-file resources --shell-file ./resources/minshell.html ../../raylib/src/libraylib.a -s ASYNCIFY

OBJ_NAME = feam
WEB_OBJ_NAME = feam.html

.PHONY: all clean native web

all: native web

native: $(NATIVE_OBJS)
	$(CC) $(NATIVE_OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	rm -f $(NATIVE_OBJS) $(WEB_OBJS) 

web: $(WEB_OBJS)
	$(EMCC) $(WEB_OBJS) $(EM_LINKER_FLAGS) -o $(WEB_OBJ_NAME)

%.o: %.c
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

%.web.o: %.c
	$(EMCC) $(EM_COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(NATIVE_OBJS) $(WEB_OBJS) $(OBJ_NAME) $(WEB_OBJ_NAME) $(WEB_OBJ_NAME:.html=.wasm) $(WEB_OBJ_NAME:.html=.js) $(WEB_OBJ_NAME:.html=.data)
