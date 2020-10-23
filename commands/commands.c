//
//  commads.c
//  mcfpp4
//
//  Created by Gurankio on 23/08/2020.
//  Copyright © 2020 Gurankio. All rights reserved.
//

#include "commands.h"

#include <stdlib.h>
#include <strings.h>
#include "../lib/vector.h"

typedef struct named_command {
    char * name;
    command command;
} named_command;

// 'Global' Vector
static named_command * commands;

// Null Replacements

static char * terminal_null(char ** params) {
    return *params;
}

static char * buffer_null(char ** params) {
    return *params;
}

static int intermediate_null(context * context) {
    return 0;
}

// Add Handlers

//static void add_preprocessor(char * name) {
//    named_command temp = {name, .command.type = INTERMEDIATE, .command.function.terminal = terminal_null };
//    vector_push(commands, temp);
//}

static void add_assembler(char * name, int (* intermediate)(context *)) {
    named_command temp = {name, .command.type = ASSEMBLER, .command.function.intermediate = (intermediate == NULL ? intermediate_null : intermediate) };
    vector_push(commands, temp);
}

static void add_terminal(char * name, char * (* terminal)(char **)) {
    named_command temp = {name, .command.type = TERMINAL, .command.function.terminal = (terminal == NULL ? terminal_null : terminal) };
    vector_push(commands, temp);
}

static void add_buffer(char * name, char * (* buffer)(char **)) {
    named_command temp = {name, .command.type = BUFFER, .command.function.buffer = (buffer == NULL ? buffer_null : buffer) };
    vector_push(commands, temp);
}

static void add_intermediate(char * name, int (* intermediate)(context *)) {
    named_command temp = {name, .command.type = INTERMEDIATE, .command.function.intermediate = (intermediate == NULL ? intermediate_null : intermediate) };
    vector_push(commands, temp);
}

// Public Utilities

#include "common.h"

#include "assign.h"
#include "load.h"
#include "range.h"
#include "file.h"

void commands_initialize() {
    commands = vector_create();
    
    add_assembler("assign", assign);
    
    add_intermediate("range", range);
    add_intermediate("load", load);
    add_intermediate("file", file);
    
//    add_intermediate("unique", NULL);
    
    // Minecraft Default
    add_terminal("attribute", run_join_space);
    add_terminal("advancement", run_join_space);
    add_terminal("bossbar", run_join_space);
    add_terminal("clear", run_join_space);
    add_terminal("clone", run_join_space);
    add_terminal("data", run_join_space);
    add_terminal("defaultgamemode", run_join_space);
    add_terminal("difficulty", run_join_space);
    add_terminal("effect", run_join_space);
    add_terminal("enchant", run_join_space);
    add_terminal("experience", run_join_space);
    add_terminal("fill", run_join_space);
    add_terminal("forceload", run_join_space);
    add_terminal("function", run_join_space);
    add_terminal("gamemode", run_join_space);
    add_terminal("gamerule", run_join_space);
    add_terminal("give", run_join_space);
    add_terminal("kill", run_join_space);
    add_terminal("list", run_join_space);
    add_terminal("attribute", run_join_space);
    add_terminal("locate", run_join_space);
    add_terminal("locatebiome", run_join_space);
    add_terminal("loot", run_join_space);
    add_terminal("me", run_join_space);
    add_terminal("msg", run_join_space);
    add_terminal("particle", run_join_space);
    add_terminal("playsound", run_join_space);
    add_terminal("recipe", run_join_space);
    add_terminal("replaceitem", run_join_space);
    add_terminal("say", run_join_space);
    add_terminal("schedule", run_join_space);
    add_terminal("scoreboard", run_join_space);
    add_terminal("seed", run_join_space);
    add_terminal("setblock", run_join_space);
    add_terminal("setworldspawn", run_join_space);
    add_terminal("spawnpoint", run_join_space);
    add_terminal("spectate", run_join_space);
    add_terminal("spreadplayers", run_join_space);
    add_terminal("stopsound", run_join_space);
    add_terminal("summon", run_join_space);
    add_terminal("tag", run_join_space);
    add_terminal("team", run_join_space);
    add_terminal("teammsg", run_join_space);
    add_terminal("teleport", run_join_space);
    add_terminal("tell", run_join_space);
    add_terminal("tellraw", run_join_space);
    add_terminal("time", run_join_space);
    add_terminal("title", run_join_space);
    add_terminal("tp", run_join_space);
    add_terminal("trigger", run_join_space);
    add_terminal("w", run_join_space);
    add_terminal("weather", run_join_space);
    add_terminal("worldborder", run_join_space);
    add_terminal("xp", run_join_space);
    
    add_terminal("whisper", run_join_space);

    add_buffer("align", join_space);
    add_buffer("anchored", join_space);
    add_buffer("as", join_space);
    add_buffer("at", join_space);
    add_buffer("facing", join_space);
    add_buffer("in", join_space);
    add_buffer("positioned", join_space);
    add_buffer("rotated", join_space);
    add_buffer("store", join_space);
    add_buffer("if", join_space);
    add_buffer("unless", join_space);
}

void commands_terminate() {
    vector_destroy(commands);
}

command commands_get(char * name) {
    for (int i = 0; i < vector_length(commands); i++) {
        if (strcasecmp(commands[i].name, name) == 0) {
            return commands[i].command;
        }
    }
    return (command){ERROR, NULL};
}
