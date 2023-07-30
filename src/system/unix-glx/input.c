#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "breadbox.h"

enum script_word {
    WORD_ADD,
    WORD_AXIS,
    WORD_CLEARSTACK,
    WORD_DIVIDE,
    WORD_KEY,
    WORD_MULTIPLY,
    WORD_NEGATE,
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

struct script_word_pair {
    const char *pattern;
    enum script_word word;
};

const struct script_word_pair script_lut[] = {
    {"+", WORD_ADD},
    {"axis", WORD_AXIS},
    {"clearstack", WORD_CLEARSTACK},
    {"/", WORD_DIVIDE},
    {"key", WORD_KEY},
    {"*", WORD_MULTIPLY},
    {"negate", WORD_NEGATE},
    {"-", WORD_SUBTRACT},
    {"trigger", WORD_TRIGGER}
};

#define SCRIPT_LUT_LENGTH (int)(sizeof(script_lut) / sizeof(struct script_word_pair))

void input_free(breadbox_list_t *program) {
    breadbox_list_node_t *node;
    breadbox_list_iter_t program_iter;
    breadbox_list_iter(program, &program_iter);
    while((node = breadbox_list_next(&program_iter))) {
        free(node->data);
    }
    breadbox_list_free(program);
}

int input_parse(breadbox_list_t *program, FILE *script) {
    char buffer[BUFSIZ];
    int found;
    struct script_symbol *symbol;
    char *word;
    while(fgets(buffer, BUFSIZ, script)) {
        word = strtok(buffer, " \n");
        while(word) {
            if(*word >= '0' && *word <= '9') {
                symbol = malloc(sizeof(struct script_symbol));
                symbol->type = SYMBOL_NUMBER;
                symbol->data.number = atof(word);
                breadbox_list_append(program, symbol);
            // If it's not a number or a comment, it's a word! Now we need to
            // determine whether the word is valid or not. ~Alex
            } else if(*word != '\\') {
                symbol = malloc(sizeof(struct script_symbol));
                symbol->type = SYMBOL_WORD;
                // In Forth, we'll default to clearstack if we don't know what
                // the keyword means. ~Alex
                symbol->data.word = WORD_CLEARSTACK;
                found = 0;
                for(int i = 0; i < SCRIPT_LUT_LENGTH; i++) {
                    if(!strcmp(word, script_lut[i].pattern)) {
                        symbol->data.word = script_lut[i].word;
                        found = 1;
                        break;
                    }
                }
                if(!found) {
                    breadbox_warning_internal(BBLOG_SYSTEM, "input_parse: Word \"%s\" is unknown and will be interpreted as clearstack. Unexpected behavior may occur!", word);
                }
                breadbox_list_append(program, symbol);
            }
            // Comments mean we ignore the rest of the line. Setting word to
            // NULL should do the trick. ~Alex
            if(*word == '\\') {
                word = NULL;
            } else {
                word = strtok(NULL, " \n");
            }
        }
    }
    return 0;
}

int input_update(breadbox_list_t *program, breadbox_t *engine) {
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
                breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack overflow in script! Aborting!");
                return 1;
            }
        // In the world of Forth, everything is a word or a number. So if we're
        // not dealing with a number, it's a safe assumption to say that it's
        // definitely a word, right? ~Alex
        } else {
            switch(symbol->data.word) {
                case WORD_ADD:
                    if(top >= 2) {
                        stack[top - 2] += stack[top - 1];
                        top--;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_AXIS:
                    if(top >= 2) {
                        if((int)stack[top - 1] > 31 || (int)stack[top - 1] < 0) {
                            breadbox_warning_internal(BBLOG_SYSTEM, "input_update: Unknown axis %d. Ignoring.", (int)stack[top - 1]);
                        } else {
                            engine->subscriptions.axes[(int)stack[top - 1]] = stack[top - 2];
                            breadbox_publish(engine, BBMSG_AXIS0 + (int)stack[top - 1]);
                        }
                        top -= 2;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_CLEARSTACK:
                    top = 0;
                    break;
                case WORD_DIVIDE:
                    if(top >= 2) {
                        stack[top - 2] /= stack[top - 1];
                        top--;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_KEY:
                    if(top >= 1) {
                        // TODO: How should we poll the current state of a key? ~Alex
                        stack[top - 1] = 0.0;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_MULTIPLY:
                    if(top >= 2) {
                        stack[top - 2] *= stack[top - 1];
                        top--;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_NEGATE:
                    if(top >= 1) {
                        stack[top - 1] = -stack[top - 1];
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_SUBTRACT:
                    if(top >= 2) {
                        stack[top - 2] -= stack[top - 1];
                        top--;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
                case WORD_TRIGGER:
                    if(top >= 2) {
                        if((int)stack[top - 1] > 31 || (int)stack[top - 1] < 0) {
                            breadbox_warning_internal(BBLOG_SYSTEM, "input_update: Unknown trigger %d. Ignoring.", (int)stack[top - 1]);
                        } else if((int)stack[top - 2]) {
                            breadbox_publish(engine, BBMSG_TRIGGER0 + (int)stack[top - 1]);
                        }
                        top -= 2;
                    } else {
                        breadbox_error_internal(BBLOG_SYSTEM, "input_update: Stack underflow in script! Aborting!");
                        return 1;
                    }
                    break;
            }
        }
    }
    return 0;
}