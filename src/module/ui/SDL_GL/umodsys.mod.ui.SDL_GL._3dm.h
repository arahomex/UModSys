// 
// RSDLVideoSurfaceGL::
// 

// ----------------------------------------------------------------------
// -- material
// ----------------------------------------------------------------------

void RSDLVideoSurfaceGL::emul_mmap(int idx)
{
  // disable old ones
  for(int i=idx; i<gl.tex_levels; i++) {
    gl.set_stage_ARB(i);
    gl.set_ctc_ARB(false);
    gl.set_tex2d_ARB(false);
  }
  for(int i=gl.tex_levels; i<idx; i++) {
    gl.set_stage_ARB(i);
    gl.set_ctc_ARB(false);
    gl.set_tex2d_ARB(true);
  }
  gl.tex_levels = idx;
}

bool RSDLVideoSurfaceGL::setup_material(const SMaterial *mat) {
  if(disabled_setup || disabled_render)
    return true;
  //
  int nmaps = 0;
  // reset
  es_alpha = -1; 
  es_transmode = -2;
  //
  if(mat==NULL) {
    // disable old ones
    for(int i=0; i<gl.tex_levels; i++) {
      gl.set_stage_ARB(i);
      gl.set_ctc_ARB(false);
      gl.set_tex2d_ARB(false);
    }
    gl.tex_levels = 0;
    gl.glDisable(GL_LIGHTING);
    gl.glShadeModel(GL_SMOOTH);
    emul_coloralpha_reset();
  } else {
    if(mat->components & mc_Texture) {
      const lib3d::SMaterialTexture* mt;
      int i, texlev = 0, oldlev = gl.tex_levels, ntex = mat->num_textures;
      if(ntex>max_MaterialTextures)
        ntex = max_MaterialTextures;
      //
      // enable used levels in order rmfc_Replace, rmfc_Lightmap, all other
      //
      mt = mat->textures;
      for(i=0; i<ntex; i++, mt++) {
        if(mt->flags.combine!=rmfc_Replace || mt->texture==NULL) {
          continue;
        }
        if(texlev>=gl.max_tex_level)
          break;
        tex_to_texlevel[i] = texlev;
        texlevel_to_tex[texlev] = i;
        set_tex(texlev, mt);
        texlev++;
      }
      //
      mt = mat->textures;
      for(i=0; i<ntex; i++, mt++) {
        if(mt->flags.combine!=rmfc_Lightmap || mt->texture==NULL) {
          continue;
        }
        if(texlev>=gl.max_tex_level)
          break;
        tex_to_texlevel[i] = texlev;
        texlevel_to_tex[texlev] = i;
        set_tex(texlev, mt, rmfc_Add);
        texlev++;
      }
      //
      mt = mat->textures;
      for(i=0; i<ntex; i++, mt++) {
        if(mt->flags.combine==rmfc_Lightmap || mt->flags.combine==rmfc_Replace || mt->texture==NULL) {
          continue;
        }
        if(texlev>=gl.max_tex_level)
          break;
        tex_to_texlevel[i] = texlev;
        texlevel_to_tex[texlev] = i;
        set_tex(texlev, mt);
        texlev++;
      }
      //
      // disable unused levels
      for(i=texlev; i<oldlev; i++) {
        gl.set_stage_ARB(i);
        gl.set_ctc_ARB(false);
        gl.set_tex2d_ARB(false);
      }
      //
    } else {
      // disable old ones
      for(int i=0; i<gl.tex_levels; i++) {
        gl.set_stage_ARB(i);
        gl.set_ctc_ARB(false);
        gl.set_tex2d_ARB(false);
      }
      gl.tex_levels = 0;
    }
    //
    if(light_enabled && (mat->components & mc_NoLight)==0) {
      gl.glEnable(GL_LIGHTING);
      gl.glShadeModel(GL_SMOOTH);
      if(mat->components & mc_Ambient) {
        emul_material(GL_FRONT, GL_AMBIENT, mat->ambient);
      } else {
        emul_material(GL_FRONT, GL_AMBIENT, color_zero);
      }
      if(mat->components & mc_Specular) {
        emul_material(GL_FRONT, GL_SPECULAR, mat->specular);
      } else {
        emul_material(GL_FRONT, GL_SPECULAR, color_zero);
      }
      if(mat->components & mc_Diffuse) {
        emul_material(GL_FRONT, GL_DIFFUSE, mat->diffuse);
      } else {
        emul_material(GL_FRONT, GL_DIFFUSE, color_zero);
      }
      gl.glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    } else if(mat->components & mc_NoLight) {
      gl.glDisable(GL_LIGHTING);
      gl.glShadeModel(GL_SMOOTH);
      emul_material(GL_FRONT_AND_BACK, GL_AMBIENT, f_color_zero);
      if(mat->components & mc_Ambient) {
        DColor c(mat->ambient.v[0] * saved_color.v[0], mat->ambient.v[1] * saved_color.v[1], mat->ambient.v[2] * saved_color.v[2]);
        float av = mat->ambient.v[3] * saved_alpha;
        if(c[2]==0) {
          c[2] = 0;
        }
        emul_coloralpha(&c, &av, saved_transmode);
      } else {
        emul_coloralpha_reset();
      }
      gl.glMaterialfv(GL_FRONT, GL_DIFFUSE, f_color_zero);
      gl.glMaterialfv(GL_FRONT, GL_SPECULAR, f_color_zero);
      gl.glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    } else {
      gl.glEnable(GL_LIGHTING);
      gl.glShadeModel(GL_SMOOTH);
      if(mat->components & mc_Ambient) {
        emul_material(GL_FRONT_AND_BACK, GL_AMBIENT, mat->ambient);
      } else {
        emul_material(GL_FRONT_AND_BACK, GL_AMBIENT, color_default);
        emul_coloralpha_reset();
      }
      gl.glMaterialfv(GL_FRONT, GL_DIFFUSE, f_color_zero);
      gl.glMaterialfv(GL_FRONT, GL_SPECULAR, f_color_zero);
      gl.glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    }
  }
  //
  list_updated = true;
  return true;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
