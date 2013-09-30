use strict;
use File::Basename;
use Storable qw(dclone);

my $uitf8_header = "\xEF\xBB\xBF";

our $script_path;
require "$script_path/util_crc32.pm";

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_new_guid
{
  my ($args) = @_;
  if(defined $args) {
    my $seed = msvc_xml_calc_seed($args);
    srand $seed;
#    print "Random seed is set to $seed for '$args'\n";
#    print sprintf("rand(0x10000)=%04x\n", rand(0x10000));
  }
  # 8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942
  my $guid = sprintf(
    "{%04X%04X-%04X-%04X-%04X-%04X%04X%04X}",
    int(rand(0x10000)), int(rand(0x10000)),
    int(rand(0x10000)), int(rand(0x10000)), int(rand(0x10000)),
    int(rand(0x10000)), int(rand(0x10000)), int(rand(0x10000))
  );
#  print "GUID=$guid\n";
  return $guid;
}

sub msvc_xml_calc_seed
{
  my ($args) = @_;
  my $seed = int(get_configuration_arg(\$args));
  my $basestr = get_configuration_arg(\$args);
  $seed ^= crc32($basestr) if $basestr ne '';
  return $seed;
}

sub msvc_xml_setopt
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

sub msvc_xml_getopt
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

sub msvc_xml_clone
{
  my $rv = {};
  while(my $base = shift) {
    for my $k (keys $base) {
      my $value = $base->{$k};
      my $refv = ref($value);
      if($refv eq 'HASH') {
#        print "[$k] is ref HASH\n";
        $rv->{$k} = {} if not exists $rv->{$k};
#        print "[$k] deep clone\n";
        $rv->{$k} = msvc_xml_clone($rv->{$k}, $value);
#        print "/[$k] deep clone\n";
      } elsif($refv eq 'ARRAY') {
#        print "[$k] is ref ARRAY\n";
        $rv->{$k} = dclone($value);
      } elsif($refv eq 'CODE') {
#        print "[$k] is ref CODE\n";
        $rv->{$k} = $value;
      } elsif($refv eq '') {
#        print "[$k] is not ref\n";
        $rv->{$k} = $value;
      } else {
#        print "[$k] is ref $refv\n";
        $rv->{$k} = dclone($value);
      }
    }
  }
  return $rv;
}

sub msvc_xml_gen_include {
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  #
  my $config = init_configuration();
  read_configuration($config, $name);
  apply_configuration($config, $this);
}

sub msvc_xml_gen_generate {
  my ($this, $config) = @_;
#       print "{".$this->{'type'}.",".$this->{'G'}->{'type'}.",".$this->{'G'}->{'templates'}->{'type'}."}\n";
  my $sols = $this->{'subs'};
  for my $sn (keys %$sols) {
    my $sol = $sols->{$sn};
    msvc_xml_solution_generate($sol->{'solution'}, $this->{'templates'});
    for my $pid (keys $sol->{'projects'}) {
#      print "Processing project $pid\n";
      msvc_xml_project_generate($sol->{'projects'}->{$pid}, $this->{'templates'});
    }
  }
}

