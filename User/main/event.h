#ifndef __EVENT_H
#define	__EVENT_H

#include "type_def.h"

u16 get_current_events(void);
void add_event(MmiEvent event);
void remove_event(MmiEvent event);

#endif
