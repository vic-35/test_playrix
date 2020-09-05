#pragma once

#include "Targets.h"

typedef std::shared_ptr<Target> sm_target;
// createTarget каждая фабрика создает свой класс инициализирует и возвращает указатель - стандартный шаблон Factory

// базовая фабрика
class FactoryTarget
{
public:
	FactoryTarget() {};
	virtual ~FactoryTarget() {};
	virtual sm_target createTarget() = 0;
};

// фабрика мишени 1
class Target1Factory : public FactoryTarget
{
public:
	sm_target createTarget();
};
// фабрика мишени 2
class Target2Factory : public FactoryTarget
{
public:
	sm_target createTarget();
};
// фабрика универсального обьекта  пушка бомба и пр.
class TargetUFactory : public FactoryTarget
{
public:
	sm_target createTarget() { return NULL; };
	sm_target createTarget(std::string _text);
};

// фабрика текстового обьекта
class TargetSFactory : public FactoryTarget
{
public:
	sm_target createTarget() { return NULL; };
	sm_target createTarget(int _id_text, std::string _font, IPoint _position);
};



