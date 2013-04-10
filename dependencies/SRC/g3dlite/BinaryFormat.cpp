/**
 @file BinaryFormat.cpp
  @maintainer Morgan McGuire, http://graphics.cs.williams.edu

  @created 2005-06-10
  @edited  2005-06-10
 */

#include "BinaryFormat.h"

namespace G3D {

g3d_int32 byteSize(BinaryFormat f) {
    switch (f) {
    case BOOL8_BINFMT:
    case g3d_uint8_BINFMT:
    case g3d_int8_BINFMT:
        return 1;

    case g3d_uint16_BINFMT:
    case g3d_int16_BINFMT:
        return 2;

    case FLOAT16_BINFMT:
        return 2;

    case g3d_uint32_BINFMT:
    case g3d_int32_BINFMT:
    case FLOAT32_BINFMT:
        return 4;

    case FLOAT64_BINFMT:
    case g3d_uint64_BINFMT:
    case g3d_int64_BINFMT:
        return 8;

    case INT128_BINFMT:
    case UINT128_BINFMT:
        return 16;

    case VECTOR2_BINFMT:
        return 2 * 4;

    case Vector2int16_BINFMT:
        return 2 * 2;

    case VECTOR3_BINFMT:
        return 3 * 4;

    case Vector3int16_BINFMT:
        return 3 * 2;

    case VECTOR4_BINFMT:
        return 4 * 4;

    case VECTOR4g3d_int16_BINFMT:
        return 4 * 4;

    case COLOR3_BINFMT:
        return 3 * 4;

    case Color3uint8_BINFMT:
        return 3 * 1;

    case COLOR3g3d_int16_BINFMT:
        return 3 * 2;

    case COLOR4_BINFMT:
        return 4 * 4;

    case Color4uint8_BINFMT:
        return 4 * 1;

    case COLOR4g3d_int16_BINFMT:
        return 4 * 2;

    default:
        return -1;
    }
}
}
