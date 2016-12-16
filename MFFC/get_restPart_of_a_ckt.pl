#! /usr/bin/perl

#open infile1, "c880.blif" or die $!;
#open infile2, "c880_MFFC_candidate.blif" or die $!;

#`rm "new_ckt.blif"` if (-e "new_ckt.blif");
#open(outfile, ">new_ckt.blif") || die "Cannot open new_ckt.blif$!";

use strict;
use warnings FATAL => 'all';
open infile1, "$ARGV[0]" or die $!; # full circuit
open infile2, "$ARGV[1]" or die $!; # sub-circuit

`rm "$ARGV[2]"` if (-e "$ARGV[2]"); # rest part of the circuit
open(outfile, ">$ARGV[2]") || die "Cannot open $ARGV[2]$!";

my @temp_candidate=();
while(<infile2>){
	chomp;	
	if(/^\d+\s1$/){
		next;
	}
	push @temp_candidate, $_;
}
close infile2;
print "@temp_candidate\n";

=pod
my @temp_full_ckt=();
while(<infile1>){
	chomp;
	if(/names/){
		push @temp_full_ckt, $_;	
	}
}
=cut

my $enable_print_cover = 0;
for(<infile1>){
	#print "$_\n";
	chomp;
	if(/names/){
		my $line = $_;
		my $appear_num = grep/^$line$/i,@temp_candidate;
		if($appear_num eq 0){
			$enable_print_cover = 1;
			print outfile "$_\n";
		}
		if($appear_num ne 0){
			# not print this line, and the following covers
			$enable_print_cover = 0;
		}	
	}
	#print "enable_print_cover = $enable_print_cover\n";
	elsif(/^\d+\s\d$/){
		if($enable_print_cover eq 1){
			print outfile "$_\n";
		}
	}
	else{
		print outfile "$_\n";
	}
}


close infile1, infile2, outfile;








