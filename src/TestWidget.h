#pragma once
#include "FactoryTarget.h"

///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;
	void set_level(int _level);
	std::shared_ptr<TargetString> str(int _id_text); // возвращает указатель на обект-строка TargetString

private:
	void Init(); // первоначальная инициализация обьектов
	void LoadObj(); // загрузка и создание мишеней
	
	//фабрики создания обьектов
	std::unique_ptr<Target1Factory> factory1;
	std::unique_ptr<Target2Factory> factory2;
	std::unique_ptr<TargetUFactory> factoryU;
	std::unique_ptr<TargetSFactory> factoryS;
	
	//массив обьектов мишени
	std::unique_ptr<std::vector<sm_target>> target;
	//массив обьектов строки
	std::unique_ptr<std::vector<sm_target>> strvect;

	sm_target sight; // прицел
	sm_target cannon; // пушка
	sm_target bomb; // бомба
	sm_target background; // фон 

	//Контейнер эффектов и указатели на эффекты
	EffectsContainer	effCont;
	ParticleEffectPtr eff_boom;
	ParticleEffectPtr eff_slide;
	ParticleEffectPtr eff_target;


	float timer; // таймер
	int level; // номер сцены - обпределяет какие обьекты будут активны
	
	float last_dt; // отсчет времени для анализа положения обьектов
	float last_check;




};
