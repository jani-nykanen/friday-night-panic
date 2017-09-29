/// Vectors (header)
/// (c) 2017 Jani Nykänen

#ifndef __VECTOR__
#define __VECTOR__

/// Vector 3 type
typedef struct
{
    float x,y,z;
}
VEC3;

/// Create new vector 3
/// < x X component
/// < y Y component
/// < z Z component
/// > A new vector 3
#define vec3(x,y,z) (VEC3){x,y,z}

/// Vector 2 type
typedef struct
{
    float x,y;
}
VEC2;

/// Create new vector 2
/// < x X component
/// < y Y component
/// > A new vector 2
#define vec2(x,y) (VEC2){x,y}

#endif // __VECTOR__