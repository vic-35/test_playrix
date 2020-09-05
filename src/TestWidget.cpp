#include "stdafx.h"
#include "P.h"
#include "TestWidget.h"


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	timer = 0;
	last_dt = 0;
	last_check = 0;
	level = 0;
	Init();
}

// первоначальная инициализация обьектов
void TestWidget::Init()
{

	factory1.reset(new Target1Factory);
	factory2.reset(new Target2Factory);
	factoryU.reset(new TargetUFactory);
	factoryS.reset(new TargetSFactory);


	sight = factoryU->createTarget("sight");
	sight->CalcRect(P::I().zoom_u,0,0);


	cannon = factoryU->createTarget("Cannon");
	cannon->CalcRect (20,0,0); //  задаем принудительно масштаб
	cannon->cur_pos = IPoint((cannon->rect_t.width/2), (cannon->rect_t.height/2));

	bomb = factoryU->createTarget("Bomb");
	bomb->CalcRect(20, P::I().speed/25, 0);
	

	background = factoryU->createTarget("background");
	background->CalcRect(120,0, 0);
	background->cur_pos = IPoint((background->rect_t.width/2), (background->rect_t.height/2));


	strvect.reset(new std::vector<sm_target>);// обьекты загружаем строго по порядку номеров
	strvect->push_back(factoryS->createTarget(STR_EXIT, "arial1", IPoint(WINDOW_WIDTH/2, 30)));
	strvect->push_back(factoryS->createTarget(STR_START, "arial2", IPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)));
	strvect->push_back(factoryS->createTarget(STR_TIMER, "arial1", IPoint(50, WINDOW_HEIGHT - 30)));
	strvect->push_back(factoryS->createTarget(STR_WINNER, "arial2", IPoint(WINDOW_WIDTH / 2, 150)));
	strvect->push_back(factoryS->createTarget(STR_F, "arial2", IPoint(WINDOW_WIDTH / 2, 150)));

	target.reset(new std::vector<sm_target>);
	LoadObj();
	set_level(0);
}
// возвращает указатель на обект-строка TargetString
std::shared_ptr<TargetString> TestWidget::str(int _id_text)
{
	return std::static_pointer_cast<TargetString>(strvect->at(_id_text));
	// можно возвращать указатель путем поиска в strvect но этот путь медленный ,  выбор по индексу макисимально быстрый , необходимо только создавать обьекты строго по порядку
}

//	добавление  мишеней 
void TestWidget::LoadObj()
{
	target->clear();

	for (int i = 0; i < P::I().count_target; i++)
	{
		switch (P::I().GetRandom(0, 1))
		{
		case 1: target->push_back(factory2->createTarget());
			break;
		default:
			target->push_back(factory1->createTarget());
		}
	}
}
void TestWidget::set_level(int _level)
{
	timer = 0;
	level = _level;
	bomb->enable = false;
	cannon->enable = false;	
	target->clear();

	effCont.KillAllEffects();

	for (sm_target s : *strvect){s->enable = false;}

	switch (level)
	{
	case 0: // 
		str(STR_START)->enable = true;
		str(STR_EXIT)->enable = true;
		break;
	case 1: // 
		cannon->enable = true;
		str(STR_EXIT)->enable = true;
		str(STR_TIMER)->enable = true;
		LoadObj();
		
		break;
	case 2: // 
		str(STR_START)->enable = true;
		str(STR_WINNER)->enable = true;
		str(STR_TIMER)->enable = true;
		break;
	case 3: // 
		str(STR_START)->enable = true;
		str(STR_F)->enable = true;
		str(STR_TIMER)->enable = true;
		break;
	case 255:
		break;
	default:
		break;
	}
}

