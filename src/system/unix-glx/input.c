#include "breadbox.h"

enum script_word {
    WORD_ADD,
    WORD_AXIS,
    WORD_CLEARSTACK,
    WORD_DIVIDE,
    WORD_KEY,
    WORD_MODULO,
    WORD_MULTIPLY,
    WORD_SUBTRACT,
    WORD_TRIGGER,
};

enum script_symbol_type {
    SYMBOL_NUMBER,
    SYMBOL_WORD
};

struct script_symbol {
    enum script_symbol_type type;
    union {
        float number;
        enum script_word word;
    } data;
};

int input_update(breadbox_list_t *program, breadbox_subscriptions_t *subs) {
    breadbox_list_node_t *current;
    breadbox_list_iter_t program_iter;
    float stack[1024];
    struct script_symbol *symbol;
    int top = 0;
    breadbox_list_iter(program, &program_iter);
    while((current = breadbox_list_next(&program_iter))) {
        symbol = (struct script_symbol *) current->data;
        if(symbol->type == SYMBOL_NUMBER) {
            stack[top++] = symbol->data.number;
            if(top >= 1024) {
                breadbox_warning_internal(BBLOG_SYSTEM, "Stack overflow in the input script! Aborting script!");
                return 1;
            }
        // In the world of Forth, everything is a word or a number. So if we're
        // not dealing with a number, it's a safe assumption to say that it's
        // definitely a word, right? ~Alex
        } else {
            switch(symbol->data.word) {
                case WORD_ADD:
                case WORD_AXIS:
                    break;
                case WORD_CLEARSTACK:
                    top = 0;
                    break;
                case WORD_DIVIDE:
                case WORD_KEY:
                case WORD_MODULO:
                case WORD_MULTIPLY:
                case WORD_SUBTRACT:
                case WORD_TRIGGER:
                    break;
            }
        }
    }
    return 0;
}