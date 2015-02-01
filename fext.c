#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "fext.h"

#define isbadf(fext) (((fext) == NULL) || ((fext)->fp == NULL))

/*Расширенный описатель потока*/
struct fext {
	FILE *fd;	/*стандартный описатель потока*/
	long offset;	/*смещение позиции ввода/вывода относительно её положения в момент открытия файла (см. fext_getoffset())*/
	int err;	/*код последней ошибки, при работе с этим потоком (использует значения errno из errno.h)*/
};

struct fext *
fext_open(const char *path, const char *mode) {
	struct fext *fext;

	fext = malloc(sizeof(struct fext));
	if (fext == NULL) {
		return NULL;
	}

	fext->fp = fopen(path, mode);
	if (fext->fp == NULL) {
		free(fext);
		return NULL;
	}
	
	fext->offset = 0;
	fext->err = 0;

	return fextd;
}

int
fext_close(struct fext *fext) {
	int err = 0;

	if (!isbadf(fext)) {
		if (fclose(fext->fp)) {
			err = 1;
		}
	}

	free(fext);

	return err;
}

int
fext_read(struct fext *fext, void *buf, size_t *size) {
	int n, err = 0;

	if (isbadf(fext)) {
		errno = EBADF;
		err = 1;
	} else {
		n = fread(buf, 1, *size, fext->fp);
		if (ferror(fext->fp)) {
			err = 1;
		}
	}

	if (err && (fext != NULL)) {
		fext->err = errno;
	} else {
		*size = n;
		fext->offset += n;
	}

	return err;
}

int
fext_setvbuf(struct fext *fext, void *buf, int mode, size_t size) {
	int err = 0;

	if (isbadf(fext)) {
		errno = EBADF;
		err = 1;
	} else {
		if (setvbuf(fext->fp, buf, mode, size)) {
			err = 1;
		}
	}
	
	if (err && (fext != NULL)) {
		fext->err = errno;
	}

	return err;
}

int
fext_seek(struct fext *fext, long offset, int whence) {
	int err = 0;

	if (isbadf(fext)) {
		errno = EBADF;
		err = 1;
	} else {
		if (fseek(fext->fp, offset, whence)) {
			err = 1;
		}
	}

	if (err && (fext != NULL)) {
		fext->err = errno;
	}

	return err;
}

int
fext_tell(struct fext *fext, long *offset) {
	int pos, err = 0;

	if (isbadf(fext)) {
		errno = EBADF;
		err =  1;
	} else {
		pos = ftell(fext->fp);
		if (pos == -1) {
			if (errno == ESPIPE) {
				pos = fext->offset;
			} else {
				err = 1;
			}
		}
	}

	if (err && (fext != NULL)) {
		fext->err = errno;
	} else {
		*offset = pos;
	}

	return err;
}

int
fext_geterr(struct fext *fext, int *err) {
	if (fext == NULL) {
		errno = EBADF;
		return 1;
	} else {
		*err = fext->err;
		return 0;
	}
}

int
fext_strerror(struct fext *fext, char **s) {
	int ret, err;
	
	ret = fext_geterr(fext, &err);
	if (ret == 0) {
		*s = strerror(err);
	}

	return ret;
}

int
fext_perror(struct fext *fext, const char *s) {
	char *strerr;
	int ret;

	ret = fext_strerror(fext, &strerr);

	if (ret == 0) {
		if ((s != NULL) && (strlen(s) != 0)) {
			fprintf(stderr, "%s: ", s);
		}
		fprintf(stderr, "%s\n", strerr);
	}

	return ret;
}
