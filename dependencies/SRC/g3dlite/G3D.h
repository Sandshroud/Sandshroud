/**
 @file G3D.h

 This header includes all of the G3D libraries in
 appropriate namespaces.

 @maintainer Morgan McGuire, http://graphics.cs.williams.edu

 @created 2001-08-25
 @edited  2010-03-20

 Copyright 2000-2010, Morgan McGuire.
 All rights reserved.
*/

#ifndef G3D_G3D_h
#define G3D_G3D_h

#ifndef NOMINMAX
#define NOMINMAX 1
#ifdef min
    #undef min
#endif
#ifdef max
    #undef max
#endif
#endif

#include "platform.h"
#include "units.h"
#include "ParseError.h"
#include "Random.h"
#include "Array.h"
#include "SmallArray.h"
#include "Queue.h"
#include "Crypto.h"
#include "format.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color1.h"
#include "Color3.h"
#include "Color4.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "CoordinateFrame.h"
#include "PhysicsFrame.h"
#include "PhysicsFrameSpline.h"
#include "Plane.h"
#include "Line.h"
#include "Ray.h"
#include "Sphere.h"
#include "Box.h"
#include "Box2D.h"
#include "AABox.h"
#include "WrapMode.h"
#include "Cone.h"
#include "Quat.h"
#include "stringutils.h"
#include "prompt.h"
#include "Table.h"
#include "FileSystem.h"
#include "Set.h"
#include "GUniqueID.h"
#include "BinaryFormat.h"
#include "BinaryInput.h"
#include "BinaryOutput.h"
#include "debug.h"
#include "g3dfnmatch.h"
#include "G3DGameUnits.h"
#include "g3dmath.h"
#include "uint128.h"
#include "fileutils.h"
#include "ReferenceCount.h"
#include "Welder.h"
#include "GMutex.h"
#include "PrecomputedRandom.h"
#include "MemoryManager.h"
#include "AreaMemoryManager.h"
#include "BumpMapPreprocess.h"

template<class T> struct HashTrait< G3D::ReferenceCountedPointer<T> > {
    static size_t hashCode(G3D::ReferenceCountedPointer<T> key) { return reinterpret_cast<size_t>( key.pointer() ); }
};

#include "GImage.h"
#include "CollisionDetection.h"
#include "Intersect.h"
#include "Log.h"
#include "serialize.h"
#include "TextInput.h"
#include "NetAddress.h"
#include "NetworkDevice.h"
#include "System.h"
#include "splinefunc.h"
#include "Spline.h"
#include "UprightFrame.h"
#include "LineSegment.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Color3uint8.h"
#include "Color4uint8.h"
#include "Vector2int16.h"
#include "Vector3int16.h"
#include "Vector3int32.h"
#include "Vector4int8.h"
#include "ConvexPolyhedron.h"
#include "MeshAlg.h"
#include "vectorMath.h"
#include "Rect2D.h"
#include "GCamera.h"
#include "GLight.h"
#include "KDTree.h"
#include "PointKDTree.h"
#include "TextOutput.h"
#include "MeshBuilder.h"
#include "AtomicInt32.h"
#include "GThread.h"
#include "ThreadSet.h"
#include "RegistryUtil.h"
#include "Any.h"
#include "XML.h"
#include "PointHashGrid.h"
#include "Map2D.h"
#include "Image1.h"
#include "Image1uint8.h"
#include "Image3.h"
#include "Image3uint8.h"
#include "Image4.h"
#include "Image4uint8.h"
#include "filter.h"
#include "WeakCache.h"
#include "Pointer.h"
#include "Matrix.h"
#include "ImageFormat.h"

#include <list>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "vmap/ModelInstance.h"
#include "vmap/BIH.h"
#include "vmap/BIHWrapper.h"
#include "vmap/WorldModel.h"
#include "vmap/DynamicTree.h"
#include "vmap/TileAssembler.h"
#include "vmap/MapTree.h"
#include "vmap/VMapManager.h"

#endif

