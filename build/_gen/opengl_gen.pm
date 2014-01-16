use strict;

our $script_path;
our $dbglevel;
our ($x_gl, $x_glx, $x_wgl, $x_egl);

sub gen_find_group($$$)
{
  my ($profile, $sec, $name) = @_;
  my $R = $profile->{'registry'}->[0];
  #
  # find in features
  for my $key (keys %{$R->{'feature'}}) {
    my $val = $R->{'feature'}->{$key};
    for my $rq (@{$val->{'require'}}) {
#      next if exists $rq->{'comment'};
      next if not exists $rq->{$sec};
      return 'feature,'.$key if exists $rq->{$sec}->{$name};
    }
  }
  #
  # find in extensions
  for my $key (keys %{$R->{'extensions'}->[0]->{'extension'}}) {
    my $val = $R->{'extensions'}->[0]->{'extension'}->{$key};
    for my $rq (@{$val->{'require'}}) {
#      next if exists $rq->{'comment'};
      next if not exists $rq->{$sec};
      return 'extension,'.$key if exists $rq->{$sec}->{$name};
    }
  }
  #
  return 'general,general';
}

sub gen_func_arg($$$$)
{
  my ($def,$isfull,$namepre,$namesuf) = @_;
  my $con = $def->{'content'};
  my $name;
  if($isfull) {
    my $n = $def->{'name'}->[0];
    $n =~ s/^\s+//; $n =~ s/\s+$//;
    $name = $namepre . $def->{'name'}->[0] . $namesuf;
  } else {
    $name = $namepre . $namesuf;
  }
  $name = ' '.$name if $name ne '';
  my $t = $def->{'ptype'}->[0];
  $t =~ s/^\s+//; $t =~ s/\s+$//;
  if(not defined $con) {
    return $t.$name;
  } elsif(ref $con eq '') {
    $con =~ s/^\s+//; $con =~ s/\s+$//;
    $con = ' '.$con if $con ne '';
    return $t.$con.$name;
  } elsif(ref $con eq 'ARRAY') {
    my ($c1, $c2) = @$con;
    $c1 =~ s/^\s+//; $c1 =~ s/\s+$//; $c1 = $c1.' ' if $c1 ne '';
    $c2 =~ s/^\s+//; $c2 =~ s/\s+$//; $c2 = ' '.$c2 if $c1 ne '';
    return $c1.' '.$t.$c2.$name;
  } else {
    die 'invalid content '.ref $con;
  }
}

sub gen_func1($;$)
{
  my ($def,$full) = @_;
  my $rv = '';
  my $par = $def->{'param'};
  my $delim = $full ? ', ' : ',';
  for my $a (@$par) {
    $rv .= $delim if $rv ne '';
    $rv .= gen_func_arg($a, $full, '', '');
  }
  my $pros = gen_func_arg($def->{'proto'}->[0], $full, '(APIENTRYP ', ')');
  return "$pros($rv)";
}

sub gen_funcs($$$$)
{
  my ($profile, $mode, $requires, $fid) = @_;
  my ($filename1, $filename2) = ("$fid.def.h", "$fid.ldr.h"); 
  my ($F, @lines1, @lines2, %groups);
  #
  print "  Generate files $filename1, $filename2 with array(".(@$requires).")\n" if $dbglevel>2;
  #
  my $cmds = $profile->{'registry'}->[0]->{'commands'}->[0]->{'command'};
  #
  for my $val (@$cmds) {
    my $funcname = $val->{'proto'}->[0]->{'name'}->[0];
    my $group = gen_find_group($profile, 'command', $funcname);
    my $ok = 0;
    if($mode eq 'command') {
      for my $m (@$requires) {
        if( $funcname =~ $m ) {
          $ok = 1;
          last;
        }
      }
    } elsif($mode eq 'group') {
      for my $m (@$requires) {
        if( $group =~ $m ) {
          $ok = 1;
          last;
        }
      }
    } else {
      $ok = 1;
    }
    #
    next if !$ok;
    push @lines1, gen_func1($val, 1).';';
    $groups{$group} = [] if not exists $groups{$group};
    my $funcdef = gen_func1($val, 0);
    push @{$groups{$group}}, "  GL_Link($funcname, \"$funcname\", $funcdef)";

  }
  #
  for my $gn (keys %groups) {
    my $grp = $groups{$gn};
    push @lines2, "GL_LinkGroupBegin($gn)";
    for my $line (@$grp) {
      push @lines2, $line;
    }
    push @lines2, "GL_LinkGroupEnd($gn)";
  }
  #
  print "  Save $filename1\n" if $dbglevel>2;
  file_save_lines($filename1, \@lines1);
  #
  print "  Save $filename2\n" if $dbglevel>2;
  file_save_lines($filename2, \@lines2);
}

sub gen_funcs_from($)
{
  my ($filename) = @_;
  my @lines = file_load_lines($filename);
  my $funlist = [];
  my ($sfn, $path, $suf) = fileparse($filename);
  my $id = $path . $sfn;
  my $mode = 'command';
  my $lineno = 0;
  for my $line0 (@lines) {
    $line0 =~ s/[\n\r]//g;
    my $line = $line0;
    $lineno++;
    $line =~ s/^\s*//g;
    $line =~ s/\s*$//g;
    #
    if($line eq '' or $line =~ /^#/) { # comment
      next;
    } elsif($line =~ /^>(.+)$/) { # write to file
      my $newid = $1;
      if(@$funlist and $id ne '') {
        gen_funcs($x_gl, $mode, $funlist, $id);
        $funlist = [];
      }
      $id = $newid;
    } elsif($line =~ /^MODE (\w+)$/ or $line =~ /^mode (\w+)$/) { # mode
      $mode = $1;
    } elsif($line =~ /^([\.\+\*\[\]\w\\\(\)\:\?\!\^\$]+)$/) {
      push @$funlist, qr{$1};
    } else {
      die "Invalid line '$line0' $lineno";
    }
  }
  if(@$funlist and $id ne '') {
    gen_funcs($x_gl, $mode, $funlist, $id);
    $funlist = [];
  }
}

return 1;
