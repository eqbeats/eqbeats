#ifndef __EQBEATS_H
#define __EQBEATS_H

#ifdef __cplusplus
extern "C" {
#endif

struct eqbeats_ctx {
	struct pg_conn *pg;
	const char *root;
};

enum {
	EQBEATS_INIT_SETUP = 0x01,
};

int eqbeats_init(struct eqbeats_ctx *eq, const char *pg_options, const char *eqbeats_dir, int flags);
int eqbeats_health_check(struct eqbeats_ctx *eq);
void eqbeats_end(struct eqbeats_ctx *eq);

#ifdef __cplusplus
}
#endif

#endif /* __EQBEATS_H */
