.DEFAULT_GOAL     := debug

CC                := gcc
AR                := ar
ARFLAGS           := rcs

CFLAGS            := -Werror -Wpedantic -std=gnu99
DEBUG_CFLAGS      := -O0 -g -DENN_DEBUG_MODE
RELEASE_CFLAGS    := -Ofast -fshort-enums -static -UENN_DEBUG_MODE

CORE_DIR          := ../Core
DEP_DIR           := ../Dep
APP_DIR           := ../App
OBJ_DIR           := Obj
LIB_DIR           := Lib

SEARCH_PATHS      := * */* */*/* */*/*/* */*/*/*/* */*/*/*/*/*

CORE_SRC          := $(wildcard $(addsuffix .c, $(addprefix $(CORE_DIR)/, $(SEARCH_PATHS))))
CORE_HEADERS      := $(wildcard $(addsuffix .h, $(addprefix $(CORE_DIR)/, $(SEARCH_PATHS))))
CORE_DIRS         := $(CORE_DIR) $(wildcard $(addsuffix /, $(addprefix $(CORE_DIR)/, $(SEARCH_PATHS))))

APP_SRC           := $(wildcard $(addsuffix .c, $(addprefix $(APP_DIR)/, $(SEARCH_PATHS))))
APP_HEADERS       := $(wildcard $(addsuffix .h, $(addprefix $(APP_DIR)/, $(SEARCH_PATHS))))

DEP_SRC           := $(wildcard $(addsuffix .c, $(addprefix $(DEP_DIR)/, $(SEARCH_PATHS))))
DEP_HEADERS       := $(wildcard $(addsuffix .h, $(addprefix $(DEP_DIR)/, $(SEARCH_PATHS))))
DEP_LIB           := $(wildcard $(addsuffix .a, $(addprefix $(DEP_DIR)/, $(SEARCH_PATHS))))

ALL_SRC	          := $(CORE_SRC) $(APP_SRC) $(DEP_SRC)
SOURCE_NAMES      := $(notdir $(ALL_SRC))
ifneq ($(words $(SOURCE_NAMES)), $(words $(sort $(SOURCE_NAMES))))
        $(error Source filenames must be unique to store object files without source directories)
endif

DEBUG_OBJ_DIR     := $(OBJ_DIR)/Debug
RELEASE_OBJ_DIR   := $(OBJ_DIR)/Release

DEBUG_CORE_OBJS   := $(addprefix $(DEBUG_OBJ_DIR)/, $(notdir $(CORE_SRC:.c=.o)))
DEBUG_APP_OBJS    := $(addprefix $(DEBUG_OBJ_DIR)/, $(notdir $(APP_SRC:.c=.o)))
DEBUG_DEP_OBJS    := $(addprefix $(DEBUG_OBJ_DIR)/, $(notdir $(DEP_SRC:.c=.o)))
RELEASE_CORE_OBJS := $(addprefix $(RELEASE_OBJ_DIR)/, $(notdir $(CORE_SRC:.c=.o)))
RELEASE_APP_OBJS  := $(addprefix $(RELEASE_OBJ_DIR)/, $(notdir $(APP_SRC:.c=.o)))
RELEASE_DEP_OBJS  := $(addprefix $(RELEASE_OBJ_DIR)/, $(notdir $(DEP_SRC:.c=.o)))

CORE_DEBUG_LIB    := $(LIB_DIR)/core_debug.a
CORE_RELEASE_LIB  := $(LIB_DIR)/core_release.a
INCLUDES          := -iquote $(DEP_DIR) -iquote $(CORE_DIR) -iquote $(APP_DIR)

ifeq ($(OS),Windows_NT)
	TARGET           := ../../main.exe
	PLATFORM_LIBS    := -lopengl32 -lgdi32 -lws2_32
	RELEASE_LDFLAGS  := -mwindows
else
	TARGET  := ../../main
	ifeq ($(shell uname -s),Darwin)
		PLATFORM_LIBS := -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
	else
		PLATFORM_LIBS := -lGL -lm -ldl -lpthread -lX11 -lXrandr -lXi -lXcursor -lXinerama
	endif
endif

VPATH   := $(sort $(dir $(ALL_SRC)))
.PHONY: debug release clean

debug: $(DEBUG_APP_OBJS) $(DEBUG_DEP_OBJS) $(CORE_DEBUG_LIB)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -o $(TARGET) $(DEBUG_APP_OBJS) $(DEBUG_DEP_OBJS) $(CORE_DEBUG_LIB) $(DEP_LIB) $(PLATFORM_LIBS)

release: $(RELEASE_APP_OBJS) $(RELEASE_DEP_OBJS) $(CORE_RELEASE_LIB)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) $(RELEASE_LDFLAGS) -o $(TARGET) $(RELEASE_APP_OBJS) $(RELEASE_DEP_OBJS) $(CORE_RELEASE_LIB) $(DEP_LIB) $(PLATFORM_LIBS)

$(CORE_DEBUG_LIB): $(DEBUG_CORE_OBJS) $(CORE_DIRS)
	@mkdir -p $(LIB_DIR)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(DEBUG_CORE_OBJS)

$(CORE_RELEASE_LIB): $(RELEASE_CORE_OBJS) $(CORE_DIRS)
	@mkdir -p $(LIB_DIR)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(RELEASE_CORE_OBJS)

$(DEBUG_CORE_OBJS) $(RELEASE_CORE_OBJS): $(CORE_HEADERS) $(DEP_HEADERS)
$(DEBUG_APP_OBJS) $(RELEASE_APP_OBJS): $(APP_HEADERS) $(CORE_HEADERS) $(DEP_HEADERS)
$(DEBUG_DEP_OBJS) $(RELEASE_DEP_OBJS): $(DEP_HEADERS)

$(DEBUG_OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $(INCLUDES) $< -o $@

$(RELEASE_OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(RELEASE_OBJ_DIR)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -c $(INCLUDES) $< -o $@

clean:
	$(RM) -r $(OBJ_DIR) $(LIB_DIR)
	$(RM) $(TARGET)
