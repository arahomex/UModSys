use strict;
use warnings;

our $generators;
our $script_path;

sub makefile_setopt
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


#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_mask_end
{
  my ($this) = @_;
  my $pfiles = $this->{'parent'}->{'filter'}->{'files'};
  for my $file (@{$this->{'files'}}) {
#    push @$pfiles, makefile_filter_fileinfo($file, $this->{'xopts'});
  }
}

sub makefile_mask_cmd
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

sub makefile_mask_xoption
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  $this->{'xopts'}->{$name} = $value;
}

sub makefile_mask_begin
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
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_filegroup_fileinfo
{
  my ($filename, $xopts) = @_;
  $xopts = undef if (defined $xopts) and (%$xopts==0);
  return {
#    'filename' => makefile_path_win32($filename),
    'xopts' => $xopts
  };
}

sub makefile_filegroup_option
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
#  makefile_setopt($this->{'filter'}->{'opts'}, $conf, $name, $value);
}

sub makefile_filegroup_file
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
  push @{$this->{'filter'}->{'files'}}, makefile_filegroup_fileinfo($name);
}

sub makefile_filegroup_begin
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
    'filegroups' => {},
    'files' => [],
    'opts' => $opts,
  };
  my $ret = {
    'type' => 'file-group',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'filegroup' => $rv,
    'filegroups' => $rv->{'filegroups'},
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
  $this->{'filegroups'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub makefile_project_cmd
{
  my ($this, $cmd, $args) = @_;
  if($cmd eq 'depend') {
    my $name = get_configuration_arg_exp(\$args, $this);
    push @{$this->{'project'}->{'depends'}}, $name;
  } elsif($cmd eq 'includes') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      #push @{$this->{'project'}->{'includes'}}, makefile_path_win32($name);
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
      push @{$this->{'project'}->{'libs'}}, $name.'.lib';
    }
  } elsif($cmd eq 'mode') {
    my $mode = get_configuration_arg_exp(\$args, $this);
    $this->{'project'}->{'mode'} = $mode;
  } elsif($cmd eq 'disable') {
    $this->{'project'}->{'mode'} = 'dummy';
  }
}

sub makefile_project_option
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
#  makefile_setopt($this->{'project'}->{'opts'}, $conf, $name, $value);
}

sub makefile_project_begin
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
    'filters' => {},
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
  };
  my $ret = {
    'type' => 'project',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'project' => $rv,
    'filegroups' => $rv->{'groups'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
#       my ($this) = @_;
#       makefile_project_generate($this->{'project'}, $this->{'G'}->{'templates'});
    },
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
  };
  $this->{'projects'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
}

sub makefile_project_generate_files
{
  my ($proj, $template, $fout, $filters, $FF_PAD) = @_;
  my $line;
  #
q^
  for my $FILTER_NAME (sort keys %$filters) {
    my $filter = $filters->{$FILTER_NAME};
    #
    $line = eval("<<EOT\n".$template->{'project-ff-begin'}."EOT");
    print $fout $line;
    #
    makefile_project_generate_files($proj, $template, $fout, $filter->{'filters'}, $FF_PAD."\t");
    #
    for my $file_info (@{$filter->{'files'}}) {
      my $FILE_PATH = $file_info->{'filename'};
      my $xopts = $file_info->{'xopts'};
      $line = eval("<<EOT\n".$template->{'project-ff-file-begin'}."EOT");
      print $fout $line;
      if(defined $xopts) {
        my $platforms = $proj->{'.platforms'};
        my $confs = $proj->{'.confs'};
        for my $PLATFORM_NAME (@$platforms) {
          for my $CONF_NAME (@$confs) {
            $line = eval("<<EOT\n".$template->{'project-ff-file-xopt-begin'}."EOT");
            print $fout $line;
            #
            my $tools = {};
            for my $xok (keys %$xopts) {
              my ($tool, $key) = split '\_', $xok;
              if(exists $tools->{$tool}) {
                $tools->{$tool}->{$key} = $xopts->{$xok};
              } else {
                $tools->{$tool} = { $key => $xopts->{$xok} };
              }
            }
            #
            for my $TOOL_ID (keys %$tools) {
              $line = eval("<<EOT\n".$template->{'project-ff-file-xopt-tbegin-'.$TOOL_ID}."EOT");
              print $fout $line;
              my $tool = $tools->{$TOOL_ID};
              for my $KEY (keys %$tool) {
                my $VALUE = $tool->{$KEY};
                $line = eval("<<EOT\n".$template->{'project-ff-file-xopt-tvalue'}."EOT");
                print $fout $line;
              }
              $line = eval("<<EOT\n".$template->{'project-ff-file-xopt-tend'}."EOT");
              print $fout $line;
            }
            #
            $line = eval("<<EOT\n".$template->{'project-ff-file-xopt-end'}."EOT");
            print $fout $line;
          }
        }
      
      }
      $line = eval("<<EOT\n".$template->{'project-ff-file-end'}."EOT");
      print $fout $line;
    }
    #
    $line = eval("<<EOT\n".$template->{'project-ff-end'}."EOT");
    print $fout $line;
  }
^ if 0;

}

