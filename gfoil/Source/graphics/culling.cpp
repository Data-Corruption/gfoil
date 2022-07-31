#include "culling.h"

bool gfoil::culling::is_enabled;
gfoil::culling::targets gfoil::culling::target;

extern void gfoil::culling::enable() {
	if (is_enabled)
		return;
	glEnable(GL_CULL_FACE);
	is_enabled = true;
}
extern void gfoil::culling::disable() {
	if (!is_enabled)
		return;
	glDisable(GL_CULL_FACE);
	is_enabled = false;
}
extern void gfoil::culling::set_target(targets t) {
	if (target == t)
		return;
	glCullFace((GLenum)t);
	target = t;
}