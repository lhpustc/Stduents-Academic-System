delimiter //
create function get_quarter()
returns char(2)
begin
  declare quarter char(2);
  set @month=month(sysdate());
  if @month>=9 and @month<=12 then set quarter="Q1";
  elseif @month>=1 and @month<=3 then set quarter="Q2";
  elseif @month>=4 and @month<=6 then set quarter="Q3";
  else set quarter="Q4";
  end if;
return quarter;
end //
delimiter ;

#drop function get_quarter;