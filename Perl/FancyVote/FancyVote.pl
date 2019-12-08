#!/usr/bin/perl

use strict;
use warnings;
print "========================================================| FancyVote |===================================================\n";
print 'In the very beginning, if you want to know something about this program, please type \'h\'[help], if not, please type enter:';
my $help = <STDIN>;
chomp $help;
if ($help =~ /^h$/i)
{
open(HELP,'help.txt');
my @helpinf = <HELP>;
close HELP;
print @helpinf,"\n\n";
print "\nEnter any words to continue:";
my $pause = <STDIN>;}
print 'This program is used for lanuching a vote among serveral people.At first, we should set up a group, 3~10 members allowed. If you have set up before, please type enter. If not, please enter your group name:';
my $Groupname = <STDIN>;
chomp $Groupname;
my @Gmember = ();
my @Groupfile;
my %GroupFile = ();
my $account;
my $Gnu;
my @rank = ();
my @selfans = ();
my @rankScore;
my @selfansScore;
my @finalScore;
if ($Groupname =~ /^\s*$/)
{print "You have found a group before, let\'s continue our vote.....\n";
print 'Please enter your group name:';
$Groupname = <STDIN>;
chomp $Groupname;
unless (open(GROUPFILE,"$Groupname.txt"))
{
print "Group \"$Groupname\" doesn\'t exist!\n";
exit;
}
@Groupfile = <GROUPFILE>;
close GROUPFILE;
chomp @Groupfile;
$Gnu = (scalar @Groupfile)/2;
print "Group \"$Groupname\" includes $Gnu members:\n";
for(my $i=0;$i<$Gnu;$i++)
{
$GroupFile{$Groupfile[$i*2]} = $Groupfile[$i*2+1];
$Gmember[$i] = $Groupfile[$i*2];
print $Groupfile[$i*2],"\t";
}
print "\n";
}
else
{open(my $GROUP,">$Groupname.txt");
print 'How manny members do you want to add to this group:';
$Gnu = <STDIN>;
chomp $Gnu;
while($Gnu<3||$Gnu>10)
{
print "Only 3~10 allowed!Please type again:";
$Gnu = <STDIN>;
chomp $Gnu;
}
my $j;
my $name;
for (my $i = 0;$i < $Gnu;$i++)
{
$j = $i+1;
print "group member $j:";
$name = <STDIN>;
chomp $name;
$Gmember[$i] = $name;
}
print "The $Gnu members have been added.\n";
print "The members include: \n@Gmember\n";
print "Please sign in one by one:\n";

#Now, there is a subroutine for everyone in this group signing in one by one
#The account( group members' name) and passwords will be saved in a file, which only the host visable
#The argument "GroupFile" is a hash, its keys are group members' name, and every key saves the password
%GroupFile = Sign_In(\@Gmember,$GROUP);
print "You have make $Gnu accounts for every member in group \"$Groupname\" sucessfully!\n";
}
print "\nThere are 3 items for our later vote:\n";
my $Item1 = "Who is the most follish one in your group?";
my $Item2 = "Who is the one like a diaoer most in your group?";
my $Item3 = "Who is the one like a rotten girl most in your group?";
print "1. $Item1\n";
print "2. $Item2\n";
print "3. $Item3\n\n";
print "Item 1 is available for girls and boys both, and Item 2 is available for boys and Item 3 is available for girls.\n";
print "Please select the item you want to launch a vote [1-3]:";
my $Item = <STDIN>;
chomp $Item;
print "Now, please use the accounts you make before to continue our vote, please log in one by one:\n";

#Now, there is a subroutine for everyone in this group loging in one by one, then voting and self-testing
#There is no need to follow the order, just random
for(my $i=0;$i<$Gnu;$i++)
{
#Argument 'account' saves the name of a member loging in this time
$account = Log_In(\%GroupFile);
#Argument 'rank' saves names of the other members from the most to the worst
@rank = Vote_For_Others($account,$Item,\@Gmember);
#Argument 'selfans' saves the answers as A,B,C and D for self-testing questions
@selfans = Self_Test($Item);
@finalScore = Mark_Result(\@rankScore,\@rank,\@selfansScore,\@selfans,$account,\@Gmember,$i);
}
print "**The final score of every member**\n";
print "Attention: the more score you get, the more foolish, diaosi or fu you are!\n";
print "---------------------------RESULT----------------------------\n";
print "account\tfinalScore\trankScore\tselfansScore\n";
for (my $i=0;$i<scalar @Gmember;$i++)
{
print $Gmember[$i],"\t",$finalScore[$i],"\t\t",$rankScore[$i],"\t\t",$selfansScore[$i],"\n";
}
exit;


