#pragma once

class Target 
{
public:
	Target();
	virtual ~Target() {}	virtual std::string info() = 0;
	virtual void Draw() = 0; 
	void DrawBase(); // ������� ��������� �� ���������
	void CalcRect(int _scale,int _speed,int _angle); // ���������� �������� �������, ��������� �������� � ���� ��������
	void UpdateR(); //������� ������� 
	virtual void Move() = 0;
	void MoveBall(); // �������� 
	void MoveBase();
	bool Check(IPoint new_pos, IRect rect_new); // �������� ����������� � ������ �������� 

	bool enable; // ���������� �� �����������
	Render::Texture *t; 
	IRect rect_t; // �������
	IPoint cur_pos; // ��������� �� ������
	float scale; // �������
	float angle;// ����
	float rotate;// ������� 
	float angle_move; // ���� ��������
	int speed_move; // �������� ��������
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