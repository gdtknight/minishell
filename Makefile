# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 18:16:41 by yoshin            #+#    #+#              #
#    Updated: 2025/08/30 10:29:13 by yoshin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ***************** #
#    Color Codes    #
# ***************** #

BLACK				=	'\033[38;5;0m'
MAROON				=	'\033[38;5;1m'
GREEN				=	'\033[38;5;2m'
CYAN_BLUE			=	'\033[38;5;69m'
LIGHT_GREEN			=	'\033[38;5;46m'
CYAN				=	'\033[38;5;81m'
OLIVE				=	'\033[38;5;3m'
NAVY				=	'\033[38;5;4m'
PURPLE				=	'\033[38;5;5m'
TEAL				=	'\033[38;5;6m'
SILVER				=	'\033[38;5;7m'
GREY				=	'\033[38;5;8m'
RED					=	'\033[38;5;9m'
LINE				=	'\033[38;5;10m'
YELLOW				=	'\033[38;5;11m'
LIGHT_YELLOW		=	'\033[38;5;226m'
BLUE				=	'\033[38;5;12m'
FUCHSIA				=	'\033[38;5;13m'
AQUA				=	'\033[38;5;14m'
WHITE				=	'\033[38;5;15m'
ORANGE				=	'\033[38;5;208m'
RESET				=	'\033[0m'

# Makefile v1.0

CC					=	gcc

NAME				=	minishell

HEADER_DIR			=	includes
SRC_DIR				=	srcs
TEST_DIR			=	tests
TEST_BIN_DIR		=	$(TEST_DIR)/bin
TEST_BUILD_DIR		=	$(TEST_DIR)/build

BUILD_DIR			=	build
BIN_DIR				=	bin

# SRCS				:=	$(shell find ./$(SRC_DIR) -name '*.c')
SRCS				:=	./srcs/hashmap/hashmap_put.c \
						./srcs/hashmap/hashmap_utils.c \
						./srcs/hashmap/hashmap_entry.c \
						./srcs/hashmap/hashmap_get.c \
						./srcs/utils/ft_multiplejoin.c \
						./srcs/utils/pair_validator.c \
						./srcs/utils/env_utils.c \
						./srcs/utils/get_next_line_utils_bonus.c \
						./srcs/utils/char_utils.c \
						./srcs/utils/get_next_line_bonus.c \
						./srcs/utils/ft_lltoa.c \
						./srcs/evaluation/eval_pipeline.c \
						./srcs/evaluation/eval_heredoc.c \
						./srcs/evaluation/eval_heredoc_utils.c \
						./srcs/evaluation/eval_prefix_assign.c \
						./srcs/evaluation/eval.c \
						./srcs/evaluation/eval_utils.c \
						./srcs/evaluation/eval_io_redir_utils.c \
						./srcs/evaluation/eval_io_redir.c \
						./srcs/evaluation/eval_command.c \
						./srcs/evaluation/eval_command_utils.c \
						./srcs/builtin/builtin_cd.c \
						./srcs/builtin/builtin_exit.c \
						./srcs/builtin/builtin_cd_utils.c \
						./srcs/builtin/builtin_export.c \
						./srcs/builtin/builtin_exit_utils.c \
						./srcs/builtin/builtin_echo.c \
						./srcs/builtin/builtin_env.c \
						./srcs/builtin/builtin_unset.c \
						./srcs/builtin/builtin_pwd.c \
						./srcs/expand/expand_token.c \
						./srcs/expand/split_field.c \
						./srcs/expand/expand_tilde_utils.c \
						./srcs/expand/expand_tilde.c \
						./srcs/expand/expand_utils.c \
						./srcs/expand/expand_heredoc.c \
						./srcs/expand/expand_param.c \
						./srcs/expand/expand_param_utils.c \
						./srcs/expand/expand_common.c \
						./srcs/expand/expand_heredoc_utils.c \
						./srcs/expand/expand_token_create.c \
						./srcs/expand/masking_utils.c \
						./srcs/tokenize/token_list.c \
						./srcs/tokenize/token_printer.c \
						./srcs/tokenize/token_validatation.c \
						./srcs/tokenize/tokenizer.c \
						./srcs/tokenize/tokenizer_value.c \
						./srcs/tokenize/tokenizer_utils.c \
						./srcs/tokenize/token_factory.c \
						./srcs/ast/print_node.c \
						./srcs/ast/cmd_node.c \
						./srcs/ast/io_redir_node.c \
						./srcs/ast/node_utils.c \
						./srcs/ast/parse_input.c \
						./srcs/ast/word_node.c \
						./srcs/ast/remove_node.c \
						./srcs/ast/construct_node.c \
						./srcs/execution/execute_utils.c \
						./srcs/execution/execute_command.c \
						./srcs/execution/execute_builtin.c \
						./srcs/shell/shell_signal.c \
						./srcs/shell/shell_data.c \
						./srcs/shell/tty_restore.c \
						./srcs/shell/shell_signal_handler.c \
						./srcs/shell/shell_input.c \
						./srcs/main.c


# OBJS 				:=	$(patsubst ./$(SRC_DIR)/%.c, ./$(BUILD_DIR)/%.c.o, $(addprefix ./$(SRC_DIR)/, $(SRCS)))
OBJS 				:=	$(patsubst ./$(SRC_DIR)/%.c, ./$(BUILD_DIR)/%.c.o, $(SRCS))

