#!/bin/env perl

use strict;
use File::Basename;

our $script_path;
our $generators = {};
our $config = {};
my($filename, $suffix);
($filename, $script_path, $suffix) = fileparse(__FILE__);


require "$script_path/gen_config.pm";
require "$script_path/gen_msvc9.pm";
require "$script_path/gen_msvc8.pm";


init_configuration();
read_configuration('gen_options.txt');
apply_configuration();

#my $gen = $generators->{'msvc9'};
#$gen->{'generator'}($gen, $config);

