# **************************************************************************** #
#                                🏛️ PHILOSOPHERS 🏛️                            #
# **************************************************************************** #

# 🔹 Nom de l'exécutable
NAME = philo

# ⚙️ Compilateur et options
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP

# 📁 Dossiers
SRC_DIR = src
OBJ_DIR = obj
DEP_DIR = dep
INCLUDE_DIR = include

# 📄 Fichiers source
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/init_and_parse.c $(SRC_DIR)/threads.c \
	  $(SRC_DIR)/activity.c $(SRC_DIR)/utils_lib.c $(SRC_DIR)/threads_utils.c \
	  $(SRC_DIR)/utils.c

# 🗃️ Fichiers objets et dépendances
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP = $(SRC:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

# 📚 Fichiers headers
HEADERS = $(INCLUDE_DIR)/philosophers.h

# 🎯 Règles principales
all: $(NAME)
	@echo "\033[32m✅ Compilation réussie : $(NAME)\033[0m"

$(NAME): $(OBJ) $(HEADERS)
	@echo "\033[34m🔨 Création de l'exécutable $(NAME)...\033[0m"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

# 🛠️ Compilation des fichiers source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ -MF $(DEP_DIR)/$*.d

# 🧹 Nettoyage
clean:
	@echo "\033[33m🗑️ Suppression des fichiers objets...\033[0m"
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	@echo "\033[31m🗑️ Suppression de l'exécutable...\033[0m"
	rm -f $(NAME)

re: fclean all

# 📎 Inclusion des dépendances
-include $(DEP)

.PHONY: all clean fclean re
