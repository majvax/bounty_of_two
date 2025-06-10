#pragma once


/**
 * @brief A utility to build a get method on top of a property.
 *
 * @param type The type of the property.
 * @param method The name of the method to be generated.
 * @param name The machine-readable name of the property.
 */
#ifndef BOO_GETTER_IMPL
#define BOO_GETTER_IMPL(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */ \
    type Get##method() const { \
        return name; \
    }
#endif

/**
 * @brief A utility to build a set method on top of a property.
 * 
 * @param type The type of the property.
 * @param method The name of the method to be generated.
 * @param name The machine-readable name of the property.
 */
#ifndef BOO_SETTER_IMPL
#define BOO_SETTER_IMPL(type, method, name) \
    /** Sets the name value for the object. @param value The value of which to set name to. */ \
    void Set##method(type value) { \
        name = value; \
    }
#endif


#ifndef GETTER_SETTER
/**
 * @brief A utility to build get and set methods on top of a property.
 *
 * @param type The type of the property.
 * @param method The name of the method to be generated.
 * @param name The machine-readable name of the property.
 */
#define GETTER_SETTER(type, method, name) \
    BOO_GETTER_IMPL(type, method, name) \
    BOO_SETTER_IMPL(type, method, name)
#endif
