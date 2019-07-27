TARGET    =$(OBJECTS)
SRC_DIR   :=./src
INC_DIR   :=./include
TEST_DIR  :=./test
OBJ_DIR   :=./obj
BUILD_DIR :=./bin
SOURCES   :=$(wildcard $(SRC_DIR)/*.c)
TEST_SRC  :=$(wildcard $(TEST_DIR)/*_tests.c)
OBJECTS   :=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TESTS     :=$(patsubst %.c,%,$(TEST_SRC))
DEPENDS   :=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SOURCES)) \
			$(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.d,$(TEST_SRC))

INCLUDES  :=$(addprefix -I, $(INC_DIR))
# enables automatic dependency tracking:
CPPFLAGS += -MMD -MP
CFLAGS   += -Wall -Wextra -std=gnu11 -Os -DNDEBUG
LDFLAGS  :=
LDLIBS   :=
DEBUG_FLAGS := -g -Werror -fsanitize=address -fno-omit-frame-pointer -O0
CC := gcc

.PHONY: all
all: $(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(LINK.c) $(INCLUDES) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR)/%.d | $(BUILD_DIR) $(OBJ_DIR)
	$(COMPILE.c) $(INCLUDES) $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	@mkdir -p $(BUILD_DIR) $(OBJ_DIR)

.PHONY: dev
dev: CFLAGS := $(filter-out -Os -DNDEBUG, $(CFLAGS))
dev: CFLAGS += $(DEBUG_FLAGS)
dev: clean all

####################
# Unit Tests
####################
$(TESTS): $(TEST_DIR)/% : $(TEST_DIR)/%.c $(OBJECTS)
	$(LINK.c) $(INCLUDES) $^ $(LDLIBS) -o $@

.PHONY: test
test: CFLAGS := $(filter-out -Os -DNDEBUG, $(CFLAGS))
test: CFLAGS += $(DEBUG_FLAGS)
test: clean all $(TESTS)
	cd $(TEST_DIR) && sh runtests.sh

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR) $(OBJ_DIR) $(TESTS)
	$(RM) -r $(TEST_DIR)/*.d $(TEST_DIR)/*.log $(TEST_DIR)/*.dSYM


#########################################
# Dependency Tracking
#########################################

# Empty pattern rule to match dependency (*.d) files (i.e. makefiles),
# so make won't fail if dependency doesn't exist
%.d: ;

# Mark dependency files as precious so they won't be deleted as intermediates
.PRECIOUS: %.d

# Include any dependency files that exist, and
# suppress error message for ones that don't yet (via hyphen)
-include $(DEPENDS)