# TEST_SRCS			:=	$(shell find ./$(TEST_DIR) -name '*.c')
# TEST_OBJS			:=	$(patsubst ./$(TEST_DIR)/%.c, ./$(TEST_BUILD_DIR)/%.c.o, $(TEST_SRCS))
# TEST_TARGET_OBJS	:=	$(patsubst ./$(TEST_DIR)/**/%_test.c, ./$(BUILD_DIR)/**/*.c.o, $(TEST_SRCS))
# TEST_BINS			:=	$(patsubst ./$(TEST_DIR)/%.c, ./$(TEST_BIN_DIR)/%, $(TEST_SRCS))

LIB_NAMES			=	ft
LIBS				=	$(addprefix lib, $(LIB_NAMES))
LIB_ACHIEVES		=	$(addsuffix .a, $(LIBS))

CFLAGS				=	-Wall -Wextra -Werror

# **************** #
#    Debug Flag    #
# **************** #

ifeq ($(debug), 1)
	override CFLAGS	+=	--debug -g -DDEBUG
	LIB_DEBUG = debug=1
endif

# ********************** #
#    OS Specific Flag    #
# ********************** #

OS := $(shell uname)

ifeq ($(OS), Linux)
	READLINE_HEADER =
	READLINE_LINK =
else ifeq ($(OS), Darwin)
	READLINE_HEADER = -I/opt/homebrew/opt/readline/include -I/usr/local/opt/readline/include
	READLINE_LINK = -L/opt/homebrew/opt/readline/lib -L/usr/local/opt/readline/lib
endif

.PHONY: all clean fclean re bonus debug
all: $(NAME)

$(NAME): $(OBJS) | $(BIN_DIR) $(LIB_ACHIEVES)
	@echo "$(CYAN_BLUE)[$(NAME)] $(RED)compile $(BIN_DIR)/$@ ...$(RESET)"
	@$(PREFIX) $(CC) $(CFLAGS) -o ./$(BIN_DIR)/$@ $^ \
		$(foreach lib, $(LIB_NAMES), -L./lib$(lib) -l$(lib)) \
		$(READLINE_LINK) \
		-lreadline
	@echo "$(CYAN_BLUE)[$(NAME)] $(ORANGE)$(BIN_DIR)/$@ compiled!$(RESET)"
	@cp $(BIN_DIR)/$@ $@
	@echo "$(CYAN_BLUE)[$(NAME)] $(ORANGE)$(BIN_DIR)/$@ copied!$(RESET)"

$(BIN_DIR):
	@mkdir -p ./$@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN_BLUE)[$(NAME)] $(RED)compile $< ...$(RESET)"
	@$(PREFIX) $(CC) $(CFLAGS) -o $@ -c $< -I./$(HEADER_DIR) \
		$(foreach lib, $(LIB_NAMES), -I./lib$(lib)/$(HEADER_DIR)) \
		$(READLINE_HEADER)
	@echo "$(CYAN_BLUE)[$(NAME)] $(GREEN)$< compiled!$(RESET)"

$(LIB_ACHIEVES): %.a:
	@for lib in $(LIBS); do \
		if [ ! -f ./$$lib/$$lib.a ]; then \
			echo "$(LIGHT_YELLOW)[$$lib]$(RED) not exists."; \
			echo "$(LIGHT_YELLOW)[$$lib]$(RED) execute make$(RESET)"; \
			$(MAKE) $(LIB_DEBUG) -C ./$$lib; \
		else \
			echo "$(LIGHT_YELLOW)[$$lib]$(GREEN)exists, skipping make.$(RESET)"; \
		fi \
	done

clean:
	@echo "$(CYAN_BLUE)[$(NAME)]$(RESET) clean"
	@rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)

fclean: clean
	@echo "$(CYAN_BLUE)[$(NAME)]$(RESET) fclean"
	@rm -rf $(NAME) $(BIN_DIR) $(TEST_BIN_DIR)
	@for lib in $(LIBS); do \
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

dev:
	@make fclean
	@make debug=1 all
	@make debug=1 bonus

test:
ifeq ($(TEST),)
	@$(MAKE) debug=1 dev
	@$(MAKE) debug=1 do_test
else
	@$(MAKE) debug=1 dev
	@$(MAKE) debug=1 do_single_test TEST=$(TEST)
endif

do_test: $(TEST_BINS)
	@for bin in $(TEST_BINS); do \
		echo "\n\n$(CYAN_BLUE)[$(NAME)] $(PURPLE)[Test] $$bin ...$(RESET)"; \
		./$$bin; \
	done

do_single_test: $(TEST_BIN_DIR)/$(TEST)
	@echo "\n\n$(CYAN_BLUE)[$(NAME)] $(PURPLE)[Test] $@ ...$(RESET)"
	@./$<

$(TEST_BIN_DIR)/% : $(TEST_BUILD_DIR)/%.c.o
	@mkdir -p $(dir $@)
	@$(PREFIX) $(CC) $(CFLAGS) -o $@ $< \
		$(filter-out ./$(BUILD_DIR)/main.c.o, $(wildcard ./$(BUILD_DIR)/**/*.c.o)) \
		$(foreach lib, $(LIB_NAMES), -L./lib$(lib) -l$(lib)) \
		$(READLINE_LINK) \
		-lreadline
	@echo "$(CYAN_BLUE)[$(NAME)] $(ORANGE)$(BIN_DIR)/$@ compiled!$(RESET)"

$(TEST_BUILD_DIR)/%.c.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN_BLUE)[$(NAME)] $(RED)compile $< ...$(RESET)"
	@$(PREFIX) $(CC) $(CFLAGS) -o $@ -c $< -I./$(HEADER_DIR) \
		$(foreach lib, $(LIB_NAMES), -I./lib$(lib)/$(HEADER_DIR)) \
		$(READLINE_HEADER)
	@echo "$(CYAN_BLUE)[$(NAME)] $(GREEN)$< compiled!$(RESET)"