void TestWidget::Draw()
{
	try
	{
		//
		// Получаем текущее положение курсора мыши.
		//
		IPoint mouse_pos = Core::mainInput.GetMousePos();

		background->Draw();

		// Рисуем строки разрешенные к показу
		for (sm_target s : *strvect)
		{
			if (s->enable)
			{
				s->Draw();
			}
		}
		// Рисуем все мишени 
		for (sm_target t : *target)
		{
			t->Draw();
		}

		switch (level)
		{
		case 2:
		case 3:
		case 0:
		{
			if (str(STR_START)->enable)
			{
				// -- подсветка текста при наведении курсора
				if (str(STR_START)->Check(sight->cur_pos, sight->rect_t))
				{
					Render::BeginColor(Color(255, 0, 0));
					str(STR_START)->Draw();
					Render::EndColor();
				}
				else
				{
					str(STR_START)->Draw();
				}

			}

		}break;
		case 1:
		{
			{ // Орудие следует за прицелом
				int r1 = mouse_pos.x;
				int r2 = mouse_pos.y;
				double g = sqrt(r1*r1 + r2 * r2);
				cannon->angle = P::I().NormAngle(-1 * (acos(r2 / g) * 180) / std::_Pi);
				cannon->Draw();

				if (bomb->enable)
				{
					if (bomb->cur_pos.x <0 || bomb->cur_pos.x > WINDOW_WIDTH || bomb->cur_pos.y < 0 || bomb->cur_pos.y > WINDOW_HEIGHT) bomb->enable = false;
					
					bomb->Draw();

					// Эффект вращается вокруг бомбы 
					float g1 = (bomb->rect_t.width / 3)* sqrt(2);
					float grad1 =-1* ((bomb->angle)* std::_Pi) / 180;
					int y = (float)g1 * std::cos(grad1);
					int x = (float)g1 * std::sin(grad1);

					eff_slide->SetPos( FPoint(bomb->cur_pos.x + x, bomb->cur_pos.y + y));

				}
				else
				{
					if (eff_slide)
					{
						 eff_slide->Finish();
						 eff_slide = NULL;
					}
					// позиция бомбы  следует за поворотом орудия 
					float g1 = ((cannon->rect_t.height / 2)) + ((bomb->rect_t.height / 4));
					int y = g1 * std::cos(acos(r2 / g));
					int x = g1 * std::sin(acos(r2 / g));
					bomb->cur_pos = IPoint(x + cannon->cur_pos.x, y + cannon->cur_pos.y);
				}

			}

		} break;
		}


		////
		//// Рисуем все эффекты, которые добавили в контейнер (Update() для контейнера вызывать не нужно).
		////
		effCont.Draw();

		Render::BindFont("arial");
		Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);

		sight->cur_pos = mouse_pos;
		sight->Draw();
	}
	catch (...)
	{
		P::I().Error(1, ERROR_MSG + " TestWidget::Draw " + P::I().SysErrMsg());
	}
}



