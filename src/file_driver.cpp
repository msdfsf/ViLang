#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "file_driver.h"

namespace FileDriver {

    int readFile(char* name, char** buffer) {

        // TODO: NULL CHECK

        FILE* file = fopen(name, "rb");
        if (!file) return 1;

        fseek(file, 0, SEEK_END);
		const int fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

        *buffer = (char*) malloc(fileSize + 1);
        fread(*buffer, 1, fileSize + 1, file);
        
        (*buffer)[fileSize] = '\0';

        fclose(file);
        return 0;

    }
    
}