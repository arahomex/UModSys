#!/usr/bin/perl

use strict;
use warnings;

#use Socket qw(PF_INET SOCK_STREAM pack_sockaddr_in inet_aton);
use IO::Socket::INET;
use IO::Select;
use File::Basename;
use Data::Dumper;
use Time::HiRes qw(usleep);
use Carp;

my $stderr = \*STDERR;

#-------------------------------------------------------------

sub node_new($)
{
  my ($name) = @_;
  my $this = {
    'name' => $name,
    'gates' => [],
    'services' => [],
  };
  return $this;
}

sub node_addgate($$) 
{
  my ($this, $gate) = @_;
  push @{$this->{'gates'}}, $gate;
}

sub node_tick($$)
{
  my ($this, $tick) = @_;
  for my $gate (@{$this->{'gates'}}) {
    $gate->{'tick'}->($gate, $tick);
  }
  for my $service (@{$this->{'services'}}) {
    $service->{'tick'}->($service, $tick);
  }
}

#-------------------------------------------------------------

#sub bin_get_char($)
#{
#  my ($data) = @_;
#  substr 
#}

#-------------------------------------------------------------

sub gate_tcp_data_raw32($$)
{
  my ($this, $tick) = @_;
  return undef;
}

sub gate_tcp_data_open($$)
{
  my ($this, $tick) = @_;
  while(1) {
    my $c;
    my $len = read $this->{'sock'}, $c, 1;
    return 0 if not defined $c;
    return undef if $len==0;
    my $b = \{$this->{'buf'}};
    if($c eq "\n") {
      if($$b eq "RAW32") {
        $this->{'func'} = &gate_tcp_tick_raw32;
        return 1;
      }
      return undef; # bad sequence
    }
    $$b .= $c;
  }
}

sub gate_tcp_tick_cli($$)
{
  my ($this, $tick) = @_;
  my $k;
  while(1) {
    $k = $this->{'func'}->($this, $tick);
    if(not defined $k) {
      my $sock = $this->{'sock'};
      print $stderr "Selected $sock close\n";
      $this->{'sel'}->remove($sock);
      close $sock;
      last;
    } 
    last if $k==0;
    $tick = 0;
  }
  return $k;
}

sub gate_tcp_tick_listen($$)
{
  my ($this, $tick) = @_;
  if($this->{'state'}==0) {
    my $s = IO::Socket::INET->new(
      Blocking  => 0,
      Listen    => 1,
      LocalAddr => $this->{'ip'}, 
      LocalPort => $this->{'port'},
      Proto     => 'tcp'
    ) or return;
    my $ss = IO::Select->new();
    $ss->add($s);
    $this->{'sock'} = $s;
    $this->{'sel'} = $ss;
    $this->{'state'} = 1;
    print $stderr "Listen socket created $s\n";
  } elsif($this->{'state'}==1) {
    my ($r) = IO::Select->select($this->{'sel'}, undef, undef, 0);
    for my $sock ( @$r ) {
      print $stderr "Selected $sock\n";
      if($sock == $this->{'sock'}) { # accept
        my $s2 = $sock->accept();
        print $stderr "Selected $sock accept $s2\n";
        my $cli = {
          'buf' => '',
          'sock' => $s2,
          'sel'  => $this->{'sel'},
          'func' => &gate_tcp_tick_open,
        };
        $this->{'clients'}->{$s2} = $cli;
        $this->{'sel'}->add($s2);
      } else { # read data
        print $stderr "Selected $sock client\n";
        my $k = gate_tcp_tick_cli($this->{'clients'}->{$sock}, $tick);
        delete $this->{'clients'}->{$sock} if not defined $k;
      }
    }
  }
}

sub gate_tcp_tick_connect($$)
{
  my ($this, $tick) = @_;
  if($this->{'state'}==0) {
    my $s = IO::Socket::INET->new(
      Blocking  => 0,
      Listen    => 0,
      PeerAddr  => $this->{'ip'}, 
      PeerPort  => $this->{'port'},
      Proto     => 'tcp'
    ) or return;
    my $ss = IO::Select->new();
    $ss->add($s);
    $this->{'sock'} = $s;
    $this->{'sel'} = $ss;
    $this->{'state'} = 1;
    print $stderr "Connect socket created $s\n";
  } elsif($this->{'state'}==1) {
    my $sock = $this->{'sock'};
    if($sock->connected) {
      $this->{'state'} = 2;
      print $stderr "Connect socket connected\n";
      print $sock "RAW32\n";
      $this->{'state'} = 2;
    }
    print $stderr "[connected=".$sock->connected."]";
  } elsif($this->{'state'}==2) {
    my ($r) = IO::Select->select($this->{'sel'}, undef, undef, 0);
    if( @$r!=0 ) {
      my $k = gate_tcp_tick_cli($this, $tick);
      $this->{'state'} = 0 if not defined $k;
    }
  }
}

sub gate_new_tcp($$$)
{
  my ($mode, $ip, $port) = @_;
  my $this = {
    'mode'   => $mode,
    'ip'     => $ip,
    'port'   => $port,
    'sock'   => undef,
    'tick'   => undef,
    'state'  => 0,
    'sel'    => undef,
  };
  if($mode eq 'connect') {
    $this->{'tick'} = \&gate_tcp_tick_listen;
    $this->{'clients'} = {};
  } elsif($mode eq 'listen') {
    $this->{'tick'} = \&gate_tcp_tick_connect;
    $this->{'func'} = \&gate_tcp_tick_raw32;
  } else {
    return undef;
  }
  return $this;
}

#-------------------------------------------------------------
#-------------------------------------------------------------

my $tick = 1000000; # 10 ms
my $node1 = node_new('Node_1');
my $node2 = node_new('Node_2');

node_addgate($node1, gate_new_tcp('connect', 'localhost', '7000'));
node_addgate($node2, gate_new_tcp('listen', 'localhost', '7000'));

while(1) {
  node_tick($node1, $tick);
  node_tick($node2, $tick);
  usleep($tick);
  print $stderr '.';
}

