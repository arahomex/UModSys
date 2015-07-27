#!/usr/bin perl

use strict;

our (%list1);
our (%list2);
our (%list3);

our $MAX_UTF32 = 0x200000;

our $outpath = "../../src/core/";

#-------------------------------------------

open INFILE, "<CaseFolding-3.2.0.txt" or die "Can't import";
while(my $line=<INFILE>) {
  next if substr($line,0,1) eq '#';
  if($line =~ /^\s*(\w+)\s*\;\s*(\w+)\s*\;\s*([\w\s]+)\s*\;/) {
    my ($k1, $mode, $k2) = ($1, $2, $3);
    $k2 =~ s/^\s+//; $k2 =~ s/\s+$//; while($k2 =~ s/\s+/,/) {};
    $list1{$k2} = $k1 if (($mode eq 'C') or ($mode eq 'F'));
    $list2{$k2} = $k1 if (($mode eq 'C') or ($mode eq 'S'));
    $list3{$k2} = $k1 if ($mode eq 'T');
  }
}
close INFILE;

#-------------------------------------------

our $OUTFILE;

sub plist_map($$)
{
  my ($kw, $list) = @_;
  print " -- $kw \n";
  print $OUTFILE "\n/***************** $kw */\n";
  for my $k (sort keys %$list) { 
    my @a = split /,/, $k;
    print $OUTFILE sprintf("%s1(0x%x, 0x%x) /*%s*/\n", $kw, hex $list->{$k}, hex $a[0], $k) if @a==1;
    print $OUTFILE sprintf("%s2(0x%x, 0x%x, 0x%x) /*%s*/\n", $kw, hex $list->{$k}, hex $a[0], hex $a[1], $k) if @a==2;
    print $OUTFILE sprintf("%s3(0x%x, 0x%x, 0x%x, 0x%x) /*%s*/\n", $kw, hex $list->{$k}, hex $a[0], hex $a[1], hex $a[2], $k) if @a==3;
    print $OUTFILE sprintf("%s4(0x%x, 0x%x, 0x%x, 0x%x, 0x%x) /*%s*/\n", $kw, hex $list->{$k}, hex $a[0], hex $a[1], hex $a[2], hex $a[3], $k) if @a==4;
  }
}

sub plist_q($$$$$)
{
  my ($kw, $list, $quant, $linelen, $linelen2) = @_;
  print " -- [$quant] $kw \n";
  #-------------------
  my (%grp, @refs, %rrefs, $lastgrp);
  for my $k (keys %$list) {
    my @a = split /,/, $k;
    next if @a!=1;
    my $k1 = hex $a[0];
    my $k2 = sprintf("%05x", ($k1/$quant));
    my $q;
    if(not exists $grp{$k2}) {
      $q = [];
      for(my $i=0; $i<$quant; $i++) {
        $q->[$i] = 0;
      }
      $grp{$k2} = $q;
    } else {
      $q = $grp{$k2};
    }
    $q->[$k1%$quant] = hex $list->{$k};
  }
  print sprintf("   -- grp: %d\n", 0 + keys %grp);
  for my $k (sort keys %grp) { 
    $rrefs{$k} = 0 + @refs;
    push @refs, $k;
    $lastgrp = $k;
  }
  #-------------------
  print $OUTFILE "\n/***************** $kw */\n";
  print $OUTFILE sprintf(
    "/* %d+%d=%d bytes */\n",
    4*$quant*@refs,
    1*$MAX_UTF32/$quant,
    4*$quant*@refs + 1*$MAX_UTF32/$quant
  );
  print $OUTFILE "/***************** */\n\n";
  print $OUTFILE sprintf("UTF32 %s_BMAP[%d][%d]={\n", $kw, 0 + @refs, $quant);
  for my $k (@refs) { 
    my $g = $grp{$k};
    print $OUTFILE sprintf("  {   /* 0x%s */\n", $k);
    #
    for(my $i=0; $i<$quant; $i++) {
      print $OUTFILE "    " if ($i % $linelen)==0;
      print $OUTFILE sprintf("0x%05x", $g->[$i]);
      print $OUTFILE ", " if ($i!=$quant-1);
      print $OUTFILE "\n" if (($i % $linelen)==($linelen-1)) or ($i==$quant-1);
    }
    #
    my $ch = ','; $ch = ' ' if $k eq $lastgrp;
    print $OUTFILE sprintf("  }%s  /* 0x%s */\n", $ch, $k);
  }
  print $OUTFILE sprintf("}; /* UTF32 %s_BMAP[%d][%d] */\n", $kw, 0 + @refs, $quant);
  #
  print $OUTFILE sprintf("BMAP_ELEM %s_MAP[%d]={\n", $kw, $MAX_UTF32/$quant);
  for(my $i=0; $i<$MAX_UTF32/$quant; $i++) {
    print $OUTFILE "  " if ($i % $linelen2)==0;
    my ($k2, $id) = (sprintf("%05x", $i), 0);
    $id = $rrefs{$k2}+1 if exists $rrefs{$k2};
    print $OUTFILE sprintf("%2d", $id);
    print $OUTFILE ", " if ($i!=$MAX_UTF32/$quant-1);
    print $OUTFILE "\n" if (($i % $linelen2)==($linelen2-1)) or ($i==$MAX_UTF32/$quant-1);
  }
  print $OUTFILE sprintf("}; /* BMAP_ELEM %s_MAP[%d] */\n", $kw, $MAX_UTF32/$quant);
}

