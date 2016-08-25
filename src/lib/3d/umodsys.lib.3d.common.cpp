#include <umodsys/lib/3d/lib3d.typedefs.h>
#include <umodsys/lib/3d/lib3d.material.h>
#include <umodsys/lib/3d/lib3d.control.h>
#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.geom.h>
#include <umodsys/lib/3d/lib3d.light.h>
#include <umodsys/lib/3d/lib3d.logic.h>
#include <umodsys/lib/3d/lib3d.material.h>
#include <umodsys/lib/3d/lib3d.objects.h>
#include <umodsys/lib/3d/lib3d.outlines.h>
#include <umodsys/lib/3d/lib3d.physics.h>
#include <umodsys/lib/3d/lib3d.renderer.h>
#include <umodsys/lib/3d/lib3d.scene.h>
#include <umodsys/lib/3d/lib3d.visual.h>
#include <umodsys/lib/3d/lib3d.e_font.h>
#include <umodsys/lib/3d/lib3d.e_light.h>
#include <umodsys/lib/3d/lib3d.e_outline.h>

#include <umodsys/common/syshlp.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::lib3d;


//***************************************
// lib3d::
//***************************************

sint8 lib3d::cube_face_normals[6][4] = { // [faces]{x,y,z,_}
  {0, 1,0},    // Top
  {0,-1,0},    // Bottom
  {0,0, 1},    // Front
  {0,0,-1},    // Back
  {-1,0,0},    // Left
  { 1,0,0},    // Right
};

//***************************************

uint8 lib3d::cube_quad_texpoints[6][4][2] = { // [faces][verts]{u,v}
  {
    {1,0},    // Top Right Of The Quad (Top)
    {0,0},    // Top Left Of The Quad (Top)
    {0,1},    // Bottom Left Of The Quad (Top)
    {1,1},    // Bottom Right Of The Quad (Top)
  },
  {
    {1,1},    // Top Right Of The Quad (Bottom)
    {0,1},    // Top Left Of The Quad (Bottom)
    {0,0},    // Bottom Left Of The Quad (Bottom)
    {1,0},    // Bottom Right Of The Quad (Bottom)
  },
  {
    {1,1},    // Top Right Of The Quad (Front)
    {0,1},    // Top Left Of The Quad (Front)
    {0,0},    // Bottom Left Of The Quad (Front)
    {1,0},    // Bottom Right Of The Quad (Front)
  },
  {
    {1,0},    // Top Right Of The Quad (Back)
    {0,0},    // Top Left Of The Quad (Back)
    {0,1},    // Bottom Left Of The Quad (Back)
    {1,1},    // Bottom Right Of The Quad (Back)
  },
  {
    {1,1},    // Top Right Of The Quad (Left)
    {1,0},    // Top Left Of The Quad (Left)
    {0,0},    // Bottom Left Of The Quad (Left)
    {0,1},    // Bottom Right Of The Quad (Left)
  },
  {
    {1,0},    // Top Right Of The Quad (Right)
    {1,1},    // Top Left Of The Quad (Right)
    {0,1},    // Bottom Left Of The Quad (Right)
    {0,0},    // Bottom Right Of The Quad (Right)
  },
};

//***************************************

uint8 lib3d::cube_quad_points[6][4][4] = { // [faces][verts]{x,y,z,_}
  {
    {1,1,0},    // Top Right Of The Quad (Top)
    {0,1,0},    // Top Left Of The Quad (Top)
    {0,1,1},    // Bottom Left Of The Quad (Top)
    {1,1,1},    // Bottom Right Of The Quad (Top)
  },
  {
    {1,0,1},    // Top Right Of The Quad (Bottom)
    {0,0,1},    // Top Left Of The Quad (Bottom)
    {0,0,0},    // Bottom Left Of The Quad (Bottom)
    {1,0,0},    // Bottom Right Of The Quad (Bottom)
  },
  {
    {1,1,1},    // Top Right Of The Quad (Front)
    {0,1,1},    // Top Left Of The Quad (Front)
    {0,0,1},    // Bottom Left Of The Quad (Front)
    {1,0,1},    // Bottom Right Of The Quad (Front)
  },
  {
    {1,0,0},    // Top Right Of The Quad (Back)
    {0,0,0},    // Top Left Of The Quad (Back)
    {0,1,0},    // Bottom Left Of The Quad (Back)
    {1,1,0},    // Bottom Right Of The Quad (Back)
  },
  {
    {0,1,1},    // Top Right Of The Quad (Left)
    {0,1,0},    // Top Left Of The Quad (Left)
    {0,0,0},    // Bottom Left Of The Quad (Left)
    {0,0,1},    // Bottom Right Of The Quad (Left)
  },
  {
    {1,1,0},    // Top Right Of The Quad (Right)
    {1,1,1},    // Top Left Of The Quad (Right)
    {1,0,1},    // Bottom Left Of The Quad (Right)
    {1,0,0},    // Bottom Right Of The Quad (Right)
  },
};

//***************************************
// ::
//***************************************