void TestWidget::Update(float dt)
{
	try
	{
		//
		// Обновим контейнер с эффектами
		//
		effCont.Update(dt);

		switch (level)
		{
		case 1:
			{


				if (last_check > DT_UPDATE_TARGET)
				{
					last_check = 0;

					// проверка соудаения обьектов, расчитывается  соударение только пары обьектов , они отскакивают друг от друга + случайная подкрутка
					std::vector<int> check;
					for (unsigned i = 0; i < target->size(); i++)
					{
						for (unsigned y = 0; y < target->size(); y++)
						{
							{
								if (i != y)
								{
									if (target->at(i)->Check(target->at(y)->cur_pos, target->at(y)->rect_t))
									{
										std::vector<int>::iterator find1 = std::find(check.begin(), check.end(), i);
										if (find1 == check.end()) 
										{
											target->at(i)->angle_move = P::I().NormAngle(P::I().GetRandom(0, 30) - target->at(i)->angle_move);
											target->at(i)->speed_move += 1;
											if (target->at(i)->speed_move > 6) target->at(i)->speed_move = P::I().GetRandom(2, 6);
											check.push_back(i);
										}
										std::vector<int>::iterator find2 = std::find(check.begin(), check.end(), y);
										if (find2 == check.end())
										{
											target->at(y)->angle_move = P::I().NormAngle(-1 * target->at(y)->angle_move - P::I().GetRandom(0, 30));
											target->at(y)->speed_move -= 1;
											if (target->at(y)->speed_move < 2) target->at(y)->speed_move = P::I().GetRandom(2, 6);
											check.push_back(y);
										}
									}
								}
							}
						}
					}

				}
				else
				{
					last_check += dt;
				}


				if (last_dt > DT_UPDATE_MOVE)
				{
					// поворот и движение обьектов 
					last_dt = 0;
					bomb->UpdateR();
					bomb->Move();

					if (bomb->enable)
					{
						for (auto it = target->begin(); it != target->end();) {
							if (bomb->Check((sm_target(*it))->cur_pos, (sm_target(*it))->rect_t))
							{
								// Запуск эффекта уничтожения мишени
								eff_target = effCont.AddEffect("Iskra2", FPoint((sm_target(*it))->cur_pos.x, (sm_target(*it))->cur_pos.y));
								eff_target->Finish();
								eff_target = NULL;
								it = target->erase(it);	bomb->enable = false;
							}
							else
							{
								it++;
							}
						}

						if (!bomb->enable)
						{
							float g1 = (bomb->rect_t.width / 4)* sqrt(2);
							float grad1 = -1 * ((bomb->angle_move)* std::_Pi) / 180;
							int y = (float)g1 * std::cos(grad1);
							int x = (float)g1 * std::sin(grad1);

							// Запуск эффекта взрыв
							eff_boom = effCont.AddEffect("vic1", FPoint(bomb->cur_pos.x + x, bomb->cur_pos.y + y));
							eff_boom->Finish();

						}
					}

					// Проверка на столкновение с эффектом взрыва 
					if (eff_boom)
					{
						if (eff_boom->ended)
						{
							eff_boom = NULL;
						}
						else
						{
							for (auto it = target->begin(); it != target->end();) {

								if ((sm_target(*it))->Check(IPoint( eff_boom->posX,eff_boom->posY) , bomb->rect_t))
								{
									// Запуск эффекта уничтожения мишени
									eff_target = effCont.AddEffect("Iskra2", FPoint((sm_target(*it))->cur_pos.x, (sm_target(*it))->cur_pos.y));
									eff_target->Finish();
									eff_target = NULL;
									it = target->erase(it);
								}
								else
								{
									it++;
								}
							}
						}
					}
					
					for (sm_target s : *target)
					{
						if (s->info() == "Target2")
						{
							s->UpdateR();
						}
						s->Move();
					}
				}
				else
				{
					last_dt += dt;
				}

				// вывод значения таймера или результата
				if (str(STR_TIMER)->enable)
				{
					timer += dt;
					str(STR_TIMER)->text = P::I().S(STR_TIMER) + " " + utils::lexical_cast(static_cast<int>(timer));
					str(STR_TIMER)->cur_pos = IPoint(50, WINDOW_HEIGHT - 30);

					if (timer >= P::I().time)
					{
						str(STR_TIMER)->text =utils::lexical_cast(P::I().count_target-target->size())+" "+ P::I().S(STR_HIT1) + " " + utils::lexical_cast(P::I().count_target)+" "+P::I().S(STR_HIT2)  ;
						str(STR_TIMER)->cur_pos = IPoint(150, WINDOW_HEIGHT - 30);
						set_level(3);


					}
					else
					{
						if (target->size() == 0)
						{
							str(STR_TIMER)->text = utils::lexical_cast(P::I().count_target-target->size()) + " " + P::I().S(STR_HIT1) + " " + utils::lexical_cast(P::I().count_target) + " " + P::I().S(STR_HIT2);
							str(STR_TIMER)->cur_pos = IPoint(150, WINDOW_HEIGHT - 30);
							set_level(2);
						}
					}

				}

			}
			break;
		
		}
	}
	catch (...)
	{
		P::I().Error(1, ERROR_MSG + " TestWidget::Update " + P::I().SysErrMsg());
	}
	
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{

	}
	else
	{

	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{

}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	try
	{
		switch (level)
		{
		case 0:
		case 2:
		case 3:
			if (str(STR_START)->enable)
			{ // проверка нажатия на кнопку запуска
				if (str(STR_START)->Check(sight->cur_pos, sight->rect_t))
				{
					set_level(1);
				}
			}
			break;
		case 1:
			if (bomb->enable == false) // Запуск  бомбы на движение и включение эффекта 
			{
				bomb->enable = true;
				bomb->angle_move = cannon->angle;
				bomb->rotate = 3;
				
				float g1 = (bomb->rect_t.width / 2)* sqrt(2);
				float grad1 = -1 * ((bomb->angle_move)* std::_Pi) / 180;
				int y = (float)g1 * std::cos(grad1);
				int x = (float)g1 * std::sin(grad1);
				eff_slide = effCont.AddEffect("Iskra",FPoint( bomb->cur_pos.x-x, bomb->cur_pos.y - y));
			}
			break;

		}

	}
	catch (...)
	{
		P::I().Error(1, ERROR_MSG +" TestWidget::MouseUp " + P::I().SysErrMsg());
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	// заверешение программы по кл. Esc
	if (keyCode == VK_ESCAPE) {	
		set_level(255);
		strvect->clear();
		Core::Window().Destroy();
	}
}

void TestWidget::CharPressed(int unicodeChar)
{

}
