#include <iostream>
#include <conio.h>
using namespace std;

struct SList
{
	class Basic* cobj;
	struct SList* next;
	struct SList* prev;
};

class Basic //базовый абстрактный класс
{
public: 
	virtual void foo() 
	{
		printf("Basic");
	}

};

class First : public Basic //первый потомок базового класса
{
protected:
	int x, y;
public:
	First()
	{
		x = 0; y = 0;
	}
	First(int x,int y)
	{
		this->x = x;
		this->y = y;
	}
	int getOptions()
	{
		return x, y;
	}
	void foo() override
	{
		printf("First options: %d,%d\n",x,y);
	}
	
	~First()
	{
		printf("delete First");
	}
};
class Second : public Basic //второй потомок базового класса
{
protected:
	First* p1;
	First* p2;
public:
	Second()
	{
		p1 = new First();
		p2 = new First();
	}
	Second(int x1,int y1,int x2,int y2)
	{
		p1 = new First(x1, y1);
		p2 = new First(x2, y2);
	}

	void foo() override
	{
		printf("отрезок\n");
	}
	~Second()
	{
		printf("удаление отрезка");
		delete p1;
		delete p2;
	}
};

class MyStorage
{
private:
	SList* head;
public:
	MyStorage(Basic* obj) //конструктор
	{
		// выделение памяти под корень списка
		head = (struct SList*)malloc(sizeof(struct SList));
		head->cobj = obj;
		head->next = NULL; // указатель на следующий узел
		head->prev = NULL; // указатель на предыдущий узел
	}
	void addObj(Basic* obj)
	{
		struct SList* temp, * p;
		temp = (struct SList*)malloc(sizeof(SList)); //выделение памяти для temp
		p = head->next; // сохранение указателя на следующий узел
		head->next = temp; // предыдущий узел указывает на создаваемый
		temp->cobj = obj; // сохранение поля данных добавляемого узла
		temp->next = p; // созданный узел указывает на следующий узел
		temp->prev = head; // созданный узел указывает на предыдущий узел
		if (p != NULL)
			p->prev = temp;
	}
	void deleteObj(int index)
	{
		struct SList* Sprev, * Snext;
		Sprev = head->prev; // узел, предшествующий head
		Snext = head->next; // узел, следующий за head
		for (int i = 0; i < (index - 1); i++)
		{
			if (Snext != NULL)
			{
				Sprev = Snext->prev;
				Snext = Snext->next;
			}
		}
		
		struct SList* temp;
		temp = Sprev->next;
		if (Sprev != NULL)
			Sprev->next = Snext; // переставляем указатель
		if (Snext != NULL)
			Snext->prev = Sprev; // переставляем указатель
		delete temp->cobj;
		free(temp);
	}
	void printlist()
	{
		struct SList* temp;
		temp = head;
		do {
			temp->cobj->foo();
			temp = temp->next;
		} while (temp != NULL); // условие окончания обхода
	}
};

void main()
{
	setlocale(LC_ALL, "RU");
	MyStorage storage(new First());
	for (int i = 1; i < 10; i++) 
	{
		storage.addObj(new First);
	}
	storage.printlist();
	printf("\n");
	storage.deleteObj(3);
	storage.printlist();
	/*for (int i = 0; !storage.eol(); storage.next())
		storage.getObject().someMethod();*/
}