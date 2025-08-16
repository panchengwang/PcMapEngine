#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__

#include <string.h>

#define SERIALIZE(buf,data)                 \
    {                                       \
        memcpy(buf, &data, sizeof(data));   \
        buf += sizeof(data);                \
    }

#define SERIALIZE_STRING(buf,data)                      \
    {                                                   \
        memcpy(buf, data.c_str(), data.length() + 1);   \
        buf += data.length() + 1;                       \
    }


#define DESERIALIZE(buf,data)                           \
    {                                                   \
        memcpy(&data, buf, sizeof(data));               \
        buf += sizeof(data);                            \
    }

#define DESERIALIZE_STRING(buf,data)                      \
    {                                                     \
        data = buf;                                       \
        buf += strlen(buf) + 1;                           \
    }

#endif