sub plist_case($$)
{
  my ($kw, $list) = @_;
  my %L;
  for my $k (keys %$list) { 
    $L{sprintf("%06x", hex $list->{$k})} = sprintf("%06x", hex $k);
  }
  #-------------------

  print $OUTFILE "\n/***************** $kw */\n";
  print $OUTFILE sprintf("UTF32 %s_VAL(UTF32 v)\n{\n", $kw);
  print $OUTFILE "  switch(v) {\n";
  for my $k (sort keys %L) { 
    print $OUTFILE sprintf("    case 0x%s: return 0x%s;\n", $k, $L{$k});
  }
  print $OUTFILE "    default: return v;\n";
  print $OUTFILE "  } /* case(v) */\n";
  print $OUTFILE sprintf("} /* UTF32 %s_VAL(UTF32 v) */\n\n", $kw);
}
#-------------------------------------------

q^
open $OUTFILE, ">${outpath}utf8_case_map.h" or die "Can't export";
print $OUTFILE "\n/*****************/\n";
print $OUTFILE "DEFINE_NORMAL_SIZE(".(keys %list1).")\n";
print $OUTFILE "DEFINE_FAST_SIZE(".(keys %list2).")\n";
print $OUTFILE "DEFINE_TURKEY_SIZE(".(keys %list3).")\n";
print $OUTFILE "\n/*****************/\n";
plist_map 'SET_NORMAL_U', \%list1;
plist_map 'SET_FAST_U', \%list2;
plist_map 'SET_TURKEY_U', \%list3;
print $OUTFILE "\n/*****************/\n";
close $OUTFILE;
^ if 0;

open $OUTFILE, ">${outpath}utf8_case_casefast.h" or die "Can't export";
#plist_q 'FAST_U', \%list2, 256, 8, 16;
plist_case 'FAST_U', \%list2;
close $OUTFILE;

open $OUTFILE, ">${outpath}utf8_case_qfast.h" or die "Can't export";
#plist_q 'FAST_U', \%list2, 256, 8, 16;
plist_q 'FAST_U', \%list2, 256, 8, 16;
close $OUTFILE;

q^
open $OUTFILE, ">utf8_case_f.h" or die "Can't export";
plist_q 'FAST_U', \%list2, 256;
close $OUTFILE;

open $OUTFILE, ">utf8_case_t.h" or die "Can't export";
plist_q 'TURKEY_U', \%list3, 256;
close $OUTFILE;

^ if 0;

#-------------------------------------------
