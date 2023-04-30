#include <core/object/builtins/property_object.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PropertyObject, t, TypeFlags_MethodDescriptor)
	{
		t.tp_descr_get = (DescrGetFunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
		{
			return ((PROPERTY &)self).get(obj);
		};

		t.tp_descr_set = (DescrSetFunc)[](OBJ self, OBJ obj, OBJ value) -> Error_
		{
			return ((PROPERTY &)self).set(obj, value);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}