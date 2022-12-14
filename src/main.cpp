#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>

#include "compiler.h"
#include "logger.h"

#define OPTION_SYMBOL '-'

int main(int argc, char* argv[]) {

	int atLeastOneLangSet = 0;
	
	for (int i = 1; i < argc; i++) {
		
		if (argv[i][0] != OPTION_SYMBOL) {
			Compiler::mainFile = argv[i];
			continue;
		}

		char* const option = argv[i] + 1;

		// LOOK AT : think about 'pre-prepared' hash or something
		if (!strcmp(option, "ol")) {
			// output lang

			if (i + 1 > argc) {
				Logger::log(Logger::ERROR, "Argument requaried for option -ol!\nFollowing options are avaliable: c, asm, itself_console\n");
				return -1;
			}

			i++;

			atLeastOneLangSet = 1;
			char* const option = argv[i];
			
			if (!strcmp(option, "c")) {
				Compiler::outLangs = Compiler::outLangs | Compiler::C_LANG;
				continue;
			}

			if (!strcmp(option, "asm")) {
				Compiler::outLangs = Compiler::outLangs | Compiler::ASM_LANG;
				continue;
			}

			if (!strcmp(option, "itself_console")) {
				Compiler::outLangs = Compiler::outLangs | Compiler::ITSELF_CONSOLE_LANG;
				continue;
			}

			Logger::log(Logger::ERROR, "Unknown language!\nFollowing options are avaliable: c, asm, itself_console\n");

			return -1; // TODO : add error

		} else if (!strcmp(option, "of")) {
			// output file

			if (i + 1 > argc) {
				Logger::log(Logger::ERROR, "Argument requaried for option -of!\nFile has to be without the extension!\n");
				return -1;
			}

			i++;

			Compiler::outFile = argv[i];


		} else if (!strcmp(option, "od")) {
			// output file

			if (i + 1 > argc) {
				Logger::log(Logger::ERROR, "Argument requaried for option -od!\n");
				return -1;
			}

			i++;

			Compiler::outDir = argv[i];


		} else if (!strcmp(option, "if")) {
			// input file

			if (i + 1 > argc) {
				Logger::log(Logger::ERROR, "Argument requaried for option -if!\nFile has to have an extension!\n");
				return -1;
			}

			i++;

			Compiler::mainFile = argv[i];

		}
	
	}

	if (!atLeastOneLangSet) Compiler::outLangs = Compiler::outLangs | Compiler::C_LANG;

	auto cmpStartTime = std::chrono::high_resolution_clock::now();
	Compiler::compile();
	std::chrono::duration<double, std::milli> cmpElapsedTime = std::chrono::high_resolution_clock::now() - cmpStartTime;

	printf("\nCompilation time was: %.2f ms.\n", cmpElapsedTime.count());

	return 0;

}

// -ol -> output lang
//		- c, asm, itself_console

// -of -> output file
//		- filename without the extension

// -od -> output directory
//		- dirname

// -if -> input file
//		- filename with the extension
