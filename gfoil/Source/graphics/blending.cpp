#include "blending.h"

bool gfoil::blending::is_enabled;
gfoil::blending::factors gfoil::blending::s_factor;
gfoil::blending::factors gfoil::blending::d_factor;

void gfoil::blending::enable() {
	if (is_enabled)
		return;
	glEnable(GL_BLEND);
	is_enabled = true;
}
void gfoil::blending::disable() {
	if (!is_enabled)
		return;
	glDisable(GL_BLEND);
	is_enabled = false;
}
void gfoil::blending::set_function(factors s, factors d) {
	if ((s_factor == s) && (d_factor == d))
		return;
	glBlendFunc((GLenum)s, (GLenum)d);
	s_factor = s;
	d_factor = d;
}