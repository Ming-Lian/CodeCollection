#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;

=head1 Usage

	$0 -i <input> -o <output> -m <matrix_exist>
	
=head1 Parameters

	-i	[str]	Input plain text file
	-o	[str]	Output file
	-m	[logic]	Exist the distance and direction matrix:T(true) or F(false)[default:False] 
=cut
my ($input,$output,$matrix_exist);
GetOptions(
	"i:s"=>\$input,
	"o:s"=>\$output,
	"m:s"=>\$matrix_exist
	);
$matrix_exist="F" unless (defined($matrix_exist));
die `pod2text $0` if ((!$input) or (!$output));
# my $path = `cd $(dirname "$0");pwd`;
our %chars_posi;
our (@dis_m,@dir_m);

#######################################
#      Construct or Load Matrixs      #
#######################################

#-----matrixs not exist, construct-----
if($matrix_exist eq "F"){
	print "*******************Construct matrix******************************\n";
	#get input from keyboard
	my $i = 1;
	my @horizon;
	print "Note:1.follow rightward and downward order to input!\n";
	print "Note:2.input 'q' to stop current direction input!\n";
	my $seq;
	do{
		print "1.please input the characters rightward[$i]:";
		$seq = <STDIN>;
		chomp $seq;
		if($seq ne "q"){
			push @horizon,$seq;
		}
		$i++;
	}while($seq ne "q");
	$i = 1;
	my @principal;
	do{
		print "2.please input the characters principal diagonal(\\)[$i]:";
		$seq = <STDIN>;
		chomp $seq;
		if($seq ne "q"){
			push @principal,$seq;
		}
		$i++;
	}while($seq ne "q");
	$i = 1;
	my @vice;
	do{
		print "3.please input the characters vice diagonal(/)[$i]:";
		$seq = <STDIN>;
		chomp $seq;
		if($seq ne "q"){
			push @vice,$seq;
		}
		$i++;
	}while($seq ne "q");
	#construct a hash variable to restore the position of characters in matrxs
	my $chars = join "",@horizon;
	my @Chars = split //,$chars;
	
	$i = 0;
	for my $tmp (@Chars){
		$chars_posi{$tmp} = $i;
		$i++;
	}
	#construct 2 matrixs
	ConstructMatrix(\%chars_posi,\@horizon,\@principal,\@vice);
	#write these 2 matrixs into 2 plain files
	open DIS,"keyboard_dis_matrix.txt" or die "$!\n";
	open DIR,"keyboard_dir_matrix.txt" or die "$!\n";
	my $character_print = join "\t",@Chars;
	print DIS "$character_print\n";
	print DIR "$character_print\n";
	my $lines_num = scalar @Chars;
	for (my $i=0;$i<$lines_num;$i++){
		for (my $j=0;$j<$lines_num;$j++){
			if (defined($dis_m[$i][$j])){
				print DIS "$dis_m[$i][$j]\t";
			}
			else{
				print DIS "0\t";
			}
			if (defined($dir_m[$i][$j])){
				print DIR "$dir_m[$i][$j]\t";
			}
			else{
				print DIR "0\t";
			}
		}
		print DIS "\n";
		print DIR "\n";
	}
	close DIS;
	close DIR;
}
#------matrixs exist,load--------
elsif($matrix_exist eq "T"){
	open DIS,"<keyboard_dis_matrix.txt" or die "$!\n";
	open DIR,"<keyboard_dir_matrix.txt" or die "$!\n";
	my $chars=<DIS>;
	<DIR>;
	chomp $chars;
	#construct a hash variable to restore the position of characters in matrxs
	my @Chars=split /\t/,$chars;
	my $i = 0;
	for my $tmp (@Chars){
		$chars_posi{$tmp} = $i;
		$i++;
	}
	#load 2 matrixs
	my $lines_num=scalar @Chars;
	for (my $i=0;$i<$lines_num;$i++){
		my $row_dis = <DIS>;
		chomp $row_dis;
		my @Row_dis = split /\t/,$row_dis;
		my $row_dir = <DIR>;
		chomp $row_dir;
		my @Row_dir = split /\t/,$row_dir;
		for (my $j=0;$j<$lines_num;$j++){
			$dis_m[$i][$j] = $Row_dis[$j];
			$dir_m[$i][$j] = $Row_dir[$j];
		}
	}
	close DIS;
	close DIR;
}
else{
	print "[erro] input wrong\n";
	die `pod2text $0`;
}
	
	
#######################################################
#      	Search Keyboard Pattern by SlideWindow        #
#######################################################	
	
	
open(IN,"<$input") or die;
open(OUT,">$output") or die;
while(<IN>){
	chomp;
	my $password = $_;
	$password = Chars_Transformation($password);
	my $search_start=0;
	my @region;
	my @direction;
	my $i=0;
	my ($find_flag,$start,$end,$dir);
	do{
		($find_flag,$start,$end,$dir)=SlideWindow_SearchAndExtend($password,$search_start);
		if($find_flag){
			$search_start=$end+1;
			$i++;
			push @region,join(":",$start,$end);
			push @direction,$dir;
		}
	}while($find_flag and $search_start<length($password)-2);
	#write result into plain text file
	if($i){
		my $regions = join ";",@region;
		my $dirs = join ";",@direction;
		print OUT "$_\t$i\t$regions\t$dirs\n";
	}
}
close IN;
close OUT;


