#include <stdlib.h>

#include "robj.h"

/*Объект-обёртка*/
struct robj {
	void *obj;	/*исходный объект*/
	struct riface iface;	/*реализация интерфейса исходного объекта*/
}

struct robj *
rwrap(void *obj, struct riface *iface) {
	struct robj robj;

	if ((obj == NULL) || (iface == NULL)) {
		return NULL;
	}

	robj = malloc(sizeof(struct robj));
	if (robj != NULL) {
		robj->obj = obj;
		robj->iface = iface;
	}

	return robj;
}

void *
runwrap(struct robj *robj) {
	void *obj = NULL;

	if (robj != NULL) {
		obj = robj->obj;
	}
	free(robj);

	return obj;
}

int
rfirstbuf(struct robj *robj, void **buf, long *size) {
	if (robj == NULL) {
		return 1;
	}
	return robj->iface->firstbuf(robj->obj, buf, size);
}

int
rnextbuf(struct robj *robj, void **buf, long *size) {
	if (robj == NULL) {
		return 1;
	}
	return robj->iface->nextbuf(robj->obj, buf, size);
}

int
rgetpos(struct robj *robj, long *pos) {
	if (robj == NULL) {
		return 1;
	}
	return robj->iface->getpos(robj->obj, pos);
}

int
rsetpos(struct robj *robj, long pos) {
	if (robj == NULL) {
		return 1;
	}
	return robj->iface->setpos(robj->obj, pos);
}

int
rgeterr(struct robj *robj, int *err) {
	if (robj == NULL) {
		return 1;
	}
	return robj->iface->geterr(robj->obj, err);
}

