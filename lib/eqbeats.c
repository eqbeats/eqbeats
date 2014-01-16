#include <eqbeats.h>
#include <stdlib.h>
#include <string.h>

static const char *determine_root(const char *root)
{
	if (root)
		return root;
	root = getenv("EQBEATS_DIR");
	if (root)
		return root;
	return DEFAULT_EQBEATS_DIR;
}

int eqbeats_init(struct eqbeats_ctx *eq, const char *pg_options, const char *eqbeats_dir, int flags)
{
	memset(eq, 0, sizeof(*eq));
	eq->root = determine_root(eqbeats_dir);
	return 0;
}

void eqbeats_end(struct eqbeats_ctx *eq)
{
}
