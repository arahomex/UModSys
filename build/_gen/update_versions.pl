#!/bin/env perl

use strict;
#use File::Basename;
#use File::Slurp;
use File::stat;

#our $script_path;
#our $generators = {};
#my($filename, $suffix);
#($filename, $script_path, $suffix) = fileparse(__FILE__);

our %sets;
our @files; our @names;
our ($genpath, $gentype, $mode);
our $dbglevel = 1;

# version numbers: major, minor, build, time

#-------------------------------------------------------------

our $check_types = {
  'date' => {
    'init' => sub($) {
      my ($check) = @_;
      my $args = $check->{'args'};
      $check->{'masks'} = [];
      if($args =~ s/^(\S+?)\s+?//) {
        $check->{'path'} = $1;
        while($args =~ s/^(\S+?)\s+?//) {
          push @{$check->{'masks'}}, $1 if $1 ne '';
        }
        push @{$check->{'masks'}}, $args if $args ne '';
      } else {
        $check->{'path'} = $args;
        push @{$check->{'masks'}}, '*';
      }
      $check->{'path'} =~ s/\/$//;
    },
    'check' => sub($$) {
      my ($check, $verfiles) = @_;
      # find all files in list
      my %files; my %dates;
      my $path = $check->{'path'};
      for my $mask (@{$check->{'masks'}}) {
        if($mask =~ s/^!// or $mask =~ s/^~//) {
          add_checks_files(\%files, $path, $mask, 0);
        } else {
          add_checks_files(\%files, $path, $mask, 1);
        }
      }
      for my $x (@$verfiles) {
        delete $files{$x} if exists $files{$x};
        my $s = stat($x);
        next if not defined $s;
        $dates{$x} = $s;
      }
      # check stat time
      for my $fn (keys %files) {
        my $inf = stat($fn);
        next if not defined $inf;
        for my $x (@$verfiles) {
          print "'$fn' time=".$inf->mtime."/".$dates{$x}->mtime."/".($dates{$x}->mtime - $inf->mtime)."\n"  if $dbglevel>4;
          next if (not exists $dates{$x}) or ($dates{$x}->mtime >= $inf->mtime);
          print "version file '$x' is older than '$fn'\n" if $dbglevel>0;
          return 1;
        }
      }
      return 0; # check failed
    },
  },
};

#-------------------------------------------------------------

