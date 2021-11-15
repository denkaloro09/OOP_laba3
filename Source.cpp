#include <iostream>
#include <conio.h>
using namespace std;

class Shape 
{
public: 
	virtual void draw()
	{
		printf("рисование фигуры");
	}
	~Shape()
	{
		printf("удаление фигуры");
	}

};

class Point : public Shape
{
protected:
	int x, y;
public:
	Point()
	{
		x = 0; y = 0;
	}
	Point(int x,int y)
	{
		this->x = x;
		this->y = y;
	}

	void draw() override
	{
		printf("координаты точки - %d,%d\n",x,y);
	}
	
	~Point()
	{
		printf("удаление точки");
	}
};
class Section : public Shape
{
protected:
	Point* p1;
	Point* p2;
public:
	Section()
	{
		
	}
	Section(int x1,int y1,int x2,int y2)
	{
		p1 = new Point(x1, y1);
		p2 = new Point(x2, y2);
	}

	void draw() override
	{
		printf("отрезок");
	}
	~Section()
	{
		printf("удаление отрезка");
		delete p1;
		delete p2;
	}
};

class MyStorage
{
private:
	Shape** objs;
	int size;
public:
	MyStorage(int size)
	{
		this->size = size;
		objs = new Shape * [size];
	}
	void setObject(int i,Shape *obj)
	{
		objs[i] = obj;
	}
	Shape& getObject(int i)
	{
		return *objs[i];
	}
	int getCount()
	{
		return size;
	}
};

void main()
{
	setlocale(LC_ALL, "RU");
	// создаем хранилище
	MyStorage storage(10);
	// добавляем в него объекты
	for (int i = 0; i < storage.getCount(); i++)
		storage.setObject(i, new Point());
	// обращаемся поочередно ко всем
	for (int i = 0; i < storage.getCount(); i++)
		storage.getObject(i).draw();
}