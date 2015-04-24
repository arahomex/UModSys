use strict;
use warnings;

use File::Basename;
use Data::Dumper;

our $generators;
our $script_path;
our ($fout, $pg, $proj, $xmap);
our ($FILE_PATH, $FNAME, $FPATH, $FEXT);
our ($TARGET_TNAME, $TARGET_CNAME, $TARGET_CONF_TNAME, $TARGET_CONF_CNAME);
our ($PROJECT_NAME, $PROJECT_CONTENTS, $PROJECT_TNAME, $PROJECT_CNAME, $PROJECT_TPNAME, $PROJECT_CPNAME);
our ($PROJECT_ID);
our ($MODE, @PROJECT_INCLUDES, @PROJECT_DEFINES, @PROJECT_DEPENDS, @PROJECT_LIBS, @PROJECT_LIBPATH);
our ($OPT_CFLAGS, $OPT_LDFLAGS, $OPT_CXXFLAGS);
our ($OPT_lib_NAME, $OPT_solib_NAME, $OPT_plugin_NAME, $OPT_console_NAME);
our ($OPT_lib_OUT, $OPT_solib_OUT, $OPT_plugin_OUT, $OPT_console_OUT);
our ($OPT_lib_OPTIONS, $OPT_solib_OPTIONS, $OPT_plugin_OPTIONS, $OPT_console_OPTIONS);
our ($PROJECTGROUP_NAME, $PROJECTGROUP_TNAME, $PROJECTGROUP_CNAME);
our ($FILEGROUP_NAME);

sub makefile_setopt($$$$)
{
   my ($opts, $conf, $name, $value) = @_;
   my $c;
   if(exists $opts->{$conf}) {
     $c = $opts->{$conf};
   } else {
     $c = {};
     $opts->{$conf} = $c;
   }
   $c->{$name} = $value;
}

sub makefile_getopt
{
  my ($conf, $optname, @refs) = @_;
#  print "find opt '$conf'.'$optname' at #".@refs."\n";
  my $lev = 0;
  for my $ref (@refs) {
    $lev++;
    next if not exists $ref->{$conf};
    next if not exists $ref->{$conf}->{$optname};
#    print "  found opt '$conf'.'$optname' => '".$ref->{$conf}->{$optname}." at #$lev'\n";
    return $ref->{$conf}->{$optname};
  }
  return undef;
}

