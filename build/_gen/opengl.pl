#!/bin/env perl

use strict;
use File::Basename;
use XML::Simple;
use Data::Dumper;

#----------------------------------------------------------------

our $script_path;
our $generators = {};
my($filename, $suffix);
($filename, $script_path, $suffix) = fileparse(__FILE__);

our $opt_refresh = 0;
our $dbglevel = 5;
our @funcs;
our ($x_gl, $x_glx, $x_wgl, $x_egl);

#----------------------------------------------------------------

require "$script_path/opengl_pre.pm";
require "$script_path/opengl_gen.pm";

#----------------------------------------------------------------

# 1. analyse args

for my $arg (@ARGV) {
  if($arg =~ /\-\-xml/) {
    $opt_refresh = 1;
    next;
  }
  if($arg =~ /\-\-funcs:(.*)/) {
    push @funcs, $1;
    next;
  }
  #
  if($arg =~ /\-\-debug:(.*)/) {
    $dbglevel = $1+0;
    next;
  }
  die "Unknown argument: $arg";
}

# 2. load profiles

if($opt_refresh) {
  print "load data from XML\n" if $dbglevel>0;
  ($x_gl, $x_egl, $x_wgl, $x_glx) = (
    pre_convert('gl'), pre_convert('egl'),
    pre_convert('wgl'), pre_convert('glx')
  )
} else {
  print "load data from TXT\n" if $dbglevel>0;
  ($x_gl, $x_egl, $x_wgl, $x_glx) = (
    pre_load('gl'), pre_load('egl'),
    pre_load('wgl'), pre_load('glx')
  )
}

# 3. generate functions
for my $fn (@funcs) {
  gen_funcs_from($fn);
}

print "OpenGL loader generator version 0.05 done\n";
