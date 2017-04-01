delimiter //
create procedure present_available_courses(IN sid char(4))

begin
	# find the quarter and next quarter
	set @quarter=get_quarter();
	set @year=year(sysdate());
	set @next_year=@year;
	set @s_MaxEnroll=(select MaximumPreferredEnrollment from student where Id=sid);
	set @non_clrmType=(select NonPreferredClassroomType from student where Id=sid);

	if @quarter="Q1" then 
	   set @next_quarter="Q2";
	   set @next_year=(@year+1);
	elseif @quarter="Q2" then 
	   set @next_quarter="Q3";
	elseif @quarter="Q3" then
	   set @next_quarter="Q4"; 
	else 
	   set @next_quarter="Q1"; 
	end if;



	# select the courses
	select UoSCode,UoSName,Semester,Year from unitofstudy natural join uosoffering natural join lecture
	where ((semester=@quarter and year=@year) or (semester=@next_quarter and year=@next_year))
	and (@s_MaxEnroll is NULl or MaxEnrollment<=@s_MaxEnroll) 
	and (@non_clrmType is NULL or ClassroomId not in (select ClassroomId from classroom where type=@non_clrmType));

end //
delimiter ;

#drop procedure present_available_courses;