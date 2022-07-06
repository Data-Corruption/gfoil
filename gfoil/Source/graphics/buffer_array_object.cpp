#include "buffer_array_object.h"

#include "../system/system.h"

// ----==== Members ====----

unsigned int gfoil::buffer_array_object::bound_buffer_array_object_ID = 0;

// ----==== Methods ====----

void gfoil::buffer_array_object::generate() {
	if (this->ID != 0) {
		gfoil::system::log::warn("Attempting to generate a buffer array object that has already been generated!");
	}else {
		glGenVertexArrays(1, &this->ID);
	}
}
void gfoil::buffer_array_object::destroy() {
	if (this->ID == 0) {
		gfoil::system::log::warn("Attempting to destroy a buffer array object that is not generated!");
	} else {
		glDeleteVertexArrays(1, &this->ID);
		this->ID = 0;
	}
}

void gfoil::buffer_array_object::bind() {
	if (this->ID == 0)
		system::log::error("Attempting to bind a buffer array object that is not generated!");

	if (bound_buffer_array_object_ID != this->ID) { 
		glBindVertexArray(this->ID); 
		bound_buffer_array_object_ID = this->ID;
	}
}
void gfoil::buffer_array_object::unbind() {
	if (bound_buffer_array_object_ID != 0) {
		glBindVertexArray(0);
		bound_buffer_array_object_ID = 0;
	}
}