#include "stdafx.h"
#include "P.h"
#include "FactoryTarget.h"


// createTarget каждая фабрика создает свой класс инициализирует и возвращает указатель - стандартный шаблон Factory

sm_target Target1Factory::createTarget() {
	sm_target t(new Target1);
	t->t = Core::resourceManager.Get<Render::Texture>("Target1");
	t->CalcRect(P::I().zoom_t1, 0, 0);
	t->cur_pos = IPoint(P::I().GetRandom(0,WINDOW_WIDTH), P::I().GetRandom(WINDOW_FREE_ZONE, WINDOW_HEIGHT));

	t->angle_move = P::I().GetRandom(0, 360);
	t->speed_move= P::I().GetRandom(2, 5);


	return t;
}

sm_target Target2Factory::createTarget() {
	sm_target t(new Target2);
	t->t = Core::resourceManager.Get<Render::Texture>("Target2");
	t->CalcRect(P::I().zoom_t2, 0, 0);
	t->cur_pos = IPoint(P::I().GetRandom(0, WINDOW_WIDTH), P::I().GetRandom(WINDOW_FREE_ZONE, WINDOW_HEIGHT));

	t->angle_move = P::I().GetRandom(0, 360);
	t->speed_move = P::I().GetRandom(2, 5);

	t->rotate = P::I().GetRandom(1, 4);
	return t;
}

sm_target TargetUFactory::createTarget(std::string _text) {
	sm_target t(new TargetU());
	t->t = Core::resourceManager.Get<Render::Texture>(_text);
	return t;
}



sm_target TargetSFactory::createTarget(int _id_text, std::string _font, IPoint _position) {
	sm_target t(new TargetString(P::I().S(_id_text), _font, _position));
	return t;
}


