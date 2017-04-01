delimiter //
create function hasnottaken(sid char(4), cid char(8))
returns boolean
begin
set @quarter=get_quarter();
set @year=year(sysdate());

if (cid not in (select UoSCode from transcript where StudId=sid and 
(Year<@year or (Year=@year and before_quarter(Semester,@quarter))))) then return true;
else return false;

end if;
end //
delimiter ;

