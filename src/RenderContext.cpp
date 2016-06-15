#include "RenderContext.h"

GlobalObjectParam *RenderContext::s_globalObjectParam = nullptr;
Transform *RenderContext::s_transform = nullptr;

GlobalObjectParam* RenderContext::globalObjectParam() {
	if (s_globalObjectParam == nullptr)
		s_globalObjectParam = new GlobalObjectParam();
	return s_globalObjectParam;
}

void RenderContext::globalObjectParam(GlobalObjectParam &globalObjectParam) {
	s_globalObjectParam = &globalObjectParam;
}

void RenderContext::globalObjectParam(GlobalObjectParam *globalObjectParam) {
	s_globalObjectParam = globalObjectParam;
}


Transform* RenderContext::transform() {
	if (s_transform == nullptr)
		s_transform = new Transform();
	return s_transform;
}

void RenderContext::transform(Transform &transform) {
	s_transform = &transform;
}

void RenderContext::transform(Transform *transform) {
	s_transform = transform;
}