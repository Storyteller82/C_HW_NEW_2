#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct list     // Новый тип данных Структура для хранения адреса и размера (СПИСОК)
{
    void *address;      // указатель на адрес неизвестного типа данных
    size_t size;        // размер (определенный разрядностью системы)
    char comment[64];   // комментарий сколько чего где адреса и т.д.
    struct list *next;  // структура этой же структуры внутри структуры (рекурсия?)
} list;                 // объявляем переменную типа структуры list (list list)

list *memlist = NULL;    // указатель на список объявляем пока без адреса

uint64_t findMaxBlock(list *head)
{
    uint64_t adr = 0;

    if(head == NULL)
    {
        return adr;
    }

    list *tmp = malloc(sizeof(list));
    tmp = head;
    
     while(head)
    {
        if (tmp->size <= head->size)
        {
            tmp->size = head->size;
            tmp->address = head->address;
        }
       
        head = head->next;
    }

    adr = (uint64_t)tmp->address;

    return adr;
}

void insert(list **head, void *address, size_t size, char *comment)     // функция вставки нового куска памяти (выделить память)
{
    list *tmp = malloc(sizeof(list));           // временный указатель типа структуры с адресом выделенного участка памяти размером со структуру
    tmp->next = *head;                          // указателю нехт присваиваем адрес головы
    tmp->address = address;                     // указателю адреса присваиваем адрес
    tmp->size = size;                           // в размер заносим размер 
    sprintf(tmp->comment,"%s",comment);         // заносим в комментарий свой комментарий
    *head = tmp;                                // присваиваем голоbе адрес выделенной памяти
}

_Bool delete(list **head, void *address)    // удаление одной головы с адресом
{
    if(*head == NULL)                       // если адрес стал равен нулю, (т.е. нету больше?)
        return 0;                           // завершаем 0
    list *del = NULL;                       // в другом случае, указателю типа структуры присваиваем адрес нуль (обнуляем)
    if( (*head)->address == address)        // если адрес в голове и переданный адрес равны
    {
        del = *head;                        // копируем структуры
        *head = (*head)->next;              // в голову кладем значение следующей структуры
        free(del);                          // и освобождаем участок памяти ДЕЛ
        return  1;                          // завершаем 1
    }
    list *tmp = *head;                          // заводим указатель типа структуры и присваиваем ему структуру головы
    while( tmp->next )                          // пока не кончились структуры (указатель не НУЛЬ) выполняем:
    {
        if( tmp->next->address == address )     // если в структуре тмп внутри структуры нехт адрес равен адресу, который передали
        {
            del = tmp->next;                    // присваиваем структуре дел структуру, которая внутри структуры тмп
            tmp->next = del->next;              // а внутренней структуре присваиваем дел нехт
            free(del);                          // и освобождаем память дел 
            return 1;                           // возвращаем 1
        }
        tmp=tmp->next;                          // в структуру тмп заносим структуру внутри стурктуры тмп
    }
    return 0;                                   // завершаем 0
}

void printList(list *head)                      // распечатываем структуру головы
{
    if(head == NULL)                            // если голова кончилась
    {
        printf("No memory leak detect\n");      // нет выделенной памяти
    }
    while(head)                                 // если не кончилась, и пока не кончилась записи коменнтариев
    {
        printf("%s\n",head->comment);           // печатаем комментарии 
        head = head->next;                      // копируем структуру в структуру. 
    }
}

void* my_malloc(size_t size, const char *file, int line, const char *func)  // выделене памяти своя функция
{
    void *ptr = malloc(size);                                                               // указатель на выделенную память размером сайз
    char coment[64] = {0};                                                                  // объявление коментария
    sprintf (coment,"Allocated = %s, %i, %s, %p[%li]", file, line, func, ptr, size);        // заносим в комент чего где и сколько выделили
    insert(&memlist,ptr,size,coment);                                                       // вставляем запись в список
    
    return ptr;                                                                             // возвращаем указатель на выделенную память
}

void my_free(void *ptr, const char *file, int line, const char *func)       // освобождение памяти 
{
    free(ptr);                                                              // удаляем по адресу
    delete(&memlist, ptr);                                                  // удаляем запись по адресу
}

#define malloc(X) my_malloc( (X), __FILE__, __LINE__, __FUNCTION__)
#define free(X) my_free( (X), __FILE__, __LINE__, __FUNCTION__)

int main(void) 
{
    int *p = malloc( sizeof(int) );             // указателю присваиваем адрес выделенной памяти ИНТ (см. дефайн)
    int *aef = malloc(sizeof(int)*100);           // указателю присваиваем адрес ИНТ*10
    int *ar = malloc(sizeof(int)*10);           // указателю присваиваем адрес ИНТ*10
    int *ae = malloc(sizeof(int)*11);           // указателю присваиваем адрес ИНТ*10
    
    findMaxBlock(memlist);
    *p = 5;                                     // по указателю сохраняем значение 5
   // free(p);                                    // очищаем по адресу
    p = malloc(sizeof(int));                    // запихиваем новый адрес новой памяти

   // free(p);                                    // очищаем опять туже 
    //printList(memlist);                         // функция чего там с памятью распечатать. выдать то, что не очистили. 

    return 0;
}

