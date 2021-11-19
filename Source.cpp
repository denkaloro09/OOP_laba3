#include <iostream>
#include <conio.h>
#include <ctime>
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
		printf_s("Basic");
	}
	virtual~Basic()
	{
		//printf("удаление родителя\n");
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
	First(const First& f)
	{
		x = f.x;
		y = f.y;
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
		printf_s("First options: %d,%d\n",x,y);
	}
	~First()
	{
		//printf("удаление точки\n");
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
	Second(const Second& s)
	{
		p1 = new First(*(s.p1));
		p2 = new First(*(s.p2));
	}

	void foo() override
	{
		printf_s("Second element\n");
	}
	~Second()
	{
		//printf("удаление отрезка\n");
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
		if (index != 0 && head != NULL) {
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
		else
		{
			head = (struct SList*)malloc(sizeof(struct SList));
			head->cobj = obj;
			head->next = NULL; // указатель на следующий узел
			head->prev = NULL; // указатель на предыдущий узел
		}
	}
	void deleteObj(int index) //удаление объекта из списка
	{
		if (index != 0 && head != NULL && index <= getCount()) {
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
			if(Snext == NULL && Sprev != NULL) //удаление последнего элемента в списке
			{
				struct SList* temp;
				temp = Sprev->next;
				Sprev->next = NULL;
				delete temp->cobj;
				free(temp);
			}
			else if (Sprev != NULL && Snext != NULL) //удаление элемента внутри списка
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
			else if(Snext !=  NULL && Sprev == NULL) //удаление корня списка
			{
				struct SList* temp;
				temp = head->next;
				temp->prev = NULL;
				delete head->cobj;
				free(head);   // освобождение памяти текущего корня
				head = temp;
			}
			else //удаление корня, если он единственный член списка
			{
				delete head->cobj; //полное удаление списка
				head = NULL;
				free(head);
			}
		}
	}
	int getCount() //количество элементов
	{
		if (head != NULL)
		{
			int count = 1;
			struct SList* Sprev, * Snext;
			Sprev = head->prev; // узел, предшествующий head
			Snext = head->next; // узел, следующий за head
			for (int i = 1; Snext != NULL; i++)
			{
				if (Snext != NULL)
				{
					Sprev = Snext->prev; //двигаемся вперед по списку
					Snext = Snext->next;
					count++;
				}
			}
			return count;
		} 
		else 
		{
			return 0;
		}
	}
	void fooStorage()
	{
		if (head != NULL)
		{
			struct SList* temp;
			temp = head;
			do {
				temp->cobj->foo();
				temp = temp->next;
			} while (temp != NULL); // условие окончания обхода
		} 
		else
		{
			printf_s("пусто!");
		}
	}
	void fooObj(int index)
	{
		if (head != NULL) {

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
			if (Snext == NULL && Sprev == NULL)
			{
				head->cobj->foo();
			}
			else if (Snext == NULL && Sprev != NULL)
			{
				Sprev->next->cobj->foo();
			}
			else
			{
				Snext->prev->cobj->foo();
			}
		}
	}
	~MyStorage()
	{
		if (head != NULL)
		{
			int a = getCount();
			for (int i = a; i > 0; i--)
			{
				deleteObj(i);
			}
		}
	}
};

void main()
{
	srand(time(0));
	setlocale(LC_ALL, "RU");
	MyStorage storage(new Second()); 
	for (int i = 1; i < 50; i++) //заполнение хранилища
	{
		int d = rand();
		if(d % 2 == 0)
		{
			storage.addObj(new First, i);
		} 
		else 
		{
			storage.addObj(new Second, i);
		}
	}

	storage.fooStorage();
	printf_s("%d\n", storage.getCount());
	unsigned int st = clock();

	for(int i = 1; i < 100; i++) //100 раз
	{
		int a = rand() % 3;
		int d = rand() % storage.getCount();
		switch (a)
		{
		case 0:
			d = rand() % 2;
			if (d == 0)
			{
				storage.addObj(new First, i);
			}
			else
			{
				storage.addObj(new Second, i);
			}
			break;
		case 1:
			storage.deleteObj(d);
			break;
		case 2:
			storage.fooObj(d);
			break;
		}
	}

	unsigned int et = clock(); // конечное время
	unsigned int pst = et - st; // искомое время
	storage.fooStorage();
	printf_s("%d\n", storage.getCount());
	printf_s("%d\n", pst);
	_getch();
	st = clock();

	for (int i = 1; i < 1000; i++)
	{
		int a = rand() % 3;
		int d = rand() % storage.getCount();
		switch (a)
		{
		case 0:
			d = rand() % 2;
			if (d == 0)
			{
				storage.addObj(new First, i);
			}
			else
			{
				storage.addObj(new Second, i);
			}
			break;
		case 1:
			storage.deleteObj(d);
			break;
		case 2:
			storage.fooObj(d);
			break;
		}
	}

	et = clock(); // конечное время
	pst = et - st; // искомое время
	storage.fooStorage();
	printf_s("%d\n", storage.getCount());
	printf_s("%d\n", pst);
	_getch();
	st = clock();
	for (int i = 1; i < 10000; i++) //10000 раз
	{
		int a = rand() % 3;
		int d = rand() % storage.getCount();
		switch (a)
		{
		case 0:
			d = rand() % 2;
			if (d == 0)
			{
				storage.addObj(new First, i);
			}
			else
			{
				storage.addObj(new Second, i);
			}
			break;
		case 1:
			storage.deleteObj(d);
			break;
		case 2:
			storage.fooObj(d);
			break;
		}
	}
	et = clock(); // конечное время
	pst = et - st; // искомое время
	storage.fooStorage();
	printf_s("%d\n", storage.getCount());
	printf_s("%d\n", pst);
	storage.~MyStorage();
 }