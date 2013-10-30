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

my $input_file = 'gen_options.txt';
my $input_path = '';

if(@ARGV==1) {
 $input_file = $ARGV[0];
} elsif(@ARGV==2) {
 $input_path = $ARGV[0];
 $input_file = $ARGV[1];
}

my $config = init_configuration();
read_configuration($config, $input_file);
my $gen = apply_configuration($config);
exec_generator($config, $gen);

print "Generator version 0.37 done\n";