sub makefile_getopta
{
  my ($conf, $optname, @refs) = @_;
#  print "find opt '$conf'.'$optname' at #".@refs."\n";
  my $lev = 0;
  my @rv;
  for my $ref (@refs) {
    $lev++;
    next if not exists $ref->{$conf};
    next if not exists $ref->{$conf}->{$optname};
#    print "  found opt '$conf'.'$optname' => '".$ref->{$conf}->{$optname}." at #$lev'\n";
    push @rv, $ref->{$conf}->{$optname};
  }
  return @rv;
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_genfile_cpp($$$$)
{
  my ($state, $file_info, $template, $filegroup) = @_;
#  print_stack();
#  print "[$template]\n";
  eprint $fout, eval("<<EOT\n".$template->{'file-cpp'}."EOT"), $template->{'file-cpp'};
}

sub makefile_genfile_c($$$$)
{
  my ($state, $file_info, $template, $filegroup) = @_;
  eprint $fout, eval("<<EOT\n".$template->{'file-c'}."EOT");
}

sub makefile_genfile_S($$$$)
{
  my ($state, $file_info, $template, $filegroup) = @_;
  eprint $fout, eval("<<EOT\n".$template->{'file-S'}."EOT");
}

my $u_suffix = qr/\.[^.]*/;

sub makefile_genfile($$$$)
{
  my ($state, $file_info, $template, $filegroup) = @_;
  local $pg = $state->{'pg'};
  local $proj = $state->{'project'};
  local $xmap = $state->{'xmap'};
  local $FILE_PATH = $file_info->{'filename'};
  #
  eprint $fout, eval("<<EOT\n".$template->{'project-ff-file-begin'}."EOT");
  #
  local ($FNAME, $FPATH, $FEXT) = fileparse($FILE_PATH, $u_suffix);
  my $G = $template->{'file_generators'};
#  print "[$FNAME $FPATH $FEXT]\n";
  if(exists $G->{$FEXT}) {
#    print "[$template]\n";
    $G->{$FEXT}->($state, $file_info, $template, $filegroup);
  }
  #
  eprint $fout, eval("<<EOT\n".$template->{'project-ff-file-end'}."EOT");
}


#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_mask_end($)
{
  my ($this) = @_;
  my $pfiles = $this->{'parent'}->{'group'}->{'files'};
  for my $file (@{$this->{'files'}}) {
    push @$pfiles, makefile_filegroup_fileinfo($file, $this->{'xopts'});
  }
}

sub makefile_mask_cmd($$$)
{
  my ($this, $cmd, $args) = @_;
  if($cmd eq 'include') {
    while(my $mask = get_configuration_arg_exp(\$args, $this->{'parent'})) {
      my $mypath = $this->{'name'};
      my $path = filename_skipdirs($mypath, $this->{'skip'});
      my $files = filename_search_files($path, $mask);
      if(defined $files) {
        my $nv = $this->{'files'};
        for my $file (@$files) {
          push @$nv, $mypath . (substr $file, length $path);
        }
      }
    }
  } elsif($cmd eq 'exclude') {
    while(my $mask = get_configuration_arg_exp(\$args, $this->{'parent'})) {
      my $nv = [];
      for my $file (@{$this->{'files'}}) {
        push @$nv, $file if not filename_mask_match($mask, $file);
      }
      $this->{'files'} = $nv;
    }
  }
}

sub makefile_mask_xoption($$$)
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  $this->{'xopts'}->{$name} = $value;
}

sub makefile_mask_begin($$$)
{
  my ($this, $keyname, $args) = @_;
  my $skip = int(get_configuration_arg_exp(\$args, $this));
  my $path = get_configuration_arg_exp(\$args, $this);
  #
  my $ret = {
    'type' => 'file-mask',
    'name' => $path,
    'skip' => $skip,
    #
    'parent' => $this,
    'files' => [],
    'xopts' => {},
    #
    'end' => \&makefile_mask_end,
    'commands' => {
      'include' => \&makefile_mask_cmd,
      'exclude' => \&makefile_mask_cmd,
      'xoption' => \&makefile_mask_xoption,
    },
  };
  return $ret;
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_filegroup_fileinfo($;$)
{
  my ($filename, $xopts) = @_;
  $xopts = undef if (defined $xopts) and (%$xopts==0);
  return {
    'filename' => $filename,
    'xopts' => $xopts
  };
}

sub makefile_filegroup_option($$$)
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
#  makefile_setopt($this->{'group'}->{'opts'}, $conf, $name, $value);
}

sub makefile_filegroup_file($$$)
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
  push @{$this->{'group'}->{'files'}}, makefile_filegroup_fileinfo($name);
}

