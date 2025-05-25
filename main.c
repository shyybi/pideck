#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "src/config/config.h"
#include "src/ui/ui.h"

int	main(void) {
	ui_run();
	config_init(); 
	return (0);
}