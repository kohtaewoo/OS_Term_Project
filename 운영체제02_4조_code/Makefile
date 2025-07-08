# Compiler and flags
CC = gcc
CFLAGS = -fcommon -D _GNU_SOURCE
LDFLAGS = -lpthread -lncurses
RM = rm -rf

# Project name and directories
NAME = LINUXSHELL
SRC_DIR = source
OBJ_DIR = object
INC_DIR = include

# Source and object files
SRCS = main.c data_structures.c filesystem_management.c cat.c cd.c tar.c thread.c \
       chown.c system.c find.c mkdir.c rm.c ls.c permission.c pwd.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Build targets
all: check_ncurses $(NAME) art_animation

# Main program build
$(NAME): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) -o $@ $(OBJS) -I$(INC_DIR) $(LDFLAGS)

# Art animation program build
art_animation: $(OBJ_DIR)/art_animation.o
	@echo "Building art animation..."
	$(CC) $(CFLAGS) -o $@ $< -lncurses

# Object files build
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)

# Check for ncurses installation
# check_ncurses:
# 	@if ! ldconfig -p | grep -q ncurses; then \
# 		echo "ncurses not found, installing..."; \
# 		sudo apt-get update; \
# 		sudo apt-get install -y libncurses5-dev libncursesw5-dev; \
# 	else \
# 		echo "ncurses is installed"; \
# 	fi

# Clean up
clean:
	@echo "Cleaning..."
	$(RM) $(OBJ_DIR)/*.o

fclean: clean
	@echo "Full clean..."
	$(RM) $(NAME) art_animation

# Rebuild
re: fclean all

.PHONY: all clean fclean re check_ncurses