sub makefile_filegroup_begin($$$)
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
#  print "group{$name}\n";
  if(exists $this->{'subs'}->{$name}) {
#    print "Re-use $name of type ".$this->{'subs'}->{$name}->{'type'}."\n";
    return $this->{'subs'}->{$name};
  }
  #
  my $opts = {};
  my $rv = {
    'sets' => set_new($this),
    'parent' => $this,
    'name' => $name,
    'groups' => {},
    'files' => [],
    'opts' => $opts,
  };
  my $ret = {
    'type' => 'file-group',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'group' => $rv,
    'groups' => $rv->{'groups'},
    'sets' => $rv->{'sets'},
    'option' => \&makefile_filegroup_option,
    #
    'set' => \&set_value,
    'begins' => {
      'file-group' => \&makefile_filegroup_begin,
      'mask' => \&makefile_mask_begin,
    },
    'commands' => {
      'file' => \&makefile_filegroup_file,
    },
  };
  $this->{'groups'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_project_cmd($$$)
{
  my ($this, $cmd, $args) = @_;
  if($cmd eq 'depend') {
    my $name = get_configuration_arg_exp(\$args, $this);
    push @{$this->{'project'}->{'depends'}}, $name;
  } elsif($cmd eq 'includes') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      push @{$this->{'project'}->{'includes'}}, $name;
    }
  } elsif($cmd eq 'defines') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      push @{$this->{'project'}->{'defines'}}, $name;
    }
  } elsif($cmd eq 'libpath') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      push @{$this->{'project'}->{'libpath'}}, $name;
    }
  } elsif($cmd eq 'libs') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      push @{$this->{'project'}->{'libs'}}, $name;
    }
  } elsif($cmd eq 'mode') {
    my $mode = get_configuration_arg_exp(\$args, $this);
    $this->{'project'}->{'mode'} = $mode;
  } elsif($cmd eq 'disable') {
    $this->{'project'}->{'mode'} = 'dummy';
  }
}

sub makefile_project_option($$$)
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  makefile_setopt($this->{'project'}->{'opts'}, $conf, $name, $value);
}

sub makefile_project_end($)
{
  my ($this) = @_;
  my $proj = $this->{'project'};
  my @confs = split / /, makefile_getopt('[]', 'Configurations', @{$proj->{'a-opts'}});
  $proj->{'confs'} = \@confs;
  my @platforms = split / /, makefile_getopt('[]', 'Platforms', @{$proj->{'a-opts'}});
  $proj->{'platforms'} = \@platforms;
  #
#  print $proj->{'name'}.": '@confs' '@platforms'\n";
  #
  for my $x (@confs) {
    $proj->{'pc-targets'}->{'configs'}->{$x}++;
  }
  for my $x (@platforms) {
    $proj->{'pc-targets'}->{'platforms'}->{$x}++;
  }
}

sub makefile_project_begin($$$)
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg(\$args);
  if(exists $this->{'subs'}->{$name}) {
#    print "Re-use $name of type ".$this->{'subs'}->{$name}->{'type'}."\n";
    return $this->{'subs'}->{$name};
  }
  #
  my $opts = {};
  my $rv = {
    'sets' => set_new($this),
    'projectgroup' => $this,
    'name' => $name,
    'mode' => '',
    'filename' => "Makefile-project-$name",
    'groups' => {},
    'platforms' => {},
    'configurations' => {},
    'depends' => [],
    #
    'includes' => [],
    'defines' => [],
    'libpath' => [],
    'libs' => [],
    #
    'opts' => $opts,
    'a-opts' => [$opts, (@{$this->{'projectgroup'}->{'a-project-opts'}})], 
    #
    'pc-targets' => $this->{'pc-targets'},
  };
  my $ret = {
    'type' => 'project',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'project' => $rv,
    'groups' => $rv->{'groups'},
    'sets' => $rv->{'sets'},
    #
    'end' => \&makefile_project_end,
    'set' => \&set_value,
    'option' => \&makefile_project_option,
    'begins' => {
      'file-group' => \&makefile_filegroup_begin
    },
    'commands' => {
      'mode' => \&makefile_project_cmd,
      'depend' => \&makefile_project_cmd,
      'includes' => \&makefile_project_cmd,
      'defines' => \&makefile_project_cmd,
      'libpath' => \&makefile_project_cmd,
      'libs' => \&makefile_project_cmd,
      'disable' => \&makefile_project_cmd,
    },
    'pc-targets' => $this->{'pc-targets'},
  };
  $this->{'projects'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
}

sub makefile_project_generate_files($$$$);

