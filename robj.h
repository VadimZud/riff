/*Специальная обёртка, позволяющая обращаться к разным файлоподобным объектам по одному интерфейсу (для чтения).*/

#ifndef ROBJ_H
#define ROBJ_H

/*Объект-обёртка.*/
struct robj;

/*Чтобы обернуть объект, следует реализовать и предоставить в виде структуры представленный ниже интерфейс.*/
struct riface {
	int (*firstbuf)(void *obj, void **buf, long *size);
	int (*nextbuf)(void *obj, void **buf, long *size);
	int (*getpos)(void *obj, long *pos);
	int (*setpos)(void *obj, long pos);
	int (*geterr)(void *obj, int *err);
}

/*Обернуть объект obj с интерфейсом iface.
В случае успеха возвращает указатель на обёрнутый объект.
В случае неудачи возвращает NULL.*/
struct robj *rwrap(void *obj, struct riface *iface);

/*Уничтожить обёртку robj.
Возвращает указатель на исходный объект (если robj == NULL, то возвращается NULL).*/
void *runwrap(struct robj *robj);

/*Следующие функции оборачивают соответствующие им из структуры riface и повторяют их по поведению.
Далее в комментариях описано рекомендуемое поведение.
Примечание: если robj == NULL, то функции возвращают 1. Это может создавать трудности, если коды возврата ваших функций
не соответствуют рекомендуемым, однако введение более сложной обработки для единственного исключения обёрки, по мнению автора, неоправдано.
Так или иначе, самостоятельная проверка результата функции rwrap() однозначно избавит вас от этой проблемы.*/

/*Читает не более *size байт с начала данных объекта robj в буфер *buf.
Число прочитанных байт записывается в *size.
В случае успеха возвращает 0, в случае ошибки - 1.
Функция определена для совместимости с объектами, у которых получение первого буфера данных,
по сравнению с последующими, требует дополнительных действий - чтение данных следует начинать с этой функции.
В случае повторного вызова чтение опять выполняется с начала данных объекта robj, а если вернутся к началу невозможно, 
завершается с ошибкой (Лучше сразу сказать, что начальные данные больше недоступны, чем вводить в заблуждение, выдавая текущие).
Передача буфера в виде **buf определена для тех объектов, которые во время чтения могут самостоятельно распределять память.*/
int rfirstbuf(struct robj *robj, void **buf, long *size);

/*Читает очередную порцию данных из объекта robj.
Перед использованием этой функции должен быть выполнен хотя бы один вызов rfirstbuf().
В целом, поведение такое же, как у rfirstbuf().*/
int rnextbuf(struct robj *robj, void **buf, long *size);

/*Получить текущую позицию чтения для объекта robj.
Позиция представляет собой смещение относительно начала данных и записывается в *pos.
В случае успеха возвращает 0, в случае ошибки - 1.*/
int rgetpos(struct robj *robj, long *pos);

/*Установить позицию чтения для объекта robj равной pos.
Позиция представляет собой смещение относительно начала данных.
В случае успеха возвращает 0, в случае ошибки - 1.*/
int rsetpos(struct robj *robj, long pos);

/*Получить код последней ошибки, произошедшей с объектом robj.
В случае успеха возвращает 0, в случае ошибки - 1.*/
int rgeterr(struct robj *robj, int *err);

#endif