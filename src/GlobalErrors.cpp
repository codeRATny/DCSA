#include "GlobalErrors.hpp"

static const char *unknown_err_code(int err)
{
    static char buf[] = "Server error";
    return buf;
}

namespace errors
{
#define ERR_NAME_GEN(name, _) \
    case ERR_##name:          \
        return #name;
    const char *ErrName(int err)
    {
        switch (err)
        {
            ERROR_MAP(ERR_NAME_GEN)
        }
        return unknown_err_code(err);
    }
#undef ERR_NAME_GEN

#define STRERROR_GEN(name, msg) \
    case ERR_##name:            \
        return msg;
    const char *StrError(int err)
    {
        switch (err)
        {
            ERROR_MAP(STRERROR_GEN)
        }
        return unknown_err_code(err);
    }
#undef STRERROR_GEN
}