sub makefile_project_generate_files($$$$)
{
  my ($state, $fgs, $template, $ff_pad) = @_;
#  my $line;
  my ($pg, $proj, $xmap) = ($state->{'pg'}, $state->{'project'}, $state->{'xmap'});
  local $FILEGROUP_NAME = '';
  for $FILEGROUP_NAME (sort keys %$fgs) {
    my $filegroup = $fgs->{$FILEGROUP_NAME};
    eprint $fout, eval("<<EOT\n".$template->{'project-ff-begin'}."EOT");
    makefile_project_generate_files($state, $filegroup->{'groups'}, $template, $ff_pad+1);
    for my $file_info (@{$filegroup->{'files'}}) {
      makefile_genfile($state, $file_info, $template, $filegroup);
    }
    eprint $fout, eval("<<EOT\n".$template->{'project-ff-end'}."EOT");
  }
  #
}

sub makefile_project_modemap($)
{
  my ($mode) = @_;
  die "Undefined mode" if not defined $mode;
  return 'console' if ($mode eq 'binary') or ($mode eq 'app') or ($mode eq 'gui');
  return 'solib' if ($mode eq 'shared');
  return 'lib' if ($mode eq 'library');
  return $mode if ($mode eq 'console') or ($mode eq 'dummy') or ($mode eq 'lib') or ($mode eq 'solib') or ($mode eq 'plugin');
  return 'unknown';
}

sub makefile_project_getdep($$)
{
  my ($state, $id) = @_;
  my ($pg, $pgn);
  if($id =~ /^(.+)\:\:(.+)$/) {
    ($pgn, $id) = ($1, $2);
    my $a = $state->{'pgs'};
    die "No projectgroup '$pgn'" if not exists $a->{$pgn};
    $pg = $a->{$pgn};
  } else {
    $pg = $state->{'pg'};
    $pgn = $pg->{'name'};
  }
  die "No project '$id' in projectgroup '$pgn'\n"
    ."but ".join(', ', keys %{$pg->{'projects'}})."\n"
    if not exists $pg->{'projects'}->{$id};
  return ($pg->{'projects'}->{$id}, $pgn, $id);
}