sub msvc_xml_path_win32 {
  my ($filename) = @_;
  while($filename =~ s/\//\\/) {
  }
  return $filename;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_solution_generatesproj
{
  my ($sol, $template, $fout, $projects, $proj_name, $uproj) = @_;
  return if exists $uproj->{$proj_name};
  return if not exists $projects->{$proj_name};
  #
  print "\$proj_name=`$proj_name`\n";
  #
  my $line;
  my $proj = $projects->{$proj_name};
  my ($PROJECT_TYPE_GUID, $PROJECT_NAME, $PROJECT_PATH, $PROJECT_GUID) = (
    $template->{'project-type-c++'}, 
    $proj->{'name'}, 
    $proj->{'filename'},
    $proj->{'GUID'},
  );
  #
  $line = eval("<<EOT\n".$template->{'solution-project-begin'}."EOT");
  print $fout $line;
  #
  if(@{$proj->{'depends'}}) {
    $line = eval("<<EOT\n".$template->{'solution-project-dep-begin'}."EOT");
    print $fout $line;
    foreach my $dpn (@{$proj->{'depends'}}) {
      die "Depends resolution for project $proj_name ref $dpn failed" if not exists $projects->{$dpn};
      my $proj2 = $projects->{$dpn};
      my $PROJECT_DEP_GUID = $proj2->{'GUID'};
      die "Depends resolution for project $proj_name ref $dpn failed due to null GUID" if $PROJECT_DEP_GUID eq '';
      $line = eval("<<EOT\n".$template->{'solution-project-dep-entry'}."EOT");
      print $fout $line;
    }
    $line = eval("<<EOT\n".$template->{'solution-project-dep-end'}."EOT");
    print $fout $line;
  }
  #
  $line = eval("<<EOT\n".$template->{'solution-project-end'}."EOT");
  print $fout $line;
  $uproj->{$proj_name} = $proj_name;
}

sub msvc_xml_solution_generate
{
  my ($sol, $template) = @_;
  my ($filename, $projects) = ($sol->{'filename'}, $sol->{'projects'});
  my ($fout);
  make_filename_dir($filename);
  open $fout,'>',$filename or die "File '$filename' create error.";
  print $fout $uitf8_header;
  print $fout $template->{'solution-begin'};
  my $uproj = {};
  foreach my $proj_name (@{$sol->{'project-order'}}) {
    msvc_xml_solution_generatesproj($sol, $template, $fout, $projects, $proj_name, $uproj);
  }
  foreach my $proj_name (sort keys %$projects) {
    msvc_xml_solution_generatesproj($sol, $template, $fout, $projects, $proj_name, $uproj);
  }
  print $fout "\n";
  close $fout;
  print "Written solution file '$filename'\n";
}

sub msvc_xml_solution_option
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
#  print "option $name $args\n";
  if($name eq 'random-seed') {
    my $seed = msvc_xml_calc_seed($args);
    srand $seed;
    print "Random seed is set to $seed for '$args'\n";
    return;
  } elsif($name eq 'project-order') {
    my @po = split / /, $args;
    $this->{'solution'}->{'project-order'} = \@po;
  } elsif($name eq 'project') {
    my $conf = get_configuration_arg(\$args);
    my $name = get_configuration_arg(\$args);
    my $value = get_configuration_arg_exp(\$args, $this);
    msvc_xml_setopt($this->{'solution'}->{'project-opts'}, $conf, $name, $value);
#    print "setopt '$conf'.'$name' => '$value'\n";
  }
}

sub msvc_xml_solution_cmd
{
  my ($this, $cmd, $args) = @_;
}


