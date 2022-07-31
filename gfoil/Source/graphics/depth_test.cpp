#include "depth_test.h"

bool gfoil::depth_test::is_enabled;
gfoil::depth_test::functions gfoil::depth_test::function;

void gfoil::depth_test::enable() {
	if (is_enabled)
		return;
	glEnable(GL_DEPTH_TEST);
	is_enabled = true;
}
void gfoil::depth_test::disable() {
	if (!is_enabled)
		return;
	glDisable(GL_DEPTH_TEST);
	is_enabled = false;
}
void gfoil::depth_test::set_function(functions f) {
	if (function == f)
		return;
	glDepthFunc((GLenum)f);
	function = f;
}