# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 18:16:41 by yoshin            #+#    #+#              #
#    Updated: 2025/07/14 11:26:22 by yoshin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile v1.0

NAME			=	libft

CFLAGS			=	-Wall -Wextra -Werror
ARFLAGS			=	rcs

HEADER_DIR		=	includes
SRC_DIR			=	srcs
BUILD_DIR		=	build
BIN_DIR			=	bin

SRCS			:=	$(shell find ./$(SRC_DIR) -name '*.c')
OBJS			:=	$(SRCS:%=$(BUILD_DIR)/%.o)

# **************************************************************************** #
#    - Debug Flag                                                              #
# **************************************************************************** #

ifeq ($(debug), 1)
	override CFLAGS	+=	-g -DDEBUG
endif

# **************************************************************************** #
#    - Color Codes                                                             #
# **************************************************************************** #

RED			=	'\e[31m'
GREEN		=	'\e[32m'
BLUE		=	'\e[34m'
RESET		=	'\e[0m'

.PHONY: all clean fclean re bonus dev
all: $(NAME).a

$(NAME).a: $(OBJS) | $(BIN_DIR)
	@mkdir -p $(dir $@)
	@echo -e $(RED)compile $(BIN_DIR)/$@.a ...$(RESET) 
	@$(AR) $(ARFLAGS) $(BIN_DIR)/$@ $^
	@echo $(GREEN)compile $(BIN_DIR)/$@ completed !$(RESET) 
	@cp $(BIN_DIR)/$(NAME).a $(NAME).a

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo -e $(RED)compile $< ...$(RESET) 
	@$(CC) $(CFLAGS) -o $@ -I/usr/include -Imlx_linux -O3 -c $< -I$(HEADER_DIR)
	@echo $(GREEN)compile $< completed !$(RESET) 

$(BIN_DIR):
	mkdir -p $@

clean: 
	rm -rf $(BIN_DIR) $(BUILD_DIR)

fclean: clean
	rm -rf $(NAME).a $(HEADERS).gch

re: fclean all

dev: fclean
	make debug=1 all
