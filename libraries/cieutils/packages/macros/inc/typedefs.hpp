#ifndef CIE_UTILS_MACROS_TYPEDEFS_HPP
#define CIE_UTILS_MACROS_TYPEDEFS_HPP

// --- STL Includes ---
#include <memory>


/// Define smart pointers to the class as member typedefs
#define CIE_DEFINE_CLASS_POINTERS( CLASS_NAME )         \
    using SharedPointer = std::shared_ptr<CLASS_NAME>;  \
    using UniquePointer = std::unique_ptr<CLASS_NAME>;  \
    using WeakPointer   = std::weak_ptr<CLASS_NAME>;



#endif