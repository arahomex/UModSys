//------------------------------------
// RVisualizerScene::
//------------------------------------

//------------------------------------
//------------------------------------
// IVisualizer

//------------------------------------
// links

IRenderer* RVisualizerScene::renderer_get(void) const
{
  return renderer;
}

bool RVisualizerScene::renderer_set(IRenderer* r)
{
  renderer = r;
  clear_cache();
  return true;
}

//------------------------------------
// visualize

bool RVisualizerScene::render_3d(int phaseid, bool isalpha, IRenderDriver* drv)
{
  queue.Clear();
  ObjectQueueSocket qs(queue);
  bool ret = true;
  for(size_t i=0; i<~scenes; i++) {
    const Scene& s = scenes(i);
    if(!s.mapper->queue_fill(qs, phaseid, isalpha))
      ret = false;
  }
  //
  SRenderState S(renderer, drv);
  for(size_t i=0; i<~queue; i++) {
    if( S.render(queue(i)) )
      ret = false;
  }
  queue.Clear();
  return ret;
}

bool RVisualizerScene::render_2d(IRenderDriver* drv)
{
  return false;
}

//------------------------------------
//------------------------------------
// IVisualizerScene

//------------------------------------
// scene

size_t RVisualizerScene::scene_count(void) const
{
  return ~scenes;
}

IVisualizerScene::Scene RVisualizerScene::scene_get(size_t idx) const
{
  if(idx<~scenes)
    return scenes(idx);
  return Scene(NULL, NULL);
}

size_t RVisualizerScene::scene_index(const Scene &s) const
{
  if(!s.valid()) {
    for(size_t i=0; i<~scenes; i++) {
      if(scenes(i)==s)
        return i;
    }
  }
  return array_index_none;
}

bool RVisualizerScene::scene_add(const Scene &s)
{
  if(!s.valid() || scene_index(s)!=array_index_none)
    return false;
  if(!scenes.Push(s))
    return false;
  s.mapper->renderer_set(renderer);
  s.mapper->node_update_all(s.scene);
  return true;
}

bool RVisualizerScene::scene_remove(const Scene &s)
{
  if(!s.valid())
    return false;
  size_t idx = scene_index(s);
  if(idx==array_index_none)
    return false;
  return scenes.RemoveAt(idx);
}

//------------------------------------
//------------------------------------

void RVisualizerScene::clear_cache(void)
{
  for(size_t i=0; i<~scenes; i++) {
    const Scene& s = scenes(i);
    s.mapper->renderer_set(renderer);
    s.mapper->node_update_all(s.scene);
  }
}

//------------------------------------
//------------------------------------

RVisualizerScene::RVisualizerScene(DOwner *pv, const SParameters& args)
: refs(pv) 
{
}

RVisualizerScene::~RVisualizerScene(void)
{
}

//------------------------------------
// 
//------------------------------------
