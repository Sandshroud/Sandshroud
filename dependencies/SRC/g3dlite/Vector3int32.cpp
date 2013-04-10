/**
 @file Vector3int32.cpp
 
 @author Morgan McGuire, http://graphics.cs.williams.edu
  
 @created 2008-07-01
 @edited  2008-07-01
 */

#include "platform.h"
#include "g3dmath.h"
#include "Vector3int32.h"
#include "Vector3int16.h"
#include "Vector3.h"
#include "BinaryInput.h"
#include "BinaryOutput.h"
#include "format.h"

namespace G3D {

Vector3int32::Vector3int32(const class Vector3& v) {
    x = (g3d_int32)iFloor(v.x + 0.5);
    y = (g3d_int32)iFloor(v.y + 0.5);
    z = (g3d_int32)iFloor(v.z + 0.5);
}


Vector3int32::Vector3int32(const class Vector3int16& v) {
    x = v.x;
    y = v.y;
    z = v.z;
}


Vector3int32::Vector3int32(class BinaryInput& bi) {
    deserialize(bi);
}


void Vector3int32::serialize(class BinaryOutput& bo) const {
    bo.writeg3d_int32(x);
    bo.writeg3d_int32(y);
    bo.writeg3d_int32(z);
}


void Vector3int32::deserialize(class BinaryInput& bi) {
    x = bi.readg3d_int32();
    y = bi.readg3d_int32();
    z = bi.readg3d_int32();
}

std::string Vector3int32::toString() const {
    return G3D::format("(%d, %d, %d)", x, y, z);
}

}