sub msvc_xml_solution_begin
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
    'filename' => "$name.sln",
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
    'solution' => $rv,
    'projects' => $rv->{'projects'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
       my ($this) = @_;
    },
    'set' => \&set_value,
    'option' => \&msvc_xml_solution_option,
    'begins' => {
      'project' => \&msvc_xml_project_begin
    },
    'commands' => {
    },
  };
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_project_generate_files
{
  my ($proj, $template, $fout, $filters, $FF_PAD) = @_;
  my $line;
  #
  for my $FILTER_NAME (sort keys %$filters) {
    my $filter = $filters->{$FILTER_NAME};
    #
    $line = eval("<<EOT\n".$template->{'project-ff-begin'}."EOT");
    print $fout $line;
    #
    msvc_xml_project_generate_files($proj, $template, $fout, $filter->{'filters'}, $FF_PAD."\t");
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
}

sub msvc_xml_project_generate
{
  my ($proj, $template) = @_;
  my ($filename, $filters) = ($proj->{'filename'}, );

  my ($fout, $line);
  make_filename_dir($filename);
  open $fout,'>',$filename or die "File '$filename' create error.";
#  print $fout $uitf8_header;
  #
  #------------------- BEGIN
  my ($PROJECT_NAME, $PROJECT_GUID) = ($proj->{'name'}, $proj->{'GUID'});
  my @confs = split / /, msvc_xml_getopt('[]', 'Configurations', @{$proj->{'a-opts'}});
  my @platforms = split / /, msvc_xml_getopt('[]', 'Platforms', @{$proj->{'a-opts'}});
  my $opt_vars = msvc_xml_getopt('[]', '#', @{$proj->{'a-opts'}});
  $proj->{'.platforms'} = \@platforms;
  $proj->{'.confs'} = \@confs;
  #
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
  #
  #------------------- CONFIGURATIONS
  $line = eval("<<EOT\n".$template->{'project-configs-begin'}."EOT");
  print $fout $line;
  for my $PLATFORM_NAME (@platforms) {
    for my $CONF_NAME (@confs) {
      #
      my $opt_var_all = '';
      for my $opt_var (keys $opt_vars) {
        my $opt_var_val;
        $opt_var_val = msvc_xml_getopt('!*', $opt_var, @{$proj->{'a-opts'}}) if not defined $opt_var_val;
        $opt_var_val = msvc_xml_getopt($CONF_NAME, $opt_var, @{$proj->{'a-opts'}}) if not defined $opt_var_val;
        $opt_var_val = msvc_xml_getopt('*', $opt_var, @{$proj->{'a-opts'}}) if not defined $opt_var_val;
        $opt_var_val = $opt_vars->{$opt_var} if not defined $opt_var_val;
        $opt_var_all .= "my \$OPT_$opt_var = \"$opt_var_val\";\n";
        #
        $opt_var_all .= "\$OPT_Compiler_AdditionalIncludeDirectories .= ';'.\$INCLUDES;\n"
          if ($opt_var eq 'Compiler_AdditionalIncludeDirectories') and ($INCLUDES ne '');
        $opt_var_all .= "\$OPT_Compiler_PreprocessorDefinitions .= ';'.\$DEFINES;\n"
          if ($opt_var eq 'Compiler_PreprocessorDefinitions') and ($DEFINES ne '');
      }
      #
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
      } else {
        $opt_var_all .= '$OPT_ConfigurationType="1" ;';
        $opt_var_all .= '$OPT_Linker_OutputFile = \'$(OutDir)/$(ProjectName).exe\';';
      }
      #
      $opt_var_all .= "\n";
      $opt_var_all .= '$OPT_OutputDirectory = msvc_xml_path_win32($OPT_OutputDirectory);';
      $opt_var_all .= '$OPT_IntermediateDirectory = msvc_xml_path_win32($OPT_IntermediateDirectory);';
      $opt_var_all .= '$OPT_Linker_OutputFile = msvc_xml_path_win32($OPT_Linker_OutputFile);';
      $opt_var_all .= '$OPT_Librarian_OutputFile = msvc_xml_path_win32($OPT_Librarian_OutputFile);';
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
  msvc_xml_project_generate_files($proj, $template, $fout, $proj->{'filters'}, '');
  $line = eval("<<EOT\n".$template->{'project-files-end'}."EOT");
  print $fout $line;
  #
  #------------------- END
  $line = eval("<<EOT\n".$template->{'project-end'}."EOT");
  print $fout $line;
  #
  print $fout "\n";
  close $fout;
  print "Written project file '$filename'\n";
}

sub msvc_xml_project_cmd
{
  my ($this, $cmd, $args) = @_;
  if($cmd eq 'depend') {
    my $name = get_configuration_arg(\$args);
    push @{$this->{'project'}->{'depends'}}, $name;
  } elsif($cmd eq 'includes') {
    while(my $name = get_configuration_arg_exp(\$args, $this)) {
      push @{$this->{'project'}->{'includes'}}, msvc_xml_path_win32($name);
    }
  } elsif($cmd eq 'defines') {
    while(my $name = get_configuration_arg(\$args)) {
      push @{$this->{'project'}->{'defines'}}, $name;
    }
  } elsif($cmd eq 'mode') {
    my $mode = get_configuration_arg(\$args);
    $this->{'project'}->{'mode'} = $mode;
  } elsif($cmd eq 'GUID-gen') {
    $this->{'project'}->{'GUID'} = msvc_xml_new_guid($args);
  }
}

sub msvc_xml_project_option
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  msvc_xml_setopt($this->{'project'}->{'opts'}, $conf, $name, $value);
}

sub msvc_xml_project_begin
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
    'solution' => $this,
    'name' => $name,
    'mode' => '',
    'GUID' => msvc_xml_new_guid(),
    'filename' => "$name/$name.vcproj",
    'filters' => {},
    'platforms' => {},
    'configurations' => {},
    'depends' => [],
    'includes' => [],
    'defines' => [],
    'opts' => $opts,
    'a-opts' => [$opts, (@{$this->{'solution'}->{'a-project-opts'}})], 
  };
  my $ret = {
    'type' => 'project',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'project' => $rv,
    'filters' => $rv->{'filters'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
       my ($this) = @_;
#       msvc_xml_project_generate($this->{'project'}, $this->{'G'}->{'templates'});
    },
    'set' => \&set_value,
    'option' => \&msvc_xml_project_option,
    'begins' => {
      'file-group' => \&msvc_xml_filter_begin
    },
    'commands' => {
      'mode' => \&msvc_xml_project_cmd,
      'depend' => \&msvc_xml_project_cmd,
      'includes' => \&msvc_xml_project_cmd,
      'defines' => \&msvc_xml_project_cmd,
      'GUID-gen' => \&msvc_xml_project_cmd,
    },
  };
  $this->{'projects'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_filter_fileinfo
{
  my ($filename, $xopts) = @_;
  $xopts = undef if (defined $xopts) and (%$xopts==0);
  return {
    'filename' => msvc_xml_path_win32($filename),
    'xopts' => $xopts
  };
}

sub msvc_xml_filter_option
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  msvc_xml_setopt($this->{'filter'}->{'opts'}, $conf, $name, $value);
}

