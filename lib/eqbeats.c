#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <eqbeats.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

static const char *determine_root(const char *root)
{
	if (root)
		return root;
	root = getenv("EQBEATS_DIR");
	if (root)
		return root;
	return DEFAULT_EQBEATS_DIR;
}

static int ensure_directory(const char *root, const char *dir, int create)
{
	int rc = 0;
	char *path;
	asprintf(&path, "%s/%s", root, dir);
	rc = access(path, W_OK | X_OK);
	if (rc != 0 && errno == ENOENT && create)
		rc = mkdir(path, 0777);
	if (rc != 0)
		syslog(LOG_ERR, create ? "couln't create %s: %m" : "missing %s: %m", path);
	free(path);
	return rc;
}

static int ensure_directories(const char *root, int create)
{
	int ok =
		ensure_directory(root, "", create) == 0 &&
		ensure_directory(root, "tracks/", create) == 0 &&
		ensure_directory(root, "art/", create) == 0 &&
		ensure_directory(root, "art/medium/", create) == 0 &&
		ensure_directory(root, "art/thumb/", create) == 0
	;
	return ok ? 0 : -1;
}

int eqbeats_init(struct eqbeats_ctx *eq, const char *pg_options, const char *eqbeats_dir, int flags)
{
	memset(eq, 0, sizeof(*eq));
	eq->root = determine_root(eqbeats_dir);
	return ensure_directories(eq->root, flags & EQBEATS_INIT_SETUP);
}

void eqbeats_end(struct eqbeats_ctx *eq)
{
}
