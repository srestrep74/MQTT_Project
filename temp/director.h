#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "message_builder.h"

typedef struct director director;

struct director
{
    void (*construct_connect)(director *self, builder *builder, struct fixed_header *fixed_header, union variable_header *variable_header, union payload *payload);
};

director *new_director(void);

#endif