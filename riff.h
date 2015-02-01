/*Объект для обработки riff-файлов на базе struct fext из fext.h*/

#ifndef RIFF_H
#define RIFF_H 1

#include <stdint.h>

#include "chunk.h"

/*Описатель riff-файла*/
struct riff;

/*Открывает riff-файл с имененем path в режиме mode. По поведению эквивалентна fext_open() из fext.h*/
struct riff *riff_open(const char *path, const char *mode);

/*Уничтожает описатель fext и закрывает связанный с ним файл. По поведению эквивалентна fext_close() из fext.h*/
int riff_close(struct riff *file);

/*Создаёт описатель chunk'а из chunk.h для первого chunk'а в файле.
Указатель на созданный описатель помещается в *chunk.
Созданный описатель необходимо уничтожать самомтоятельно.
В случае успеха возвращает 0.
В случае ошибки возвращает 1.*/
int riff_firstchunk(struct riff *file, struct chunk **chunk);

/*Создаёт описатель для chunk'а, который следует за последним полученным при помощи riff_firstchunk() или riff_nextchunk().
В остальном поведение эквивалентно riff_firstchunk.
Если код возврата равен 0, но получен ненулевой указатель, значит chunk'ов больше нет.*/
int riff_nextchunk(struct riff *file, struct chunk **chunk);

/*Получить формат файла.
Значение помещается в fmt.
В случае успеха возвращает 0.
Если riff == NULL, возвращает 1 и устанавливает в errno значение EBADF. *fmt при этом не изменяется.*/
int riff_getfmt(struct riff *file, uint32_t *fmt);

/*Получить внутренний код ошибки файла riff. Код записывается в *err.
В случае успеха возвращает 0.
Если riff == NULL, возвращает 1 и устанавливает в errno значение EBADF. *err при этом не изменяется.*/
int riff_geterr(struct riff *file, int *err);

/*Генерирует строку-сообщение, соответствующее внутреннему коду ошибки файла riff.
Указатель на строку записывается в *s.
В случае успеха возвращает 0.
Если riff == NULL, возвращает 1 и устанавливает в errno значение EBADF. *s при этом не изменяется.*/
int riff_strerror(struct riff *file, char **s); 

/*Печатает сообщение, соответствующее внутреннему коду ошибки файла riff, на стандартный вывод ошибок. Аргумент s эквивалентен функции perror().
В случае успеха возвращает 0.
Если riff == NULL, возвращает 1 и устанавливает в errno значение EBADF. Сообщение при этом не печатается.*/
int riff_perror(struct riff * file, const char *s);


#endif