sub makefile_project_generate
{
  my ($fout, $pg, $proj, $template) = @_;
  my ($filename, $filters) = ($proj->{'filename'}, );
  my ($line);
  #
  my ($PROJECTGROUP_NAME, $PROJECT_NAME) = ($pg->{'name'}, $proj->{'name'});
  my @confs = split / /, makefile_getopt('[]', 'Configurations', @{$proj->{'a-opts'}});
  my @platforms = split / /, makefile_getopt('[]', 'Platforms', @{$proj->{'a-opts'}});
  #
  my $opt_vars = $template->{'#options'};
  $proj->{'.platforms'} = \@platforms;
  $proj->{'.confs'} = \@confs;
  #
  #------------------- BEGIN
  #
  $line = eval("<<EOT\n".$template->{'project-begin'}."EOT");
  print $fout $line;
  #
  #
  #------------------- CONFIGURATIONS
  #
#  my ($PATH_TMP, $PATH_BIN) = (get_configuration_arg('', $this)
  my ($PATH_TMP, $PATH_BIN) = (set_explode($proj, '$(path-tmp)'), set_explode($proj, '$(path-bin)'));
  #
  my ($N, $C, $P);
  for my $PLATFORM_NAME (@platforms) {
    for my $CONF_NAME (@confs) {
      #
#      my $opt_var_all = '';
#      for my $opt_var (keys $opt_vars) {
#        my $opt_combiner = $opt_vars->{$opt_var};
      my ($TARGET_DEPENDS, $CLEAN_DEPENDS) = ('','');
      ($C,$P) = ($CONF_NAME, $PLATFORM_NAME);
      for $N (@{$proj->{'depends'}}) {
        $TARGET_DEPENDS .= eval('" '.$template->{'project-tp-name'}.'"');
        $CLEAN_DEPENDS .= eval('" '.$template->{'project-cp-name'}.'"');
      }
      #
      $N = $PROJECT_NAME;
      my $MODE = $proj->{'mode'};
      my $TARGET_TNAME .= eval('"'.$template->{'project-tt-name'}.'"');
      my $TARGET_CNAME .= eval('"'.$template->{'project-cc-name'}.'"');
      my $PROJECT_TNAME .= eval('"'.$template->{'project-tp-name'}.'"');
      my $PROJECT_CNAME .= eval('"'.$template->{'project-cp-name'}.'"');
      my $PROJECTGROUP_TNAME .= eval('"'.$template->{'project-tg-name'}.'"');
      my $PROJECTGROUP_CNAME .= eval('"'.$template->{'project-cg-name'}.'"');
      #
      $line = eval("<<EOT\n".$template->{'project-config-begin'}."\nEOT");
      print $@ if $@;
      print $fout $line;
      #
      my $dummy = 0;
      if($MODE eq 'console' or $MODE eq 'binary') {
        $line = eval("<<EOT\n".$template->{'project-config-M:console'}."\nEOT");
      } elsif($MODE eq 'lib') {
        $line = eval("<<EOT\n".$template->{'project-config-M:lib'}."\nEOT");
      } elsif($MODE eq 'solib' or $MODE eq 'shared') {
        $line = eval("<<EOT\n".$template->{'project-config-M:solib'}."\nEOT");
      } elsif($MODE eq 'app' or $MODE eq 'gui') {
        $line = eval("<<EOT\n".$template->{'project-config-M:console'}."\nEOT");
      } elsif($MODE eq 'dummy') {
        $dummy = 1;
        $line = eval("<<EOT\n".$template->{'project-config-M:dummy'}."\nEOT");
      } else {
        $line = eval("<<EOT\n".$template->{'project-config-M:console'}."\nEOT");
      }
      print $@ if $@;
      print $fout $line;
      #
      if(not $dummy) {
        $line = eval("<<EOT\n".$template->{'project-config-M-general'}."\nEOT");
        print $@ if $@;
        print $fout $line;
      }
      #
      $line = eval("<<EOT\n".$template->{'project-config-end'}."\nEOT");
      print $@ if $@;
      print $fout $line;
    }
  }
q^
  #
#  my $opt_vars = makefile_getopt('[]', '#', @{$proj->{'a-opts'}});
  #
  my ($TARGET_DEPS, $
  $line = eval("<<EOT\n".$template->{'project-begin'}."EOT");
  print $fout $line;
  #
  #------------------- PLATFORMS
  $line = eval("<<EOT\n".$template->{'project-platforms-begin'}."EOT");
  print $fout $line;
  for my $PLATFORM_NAME (@platforms) {
    $line = eval("<<EOT\n".$template->{'project-platforms-entry'}."EOT");
    print $fout $line;
  }
  $line = eval("<<EOT\n".$template->{'project-platforms-end'}."EOT");
  print $fout $line;
  #
  #
  #------------------- INCLUDES, DEFINES
  my $INCLUDES = '';
  if({$proj->{'includes'}}) {
    $INCLUDES = join ';', @{$proj->{'includes'}};
  }
  my $DEFINES = '';
  if({$proj->{'defines'}}) {
    $DEFINES = join ';', @{$proj->{'defines'}};
  }
  my $LIB_INCLUDES = '';
  if({$proj->{'libpath'}}) {
    $LIB_INCLUDES = join ';', @{$proj->{'libpath'}};
  }
  my $LIBS = '';
  if({$proj->{'libs'}}) {
    $LIBS = join ' ', @{$proj->{'libs'}};
  }
  #
  #------------------- CONFIGURATIONS
  $line = eval("<<EOT\n".$template->{'project-configs-begin'}."EOT");
  print $fout $line;
  for my $PLATFORM_NAME (@platforms) {
    for my $CONF_NAME (@confs) {
      #
      my $opt_var_all = '';
      for my $opt_var (keys $opt_vars) {
        my $opt_combiner = $opt_vars->{$opt_var};
#print "\$opt_var='$opt_var' \$opt_combiner='$opt_combiner'\n";
        my $opt_var_val = &$opt_combiner(
          makefile_getopt('!*', $opt_var, @{$proj->{'a-opts'}}),
          makefile_getopt($CONF_NAME, $opt_var, @{$proj->{'a-opts'}}),
          makefile_getopt($PLATFORM_NAME, $opt_var, @{$proj->{'a-opts'}}),
          makefile_getopt('*', $opt_var, @{$proj->{'a-opts'}})
        );
        $opt_var_all .= "my \$OPT_$opt_var = \'$opt_var_val\';\n";
        #
        $opt_var_all .= "\$OPT_Compiler_AdditionalIncludeDirectories .= ';'.\$INCLUDES;\n"
          if ($opt_var eq 'Compiler_AdditionalIncludeDirectories') and ($INCLUDES ne '');
        $opt_var_all .= "\$OPT_Compiler_PreprocessorDefinitions .= ';'.\$DEFINES;\n"
          if ($opt_var eq 'Compiler_PreprocessorDefinitions') and ($DEFINES ne '');
        $opt_var_all .= "\$OPT_Linker_AdditionalDependencies .= ' '.\$LIBS;\n"
          if ($opt_var eq 'Linker_AdditionalDependencies') and ($LIBS ne '');
        $opt_var_all .= "\$OPT_Linker_AdditionalLibraryDirectories .= ';'.\$LIB_INCLUDES;\n"
          if ($opt_var eq 'Linker_AdditionalLibraryDirectories') and ($LIB_INCLUDES ne '');
      }
      #
      if($proj->{'mode'} eq 'console' or $proj->{'mode'} eq 'binary') {
        $opt_var_all .= '$OPT_ConfigurationType="1"; ';
        $opt_var_all .= '$OPT_Linker_OutputFile = \'$(OutDir)/$(ProjectName).exe\';';
      } elsif($proj->{'mode'} eq 'lib') {
        $opt_var_all .= '$OPT_ConfigurationType="4"; ';
        $opt_var_all .= '$OPT_Librarian_OutputFile = \'$(OutDir)/$(ProjectName).lib\';';
      } elsif($proj->{'mode'} eq 'solib' or $proj->{'mode'} eq 'shared') {
        $opt_var_all .= '$OPT_ConfigurationType="2"; ';
        $opt_var_all .= '$OPT_Linker_OutputFile = \'$(OutDir)/$(ProjectName).dll\';';
      } elsif($proj->{'mode'} eq 'app' or $proj->{'mode'} eq 'gui') {
        $opt_var_all .= '$OPT_ConfigurationType="1"; ';
        $opt_var_all .= '$OPT_Linker_OutputFile = \'$(OutDir)/$(ProjectName).exe\';';
        $opt_var_all .= '$OPT_Linker_SubSystem = "2";';
      } elsif($proj->{'mode'} eq 'dummy') {
        $opt_var_all .= '$OPT_ConfigurationType="10"; ';
      } else {
        $opt_var_all .= '$OPT_ConfigurationType="1" ; ';
        $opt_var_all .= '$OPT_Linker_OutputFile = \'$(OutDir)/$(ProjectName).exe\';';
      }
      #
      if(lc $PLATFORM_NAME eq 'win32') {
        $opt_var_all .= '$OPT_Linker_TargetMachine = \'1\';'; # x86=1
      } elsif(lc $PLATFORM_NAME eq 'x64') {
        $opt_var_all .= '$OPT_Linker_TargetMachine = \'17\';'; # x64=17
      } else {
        $opt_var_all .= ''; # unknown platform
      }
      #
      $opt_var_all .= "\n";
      $opt_var_all .= '$OPT_OutputDirectory = makefile_path_win32($OPT_OutputDirectory);';
      $opt_var_all .= '$OPT_IntermediateDirectory = makefile_path_win32($OPT_IntermediateDirectory);';
      $opt_var_all .= '$OPT_Linker_OutputFile = makefile_path_win32($OPT_Linker_OutputFile);';
      $opt_var_all .= '$OPT_Librarian_OutputFile = makefile_path_win32($OPT_Librarian_OutputFile);';
      $opt_var_all .= "\n";
      #
      my $list = [
        $template->{'project-config-begin'},
        $template->{'project-config-compiler'},
        $template->{'project-config-linker'},
        $template->{'project-config-librarian'},
        $template->{'project-config-aux'},
        $template->{'project-config-end'},
      ];
      $opt_var_all .= <<'EOT_END';

        for my $eval_line (@$list) {
          print $fout eval("<<EOT\n${eval_line}EOT");
          print $@ if $@; 
        }
EOT_END

#      print $opt_var_all;
no strict 'vars';
      eval $opt_var_all;
      print $@ if $@; 
use strict 'vars';
    }
  }
  $line = eval("<<EOT\n".$template->{'project-configs-end'}."EOT");
  print $fout $line;
  #
  #------------------- FILES
  $line = eval("<<EOT\n".$template->{'project-files-begin'}."EOT");
  print $fout $line;
  if($proj->{'mode'} ne 'dummy') {
    makefile_project_generate_files($proj, $template, $fout, $proj->{'filters'}, '');
  }
  $line = eval("<<EOT\n".$template->{'project-files-end'}."EOT");
  print $fout $line;
  #
  #
^ if 0;
  #
  $line = eval("<<EOT\n".$template->{'project-end'}."EOT");
  print $fout $line;
  #
  #------------------- END
  print "Written project data $PROJECT_NAME\n";
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------


sub makefile_projectgroup_option
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
#  print "option $name $args\n";
  if($name eq 'random-seed') {
#    my $seed = makefile_calc_seed($args);
#    srand $seed;
#    print "Random seed is set to $seed for '$args'\n";
    return;
  } elsif($name eq 'project-order') {
    my @po = split / /, $args;
#    $this->{'solution'}->{'project-order'} = \@po;
  } elsif($name eq 'project') {
    my $conf = get_configuration_arg(\$args);
    my $name = get_configuration_arg(\$args);
    my $value = get_configuration_arg_exp(\$args, $this);
#    makefile_setopt($this->{'solution'}->{'project-opts'}, $conf, $name, $value);
#    print "setopt '$conf'.'$name' => '$value'\n";
  }
}

sub makefile_projectgroup_begin
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg(\$args);
  if(exists $this->{'subs'}->{$name}) {
#    print "Re-use $name of type ".$this->{'subs'}->{$name}->{'type'}."\n";
    return $this->{'subs'}->{$name};
  }
  #
  my $project_opts = {};
  my $rv = {
    'sets' => set_new($this),
    'name' => $name,
    'filename' => "Makefile-$name",
    'projects' => {},
    'project-order' => [],
    'project-opts' => $project_opts,
    'a-project-opts' => [$project_opts, $this->{'a-project-opts'}],
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
  };
  $this->{'subs'}->{$name} = $ret;
  return $ret;
}

