/**
 @file BinaryFormat.h
 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 
 @author  2005-06-03
 @edited  2005-06-03

 Copyright 2000-2005, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_BINARYFORMAT_H
#define G3D_BINARYFORMAT_H

#include "platform.h"
#include "g3dmath.h"

namespace G3D {

class Vector2;
class Vector2int16;
class Vector3;
class Vector3int16;
class Vector4;
class Vector4g3d_int16;
class Color3;
class Color3uint8;
class Color4;
class Color4uint8;

/**
 Some values like float16 and int128 have no current CPU data structure that implements them but are useful
 for file formats and for GPUs.

 CHUNK_BINFMT data follows the protocol.
 */
// Must be packed int 16 bits for the chunk reader
// We can't name these just "g3d_int8" etc. because some libraries #define names like that
enum BinaryFormat {
    FIRST_BINFMT = 1000,

    BOOL8_BINFMT,
    g3d_uint8_BINFMT, g3d_int8_BINFMT, g3d_uint16_BINFMT, g3d_int16_BINFMT, g3d_uint32_BINFMT, g3d_int32_BINFMT, g3d_uint64_BINFMT, g3d_int64_BINFMT, UINT128_BINFMT, INT128_BINFMT,
    FLOAT16_BINFMT, FLOAT32_BINFMT, FLOAT64_BINFMT,
    VECTOR2_BINFMT, Vector2int16_BINFMT,
    VECTOR3_BINFMT, Vector3int16_BINFMT,
    VECTOR4_BINFMT, VECTOR4g3d_int16_BINFMT,
    COLOR3_BINFMT, Color3uint8_BINFMT, COLOR3g3d_int16_BINFMT,
    COLOR4_BINFMT, Color4uint8_BINFMT, COLOR4g3d_int16_BINFMT,
    STRING_BINFMT, STRINGEVEN_BINFMT, STRING8_BINFMT, STRING16_BINFMT, STRING32_BINFMT,

    CHUNK_BINFMT,

    CUSTOM_BINFMT,

    LAST_BINFMT
};

}

/** A macro that maps G3D types to format constants.
    (e.g. binaryFormatOf(Vector3) == VECTOR3_BINFMT).
*/
// This implementation is designed to meet the following constraints:
//   1. Work around the many MSVC++ partial template bugs
//   2. Work for primitive types (e.g. int)
#define binaryFormatOf(T) (G3D::_internal::_BinaryFormat<T>::x())

namespace G3D {
namespace _internal {


template<class T> class _BinaryFormat {
public:
    static BinaryFormat x() {
        return CUSTOM_BINFMT;
    }
};
}}


/**
 Macro to declare the underlying format (as will be returned by glFormatOf)
 of a type.  For example,

  <PRE>
    DECLARE_BINARYFORMATOF(Vector4, VECTOR4_BINFMT)
  </PRE>

  Use this so you can make vertex arrays of your own classes and not just 
  the standard ones.
 */
#define DECLARE_BINARYFORMATOF(CType, EnumType)      \
namespace G3D {                                      \
    namespace _internal {                            \
        template<> class _BinaryFormat<CType> {      \
        public:                                      \
        static BinaryFormat x()  {                   \
                return EnumType;                     \
            }                                        \
        };                                           \
    }                                                \
}

DECLARE_BINARYFORMATOF( bool,          BOOL8_BINFMT )

DECLARE_BINARYFORMATOF( g3d_uint8,         g3d_uint8_BINFMT )
DECLARE_BINARYFORMATOF( g3d_int8,          g3d_int8_BINFMT )
DECLARE_BINARYFORMATOF( g3d_uint16,        g3d_uint16_BINFMT )
DECLARE_BINARYFORMATOF( g3d_int16,         g3d_int16_BINFMT )
DECLARE_BINARYFORMATOF( g3d_uint32,        g3d_uint32_BINFMT )
DECLARE_BINARYFORMATOF( g3d_int32,         g3d_int32_BINFMT )
DECLARE_BINARYFORMATOF( g3d_uint64,        g3d_uint64_BINFMT )
DECLARE_BINARYFORMATOF( g3d_int64,         g3d_int64_BINFMT )

DECLARE_BINARYFORMATOF( float32,       FLOAT32_BINFMT )
DECLARE_BINARYFORMATOF( float64,       FLOAT64_BINFMT )

DECLARE_BINARYFORMATOF( Vector2,       VECTOR2_BINFMT )
DECLARE_BINARYFORMATOF( Vector2int16,  Vector2int16_BINFMT )
DECLARE_BINARYFORMATOF( Vector3,       VECTOR3_BINFMT )
DECLARE_BINARYFORMATOF( Vector3int16,  Vector3int16_BINFMT )
DECLARE_BINARYFORMATOF( Vector4,       VECTOR4_BINFMT )
DECLARE_BINARYFORMATOF( Vector4g3d_int16,  VECTOR4g3d_int16_BINFMT )

DECLARE_BINARYFORMATOF( Color3,        COLOR3_BINFMT )
DECLARE_BINARYFORMATOF( Color3uint8,   Color3uint8_BINFMT )
DECLARE_BINARYFORMATOF( Color4,        COLOR4_BINFMT )
DECLARE_BINARYFORMATOF( Color4uint8,   Color4uint8_BINFMT )

namespace G3D {

/** Returns -1 if the format is custom, otherwise the byte size
    of a single element in this format.*/ 
g3d_int32 byteSize(BinaryFormat f);


} //G3D

#endif 
