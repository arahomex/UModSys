#!/bin/env perl

use strict;
use File::Basename;

our $script_path;
our $generators = {};
my($filename, $suffix);
($filename, $script_path, $suffix) = fileparse(__FILE__);

require "$script_path/gen_config.pm";
require "$script_path/gen_msvc9.pm";
require "$script_path/gen_msvc8.pm";


my $config = init_configuration();
read_configuration($config, 'gen_options.txt');
my $gen = apply_configuration($config);
exec_generator($config, $gen);

print "Generator version 0.35 done\n";
