#include <stdio.h>
#include "../ui/ui.h"

void pc_comm_set_track(const char *title, const char *artist) {
    snprintf(g_track_title, sizeof(g_track_title), "%s", title ? title : "");
    snprintf(g_artist_name, sizeof(g_artist_name), "%s", artist ? artist : "");
}