sub Sign_In{
my ($member,$GROUPFILE) = @_;
my %name_code;
my $code;
my $newcode;
my $number = scalar @$member;
for (my $i= 0; $i < $number; $i++)
{print "your account is: $$member[$i]\n";
my $j = 0;
do 
{
if ($j)
{print "Input wrong! Once again!\n";}
print "Please input your new password:";
system "stty -echo";
$code= <STDIN>;
print "\n";
chomp $code;
print "Pleaes input your password again:";
$newcode = <STDIN>;
print "\n";
chomp $newcode;
$j++;
}while($code ne $newcode);
$name_code{$$member[$i]} = $code;
print $GROUPFILE "$$member[$i]\n$code\n";
}
system "stty echo";
return %name_code;
}


sub Log_In{
  use strict;
  use warnings;

  my @keys = keys %GroupFile;
  

  print "Please enter your account name!\nNote: you have only three chances！！！\n\n";
  print "account:";
  my $account = <STDIN>;
  print "\n";
  chomp $account;
  for (my $i=0;$i<3;$i++) { 
my $maker = 0;
  for (my $j=0;$j<scalar @keys;$j++)
  {
  if ($account eq $keys[$j])
  {$maker = 1;}
  }   
	if ($maker) {
	  print "Please enter the password!\nNote: you have only three chances\n\n"; 
          last;}
        elsif ($i<2) {
	    print "!!!You enter the account is wrong, please input again:\n\n" ; 
print "account:";
	    $account = <STDIN>;
print "\n";
	    chomp $account; }       
        else { 
            print "You lost the chance!\n";
            exit;}
  }
  system "stty -echo";
print "password:";
  my $passwd = <STDIN>;
print "\n";
  chomp $passwd;
  system "stty echo";

  for (my $j=0;$j<3;$j++) { 
	if ($passwd eq $GroupFile{$account}) {
   	  print "Success of landing:\n\n"; 
	  last;}
        elsif ($j<2) {
	  print "!!!You enter the password is wrong, please input again:\n\n" ; 
	  system "stty -echo";
print "password:";
	  $passwd = <STDIN>;
print "\n";
	  chomp $passwd; 
 	  system "stty echo";}
        else { 
            print "You lost the chance!\n";
            exit;} 
  }

return $account;
}



sub Vote_For_Others{
my @rank;
my ($account,$It,$member) = @_; 
my @Vmember;
my $Vnumber;
my $accept = '';
my $maker;
for (my $i=0,my $j=0;$i<scalar @$member;$i++)
{
if ($account ne $$member[$i])
{
$Vmember[$j] = $$member[$i];
$j++;
}
}
if($It == 1){
print "Who is the most follish one in your group?\n";
}
elsif($It == 2){
print "Who is the one like a diaoer most in your group?\n";
}
else{
print"Who is the one like a rotten girl most in your group?\n";
}

print "Please input the names from the most one to the least one in @Vmember\n\n";
for (my $i=0;$i<scalar @Vmember;$i++)
{
for (my $j=0;$j<scalar @Vmember;$j++)
{
if ($accept eq $Vmember[$j])
{$Vmember[$j] = '';}
}
do
{
print "Available: @Vmember\n";
print "Please input a name:\n";
$accept = <STDIN>;
chomp $accept;
$maker = 0;
for (my $j=0;$j<scalar @Vmember;$j++)
{
if ($accept eq $Vmember[$j])
{$rank[$i] = $accept;
$maker = 1;}
}
if ($maker == 0)
{print "Input wrong! Once again!\n";}
}while($maker == 0 );
}
chomp @rank;
print "Your vote is: @rank\n";
return @rank;
}

