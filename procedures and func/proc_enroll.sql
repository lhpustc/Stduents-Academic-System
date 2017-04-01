delimiter //
create procedure enroll(IN sid char(4),IN cid char(8),IN next_q INT)
begin
declare c_code char(8);
declare max_enroll int;
declare enroll int;
declare quarter char(2);
declare cyear char(4);
declare stat INT;

set quarter=get_quarter();
set cyear=year(sysdate());

if next_q=1 then
   	if quarter="Q1" then 
	   set quarter="Q2";
       set cyear=(cyear+1);
	elseif quarter="Q2" then 
	   set quarter="Q3";
	elseif quarter="Q3" then
	   set quarter="Q4"; 
	else 
	   set quarter="Q1"; 
	end if;
end if;

select MaxEnrollment,Enrollment into max_enroll,enroll 
from uosoffering where UoSCode=cid and Semester=quarter and Year=cyear;



set @Prereq=satisfyPrereq(sid,cid);
set @NotTaken=hasnottaken(sid,cid);


if (@Prereq=true) and (max_enroll>enroll) and (@NotTaken=true) then
start transaction;
   insert into transcript values(sid,cid,quarter,cyear,NULL);
   update uosoffering set Enrollment=Enrollment+1 where UoSCode=cid and Semester=quarter and Year=cyear;
commit;
   set stat=1;
elseif enroll is NULL then
    set stat=0;
elseif @Prereq=false then
   set stat=2;
elseif (max_enroll<=enroll) then 
   set stat=3;
else set stat=4;
end if;
select stat;
end //
delimiter ;

#drop procedure enroll;