#***********************Function ConstructMatrix**************************************

sub ConstructMatrix{
	my ($chars_posi,$horizon,$principal,$vice) = @_;
	#horizon
	for (my $i=0;$i<scalar @$horizon;$i++){
		my $seq = $$horizon[$i];
		for (my $j=0;$j<length($seq);$j++){
			my ($char_1,$char_2)=split //,substr($seq,$j,2);
			my $char_1_posi = $chars_posi{$char_1};
			my $char_2_posi = $chars_posi{$char_2};
			#distance: 1 means close to each other
			if (defined($char_1_posi)&&defined($char_2_posi)){
				$dis_m[$char_1_posi][$char_2_posi] = 1;
				$dis_m[$char_2_posi][$char_1_posi] = 1;
				#direction: -1: <-;
				#			 1: ->;
				#			-2: <\;
				#			 2: \>;
				#			-3: />;
				#			 3: </;
				$dir_m[$char_1_posi][$char_2_posi] = 1;
				$dir_m[$char_2_posi][$char_1_posi] = -1;
			}
		}
	}
	#principal diagonal
	for (my $i=0;$i<scalar @$principal;$i++){
		my $seq = $$principal[$i];
		for (my $j=0;$j<length($seq);$j++){
			my ($char_1,$char_2)=split //,substr($seq,$j,2);
			my $char_1_posi = $chars_posi{$char_1};
			my $char_2_posi = $chars_posi{$char_2};
			#distance: 1 means close to each other
			if (defined($char_1_posi)&&defined($char_2_posi)){
				$dis_m[$char_1_posi][$char_2_posi] = 1;
				$dis_m[$char_2_posi][$char_1_posi] = 1;
				#direction: -1: <-;
				#			 1: ->;
				#			-2: <\;
				#			 2: \>;
				#			-3: />;
				#			 3: </;
				$dir_m[$char_1_posi][$char_2_posi] = 2;
				$dir_m[$char_2_posi][$char_1_posi] = -2;
			}
		}
	}
	#vice diagonal
	for (my $i=0;$i<scalar @$vice;$i++){
		my $seq = $$vice[$i];
		for (my $j=0;$j<length($seq);$j++){
			my ($char_1,$char_2)=split //,substr($seq,$j,2);
			my $char_1_posi = $chars_posi{$char_1};
			my $char_2_posi = $chars_posi{$char_2};
			#distance: 1 means close to each other
			if (defined($char_1_posi)&&defined($char_2_posi)){
				$dis_m[$char_1_posi][$char_2_posi] = 1;
				$dis_m[$char_2_posi][$char_1_posi] = 1;
				#direction: -1: <-;
				#			 1: ->;
				#			-2: <\;
				#			 2: \>;
				#			-3: />;
				#			 3: </;
				$dir_m[$char_1_posi][$char_2_posi] = 3;
				$dir_m[$char_2_posi][$char_1_posi] = -3;
			}
		}
	}
	
#**********************************Function SlideWindow_SearchAndExtend**********************************
	
sub SlideWindow_SearchAndExtend{
	my ($seq,$start) = @_;
	my $find_flag = 0;
	my $end =0;
	my $dir =0;
	for (;$start<length($seq)-2;$start++){
		my @kmer = split //,substr($seq,$start,3);
		my $kmer1_posi = $chars_posi{$kmer[0]};
		my $kmer2_posi = $chars_posi{$kmer[1]};
		my $kmer3_posi = $chars_posi{$kmer[2]};
		unless (defined($kmer1_posi)&&defined($kmer2_posi)&&defined($kmer3_posi)){
			last;
		}
		my $dis_12 = $dis_m[$kmer1_posi][$kmer2_posi] ? $dis_m[$kmer1_posi][$kmer2_posi] : 0;
		my $dis_23 = $dis_m[$kmer2_posi][$kmer3_posi] ? $dis_m[$kmer2_posi][$kmer3_posi] : 0;
		my $dir_12 = $dir_m[$kmer1_posi][$kmer2_posi] ? $dir_m[$kmer1_posi][$kmer2_posi] : 0;
		my $dir_23 = $dir_m[$kmer2_posi][$kmer3_posi] ? $dir_m[$kmer2_posi][$kmer3_posi] : 0;
		if($dis_12==1&&$dis_23==1&&$dir_12 == $dir_23 && $dir_12 !=0){
			#extension
			$find_flag = 1;
			$end = $start+2;
			$dir = $dir_12;
			my $former_posi = $kmer3_posi;
			my $former_dis = $dis_23;
			my $former_dir = $dir_23;
			for (my $i=$start+3;$i<length($seq);$i++){
				my $later = substr($seq,$i,1);
				my $later_posi = $chars_posi{$later};
				my $later_dis = $dis_m[$former_posi][$later_posi] ? $dis_m[$former_posi][$later_posi] : 0;
				my $later_dir = $dir_m[$former_posi][$later_posi] ? $dir_m[$former_posi][$later_posi] : 0;
				unless($former_dis==1&&$later_dis==1&&$former_dir == $later_dir && $former_dir !=0){
					last;
				}
				$end = $i;
				$former_posi = $later_posi;
				$former_dis = $later_dis;
				$former_dir = $later_dir;
			}
		}
		if($find_flag){
			last;
		}
	}
	return ($find_flag,$start,$end,$dir);
	}
}

#*****************************Fuction Chars_Transformation************************************
sub Chars_Transformation{
	my ($string) = @_;
	$string =~ s/~/`/g;$string =~ s/!/1/g;$string =~ s/\@/2/g;$string =~ s/#/3/g;$string =~ s/\$/4/g;
	$string =~ s/%/5/g;$string =~ s/\^/6/g;$string =~ s/&/7/g;$string =~ s/\*/8/g;$string =~ s/\(/9/g;
	$string =~ s/\)/0/g;$string =~ s/_/-/g;$string =~ s/\+/=/g;$string =~ s/\{/\[/g;$string =~ s/\}/\]/g;
	$string =~ s/\|/\\/g;$string =~ s/:/;/g;$string =~ s/"/'/g;$string =~ s/</,/g;$string =~ s/>/\./g;
	$string =~ s/\?/\//g;
	$string = lc($string);
	return $string;
}
	
