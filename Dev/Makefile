
$(info $(SHELL))

build: build_debug

CC = gcc
CFLAGS = -Werror -Wpedantic -std=gnu99

CORE_DIR = Core
DEP_DIR  = Dep
APP_DIR  = App

CORE_SRC = $(wildcard $(CORE_DIR)/*.c $(CORE_DIR)/*/*.c $(CORE_DIR)/*/*/*.c $(CORE_DIR)/*/*/*.c)
CORE_INC = $(CORE_DIR)

APP_SRC = $(wildcard $(APP_DIR)/*.c $(APP_DIR)/*/*.c $(APP_DIR)/*/*/*.c)
APP_INC = $(APP_DIR)

DEP_SRC = $(wildcard $(DEP_DIR)/*.c $(DEP_DIR)/*/*.c $(DEP_DIR)/*/*/*.c)
DEP_INC = $(DEP_DIR)
DEP_LIB = $(wildcard $(DEP_DIR)/*.a $(DEP_DIR)/*/*.a)

ifeq ($(OS), Windows_NT)
	DEP_LIB += -lopengl32
	DEP_LIB += -lgdi32
	DEP_LIB += -lws2_32
endif


TARGET = ../main.exe

CORE_OBJS = $(CORE_SRC:.c=.o)
%.o: %.c
	$(CC) $(CFLAGS) -c -iquote $(DEP_INC) -iquote $(CORE_INC) $< -o $@

CORE_LIB = $(CORE_DIR)/core.a
AR = ar
ARFLAGS = rcs
$(CORE_LIB): $(CORE_OBJS)
	$(AR) $(ARFLAGS) $(CORE_LIB) $^


ALL_SRC = $(DEP_SRC) $(APP_SRC)
ALL_INC = -iquote $(DEP_INC) -iquote $(CORE_INC) -iquote $(APP_INC)
# ALL_LIB = $(CORE_LIB) $(DEP_LIB)

build_debug: $(CORE_LIB)
	$(CC) $(CFLAGS) -O0 -static -o $(TARGET) $(ALL_INC) $(ALL_SRC) $(CORE_LIB) $(DEP_LIB)

build_release: $(CORE_LIB)
	$(CC) $(CFLAGS) -O3 -static -mwindows -o $(TARGET) $(ALL_INC) $(ALL_SRC) $(CORE_LIB) $(DEP_LIB)

build_preproc:
	$(CC) $(CFLAGS) -E $(ALL_INC) $(ALL_SRC) $(ALL_LIB) > preproc.c

clean: $(CORE_LIB) $(CORE_OBJS)
	rm $^