sub makefile_project_generate($$$)
{
  my ($state, $proj, $template) = @_;
  my ($pg, $xmap, $filename) = ($state->{'pg'}, $state->{'xmap'}, $proj->{'filename'});
  my ($filters);
  #
  my ($PROJECTGROUP_NAME, $PROJECT_NAME) = ($pg->{'name'}, $proj->{'name'});
  my @confs = $proj->{'confs'};
  my @platforms = $proj->{'platforms'};
  #
  my $opt_vars = $template->{'#options'};
  $proj->{'.platforms'} = \@platforms;
  $proj->{'.confs'} = \@confs;
  #
  #------------------- BEGIN
  #
  eprint $fout, eval("<<EOT\n".$template->{'project-begin'}."EOT");
  #
  #
  #------------------- CONFIGURATIONS
  #
#  my ($PATH_TMP, $PATH_BIN) = (get_configuration_arg('', $this)
  my ($PATH_TMP, $PATH_BIN) = (set_explode($proj, '$(path-out-tmp)'), set_explode($proj, '$(path-out-bin)'));
  #
  #------------------- project-level
  local @PROJECT_INCLUDES = ();
  if(@{$proj->{'includes'}}) {
    @PROJECT_INCLUDES = @{$proj->{'includes'}};
  }
  local @PROJECT_DEFINES = ();
  if(@{$proj->{'defines'}}) {
    @PROJECT_DEFINES = @{$proj->{'defines'}};
  }
  local @PROJECT_LIBS = ();
  if(@{$proj->{'libs'}}) {
    @PROJECT_LIBS = @{$proj->{'libs'}};
  }
  local @PROJECT_LIBPATH = ();
  if(@{$proj->{'libpath'}}) {
    @PROJECT_LIBPATH = @{$proj->{'libpath'}};
  }
  local @PROJECT_DEPENDS = ();
  if(@{$proj->{'depends'}}) {
    @PROJECT_DEPENDS = @{$proj->{'depends'}};
  }
  #
  local $MODE = makefile_project_modemap $proj->{'mode'};
  #------------------- conf-level
  my ($N, $C, $P);
  my ($PLATFORM_NAME, $CONF_NAME) = ($state->{'platform'}, $state->{'conf'});
  #
  local $PROJECT_CONTENTS = '';
  ($N, $C,$P) = ($PROJECT_NAME, $CONF_NAME, $PLATFORM_NAME);
  local $TARGET_TNAME = eval('"'.$template->{'project-tt-name'}.'"');
  local $TARGET_CNAME = eval('"'.$template->{'project-cc-name'}.'"');
  local $PROJECTGROUP_TNAME = eval('"'.$template->{'project-tg-name'}.'"');
  local $PROJECTGROUP_CNAME = eval('"'.$template->{'project-cg-name'}.'"');
  local $PROJECT_TNAME = eval('"'.$template->{'project-tp-name'}.'"');
  local $PROJECT_CNAME = eval('"'.$template->{'project-cp-name'}.'"');
  local $PROJECT_TPNAME = eval('"'.$template->{'project-tpp-name'}.'"');
  local $PROJECT_CPNAME = eval('"'.$template->{'project-cpp-name'}.'"');
  #
  my $opt_var_all = '';
  my ($popts, $pgopts) = ($proj->{'a-opts'}, $pg->{'a-project-opts'});
#  print ref($popts).", ".ref($pgopts)."\n";
  for my $opt_var (sort keys $opt_vars) {
    my $opt_combiner = $opt_vars->{$opt_var};
    my $opt_var_val = &$opt_combiner((
      makefile_getopta('!*', $opt_var, @$popts),
      #
      makefile_getopta($CONF_NAME, $opt_var, @$popts),
      makefile_getopta($PLATFORM_NAME, $opt_var, @$popts),
      makefile_getopta('*', $opt_var, @$popts),
      set_getsafea($proj, $opt_var),
      set_getsafea($pg, $opt_var),
    ));
    $opt_var_all .= "\$OPT_$opt_var = \"$opt_var_val\";\n";
  }
  #
#  print "$opt_var_all";
#  die;
#no strict 'vars';
  eval $opt_var_all;
  if($@) {
    die $@;
  }
#use strict 'vars';
  #
  local $PROJECT_ID = eval('"'.$template->{'project-id-name'}.'"');
  #
  for my $INCLUDE1 (@PROJECT_INCLUDES) {
    my $ai = eval('"'.$template->{'project-include1'}.'"');
    $OPT_CFLAGS .= $ai;
    $OPT_CXXFLAGS .= $ai;
  }
  for my $DEFINE1 (@PROJECT_DEFINES) {
    my $ai = eval('"'.$template->{'project-define1'}.'"');
    $OPT_CFLAGS .= $ai;
    $OPT_CXXFLAGS .= $ai;
  }
  #
  my ($TARGET_DEPENDS, $CLEAN_DEPENDS) = ('','');
  for my $DEPEND1ID (@PROJECT_DEPENDS) {
    my ($pdp, $DEPEND1G, $DEPEND1) = makefile_project_getdep $state, $DEPEND1ID;
    print STDERR "{$DEPEND1G $DEPEND1 ".Data::Dumper->Dump([$pdp], [qw(pdp)])."}" if not defined $pdp->{'mode'};
    my $MODE2 = makefile_project_modemap $pdp->{'mode'};
    #
    my $ai = eval('"'.$template->{'project-depend1C:'.$MODE2}.'"');
    $OPT_CFLAGS .= $ai;
    $OPT_CXXFLAGS .= $ai;
    #
    my $al = eval('"'.$template->{'project-depend1L:'.$MODE2}.'"');
    $OPT_LDFLAGS .= $al;
    #
    $TARGET_DEPENDS .= eval('" '.$template->{'project-depend1t'}.'"');
    $CLEAN_DEPENDS .= eval('" '.$template->{'project-depend1c'}.'"');
  }
  for my $LIBPATH1 (@PROJECT_LIBPATH) {
    my $al = eval('"'.$template->{'project-libpath1'}.'"');
    $OPT_LDFLAGS .= $al;
  }
  for my $LIB1 (@PROJECT_LIBS) {
    my $al = eval('"'.$template->{'project-lib1'}.'"');
    $OPT_LDFLAGS .= $al;
  }
  #
  my $dummy = 0;
  $dummy = 1 if $MODE eq 'dummy';
  eprint $fout, eval("<<EOT\n".$template->{'project-config-begin'}."\nEOT");
  #
  if(not $dummy) {
    eprint $fout, eval("<<EOT\n".$template->{'project-config-flags'}."\nEOT");
    {
      my $shar = $template->{'project-config-shared'};
      for my $svt (@$shar) {
        my $xx = eval("<<EOT\n".$svt."\nEOT");
        $xmap->{$xx}++;
      }
    }
    #
    #      print "$PROJECT_NAME $MODE $PLATFORM_NAME $CONF_NAME\n";
    $MODE = 'console' if $MODE eq 'console' or $MODE eq 'app' or $MODE eq 'gui';
    eprint $fout,  eval("<<EOT\n".$template->{'project-config-M:'.$MODE}."\nEOT");
    #
    if(not $dummy) {
      eprint $fout,  eval("<<EOT\n".$template->{'project-config-M-general'}."\nEOT");
      makefile_project_generate_files($state, $proj->{'groups'}, $template, 0);
    }
    #
  }
  eprint $fout, eval("<<EOT\n".$template->{'project-config-end'}."\nEOT");
  #
  eprint $fout,  eval("<<EOT\n".$template->{'project-end'}."EOT");
  #
  #------------------- END
  push @{$state->{'projects'}}, "$PROJECTGROUP_NAME::$PROJECT_NAME";
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------


sub makefile_projectgroup_option($$$)
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
#  print "option $name $args\n";
  if($name eq 'project-order') {
    my @po = split / /, $args;
#    $this->{'solution'}->{'project-order'} = \@po;
  } elsif($name eq 'project') {
    my $conf = get_configuration_arg(\$args);
    my $name = get_configuration_arg(\$args);
    my $value = get_configuration_arg_exp(\$args, $this);
    makefile_setopt($this->{'projectgroup'}->{'project-opts'}, $conf, $name, $value);
#    print "setopt '$conf'.'$name' => '$value'\n";
  } else {
    die "Unknown project-group option: $name";
  }
}

