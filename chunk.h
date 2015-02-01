#ifndef CHUNK_H
#define CHUNK_H 1

#include <stdint.h>

#include "fext.h"

/*Описатель chunk'а*/
struct chunk;

/*Создаёт новый chunk в текущей позиции файла fd*/
struct chunk *ch_new(struct fext *fd);

/*Удаляет chunk. Это действие не затрагивает файл.*/
int ch_delete(struct chunk *chunk);

int ch_getid(struct chunk *chunk, uint32_t *id);

int ch_getsize(struct chunk *chunk, uint32_t *size);

int ch_firstbuf(struct chunk *chunk, void **buf, uint32_t *size);

int ch_firstbuf(struct chunk *chunk, void **buf, uint32_t *size);

#endif
