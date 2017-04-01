delimiter //
create procedure current_course (IN sid CHAR(4),IN semst CHAR(2),IN yr CHAR(4))
begin
select UoSCode,UoSName from transcript natural join unitofstudy 
where StudId=sid and Semester=semst and year=yr;
end //
delimiter ;

#drop procedure current_course;