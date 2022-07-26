

struct Origin
{
        const char *file;
        size_t line;
        const char *func;
}
// MARCOS
#define INFO_INTERNAL_ORIGIN (struct Origin){__FILE__, __FILE__, __FUNCTION__}
