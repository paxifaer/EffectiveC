#include <stdio.h>
#include <stdlib.h>

struct AnimalVTable;

struct Animal{
  const char* name;
  const struct AnimalVTable * vtable;
};

struct AnimalVTable{
    void (*speak)(struct Animal* self);
    void (*destory)(struct Animal* self);
};

static void AnimalSpeak(struct Animal* self)
{
    printf("speak  %s donnot speak\n",&self->name);
}

static void AnimalDestory(struct Animal* self)// 使用static 保证作用于外调用不到。保证类的成员函数保护性
{
    free(self);
}

static const struct  AnimalVTable avtable = {
  .speak = AnimalSpeak,
  .destory = AnimalDestory
};

struct Animal* CreateAnimal(const char* name)
{
    // self->name = malloc(8);
    struct Animal* self = malloc(sizeof(struct Animal));
    self->name = name;
    self->vtable = &avtable;
    return self;
}

struct Cat{
   struct Animal base;
};




static void CatSpeak(struct Animal* self)
{
    struct Cat* cat = (struct Cat*)self;
    printf("speak  %s donnot speak\n",&cat->base.name);
}

static void CatDestory(struct Animal* self)
{
    free(self);
}

static const struct  AnimalVTable catable = {
  .speak = CatSpeak,
  .destory = CatDestory
};

struct Cat* CreateCat(const char* name)
{
    // self->name = malloc(8);
    struct Cat* self = malloc(sizeof(struct Cat));
    self->base.name = name;
    self->base.vtable = &catable;
    return self;
}

void animal_speak(struct Animal* animal) {
    animal->vtable->speak(animal);
}

int main()
{
    struct Animal* animals[] = {
        // (struct Animal*)Dog_create("Buddy", 3),
        (struct Animal*)CreateCat("Mimi"),
        CreateAnimal("Unknown")
    };

    // 多态调用
    for (int i = 0; i < 2; i++) {
        animal_speak(animals[i]);
    }

    // 析构所有对象
    for (int i = 0; i < 2; i++) {
        printf("begin delete! \n");
        animals[i]->vtable->destory(animals[i]);
    }
    
    return 0;
}