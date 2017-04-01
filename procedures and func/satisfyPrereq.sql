delimiter //
create function satisfyPrereq(sid char(4),cid char(8))
returns bool

begin
declare sf bool;
declare done INT default false;
#declare grade char(2);
declare pre_code char(8);
declare cur_prereq cursor for select PrereqUoSCode from requires where UoSCode=cid;
declare continue handler for not found set done=true;
set sf=true;
open cur_prereq;
check_loop: LOOP
  fetch cur_prereq into pre_code;
  if done then leave check_loop;
  end if;
  set @grade=(select Grade from transcript where StudId=sid and UoSCode=pre_code);
  if @grade is NULL or @grade="D" then set sf=false;
  end if;
end LOOP;
close cur_prereq;
return sf;
end //
delimiter ;

#drop function satisfyPrereq;