sub Self_Test{
my ($item) = @_;
my @answer;
my @foolish_question= ('Do you think you are the most stupid person?','Do you think you are the smartest person?','1+1=?','Who is smaller than bacteria?','What do you think of the program?');
my @foolish_answer= ('A:Youself	B:Don\'t know	C:Not me','A:Youself	B:Don\'t know	C:Developer','A:2	B:Not 2	C:Don\'t know','A:Viruses	B:Little bacteria	C:Son of bacteria	','A:Very well	B:Just so so	C:Bad');
my @diaosi_question= ('where would you want to go?','which would you choose to sweat?','which brands are your mobile-phone?','What would you do to relax?','Which ball games would you like?');
my @diaosi_answer= ('A:Tokyo hot	B:Tokyo	C:Japan','A:hand	B:toilet paper	C:napkin','A:walnut killer	B:OPPO/XiaoMi	C:iPhone','A:"running"	B:play games	C:listen music','A:Play badminton	B:Play tennis	C:Play golf');
my @funv_question= ('How do you go on the road of fnnv?','When do you start to be funv?','How long have you walked on the road of funv?','If a male homosexuality ask you to marry to him and give birth to a baby, what would you do?','When you watch a movie......');
my @funv_answer= ('A:I was born to be .....	B:learn it from movie.	C:Infulenceby a bad friend.','A:0-10	B:10-15	C:15-25','A:5--,-00-	B:2-5,jiecao will be the passerby	C:0-2,you are still in pur country','A:I would like to.	B:I don\'t know	C:get out','A:There is ture love between the surppoting male actor and leading male. 	B:I think the relationship between the surppoting male actor and leading male beyond the friendship.	C: They are best friends, always be.');
print "Welcome to the self_test program, $account!\n";
if ( $item == 1)
{print "Congratulations,you will have to answer five quetions to define how foolish you are!\n";
print "ATTENTION!!!!! In order to make sure you to finish the test, I havn't design the exit program.\n";
print "First, let us see how foolish you are!\n";
for (my $i=0 ; $i < 5; $i++)
{my $j= $i+1;
do{
print "The question $j: $foolish_question[$i]\n";
print "$foolish_answer[$i]\n";
print "\nPlease pick A,B or C to be your answer:";
$answer[$i]= <STDIN>;
chomp $answer[$i];
if ($answer[$i] =~ /[^ABC]/i )
{print "\nGongratulations,your answer doesn't exist! And you have to do it again\n";}
}while($answer[$i] =~ /[^ABC]/i);
}
}
elsif ( $item == 2)
{print "Congratulations,you will have to answer five quetions to define how diaosi you are!\n";
print "ATTENTION!!!!! In order to make sure you finish the test, I havn't design the exit program.\n";
print "First, let us see how diaosi you are!\n";
for (my $i=0 ; $i < 5; $i++)
{my $j= $i+1;
do{
print "The question $j: $diaosi_question[$i]\n";
print $diaosi_answer[$i];
print "\nPlease pick A,B or C to be your answer:";
$answer[$i]= <STDIN>;
chomp $answer[$i];
if ($answer[$i] =~ /[^ABC]/i )
{print "Gongratulations,your answer doesn't exist! And you have to do it again\n";}
}while($answer[$i] =~ /[^ABC]/i);
}
}
elsif ( $item == 3)
{print "Congratulations,you will have to answer five quetions to define how fu you are!\n";
print "ATTENTION!!!!! In order to make sure you finish the test, I havn't design the exit program.\n";
print "First, let us see how fu you are!\n";
for (my $i=0 ; $i < 5; $i++)
{my $j= $i+1;
do{
print "The question $j: $funv_question[$i]\n";
print $funv_answer[$i];
print "\nPlease pick A,B or C to be your answer:";
$answer[$i]= <STDIN>;
chomp $answer[$i];
if ($answer[$i] =~ /[^ABC]/i )
{print "\nGongratulations,your answer doesn't exist! And you have to do it again\n";}
}while($answer[$i] =~ /[^ABC]/i);
}
}
return @answer;
}


sub Mark_Result{
#按照引用的方式存放到参数数组中
	my ($rankScore,$rank,$selfansScore,$selfans,$account,$Gmember,$i) = @_;
	my @selfansScore;
	my @Result;
my $sum;
#这里将遍历@Gmember，将里面的名字在@rank里搜索位置，按照排名越前得分越高的原则存放到@rankScore里，@rankScore里的顺序与@Gmember相同
				
	for(my $k=0;$k<scalar @$Gmember;$k++){
		for(my $t=0;$t<scalar @$rank;$t++){
			if($$Gmember[$k] eq $$rank[$t]){
				my $h=((scalar @$rank)-$t)*20;
				$$rankScore[$k]+=$h;}
							}
										}
#这里将自评选项换算成对应分数
	for(my $j=0;$j<scalar @$selfans;$j++){
		if($$selfans[$j] =~ /[Aa]/){
			$$selfans[$j]=20;
				  }
		elsif($$selfans[$j] =~ /[Bb]/){
			$$selfans[$j]=10;
					 }
		else{
			$$selfans[$j]=5;
			}
				 						}
#自评分数加和
	foreach(@$selfans){
		$sum+=$_;  }
for (my $i=0;$i<scalar @$Gmember;$i++)
{
if ($account eq $$Gmember[$i]){
	$$selfansScore[$i]=$sum;}
}

if ($i == (scalar @$Gmember)-1)
{
for (my $i=0;$i<scalar @$Gmember;$i++)
{
$Result[$i] = 0.7 * $$rankScore[$i] + 0.3 * $$selfansScore[$i];
}
return @Result;
}
#返回结果
		  		}