our $version_files = {
  'h' => {
    'init' => sub($) {
      my ($verfile) = @_;
      my $args = $verfile->{'args'};
      my $prefix = 'VERSION';
      my @kw = ( "_MAJOR", "_MINOR", "_BUILD", "_TIME" );
      if($args =~ s/^(\S+)\s+//) {
        $verfile->{'filename'} = $1;
        while(($args =~ s/^(\w+)\:(\S+)\s+//) or ($args =~ s/^(\w+)\:(\S+)//)) {
          my ($key,$value) = ($1,$2);
          if($key eq 'prefix') {
            $prefix = $value,
          } else {
            for my $i (0..3) {
              $verfile->{'kw'}->[0] = $value if $key eq "kw$i";
            }
          }
        }
      } else {
        $verfile->{'filename'} = $args;
      }
      #
      $verfile->{'numlines'} = [undef,undef,undef,undef],
      $verfile->{'prefix'} = $prefix,
      $verfile->{'kw'} = \@kw;
      #
      $verfile->{'idata'}    = [
        "#define ${prefix}${kw[0]} ",
        "#define ${prefix}${kw[1]} ",
        "#define ${prefix}${kw[2]} ",
        "#define ${prefix}${kw[3]} ",
      ],
    },
    'in' => sub($$) {
      my ($verfile, $line) = @_;
      my $prefix = $verfile->{'prefix'};
      for my $i (0..3) {
        my $keyword = $verfile->{'kw'}->[$i];
        if($line =~ /(\s*?\#define\s+?$prefix$keyword\s+?)(\w*)\s*$/) {
          $verfile->{'idata'}->[$i] = $1;
          $verfile->{'numbers'}->[$i] = $2+0;
          $verfile->{'numlines'}->[$i] = $line;
          return 1;
        }
      }
      return 0;
    },
    'needwrite' => sub($) {
      my ($verfile) = @_;
      for my $i (0..3) {
        return 1 if not defined $verfile->{'numlines'}->[$i];
      }
      return 0;
    },
    'out' => sub($$) {
      my ($verfile, $refline) = @_;
      for my $i (0..3) {
        $$refline = $verfile->{'idata'}->[$i].$verfile->{'numbers'}->[$i] 
        if $$refline eq $verfile->{'numlines'}->[$i];
      }
    },
    'outlast' => sub($) {
      my ($verfile) = @_;
      for my $i (0..3) {
        if(not defined $verfile->{'numlines'}->[$i]) {
          $verfile->{'numlines'}->[$i] = 1;
          return $verfile->{'idata'}->[$i].$verfile->{'numbers'}->[$i];
        };
      }
      return undef;
    },
  },
  'c' => {
  },
  'c++' => {
  },
  'nsh' => {
  },
};

#-------------------------------------------------------------

sub set_get
{
  my ($key, @sets) = @_;
  for my $set (@sets) {
    return $set->{$key} if defined $set and exists $set->{$key};
  }
  return undef;
}

sub set_explode
{
  my ($line, @sets) = @_;
  my $rv = '';
  while($line ne '') {
    if($line =~ /^\$\((.*?)\)(.*)$/) {
      $line = $2;
      $rv .= set_get($1, @sets);
      next;
    }
    if($line =~ /(.*?[^\\])\$\((.*?)\)(.*)$/) {
      $rv .= $1; 
      $line = $3;
      $rv .= set_get($2, @sets);
      next;
    }
    if($line =~ /(.*?)[\\]\$(.*?)$/) {
      $rv .= $1.'$'; 
      $line = $2;
      next;
    }
    $rv .= $line;
    last;
  }
  return $rv;
}

#-------------------------------------------------------------

sub process_file($)
{
  my ($filename) = @_;
  my ($fin, $line);
  my $lineno = 0;
  my $version = undef;
  my %versions;
  #
  open $fin,'<',$filename or die "File '$filename' read error.";
  #
  while(($line=<$fin>)) {
    $line =~ s/^\s+//;
    $line =~ s/\n$//; $line =~ s/\r$//;
    $lineno++;
    next if $line eq '';
    next if substr($line,0,1) eq '#';
    next if substr($line,0,1) eq ';';
    #
    if($line =~ /^\s*version\s+?(.*?)$/) {
      my $name = $1;
      $versions{$version->{'name'}} = $version if defined $version;
      $version = {
        'name'     => $name,
        'numbers'  => [0,0,0,time()],
        'files'    => [],
        'checks'   => [],
      };
      next;
    }
    #
    if($line =~ /^\s*file[\-\.\:\_](\w+)\s+(.*)$/) {
      die "No version file processor $1" if not exists $version_files->{$1};
      my $vf = {
        'type'      => $1,
        'args'      => $2,
        'numbers'   => [0,0,0,0],
        'lines'     => [],
        'processor' => $version_files->{$1},
      };
      $vf->{'processor'}->{'init'}->($vf);
      push @{$version->{'files'}}, $vf;
      print "version file ".$vf->{'type'}." '".$vf->{'filename'}."'\n" if $dbglevel>2;
      next;
    }
    #
    if($line =~ /^\s*check[\-\.\:\_](\w+)\s+(.*)$/) {
      die "No check processor $1" if not exists $check_types->{$1};
      my $chk = { 
        'type'      => $1,
        'args'      => $2,
        'processor' => $check_types->{$1},
      };
      $chk->{'processor'}->{'init'}->($chk);
      push @{$version->{'checks'}}, $chk;
      next;
    }
    #
  }
  close $fin;
  $versions{$version->{'name'}} = $version if defined $version;
  print "Read script file '$filename' $lineno lines.\n" if $dbglevel>0;
  #
  if(defined $genpath and defined $gentype) {
    for $version (values %versions) {
      generate_file($genpath.'/', $gentype, $version);
    }
  }
  if(defined $mode) {
    my $processed = 0;
    my $all = 0;
    for my $n (@names) {
      if($n eq '*') {
        $all++; next;
      }
      next if not exists $versions{$n};
      process_version($versions{$n}, $mode);
      $processed++;
    }
    if($all) {
      for my $v (values %versions) {
        process_version($v, $mode);
        $processed++;
      }
    }
    print "Processed version groups: $processed\n" if $dbglevel>0;
  }
}

#---------------------------------
#---------------------------------

sub version_number_max($$)
{
  my ($num1, $num2) = @_;
  my $rv = 0;
  # sync major, minor
  if($num1->[0]<$num2->[0]) {
    $num1->[0] = $num2->[0];
    $num1->[1] = $num2->[1];
    $rv++;
  } elsif(($num1->[0]==$num2->[0]) and ($num1->[1]<$num2->[1])) {
    $num1->[1] = $num2->[1];
    $rv++;
  }
  # sync build, time
  if($num1->[2]<$num2->[2]) {
    $num1->[2] = $num2->[2];
    $num1->[3] = $num2->[3];
    $rv++;
  }
  return $rv;
}

sub is_version_needwrite($)
{
  my ($version) = @_;
  for my $vf (@{$version->{'files'}}) {
    if($vf->{'processor'}->{'needwrite'}->($vf)) {
      return 1;
    }
  }
  return 0;
}

sub is_version_checks($)
{
  my ($version) = @_;
  my @verfiles;
  for my $vf (@{$version->{'files'}}) {
    push @verfiles, $vf->{'filename'} if exists $vf->{'filename'};
  }
  for my $check (@{$version->{'checks'}}) {
    return 1 if $check->{'processor'}->{'check'}->($check, \@verfiles);
  }
  return 0;
}

sub process_version($$)
{
  my ($version, $mode) = @_;
  # -- load all version files
  for my $vf (@{$version->{'files'}}) {
    if(load_version_file($vf)) {
      version_number_max($version->{'numbers'}, $vf->{'numbers'});
    }
  }
  my $vnw = is_version_needwrite($version);
#  die "Invalid mode $mode";
  # -- process all checks
  if($mode eq 'build') { # increment build number
    $version->{'numbers'}->[2]++;
    $version->{'numbers'}->[3] = time();
  } elsif(($mode eq 'autobuild') or ($mode eq 'auto')) { # increment build number only if check files
    if($vnw or is_version_checks($version)) {
      $version->{'numbers'}->[2]++;
      $version->{'numbers'}->[3] = time();
    }
  } elsif(($mode eq 'autominor') or ($mode eq 'inc')) { # increment minor only if check files
    if($vnw or is_version_checks($version)) {
      $version->{'numbers'}->[1]++;
      $version->{'numbers'}->[2]++;
      $version->{'numbers'}->[3] = time();
    }
  } elsif(($mode eq 'minor') or ($mode eq 'lo')) { # increment minor
    $version->{'numbers'}->[1]++;
    $version->{'numbers'}->[2]++;
    $version->{'numbers'}->[3] = time();
  } elsif(($mode eq 'major') or ($mode eq 'hi')) { # increment major
    $version->{'numbers'}->[0]++;
    $version->{'numbers'}->[1] = 0;
    $version->{'numbers'}->[2]++;
    $version->{'numbers'}->[3] = time();
  } else {
    die "Invalid mode $mode";
  }
  print "Version [".join(',',@{$version->{'numbers'}})."] of ".$version->{'name'}." for $mode\n" if $dbglevel>1;
  # -- correct all version files
  for my $vf (@{$version->{'files'}}) {
    my $f = version_number_max($vf->{'numbers'}, $version->{'numbers'});
    if($f or $vnw) {
      save_version_file($vf);
    }
  }
}

#---------------------------------

sub load_version_file($)
{
  my ($verfile) = @_;
  my $fin; 
  my $filename = $verfile->{'filename'};
  my $lines = $verfile->{'lines'};
  my $lineno = 0;
  my $processor = $verfile->{'processor'}->{'in'};
  print "Reading version file '$filename' ...\n"  if $dbglevel>2;
  if(open $fin,'<',$filename) {
    while(my $line=<$fin>) {
      $line =~ s/\n$//; $line =~ s/\r$//;
      push @$lines, $line;
      $lineno++;
      $processor->($verfile, $line);
    }
    close $fin;
    print "Read version file '$filename' $lineno lines.\n"  if $dbglevel>1;
    return 1;
  }
  return 0;
}

sub save_version_file($)
{
  my ($verfile) = @_;
  my $fout; 
  my $filename = $verfile->{'filename'};
  my $processor = $verfile->{'processor'}->{'out'};
  my $lineno = 0;
  open($fout,'>',$filename) or die "Can't open write file $filename";
  for my $line (@{$verfile->{'lines'}}) {
    $processor->($verfile, \$line);
    print $fout $line."\n";
    $lineno++;
  }
  $processor = $verfile->{'processor'}->{'outlast'};
  while(my $line = $processor->($verfile)) {
    print $fout $line."\n";
    $lineno++;
  }
  close $fout;
  print "Written version [".join(',',@{$verfile->{'numbers'}})."] file '$filename' $lineno lines.\n" if $dbglevel>0;
}

#---------------------------------

sub generate_file($$$)
{
  my ($pathname, $gentype, $version) = @_;
  my ($fin, $line);
  my $lineno = 0;
  my $fout = undef;
  #
  open $fin,'<',$gentype or die "File '$gentype' read error.";
  #
  my $set = {
    'name' => $version->{'name'}
  };
  #
  while(($line=<$fin>)) {
    $line =~ s/\n$//;
    $line =~ s/\r$//;
    $lineno++;
    next if $line eq '';
    #
    if($line =~ /^\.(\w*)\s+(.*)/) {
      my $cmd = $1; my $args=$2;
      if($cmd eq 'FILE') {
        my $filename = $pathname.set_explode($args, $set, \%sets);
        print "open file $filename\n" if $dbglevel>3;
        open $fout,'>',$filename or die "File '$filename' create error.";
      } elsif(($cmd eq 'SET')) {
        if($args =~ /^\.(\w*)\s+(.*)/) {
          $set->{$1} = set_explode($2, $set, \%sets);
        } else {
          delete $set->{$args};
        }
      } elsif(($cmd eq 'LINE') or ($cmd eq '')) {
        print $fout $args."\n" if defined $fout;
      }
      next;
    }
    print $fout set_explode($line, $set, \%sets)."\n" if defined $fout;
  }
  close $fout if defined $fout;
  close $fin;
  print "Read generator file '$gentype' $lineno lines.\n" if $dbglevel>1;
}

#---------------------------------

sub filename_search_files($$)
{
  my ($path, $mask) = @_;
  my @fn = glob "${path}/${mask}";
  return undef if @fn==0;
  return \@fn;
}

sub add_checks_files($$$$)
{
  my ($files, $path, $mask, $is_include) = @_;
  #
  my $rf = filename_search_files($path, $mask);
  if(defined $rf) {
    for my $fn (@$rf) {
      if($is_include) {
        $files->{$fn} = 1 if not exists $files->{$fn};
      } else {
        delete $files->{$fn} if exists $files->{$fn};
      }
    }
  }
}

#-------------------------------------------------------------

for my $arg (@ARGV) {
  if($arg =~ /\-\-set\:(.*)\:(.*)/) {
    $sets{$1} = $2;
    next;
  }
  if($arg =~ /\-\-file\:(.*)/) {
    push @files, $1;
    next;
  }
  #
  if($arg =~ /\-\-genpath\:(.*)/) {
    $genpath = $1;
    next;
  }
  if($arg =~ /\-\-gentype\:(.*)/) {
    $gentype = $1;
    next;
  }
  #
  if($arg =~ /\-\-mode\:(.*)/) {
    $mode = $1;
    next;
  }
  if($arg =~ /\-\-name\:(.*)/) {
    push @names, $1;
    next;
  }
  if($arg =~ /\-\-all/) {
    push @names, '*';
    next;
  }
  if($arg =~ /\-\-debug:(.*)/) {
    $dbglevel = $1+0;
    next;
  }
  die "Unknown argument: $arg";
}

#print "Version check mode #$mode\n";
for my $fn (@files) {
  process_file($fn);
}

print "Version generator version 0.05 done\n";
