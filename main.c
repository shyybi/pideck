#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config/config.h"
#include "ui/ui.h"

int	main(void) {
	ui_run();
	config_init(); 
	return (0);
}