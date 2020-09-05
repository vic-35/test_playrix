#pragma once

class Target 
{
public:
	Target();
	virtual ~Target() {}	virtual std::string info() = 0;
	virtual void Draw() = 0; 
	void DrawBase(); // базовая отрисовка по умолчанию
	void CalcRect(int _scale,int _speed,int _angle); // вычисление размеров обьекта, установка скорости и угла поворота
	void UpdateR(); //поворот обьекта 
	virtual void Move() = 0;
	void MoveBall(); // движение 
	void MoveBase();
	bool Check(IPoint new_pos, IRect rect_new); // проверка пересечения с другим обьектом 

	bool enable; // разрешение на отображение
	Render::Texture *t; 
	IRect rect_t; // размеры
	IPoint cur_pos; // положение на экране
	float scale; // масштаб
	float angle;// угол
	float rotate;// поворот 
	float angle_move; // угол поворота
	int speed_move; // скорость движения
};

class TargetU :public Target
{
private:
	
public:

	void Draw() { DrawBase(); };
	void Move();
	std::string info() {
		return "TargetU";
	};
};


class Target1 :public Target
{
public:
	void Move();
	void Draw();
	std::string info() {
		return "Target1";
	};
};

class Target2 :public Target
{
public:
	void Move();
	void Draw();
	std::string info() {
		 return "Target2";
	};
};


class TargetString :public Target
{
private:

public:
	TargetString(std::string _text, std::string _font, IPoint _position);
	std::string font,text;

	void Draw();
	void Move();
	std::string info() {
		return "TargetS";
	};
};