sub makefile_projectgroup_begin($$$)
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg(\$args);
  if(exists $this->{'subs'}->{$name}) {
#    print "Re-use $name\n";
    return $this->{'subs'}->{$name};
  }
  #
#  print "New $name\n";
  my $project_opts = {};
  my $rv = {
    'sets' => set_new($this),
    'name' => $name,
    'filename' => "Makefile-$name",
    'projects' => {},
    'project-order' => [],
    'project-opts' => $project_opts,
    'a-project-opts' => [$project_opts, $this->{'a-project-opts'}],
    #
    'pc-targets' => $this->{'pc-targets'}
  };
  my $ret = {
    'type' => 'project-group',
    'G' => $this,
    'name' => $name,
    'subs' => {},
    #
    'projectgroup' => $rv,
    'projects' => $rv->{'projects'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
       my ($this) = @_;
    },
    'set' => \&set_value,
    'option' => \&makefile_projectgroup_option,
    'begins' => {
      'project' => \&makefile_project_begin
    },
    'commands' => {
    },
    'pc-targets' => $this->{'pc-targets'},
  };
  $this->{'subs'}->{$name} = $ret;
  return $ret;
}

sub makefile_gen_include($$$)
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  #
  my $config = init_configuration();
  read_configuration($config, $name);
  apply_configuration($config, $this);
}

