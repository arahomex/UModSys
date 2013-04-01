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
  # 8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942
  return sprintf(
    "{%04X%04X-%04X-%04X-%04X-%04X%04X%04X}",
    int(rand(0x10000)), int(rand(0x10000)),
    int(rand(0x10000)), int(rand(0x10000)), int(rand(0x10000)),
    int(rand(0x10000)), int(rand(0x10000)), int(rand(0x10000))
  );
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

sub msvc_xml_solution_generate
{
  my ($sol, $template) = @_;
  my ($filename, $projects) = ($sol->{'filename'}, $sol->{'projects'});
  my ($fout);
  make_filename_dir($filename);
  open $fout,'>',$filename or die "File '$filename' create error.";
  print $fout $uitf8_header;
  print $fout $template->{'solution-begin'};
  foreach my $proj_name (sort keys %$projects) {
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
    my $seed = int(get_configuration_arg(\$args));
    my $basestr = get_configuration_arg(\$args);
    $seed ^= crc32($basestr) if $basestr ne '';
    srand $seed;
    print "Random seed is set to $seed for '$basestr'\n";
    return;
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
    for my $FILE_PATH (@{$filter->{'files'}}) {
      $line = eval("<<EOT\n".$template->{'project-ff-file'}."EOT");
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
  #------------------- CONFIGURATIONS
  $line = eval("<<EOT\n".$template->{'project-configs-begin'}."EOT");
  print $fout $line;
  for my $PLATFORM_NAME (@platforms) {
    for my $CONF_NAME (@confs) {
      #
      my $opt_var_all = '';
      for my $opt_var (keys $opt_vars) {
        my $opt_var_val = msvc_xml_getopt($CONF_NAME, $opt_var, @{$proj->{'a-opts'}});
        $opt_var_val = msvc_xml_getopt('*', $opt_var, @{$proj->{'a-opts'}}) if not defined $opt_var_val;
        $opt_var_val = $opt_vars->{$opt_var} if not defined $opt_var_val;
        $opt_var_all .= "my \$OPT_$opt_var = \"$opt_var_val\";\n";
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
  } elsif($cmd eq 'mode') {
    my $mode = get_configuration_arg(\$args);
    $this->{'project'}->{'mode'} = $mode;
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
    },
  };
  $this->{'projects'}->{$name} = $rv;
  $this->{'subs'}->{$name} = $ret;
  return $ret;
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_filter_file
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
  push @{$this->{'filter'}->{'files'}}, msvc_xml_path_win32($name);
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
  my $rv = {
    'sets' => set_new($this),
    'parent' => $this,
    'name' => $name,
    'filters' => {},
    'files' => [],
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
    #
    'set' => \&set_value,
    'begins' => {
      'file-group' => \&msvc_xml_filter_begin
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

return 1;
