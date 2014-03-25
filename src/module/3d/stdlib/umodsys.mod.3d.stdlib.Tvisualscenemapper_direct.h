//------------------------------------
// RVisualSceneMapper_Direct::
//------------------------------------

//------------------------------------
//------------------------------------
// IVisualSceneMapper

bool RVisualSceneMapper_Direct::renderer_set(IRenderer* r)
{
  return false;
}

bool RVisualSceneMapper_Direct::node_update_all(IScene* scene)
{
  return false;
}

bool RVisualSceneMapper_Direct::queue_fill(Queue& q, int phaseid, bool isalpha)
{
  return false;
}

//------------------------------------
//------------------------------------
// ISceneProcessor

//------------------------------------
//------------------------------------
// ISceneProcessor

bool RVisualSceneMapper_Direct::on_scene_node_add(IScene* sc, HSceneNode node)
{
  return false;
}

bool RVisualSceneMapper_Direct::on_scene_node_remove(IScene* sc, HSceneNode node)
{
  return false;
}

bool RVisualSceneMapper_Direct::on_scene_node_change(IScene* sc, HSceneNode node)
{
  return false;
}

//------------------------------------
//------------------------------------


//------------------------------------
//------------------------------------

RVisualSceneMapper_Direct::RVisualSceneMapper_Direct(DOwner *pv, const SParameters& args)
: refs(pv) 
{
}

RVisualSceneMapper_Direct::~RVisualSceneMapper_Direct(void)
{
}

//------------------------------------
// 
//------------------------------------
