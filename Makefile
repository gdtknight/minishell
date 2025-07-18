# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 18:16:41 by yoshin            #+#    #+#              #
#    Updated: 2025/07/18 16:06:11 by yoshin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ***************** #
#    Color Codes    #
# ***************** #

BLACK			=	'\033[38;5;0m'
MAROON			=	'\033[38;5;1m'
GREEN			=	'\033[38;5;2m'
CYAN_BLUE		=	'\033[38;5;69m'
LIGHT_GREEN		=	'\033[38;5;46m'
CYAN			=	'\033[38;5;81m'
OLIVE			=	'\033[38;5;3m'
NAVY			=	'\033[38;5;4m'
PURPLE			=	'\033[38;5;5m'
TEAL			=	'\033[38;5;6m'
SILVER			=	'\033[38;5;7m'
GREY			=	'\033[38;5;8m'
RED				=	'\033[38;5;9m'
LINE			=	'\033[38;5;10m'
YELLOW			=	'\033[38;5;11m'
LIGHT_YELLOW	=	'\033[38;5;226m'
BLUE			=	'\033[38;5;12m'
FUCHSIA			=	'\033[38;5;13m'
AQUA			=	'\033[38;5;14m'
WHITE			=	'\033[38;5;15m'
ORANGE			=	'\033[38;5;208m'
RESET			=	'\033[0m'

# Makefile v1.0

CC				=	gcc

NAME			=	minishell

HEADER_DIR		=	includes
SRC_DIR			=	srcs

BUILD_DIR		=	build
BIN_DIR			=	bin

SRCS			:=	main.c
OBJS 			:=	$(patsubst ./$(SRC_DIR)/%.c, ./$(BUILD_DIR)/%.c.o, $(addprefix ./$(SRC_DIR)/, $(SRCS)))

LIB_NAMES		=	ft
LIBS			=	$(addprefix lib, $(LIB_NAMES))
LIB_ACHIEVES	=	$(addsuffix .a, $(LIBS))

CFLAGS			=	-Wall -Wextra -Werror

# **************** #
#    Debug Flag    #
# **************** #

ifeq ($(debug), 1)
	override CFLAGS	+=	--debug -g -DDEBUG
	LIB_DEBUG = debug=1
endif

.PHONY: all clean fclean re bonus debug
all: $(NAME)

$(NAME): $(OBJS) | $(BIN_DIR) $(LIB_ACHIEVES)
	@echo "$(CYAN_BLUE)[$(NAME)] $(RED)compile $(BIN_DIR)/$@ ...$(RESET)"
	@$(PREFIX) $(CC) $(CFLAGS) -o ./$(BIN_DIR)/$@ $^ \
		$(foreach lib, $(LIB_NAMES), -L./lib$(lib) -l$(lib))
	@echo "$(CYAN_BLUE)[$(NAME)] $(ORANGE)$(BIN_DIR)/$@ compiled!$(RESET)"
	@cp $(BIN_DIR)/$@ $@
	@echo "$(CYAN_BLUE)[$(NAME)] $(ORANGE)$(BIN_DIR)/$@ copied!$(RESET)"

$(BIN_DIR):
	@mkdir -p ./$@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN_BLUE)[$(NAME)] $(RED)compile $< ...$(RESET)"
	@$(PREFIX) $(CC) $(CFLAGS) -o $@ -c $< -I./$(HEADER_DIR) \
		$(foreach lib, $(LIB_NAMES), -I./lib$(lib)/$(HEADER_DIR))
	@echo "$(CYAN_BLUE)[$(NAME)] $(GREEN)$< compiled!$(RESET)"

$(LIB_ACHIEVES): %.a:
	@for lib in $(LIBS); do \
		if [ ! -f ./$$lib/$$lib.a ]; then \
			echo "$(LIGHT_YELLOW)[$$lib]$(RED)not exists. execute make for $(LIGHT_YELLOW)$$lib$(RESET)"; \
			$(MAKE) $(LIB_DEBUG) -C ./$$lib; \
		else \
			echo "$(LIGHT_YELLOW)[$$lib]$(GREEN)exists, skipping make.$(RESET)"; \
		fi \
	done

clean:
	@echo "$(CYAN_BLUE)[$(NAME)]$(RESET) clean"
	@rm -rf $(BUILD_DIR)

fclean: clean
	@echo "$(CYAN_BLUE)[$(NAME)]$(RESET) fclean"
	@rm -rf $(NAME) $(BIN_DIR)
	@for lib in $(LIBS); do		\
		if [ -f ./$$lib/$$lib.a ]; then \
			echo "$(LIGHT_YELLOW)[$$lib]$(RESET) exists. execute fclean."; \
			make -C ./$$lib fclean; \
		else \
			echo "$(LIGHT_YELLOW)[$$lib]$(RESET) not exists, skipping fclean."; \
		fi \
	done
	@echo "$(ORANGE)All binaries and libraries has been deleted.$(RESET)"

re: fclean all

bonus:

dev: fclean
	@make debug=1 all
	@make debug=1 bonus
