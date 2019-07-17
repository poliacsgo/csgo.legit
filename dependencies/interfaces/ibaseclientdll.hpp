#pragma once
#include "../../source-sdk/classes/client_class.hpp"

class i_base_client_dll {
public:
	c_client_class * get_client_classes( ) {
		using original_fn = c_client_class * ( __thiscall* )( i_base_client_dll* );
		return ( *( original_fn** ) this ) [ 8 ]( this );
	}
};