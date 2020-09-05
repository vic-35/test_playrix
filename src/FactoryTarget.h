#pragma once

#include "Targets.h"

typedef std::shared_ptr<Target> sm_target;
// createTarget ������ ������� ������� ���� ����� �������������� � ���������� ��������� - ����������� ������ Factory

// ������� �������
class FactoryTarget
{
public:
	FactoryTarget() {};
	virtual ~FactoryTarget() {};
	virtual sm_target createTarget() = 0;
};

// ������� ������ 1
class Target1Factory : public FactoryTarget
{
public:
	sm_target createTarget();
};
// ������� ������ 2
class Target2Factory : public FactoryTarget
{
public:
	sm_target createTarget();
};
// ������� �������������� �������  ����� ����� � ��.
class TargetUFactory : public FactoryTarget
{
public:
	sm_target createTarget() { return NULL; };
	sm_target createTarget(std::string _text);
};

// ������� ���������� �������
class TargetSFactory : public FactoryTarget
{
public:
	sm_target createTarget() { return NULL; };
	sm_target createTarget(int _id_text, std::string _font, IPoint _position);
};



