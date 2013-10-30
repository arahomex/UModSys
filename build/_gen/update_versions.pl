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
our @files;
our $mode = 0;

#-------------------------------------------------------------

sub process_file($)
{
  my ($filename) = @_;
  my ($fin, $line, $verfile);
  my $lineno = 0;
  my $checks = [];
  #
  open $fin,'<',$filename or die "File '$filename' read error.";
  #
  while(($line=<$fin>)) {
    $line =~ s/^\s+//;
    $line =~ s/\n$//;
    $line =~ s/\r$//;
    $lineno++;
    next if $line eq '';
    #
    if($line =~ /file\s+(.*?)$/) {
      my $fn = $1;
      check_files($verfile, $checks) if defined $verfile;
      $checks = [];
      $verfile = $fn;
      next;
    }
    #
    if($line =~ s/check\s+(.*?)\s+//) {
      my $path = $1;
      $path =~ s/\/$//;
      my $chk = [$path];
      while($line =~ s/(.*?)\s+//) {
        push @$chk, $1 if $1 ne '';
      }
      push @$chk, $line if $line ne '';
      push @$checks, $chk;
      next;
    }
    #
  }
  check_files($verfile, $checks) if (defined $verfile) and ($verfile ne '');
  #
  close $fin;
  print "Read file '$filename' $lineno lines.\n";
}

#---------------------------------

sub filename_search_files($$)
{
  my ($path, $mask) = @_;
#  my @fn = glob "'${path}/${mask}'";
  my @fn = glob "${path}/${mask}";
#  print "search at: '$path/$mask': @fn\n";
#  return undef if (not defined @fn) or (@fn==0);
  return undef if @fn==0;
  return \@fn;
}

#---------------------------------

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

#---------------------------------

sub check_files($$$)
{
  my ($verfile, $checks, $prefix) = @_;
  return if (not defined $verfile) or (not defined $checks) or (@$checks==0);
  #
  my $f;
  my $res = 1;
  my @fmt_lo = ("#define ${prefix}VERSION_LO ", 0, undef);
  my @fmt_hi = ("#define ${prefix}VERSION_HI ", 0, undef);
  my @lines = ();
  #
  if(open $f,'<',$verfile) {
    while(my $line=<$f>) {
      push @lines, $line;
      if($line =~ /(\s*?\#define\s+?${prefix}VERSION_LO\s+?)(\w*)\s*$/) {
        @fmt_lo = ($1, $2+0, $line);
        next;
      }
      if($line =~ /(\s*?\#define\s+?${prefix}VERSION_HI\s+?)(\w*)\s*$/) {
        @fmt_hi = ($1, $2+0, $line);
        next;
      }
    }
    close $f;
  }
  #
  if($res==0) {
    my %files;
    for my $ch (@$checks) {
      my $path = $ch->[0];
      if(@$ch==1) {
        add_checks(\%files, $path, '*', 1);
      } else {
        for(my $i=1; $i<@$ch; $i++) {
          my $mask = $ch->[$i];
          if($mask =~ s/^!// or $mask =~ s/^~//) {
            add_checks_files(\%files, $path, $mask, 0);
          } else {
            add_checks_files(\%files, $path, $mask, 1);
          }
        }
      }
    }
    delete $files{$verfile} if exists $files{$verfile};
    my $reftime = stat($verfile)->mtime;
    for my $fn (keys %files) {
      my $inf = stat($fn);
      if ((defined $inf) and ($inf->mtime>$reftime)) {
        print "'$verfile' is older than '$fn'\n";
      }
      $res++ if (defined $inf) and ($inf->mtime>$reftime);
    }
  }
  #
  if($res) {
    if($mode==0) { $fmt_lo[1]++ };
    if($mode==1) { $fmt_lo[1]=0; $fmt_hi[1]++ };
    print "'$verfile' version incremented to ".$fmt_hi[1].",".$fmt_lo[1]."\n";
    #
    if(open $f,'>',$verfile) {
      for my $line (@lines) {
        if((defined $fmt_lo[2]) and ($line eq $fmt_lo[2])) {
          print $f $fmt_lo[0].$fmt_lo[1]."\n";
          next;
        }
        if((defined $fmt_hi[2]) and ($line eq $fmt_hi[2])) {
          print $f $fmt_hi[0].$fmt_hi[1]."\n";
          next;
        }
        print $f $line."\n";
      }
      if(not defined $fmt_lo[2]) {
        print $f $fmt_lo[0].$fmt_lo[1]."\n";
        next;
      }
      if(not defined $fmt_hi[2]) {
        print $f $fmt_hi[0].$fmt_hi[1]."\n";
        next;
      }
      close $f;
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
  
  if(($arg eq '-lo') or ($arg eq '-LO')) {
    $mode = 1;
    next;
  }
  if(($arg eq '-hi') or ($arg eq '-HI')) {
    $mode = 2;
    next;
  }
}

#print "Version check mode #$mode\n";
for my $fn (@files) {
  process_file($fn);
}

