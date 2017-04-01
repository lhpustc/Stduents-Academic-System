delimiter //
create trigger low_enrollment after update on uosoffering 
for each row
begin
  if new.Enrollment<(0.5*new.MaxEnrollment) then
    set @warning=1;
    signal sqlstate '01000' set message_text="Warning: Enrollment goes below 50% of capacity!";
  end if;
end //
delimiter ;

#drop trigger low_enrollment;