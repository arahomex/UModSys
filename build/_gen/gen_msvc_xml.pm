use strict;
use File::Basename;

my $uitf8_header = "\xEF\xBB\xBF";

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
  print "find opt '$conf'.'$optname' at #".@refs."\n";
  for my $ref (@refs) {
    next if not exists $ref->{$conf};
    next if not exists $ref->{$conf}->{$optname};
    return $ref->{$conf}->{$optname};
  }
  return undef;
}

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
    my $seed = int($args);
    srand $seed;
    print "Random seed is set to $seed\n";
    return;
  } elsif($name eq 'project') {
    my $conf = get_configuration_arg(\$args);
    my $name = get_configuration_arg(\$args);
    my $value = get_configuration_arg_exp(\$args, $this);
    msvc_xml_setopt($this->{'solution'}->{'project-opts'}, $conf, $name, $value);
    print "setopt '$conf'.'$name' => '$value'\n";
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
  my $project_opts = {};
  my $rv = {
    'sets' => set_new($this),
    'name' => $name,
    'filename' => "$name.sln",
    'projects' => {},
    'project-opts' => $project_opts,
    'a-project-opts' => [$project_opts, $this->{'a-project-opts'}],
  };
  return {
    'type' => 'solution',
    'G' => $this,
    'name' => $name,
    'solution' => $rv,
    'projects' => $rv->{'projects'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
       my ($this) = @_;
#       print "{".$this->{'type'}.",".$this->{'G'}->{'type'}.",".$this->{'G'}->{'templates'}->{'type'}."}\n";
       msvc_xml_solution_generate($this->{'solution'}, $this->{'G'}->{'templates'});
    },
    'set' => \&set_value,
    'option' => \&msvc_xml_solution_option,
    'begins' => {
      'project' => \&msvc_xml_project_begin
    },
    'commands' => {
    },
  };
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
  $line = eval("<<EOT\n".$template->{'project-begin'}."EOT");
  print $fout $line;
  #
  #------------------- PLATFORMS
  my @platforms = split / /, msvc_xml_getopt('[]', 'Platforms', @{$proj->{'a-opts'}});
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
    my $name = get_configuration_arg(\$args);
  }
}

sub msvc_xml_project_option
{
  my ($this, $cmd, $args) = @_;
  my $conf = get_configuration_arg(\$args);
  my $name = get_configuration_arg(\$args);
  my $value = get_configuration_arg_exp(\$args, $this);
  msvc_xml_setopt($this->{'opts'}, $conf, $name, $value);
}

sub msvc_xml_project_begin
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg(\$args);
  my $opts = {};
  my $rv = {
    'sets' => set_new($this),
    'solution' => $this,
    'name' => $name,
    'GUID' => msvc_xml_new_guid(),
    'filename' => "$name/$name.vcproj",
    'filters' => {},
    'platforms' => {},
    'configurations' => {},
    'depends' => [],
    'opts' => $opts,
    'a-opts' => [$opts, (@{$this->{'solution'}->{'a-project-opts'}})], 
  };
  $this->{'projects'}->{$name} = $rv;
  return {
    'type' => 'project',
    'G' => $this->{'G'},
    'name' => $name,
    'project' => $rv,
    'filters' => $rv->{'filters'},
    'sets' => $rv->{'sets'},
    #
    'end' => sub {
       my ($this) = @_;
       msvc_xml_project_generate($this->{'project'}, $this->{'G'}->{'templates'});
    },
    'set' => \&set_value,
    'option' => \&msvc_xml_project_option,
    'begins' => {
      'filter' => \&msvc_xml_filter_begin
    },
    'commands' => {
      'mode' => \&msvc_xml_project_cmd,
      'depend' => \&msvc_xml_project_cmd,
    },
  };
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

sub msvc_xml_filter_file
{
  my ($this, $cmd, $args) = @_;
  my $name = get_configuration_arg_exp(\$args, $this);
  push @{$this->{'filter'}->{'files'}}, $name;
}

sub msvc_xml_filter_begin
{
  my ($this, $keyname, $args) = @_;
  my $name = get_configuration_arg(\$args);
  my $rv = {
    'sets' => set_new($this),
    'parent' => $this,
    'name' => $name,
    'filters' => {},
    'files' => [],
  };
  $this->{'filters'}->{$name} = $rv;
  return {
    'type' => 'filter',
    'G' => $this->{'G'},
    'name' => $name,
    'filter' => $rv,
    'filters' => $rv->{'filters'},
    'sets' => $rv->{'sets'},
    #
    'set' => \&set_value,
    'begins' => {
      'filter' => \&msvc_xml_filter_begin
    },
    'commands' => {
      'file' => \&msvc_xml_filter_file,
    },
  };
};

#--------------------------------------------------------------------
#--------------------------------------------------------------------
#--------------------------------------------------------------------

return 1;
