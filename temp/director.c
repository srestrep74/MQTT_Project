#include "director.h"
#include "connect_builder.h"

void construct_connect(director *self, builder *builder, struct fixed_header *fixed_header, union variable_header *variable_header, union payload *payload)
{
    builder->reset(builder);
    builder->set_fixed_header(builder, fixed_header);
    builder->set_variable_header(builder, variable_header);
    builder->set_payload(builder, payload);
}

director *new_director(void)
{
    director *director = malloc(sizeof(director));
    if (director != NULL)
    {
        director->construct_connect = construct_connect;
    }
    return director;
}