sub makefile_projectgroup_generate($$$)
{
  my ($state, $pg, $template) = @_;
  my ($projects) = ($pg->{'projects'});
  #
  local $PROJECTGROUP_NAME = $pg->{'name'};
#  print "fout=$fout\n";
  #
  eprint $fout, eval("<<EOT\n".$template->{'projectgroup-begin'}."EOT");
  #
  foreach my $proj_name (sort keys %$projects) {
    my $proj = $projects->{$proj_name};
    $state->{'proj'} = $proj;
    makefile_project_generate($state, $proj, $template);
    $state->{'proj'} = undef;
  }
#  my $uproj = {};
#  foreach my $proj_name (@{$sol->{'project-order'}}) {
#    makefile_solution_generatesproj($sol, $template, $fout, $projects, $proj_name, $uproj);
#  }
#  foreach my $proj_name (sort keys %$projects) {
#    makefile_solution_generatesproj($sol, $template, $fout, $projects, $proj_name, $uproj);
#  }
#  print $fout "\n";
  #
  eprint $fout, eval("<<EOT\n".$template->{'projectgroup-end'}."EOT");
  #
  print "Written project group '$PROJECTGROUP_NAME'\n";
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_gen_generate($$)
{
  my ($this, $config) = @_;
#       print "{".$this->{'type'}.",".$this->{'G'}->{'type'}.",".$this->{'G'}->{'templates'}->{'type'}."}\n";
  my $pgs = $this->{'subs'};
  my ($template) = ($this->{'templates'});
  #
  my $files = {};
  my $state = {
    'pgs' => $pgs,
    'fout' => $fout,
    'files' => $files,
    'projects' => [],
  };
  #
  for my $PLATFORM_NAME (keys %{$this->{'pc-targets'}->{'platforms'}}) {
    for my $CONF_NAME (keys %{$this->{'pc-targets'}->{'configs'}}) {
      my ($id, $filename, $F) = ("$PLATFORM_NAME.$CONF_NAME", "Makefile.$PLATFORM_NAME.$CONF_NAME");
      make_filename_dir($filename);
      open $F,'>',$filename or die "File '$filename' create error.";
      eprint $F, eval("<<EOT\n".$template->{'makefile-begin'}."EOT");
      #
      my $file = {
        'xmap' => {},
        'F' => $F,
        'filename' => $filename,
        'conf' => $CONF_NAME,
        'platform' => $PLATFORM_NAME,
      };
      $files->{$id} = $file;
      #
      $state->{'file'} = $file;
      $state->{'xmap'} = $file->{'xmap'};
      $state->{'platform'} = $file->{'platform'};
      $state->{'conf'} = $file->{'conf'};
      #
      local $fout = $F;
      #
#      print "fout=$fout\n";
      #
      for my $sn (keys %$pgs) {
        my $pg = $pgs->{$sn};
        $state->{'pg'} = $pg;
        makefile_projectgroup_generate($state, $pg->{'projectgroup'}, $template);
        $state->{'pg'} = undef;
      }
      #
      eprint $F,  eval("<<EOT\n".$template->{'makefile-xmap'}."EOT");
      my $xmap = $state->{'xmap'};
      for my $xk (keys %$xmap) {
        print $fout "# ".$xmap->{$xk}."\n";
        print $fout "$xk\n";
      }
      #
      eprint $F, eval("<<EOT\n".$template->{'makefile-begin'}."EOT");
      close $F;
      print "Written make file '$filename' : ".join(', ', @{$state->{'projects'}})."\n";
    }
  }
}

sub makefile_open($)
{
  my ($this) = @_;
  return $this;
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

return 1;
