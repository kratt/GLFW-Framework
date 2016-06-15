#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "Transform.h"
#include "GlobalObjectParam.h"

struct RenderContext {
	
	static GlobalObjectParam* globalObjectParam();
	static void globalObjectParam(GlobalObjectParam &globalObjectParam);
	static void globalObjectParam(GlobalObjectParam *globalObjectParam);

	
	static Transform* transform();
	static void transform(Transform &transform);
	static void transform(Transform *transform);



protected:
	static GlobalObjectParam *s_globalObjectParam;
	static Transform *s_transform;
	
};


#endif
