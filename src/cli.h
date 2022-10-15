#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

typedef struct CLIArguments {
    int logging_level;
    int us_delay;
    const char* rom_path;
} CLIArguments;

CLIArguments* cli_config_default(void);

void cli_config_destroy(CLIArguments *const conf);

int cli_config_handle(CLIArguments *const conf, int const argc, char **const argv);

#endif //YOBEMAG_CLI_H
