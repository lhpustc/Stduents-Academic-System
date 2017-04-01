delimiter //
create function before_quarter(q1 char(2),q2 char(2)) #return true if q1 before or equl to q2
returns bool
begin
declare t bool;
set t=false;
if q1="Q2" then set t=true;
elseif q1="Q3" then
   if (q2="Q3" or q2="Q4" or q2="Q1") then set t=true;
   end if;
elseif q1="Q4" then
   if (q2="Q4" or q2="Q1") then set t=true;
   end if;
else
   if q2="Q1" then set t=true;
   end if;
end if;
return t;
end //
delimiter ;
