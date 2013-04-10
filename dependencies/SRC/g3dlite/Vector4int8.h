/**
  @file Vector4int8.h
 
  Homogeneous vector class.

  @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 
  @created 2007-02-09
  @edited  2007-02-09

  Copyright 2000-2007, Morgan McGuire.
  All rights reserved.
 */

#ifndef G3D_Vector4int8_H
#define G3D_Vector4int8_H

#include "platform.h"
#include "g3dmath.h"

namespace G3D {

class Vector3;
class Vector4;

/**
 Homogeneous vector stored efficiently in four signed g3d_int8s.

 */
class Vector4int8 {
private:
    // Hidden operators
    bool operator<(const Vector4int8&) const;
    bool operator>(const Vector4int8&) const;
    bool operator<=(const Vector4int8&) const;
    bool operator>=(const Vector4int8&) const;

  
    /** For fast operations, treat this packed data structure as 
      an g3d_int32 */
    inline g3d_uint32& asg3d_int32() {
        return *reinterpret_cast<g3d_uint32*>(this);
    }

    inline const g3d_uint32& asg3d_int32() const {
        return *reinterpret_cast<const g3d_uint32*>(this);
    }

public:
    // construction
    inline Vector4int8() : x(0), y(0), z(0), w(0) {}
    
    /** Multiplies the source by 127 and clamps to (-128, 127) when converting */
    Vector4int8(const Vector4& source);

    /** Multiplies the source by 127 and clamps to (-128, 127) when converting */
    Vector4int8(const Vector3& source, g3d_int8 w);

    inline Vector4int8(g3d_int8 x, g3d_int8 y, g3d_int8 z, g3d_int8 w) : x(x), y(y), z(z), w(w) {}

    Vector4int8(class BinaryInput& b);
    void serialize(class BinaryOutput& b) const;
    void deserialize(class BinaryInput& b);

    // coordinates
    g3d_int8 x, y, z, w;

    inline operator g3d_int8* () {
        return reinterpret_cast<g3d_int8*>(this);
    }

    inline operator const g3d_int8* () const {
        return reinterpret_cast<const g3d_int8*>(this);
    }

    // access vector V as V[0] = V.x, V[1] = V.y, V[2] = V.z, etc.
    //
    // WARNING.  These member functions rely on
    // (1) Vector4int8 not having virtual functions
    // (2) the data packed in a 4*sizeof(g3d_int8) memory block
    inline g3d_int8& operator[] (int i) {
        debugAssert(i >= 0 && i <= 4);
        return ((g3d_int8*)this)[i];
    }

    const g3d_int8& operator[] (int i) const {
        debugAssert(i >= 0 && i <= 4);
        return ((const g3d_int8*)this)[i];
    }

    // assignment and comparison
    Vector4int8& operator= (const Vector4int8& other) {
        asg3d_int32() = other.asg3d_int32();
        return *this;
    }

    inline bool operator== (const Vector4int8& other) const {
        return asg3d_int32() == other.asg3d_int32();
    }

    inline bool operator!= (const Vector4int8& other) const {
        return ! (*this == other);
    }

    inline unsigned int hashCode() const {
        return asg3d_int32();
    }
};

} // namespace G3D


#endif
