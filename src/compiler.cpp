#pragma once

#include "compiler.h"
#include "parser.h"
#include "logger.h"
#include "syntax.h"

#include "itself_console_translator.h"
#include "c_translator.h"
#include "asm_translator.h"

#define NULL 0

#define runTranslator(t) {t.init(Compiler::outDir); SyntaxNode::root->print(&(t), 0);}

char* Compiler::mainFile = NULL;
char* Compiler::outFile = NULL;
char* Compiler::outDir = NULL;

int Compiler::outLangs = 0;

const int verbose = 1;

int Compiler::compile() {

    Logger::log(Logger::INFO, "Compilation started...\n");

    int err;  

    err = Parser::parse(mainFile);
    if (err) return err;
    
    Logger::log(Logger::INFO, "Parsing completed...\n");

    // TODO : make paralel
    if (outLangs & ITSELF_CONSOLE_LANG) runTranslator(translatorItselfConsole);
    if (outLangs & C_LANG) runTranslator(translatorC);
    if (outLangs & ASM_LANG) runTranslator(translatorASM);

    Logger::log(Logger::INFO, "Translation completed...\n");

    return 0;

}