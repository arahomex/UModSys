use strict;

our $script_path;
our $dbglevel;

sub file_load_string($)
{
  my ($filename) = @_;
  local $/;
  my ($F, $data);
  open $F, "<$filename" or die "Can't open file '$filename'";
  $data = <$F>;
  close $F;
  return $data;
}

sub file_load_lines($)
{
  my ($filename) = @_;
  my ($F, @data);
  open $F, "<$filename" or die "Can't open file '$filename'";
  @data = <$F>;
  close $F;
  return @data;
}

sub file_save_lines($$)
{
  my ($filename, $lines) = @_;
  my ($F);
  open $F, ">$filename" or die "Can't create file '$filename'";
  for my $line (@$lines) {
    print $F $line."\n";
  }
  close $F;
}

sub pre_convert($)
{
  my ($id) = @_;
  my ($filename1, $filename2) = ("$id.xml", "$id.txt"); 
  #
  my $xl = XML::Simple->new(
    ForceArray=>1,
    NoSort => 1,
    KeepRoot => 1,
    SuppressEmpty => 1,
    NormaliseSpace => 1
  );
  print "  Loading XML $filename1\n" if $dbglevel>2;
  my $data = $xl->XMLin("$script_path/opengl/$filename1");
  #
  print "  Generating TXT $filename2\n" if $dbglevel>2;
  my $dd = Data::Dumper->new([$data], ['data']);
  $dd->Indent(1);
  $dd->Purity(1);
  $dd->Useqq(1);
  #
  print "  Saving TXT $filename2\n" if $dbglevel>2;
  file_save_lines("$script_path/opengl/$filename2", [$dd->Dump()]);
  return $data;
}

sub pre_load($)
{
  my ($id) = @_;
  my $filename2 = "$id.txt";
  print "  Loading TXT $filename2\n" if $dbglevel>2;
  my $lines = file_load_string("$script_path/opengl/$filename2");
  my $data;
no strict 'vars';
  eval($lines);
use strict 'vars';
  return $data;
}

return 1;
