TARGET :=
SRC_DIR := .
INC_DIR := .
OBJ_DIR := .
DEP_DIR := .

vpath %.h $(INC_DIR)

INCLUDES :=
CPPFLAGS += -MMD -MP # enables automatic dependency tracking
CFLAGS += -Wall -std=gnu11 -Os -DNDEBUG
LDFLAGS :=
LDLIBS :=
DEBUG_FLAGS := -g -Wextra -Werror -fsanitize=address -fno-omit-frame-pointer -O0
CC := gcc

COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.o, $^) $(LDFLAGS) $(LDLIBS) -o $@
COMPILE_NOLINK = $(CC) -c $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

.PHONY: all
all: $(TARGET)

.PHONY: dev
dev: CFLAGS := $(filter-out -Os -DNDEBUG, $(CFLAGS))
dev: CFLAGS += $(DEBUG_FLAGS)
dev: clean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	$(COMPILE_NOLINK)


# Empty pattern rule to match dependency (*.d) files (i.e. makefiles),
# so make won't fail if dependency doesn't exist
%.d: ;

# Mark dependency files as precious so they won't be deleted as intermediates
.PRECIOUS: %.d

# The list of all source files I want to track dependencies for
SOURCES=$(wildcard $(SRC_DIR)/*.c)
# Dependency files of each source file
DEPENDS=$(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SOURCES))

# Include any dependency files that exist, and
# suppress error message for ones that don't yet (via hyphen)
-include $(DEPENDS)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(DEP_DIR)/*.d *.dSYM $(TARGET)