sub makefile_gen_include
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  #
  my $config = init_configuration();
  read_configuration($config, $name);
  apply_configuration($config, $this);
}

sub makefile_gen_generate
{
  my ($this, $config) = @_;
  my $filename = "Makefile";
#       print "{".$this->{'type'}.",".$this->{'G'}->{'type'}.",".$this->{'G'}->{'templates'}->{'type'}."}\n";
  my $pgs = $this->{'subs'};
  my ($fout, $line);
  my ($template) = ($this->{'templates'});
  open $fout,'>',$filename or die "File '$filename' create error.";
  $line = eval("<<EOT\n".$template->{'makefile-begin'}."EOT");
  print $fout $line;
  for my $sn (keys %$pgs) {
    my $pg = $pgs->{$sn};
    makefile_projectgroup_generate($fout, $pg->{'projectgroup'}, $template);
  }
  $line = eval("<<EOT\n".$template->{'makefile-end'}."EOT");
  print $fout $line;
  close $fout;
}

sub makefile_projectgroup_generate
{
  my ($fout, $pg, $template) = @_;
  my ($filename, $projects) = ($pg->{'filename'}, $pg->{'projects'});
  my ($line);
  make_filename_dir($filename);
  #
  my ($PROJECTGROUP_NAME) = ($pg->{'name'});
  #
  $line = eval("<<EOT\n".$template->{'projectgroup-begin'}."EOT");
  print $fout $line;
  #
  foreach my $proj_name (sort keys %$projects) {
    makefile_project_generate($fout, $pg, $projects->{$proj_name}, $template);
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
  $line = eval("<<EOT\n".$template->{'projectgroup-end'}."EOT");
  print $fout $line;
  #
  print "Written project group file '$filename'\n";
}

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

return 1;
