call enroll(5123,"INFO3220",0); #enroll with two prerequisites
#show warnings;
call withdraw(5123,"INFO3220"); #withdraw with warning
#show warnings;

call withdraw(5123,"COMP2007"); #withdraw without warning
call enroll(5123,"COMP2007",1);
call withdraw(5123,"INFO2110"); #already graded

