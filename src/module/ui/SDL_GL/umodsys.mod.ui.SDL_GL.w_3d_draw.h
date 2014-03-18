
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
  return false;
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
