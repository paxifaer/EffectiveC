#include <stdio.h>
#include <stdlib.h>

/*---------------------- 基类 "Animal" 定义 ----------------------*/
struct AnimalVTable;

typedef struct {
    const struct AnimalVTable* vtable; // 虚函数表指针
    const char* name;                  // 成员变量
} Animal;

struct AnimalVTable {
    void (*speak)(Animal* self);       // 虚函数指针
    void (*destroy)(Animal* self);     // 析构函数
};



static void Animal_speak(Animal* self) {
    printf("%s: aaa ...\n", self->name);
}

static void Animal_destroy(Animal* self) {
    free(self); // 释放 Animal 的内存
}

static const struct AnimalVTable Animal_vtable = {
    .speak = Animal_speak,
    .destroy = Animal_destroy
};

Animal* Animal_create(const char* name) {
    Animal* obj = malloc(sizeof(Animal));
    obj->name = name;
    obj->vtable = &Animal_vtable; // 初始化虚函数表指针
    return obj;
}
/*---------------------- 派生类 "Dog" 定义 ----------------------*/
typedef struct {
    Animal base;    // 基类作为第一个成员
    int bone_count; // 派生类特有成员
} Dog;

static void Dog_speak(Animal* self) {
    Dog* dog = (Dog*)self; // 类型转换
    printf("%s: Woof! I have %d bones\n", dog->base.name, dog->bone_count);
}

static void Dog_destroy(Animal* self) {
    Dog* dog = (Dog*)self;
    free(dog); // 释放 Dog 的内存
}

static const struct AnimalVTable Dog_vtable = {
    .speak = Dog_speak,
    .destroy = Dog_destroy
};

Dog* Dog_create(const char* name, int bones) {
    Dog* dog = malloc(sizeof(Dog));
    dog->base.vtable = &Dog_vtable; // 设置派生类虚表
    dog->base.name = name;
    dog->bone_count = bones;
    return dog;
}

/*---------------------- 派生类 "Cat" 定义 ----------------------*/
typedef struct {
    Animal base;
    int fish_count;
} Cat;

static void Cat_speak(Animal* self) {
    Cat* cat = (Cat*)self;
    printf("%s: Meow! I have %d fish\n", cat->base.name, cat->fish_count);
}

static void Cat_destroy(Animal* self) {
    Cat* cat = (Cat*)self;
    free(cat); // 释放 Cat 的内存
}

static const struct AnimalVTable Cat_vtable = {
    .speak = Cat_speak,
    .destroy = Cat_destroy
};

Cat* Cat_create(const char* name, int fish) {
    Cat* cat = malloc(sizeof(Cat));
    cat->base.vtable = &Cat_vtable;
    cat->base.name = name;
    cat->fish_count = fish;
    return cat;
}

/*---------------------- 多态调用演示 ----------------------*/
void animal_speak(Animal* animal) {
    animal->vtable->speak(animal);
}

int main() {
    // 创建不同子类对象，但用基类指针存储
    Animal* animals[] = {
        (Animal*)Dog_create("Buddy", 3),
        (Animal*)Cat_create("Mimi", 2),
        Animal_create("Unknown")
    };

    // 多态调用
    for (int i = 0; i < 3; i++) {
        animal_speak(animals[i]);
    }

    // 析构所有对象
    for (int i = 0; i < 3; i++) {
        printf("begin delete! \n");
        animals[i]->vtable->destroy(animals[i]);
    }

    return 0;
}