sub msvc_xml_filter_file
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
  push @{$this->{'filter'}->{'files'}}, msvc_xml_filter_fileinfo($name);
}

sub msvc_xml_filter_begin
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
    'parent' => $this,
    'name' => $name,
    'filters' => {},
    'files' => [],
    'opts' => $opts,
  };
  my $ret = {
    'type' => 'file-group',
    'G' => $this->{'G'},
    'name' => $name,
    'subs' => {},
    #
    'filter' => $rv,
    'filters' => $rv->{'filters'},
    'sets' => $rv->{'sets'},
    'option' => \&msvc_xml_filter_option,
    #
    'set' => \&set_value,
    'begins' => {
      'file-group' => \&msvc_xml_filter_begin,
      'mask' => \&msvc_xml_mask_begin,
    },
    'commands' => {
      'file' => \&msvc_xml_filter_file,
    },
  };
  $this->{'filters'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_mask_end
{
  my ($this) = @_;
  my $pfiles = $this->{'parent'}->{'filter'}->{'files'};
  for my $file (@{$this->{'files'}}) {
    push @$pfiles, msvc_xml_filter_fileinfo($file, $this->{'xopts'});
  }
}

sub msvc_xml_mask_cmd
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

sub msvc_xml_mask_xoption
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  $this->{'xopts'}->{$name} = $value;
}

sub msvc_xml_mask_begin
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
    'end' => \&msvc_xml_mask_end,
    'commands' => {
      'include' => \&msvc_xml_mask_cmd,
      'exclude' => \&msvc_xml_mask_cmd,
      'xoption' => \&msvc_xml_mask_xoption,
    },
  };
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

return 1;
