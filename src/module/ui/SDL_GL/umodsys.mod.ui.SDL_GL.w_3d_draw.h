
//***************************************
// RRenderDriver3D::
//***************************************

//
//***************************************
// -- render primitives by lists
// enable lists, up to 4, first list always have rpc_Point, other never have it
/*
bool RRenderDriver3D::setup_lists(int render_primitive_components, const int* list, int listid=0)
{
  return false;
}

bool RRenderDriver3D::setup_lists2(int count, int render_primitive_components, const int* list, ...)
{
  return false;
}

bool RRenderDriver3D::render_primitive(eRenderPrimitiveType primitive, int count, int starts=0, ...)
{
  return false;
}

bool RRenderDriver3D::render_primitive(eRenderPrimitiveType primitive, int count, const int* starts)
{
  return false;
}
*/

bool RRenderDriver3D::render_primitive(eRenderPrimitiveType primitive, int count, int starts=0)
{
  if(!IsPhmV(phm_3D))
    return false;
  GLenum mode;
  switch(primitive) {
    case rp_Point:       mode = GL_POINTS; break;
    case rp_Line:        mode = GL_LINES; break;
    case rp_Triangle:    mode = GL_TRIANGLES; break;
    case rp_Quad:        mode = GL_QUADS; break;
    case rp_Poly:        mode = GL_POLYGON; break;
    case rp_Strip:       mode = GL_TRIANGLE_STRIP; break;
    case rp_Fan:         mode = GL_TRIANGLE_FAN; break;
    case rp_PolyLine:    mode = GL_LINE_LOOP; break;
    case rp_MultiLine:   mode = GL_LINE_STRIP; break;
    //
    case rp_Null:
    case rp_Unknown:
    default:
      return false;
  }
  gl.glDrawArrays(mode, starts, count);
  return true;
}

//***************************************
// -- render pictures

void RRenderDriver3D::render_picture(const SPicture& A)
{
}

void RRenderDriver3D::render_picture(const SPicture& A, const SPicture& B, DColorElem trans)
{
}

void RRenderDriver3D::render_particles(const SObjectParticle* op)
{
}

//***************************************
// -- render text

void RRenderDriver3D::render_text_3d(BStrL piclist, int count)
{
}

//***************************************
// END
//***************************************
