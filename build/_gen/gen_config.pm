use strict;

our $config;
our $generators;

#---------------------------------
#---------------------------------
#---------------------------------

sub set_get
{
  my ($this, $var) = @_;
  my $sets = $this->{'sets'};
#  print "get '$var'\n";
  for my $ss (@$sets) {
    return $ss->{$var} if exists $ss->{$var};
  }
#  print "  got none\n";
  return undef;
}

sub set_explode
{
  my ($this, $line) = @_;
  my $rv = '';
  while($line ne '') {
    if($line =~ /^\$\((.*?)\)(.*)$/) {
      print "['$1'] '$2'\n";
      $line = $2;
      $rv .= set_get($this, $1);
      next;
    }
    if($line =~ /(.*?[^\\])\$\((.*?)\)(.*)$/) {
      print "'$1' ['$2'] '$3'\n";
      $rv .= $1; 
      $line = $3;
      $rv .= set_get($this, $2);
      next;
    }
    $rv .= $line;
    last;
  }
  return $rv;
}

sub set_value
{
  my ($this, $cmd, $args) = @_;
  my $var = get_configuration_arg(\$args);
  $args =~ s/\s*$//;
  my $value =  set_explode($this, $args);
  print "set '$args' '$var'='$value'\n";
  $this->{'sets'}->[0]->{$var} = $value;
}

sub set_new
{
  my ($parent) = @_;
  return [{}, (@{$parent->{'sets'}})];
}


sub get_configuration_arg
{
  my ($line) = @_;
  return undef if not defined $$line; 
  return $1 if $$line =~ s/^\"(.+?)\"\s+(.*)/$2/;
  return $1 if $$line =~ s/^\'(.+?)\'\s+(.*)/$2/;
  return $1 if $$line =~ s/^\`(.+?)\`\s+(.*)/$2/;
  return $1 if $$line =~ s/^(.+?)\s+(.*)/$2/;
  my $rv = $$line;
  $$line = '';
  return $rv;
}

sub get_configuration_arg_exp
{
  my ($line, $this) = @_;
  my $rv = get_configuration_arg($line);
  return $rv if not defined $rv;
  return set_explode($this, $rv);
}

#---------------------------------
#---------------------------------
#---------------------------------

sub init_configuration
{
  $config = {
    'lines' => undef,
    'lineno' => undef,
    'generator' => undef,
    'block' => undef,
  };
}

sub read_configuration
{
  my ($filename) = @_;
  my ($line, $fin);
  #
  my ($lines, $lineno) = ([], 0);
  $config->{'lines'} = $lines;
  #
  open $fin,'<',$filename or die "File '$filename' read error.";
  while(($line=<$fin>)) {
    push @$lines, $line;
    $lineno++;
  }
  print "Read file '$filename' $lineno lines.\n";
  close $fin;
}

sub apply_configuration
{
  my ($lineno, $G, $ctx) = (0, undef, undef);
  my @stack;
  for my $line (@{$config->{'lines'}}) {
    $lineno++;
    $config->{'lineno'} = $lineno;
    $line =~ s/^\s+//;
    $line =~ s/\n$//;
    $line =~ s/\r$//;
    next if ($line eq '') or (substr($line, 0, 1) eq '#');
    #
    my $args = $line;
    my $cmd = get_configuration_arg(\$args);
    if(not defined $config->{'generator'}) {
      if($cmd eq 'generator') {
        my $genid = get_configuration_arg(\$args);
        die "Line $lineno: No generator named '$genid'" if not exists $generators->{$genid};
        $G = $generators->{$genid};
        $config->{'generator'} = $G;
        die "Line $lineno: No open for generator" if not exists $G->{'open'};
        $ctx = $G->{'open'}($G);
        $config->{'block'} = $ctx;
        @stack = ();
        print "Using generator '$genid'\n";
        next;
      }
    } else {
      if($cmd eq 'begin') {
        my $arg = get_configuration_arg(\$args);
        die "Line $lineno: Can't use begin '$arg' $args" if (not exists $ctx->{'begins'}) or (not exists $ctx->{'begins'}->{$arg});
        push @stack, $ctx;
        $ctx = $ctx->{'begins'}->{$arg}($ctx, $arg, $args);
#        print "stack #".@stack." with '".$ctx->{'type'}."' on $line\n";
        next;
      } elsif($cmd eq 'end') {
        $ctx->{'end'}($ctx) if exists $ctx->{'end'};
        die "Line $lineno: Can't use 'end' when no corresponding 'begin'" if @stack==0;
        $ctx = pop @stack;
#        print "stack #".@stack." with '".$ctx->{'type'}."' on $line\n";
        next;
      } elsif($cmd eq 'set') {
        die "Line $lineno: Can't use set '$args'" if not exists $ctx->{'set'};
        $ctx->{'set'}($ctx, $cmd, $args);
        next;
      } elsif($cmd eq 'option') {
        die "Line $lineno: Can't use option $args" if not exists $ctx->{'option'};
        $ctx->{'option'}($ctx, $cmd, $args);
        next;
      } else {
        die "Line $lineno: Can't use command '$cmd' $args" if (not exists $ctx->{'commands'}) or (not exists $ctx->{'commands'}->{$cmd});
        $ctx->{'commands'}->{$cmd}($ctx, $cmd, $args);
        next;
      }
    }
    die "Line $lineno: Invalid configuration line '$line'\n";
  }
  $ctx->{'close'}($ctx) if (defined $ctx) and (exists $ctx->{'close'});
}

return 1;
