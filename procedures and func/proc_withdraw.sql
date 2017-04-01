delimiter //
create procedure withdraw(IN sid char(4),IN cid char(8))
begin
declare stat INT;
declare vyear char(4);
declare vsemester char(2);
declare vgrade INT;

set @warning=0;
select Semester,Year,Grade into vsemester,vyear,vgrade from transcript 
where StudId=sid and UoSCode=cid;

if vsemester is NULL then set stat=0;
elseif vgrade is not NULL then set stat=1;
else 
start transaction;
   delete from transcript where StudId=sid and UoSCode=cid;
   update uosoffering set Enrollment=Enrollment-1 where UoSCode=cid and Semester=vsemester and Year=vyear;
commit;
   set stat=2;
end if;
select stat, @warning;
set @warning=0;
end //
delimiter ;

#drop procedure withdraw;