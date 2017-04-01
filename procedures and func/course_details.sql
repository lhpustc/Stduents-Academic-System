delimiter //
create procedure course_details (IN sid CHAR(4),IN c_code CHAR(8))
begin
select UoSCode,UoSName,Year,Semester,Enrollment, MaxEnrollment,faculty.Name,Grade 
from unitofstudy natural join transcript natural join uosoffering,faculty 
where StudId=sid and UoSCode=c_code and uosoffering.InstructorId=faculty.Id;
end //
delimiter ;

#drop procedure current_course;