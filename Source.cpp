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
	void addToHead(Basic* obj)
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
	void addObj(Basic* obj,int index) //добавление объекта в список
	{
		if (index != 0) {
			struct SList* Sprev, * Snext;
			Sprev = head->prev; // узел, предшествующий head
			Snext = head->next; // узел, следующий за head
			for (int i = 1; i < index; i++)
			{
				if (Snext != NULL)
				{
					Sprev = Snext->prev; //двигаемся вперед по списку
					Snext = Snext->next;
				}
			}
			if (Sprev != NULL && Snext == NULL) //если удаляется не корень списка
			{
				struct SList* temp;
				temp = (struct SList*)malloc(sizeof(SList));
				Sprev->next->next = temp; // предыдущий узел указывает на создаваемый
				temp->cobj = obj; // сохранение поля данных добавляемого узла
				temp->next = Snext; // созданный узел указывает на следующий узел
				temp->prev = Sprev->next; // созданный узел указывает на предыдущий узел
			}
			else if(Sprev != NULL && Snext != NULL)
			{
				struct SList* temp;
				temp = (struct SList*)malloc(sizeof(SList));
				Sprev->next = temp; // предыдущий узел указывает на создаваемый
				temp->cobj = obj; // сохранение поля данных добавляемого узла
				temp->next = Snext->prev; // созданный узел указывает на следующий узел
				temp->prev = Sprev; // созданный узел указывает на предыдущий узел
				Snext->prev->prev = temp;
			}
			else //удаляется корень списка
			{
				addToHead(obj);
			}
		}	
	}
	void deleteObj(int index) //удаление объекта из списка
	{
		if (index != 0) {
			struct SList* Sprev, * Snext;
			Sprev = head->prev; // узел, предшествующий head
			Snext = head->next; // узел, следующий за head
			for (int i = 1; i < index; i++)
			{
				if (Snext != NULL)
				{
					Sprev = Snext->prev; //двигаемся вперед по списку
					Snext = Snext->next;
				}
			}
			if (Sprev != NULL) //если удаляется не корень списка
			{
				struct SList* temp;
				temp = Sprev->next;
				if (Sprev != NULL)
					Sprev->next = Snext; // переставляем указатель
				if (Snext != NULL)
					Snext->prev = Sprev; // переставляем указатель
				delete temp->cobj;
				free(temp);
			}
			else //удаляется корень списка
			{
				struct SList* temp;
				temp = head->next;
				delete head->cobj;
				free(head);   // освобождение памяти текущего корня
				head = temp;
			}
		}
	}
	Basic* getObject(int index)
	{
		struct SList* Sprev, * Snext;
		Sprev = head->prev; // узел, предшествующий head
		Snext = head->next; // узел, следующий за head
		for (int i = 1; i < index; i++)
		{
			if (Snext != NULL)
			{
				Sprev = Snext->prev; //двигаемся вперед по списку
				Snext = Snext->next;
			}
		}
		return Snext->prev->cobj;
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
	MyStorage storage(new Second());
	for (int i = 1; i < 4; i++) 
	{
		storage.addObj(new First,i);
	}
	storage.printlist();
	printf("\n");
	for(int i = 5;i<10;i++)
	{
		storage.addObj(new Second,i);
	}
	storage.printlist();
	storage.deleteObj(5);
	printf("\n");
	storage.printlist();
	storage.deleteObj(1);
	storage.addObj(new First,7);
	printf("\n");
	storage.printlist();

	/*for (int i = 0; !storage.eol(); storage.next())
		storage.getObject().someMethod();*/
 }