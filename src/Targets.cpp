#include "stdafx.h"
#include "P.h"
#include "Targets.h"



Target::Target()
{
	enable = true;
	t = NULL;
	rect_t=IRect(0,0,0,0);
	cur_pos=IPoint(0,0);
	scale=1;
	angle=0;
	rotate = 0;
	angle_move = 0;
	speed_move = 0;
}

//radians = (degrees * pi) / 180;
//degrees = (radians * 180) / pi;
void Target::DrawBase()
{
	if (enable)
	{
		Render::device.PushMatrix();
		Render::device.MatrixTranslate((float)cur_pos.x - rect_t.width / 2, (float)cur_pos.y - rect_t.height / 2, 0);
		Render::device.MatrixScale(scale);

		if (angle != 0)  // поворот картинок вокруг геометрического центра
		{
			Render::device.MatrixRotate(math::Vector3(0, 0, 1), angle);
			int r1 = (rect_t.width / 2) / scale;
			int r2 = (rect_t.height / 2) / scale;
			double g = sqrt(r1*r1 + r2 * r2);
			double grad1 = acos(r2 / g) + ((angle)* std::_Pi) / 180;
			int y = g * std::cos(grad1);
			int x = g * std::sin(grad1);
			Render::device.MatrixTranslate(x - r1, y - r2, 0);
		}

		t->Draw();
		Render::device.PopMatrix();
	}
}
void Target::CalcRect(int _scale, int _speed, int _angle)
{
	scale = ((float)_scale)/100;
	speed_move = _speed;
	angle = P::I().NormAngle( _angle);

	rect_t= IRect(0,0, t->_rect_width*scale,t->_rect_height*scale);
	
}
void Target::UpdateR()
{
	if(enable && rotate != 0)
	{
		angle= P::I().NormAngle( angle+ rotate);

	}
}

void Target::MoveBase()
{
	if (enable && speed_move != 0)
	{
		double grad1 = -1*((angle_move)* std::_Pi) / 180;
		int y= (float)speed_move * std::cos(grad1);
		int x = (float)speed_move * std::sin(grad1);
		cur_pos.x += x;
		cur_pos.y += y;

	}
}
void TargetString::Move()
{
	MoveBase();
}
void TargetU::Move()
{
	MoveBase();
}
void Target::MoveBall()
{
	if (enable && speed_move != 0)
	{
		// считаем обьекты круглыми отскок от стенки 
		int r = rect_t.width / 2;
		bool x1 = (r + cur_pos.x) == WINDOW_WIDTH;
		bool x2 = cur_pos.x == r;
		bool y1 = (r + cur_pos.y) == WINDOW_HEIGHT;
		bool y2 = cur_pos.y == WINDOW_FREE_ZONE + r;

		if (x1||x2 )
		{
			angle_move = P::I().NormAngle(-1*angle_move);
		}
		else
		{
			if (y1 || y2)
			{
				angle_move = P::I().NormAngle(180-angle_move);
			}
		}
		
		double grad1 = -1 * ((angle_move)* std::_Pi) / 180;
		int y = (float)speed_move * std::cos(grad1);
		int x = (float)speed_move * std::sin(grad1);
		cur_pos.x += x;
		cur_pos.y += y;

		if (x1) cur_pos.x -= 1;
		if (x2) cur_pos.x += 1;
		if (y1) cur_pos.y -= 1;
		if (y2) cur_pos.x += 1;
	}
}
bool Target::Check(IPoint new_pos, IRect rect_new)
{
	int x1 = abs(cur_pos.x - new_pos.x);
	int y1 = abs(cur_pos.y - new_pos.y);
	int r1 = rect_t.width / 2 + rect_new.width / 2;
	int r2 = rect_t.height / 2 + rect_new.height / 2;

	if (r1 > x1 && r2 > y1) return true;

	return false;
}

void Target2::Move()
{
	MoveBall();
}
void Target1::Move()
{
	MoveBall();
}

void Target1::Draw()
{
	// считаем обьекты круглыми 
	int r = rect_t.width / 2;
	if ((r + cur_pos.x) > WINDOW_WIDTH) cur_pos.x = WINDOW_WIDTH - r;
	if (cur_pos.x < r) cur_pos.x = r;

	if ((r + cur_pos.y) > WINDOW_HEIGHT) cur_pos.y = WINDOW_HEIGHT - r;
	if (cur_pos.y < WINDOW_FREE_ZONE+r) cur_pos.y = WINDOW_FREE_ZONE + r;

	DrawBase();
};


void Target2::Draw()
{
	// считаем обьекты круглыми 
	int r = rect_t.width / 2;
	if ((r + cur_pos.x) > WINDOW_WIDTH) cur_pos.x = WINDOW_WIDTH - r;
	if (cur_pos.x < r) cur_pos.x = r;

	if ((r + cur_pos.y) > WINDOW_HEIGHT) cur_pos.y = WINDOW_HEIGHT - r;
	if (cur_pos.y < WINDOW_FREE_ZONE + r) cur_pos.y = WINDOW_FREE_ZONE + r;

	DrawBase();
};

void TargetString::Draw()
{
	Render::BindFont(font);
	Render::PrintString(cur_pos.x, cur_pos.y + rect_t.height / 2, text, 1.f, CenterAlign);
}

TargetString::TargetString(std::string _text, std::string _font, IPoint _position)
{
	text = _text;
	font = _font;
	cur_pos = _position;
	rect_t.width=Render::getStringWidth(text, font);
	rect_t.height = Render::getFontHeight(font);

}