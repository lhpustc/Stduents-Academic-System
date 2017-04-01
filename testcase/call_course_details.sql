select UoSCode,UoSName,Year,Semester,Enrollment, MaxEnrollment,faculty.Name,Grade 
from unitofstudy natural join transcript natural join uosoffering,faculty 
where StudId=3213 and UoSCode="COMP2129" and uosoffering.InstructorId=faculty.Id;

call course_details("3213","COMP2129");
