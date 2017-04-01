#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

char sid[4];

int main(){

	bool Login();
	int Student_Menu();
	char *sid;


	while(1){
		if(Login()==false) break;
		Student_Menu();
	}

	return 0;
}

MYSQL *connect_mysql(){
    MYSQL *conn;
	char *server = "localhost";
	char *user = "root";
	char *password = "0507";
	char *database = "project3-nudb";

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn,server,user,
		password,database,0,NULL,0)) {
		fprintf(stderr,"%s\n",mysql_error(conn));
	    printf("Press any key to escape...");
	    int a=scanf("%d",&a);
	    return NULL;	    
	}
	else {
		//printf("Connected successfully !\n");
		return conn;
	}
}

bool Login(){
    char in_id[4];
    char in_pswd[50];
 	char q_name[100];

 	MYSQL *conn;
 	MYSQL_RES *res;
 	MYSQL_ROW row; 
    
    MYSQL *connect_mysql();

    if ((conn=connect_mysql())==NULL) exit(1);

 	printf("\n-------------------- Login --------------------\n");

 	do{


		printf("Input your ID or quit by typing quit: \n");
		scanf("%s",&in_id);
		if (!strcmp(in_id,"quit")) return false;

		//send SQL query
		strcpy(q_name,"select Password from student where Id=");
		strcat(q_name,in_id);
		strcat(q_name,";");


		if(mysql_query(conn,q_name)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}


	    res = mysql_use_result(conn);
	    row = mysql_fetch_row(res);
	    if(row==NULL) printf("No such student ID!\n");
	}while(row==NULL);


	do{
		//printf("%s\n", row[0]);
		printf("Input your password:\n");
		scanf("%s",&in_pswd);
	}while (strcmp(in_pswd,row[0]));


	strcpy(sid,in_id);
	mysql_free_result(res);
	printf("Login successfully!\n");
	mysql_close(conn);
	return true;
}

int Student_Menu(){
    int disp_current_courses();
    int Enroll();

	//printf("Student_Menu: %s\n", sid);
    int option;
	while(1) {
	    printf("\n-------------------- Student Menu --------------------\n");

		option=disp_current_courses();
		
		
		switch (option){
		case 1: Transcript(); break;
		case 2: Enroll_Menu(); break; 
		case 3: Withdraw(); break;
		case 4: Personal_Details(); break;
		case 5: printf("Logged out!\n"); return 0; //logout
		default: continue;
	    } 
	}
}

int disp_current_courses(){
	//printf("disp_current_courses: %s\n", sid);
	char cmd_dsp[50];
	int mon;
	char quarter[2];
	char year[4];
	int option=0;

    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	//get the local time from C
	time_t t=time(NULL);
	struct tm *tm=localtime(&t);
	mon=tm->tm_mon+1;
	sprintf(year,"%d",tm->tm_year+1900);

    if (mon>=9 && mon<=12) strcpy(quarter,"Q1");
    else if(mon>=1 && mon<=3) strcpy(quarter,"Q2");
    else if(mon>=4 && mon<=6) strcpy(quarter,"Q3");
    else strcpy(quarter,"Q4");	

    if ((conn=connect_mysql())==NULL) exit(1);

    sprintf(cmd_dsp,"call current_course(\"%s\",\"%s\",\"%s\");",sid,quarter,year);

	if (mysql_query(conn, cmd_dsp)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}    

    printf("Your current Courses: \n");
	res=mysql_use_result(conn);
    while((row=mysql_fetch_row(res)) != NULL){
    	printf("%s, %s\n", row[0],row[1]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    //display options for next step
    printf("\nNext Step: \n");
    printf("Transcript--1\n");
    printf("Enroll------2\n");
    printf("Withdraw----3\n");
    printf("Personal_Details--4\n");
    printf("Logout------5\n");

    scanf("%d",&option);
    return option;
}

int Transcript(){
	void Course_Details();

    do {
	    MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;
	    if ((conn=connect_mysql())==NULL) exit(1);

		printf("\n-------------------- Transcript --------------------\n");


	 	char q_name[100]="select UoSCode,Grade from transcript where StudId=";
		strcat(q_name,sid);
		strcat(q_name,";");


		if(mysql_query(conn,q_name)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		printf("Your Full Transcript: \n Course Code, Grade\n");
		res=mysql_use_result(conn);
	    while((row=mysql_fetch_row(res)) != NULL){
	    	printf("%s, %s\n", row[0],row[1]);
	    }

	    mysql_free_result(res);
	    mysql_close(conn);

	    printf("Enter 1 for course details, or 2 to go back: \n");
	    int option;
	    scanf("%d",&option);
	   	while (option!=1 && option!=2){
	   		printf("Enter again: \n");
	   		scanf("%d",&option);
	   	} ;
	    if (option==1) Course_Details();
	    else break;
	}while(1);

    return 0;
}

void Course_Details(){
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
    if ((conn=connect_mysql())==NULL) exit(1);

    char UoSCode[8];
	printf("Input the course code: \n");
	scanf("%s",&UoSCode);


 	char q_name[100];
 	sprintf(q_name,"call course_details(\"%s\",\"%s\");",sid, UoSCode);

	if(mysql_query(conn,q_name)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	printf("The course information:\n Course Code, Course Title, Year, Quarter, Enrollment, MaxEnrollment, Lecturer, Grade \n");
	res=mysql_use_result(conn);
    while((row=mysql_fetch_row(res)) != NULL){
    	printf("%s, %s, %s, %s, %s, %s, %s, %s\n", row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    int input;
    printf("Press any key to return...\n");
    scanf("%d",&input);
}


int Enroll_Menu(){
	int present_available_courses();
	int enroll();
	int option;
	do{
		option=present_available_courses();
		if (option==1) enroll();
		else break; 
	}while(1);

	return 0;
}

int present_available_courses(){
	int option;

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	if((conn=connect_mysql())==NULL) exit(1);

	printf("\n-------------------- Available Courses --------------------\n");

	char q_name[100];
	sprintf(q_name,"call present_available_courses(%s)",sid);

	if(mysql_query(conn,q_name)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	//printf("Your Full Transcript: \n Course Code, Grade\n");
	res=mysql_use_result(conn);
    while((row=mysql_fetch_row(res)) != NULL){
    	printf("%s, %s, %s, %s\n", row[0],row[1],row[2],row[3]);
    }

    mysql_free_result(res);
    mysql_close(conn);

   	printf("Enter 1 to enroll in a new course, or 2 to go back\n");
   	scanf("%d",&option);
   	while (option!=1 && option!=2){
   		printf("Enter again: \n");
   		scanf("%d",&option);
   	} ;
    return option;
}


int enroll(){
	int to_back;
	do{
		int option;
		char c_code[8];
		printf("Enter the course code: \n");
	    scanf("%s",c_code);
	    printf("Which quarter (0 for current quarter and 1 for next quarter): \n");
	    scanf("%d",&option);

		MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;
		if((conn=connect_mysql())==NULL) exit(1);

		char q_name[100];
		if (option==0) sprintf(q_name,"call enroll(%s,\"%s\",0);",sid,c_code);
		else sprintf(q_name,"call enroll(%s,\"%s\",1);",sid,c_code);

		//strcat(q_name,"\nselect @stat;");

		printf("%s\n",q_name);


		if(mysql_query(conn,q_name)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		//printf("Your Full Transcript: \n Course Code, Grade\n");
		res=mysql_use_result(conn);
		row=mysql_fetch_row(res);
		if (!strcmp(row[0],"0")) printf("No course offered! \n");
		else if (!strcmp(row[0],"1")) printf("Enrolled successfully!\n");
		else if (!strcmp(row[0],"2")) {
			printf("Prerequisite course(s) is needed:\n");
			//mysql_free_result(res);
			//mysql_close(conn);
			mysql_next_result(conn);

			char query[100];
		    if((conn=connect_mysql())==NULL) exit(1);
			sprintf(query,"select PrereqUoSCode from requires where UoSCode=\"%s\";",c_code);
			if(mysql_query(conn,query)){
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}
				
			res=mysql_use_result(conn);
	        while((row=mysql_fetch_row(res)) != NULL){
	    	    printf("%s\n", row[0]);
	        }        
		}
		else if (!strcmp(row[0],"3")) printf("Reach MaxEnrollment!\n");
		else printf("You have taken this course!\n");
	  

	    mysql_free_result(res);
	    mysql_close(conn);

	    printf("Enter 1 to contiue, or 2 to back\n");
	    scanf("%d",&to_back);
	    if (to_back==2) break;
    }while(1);
	return 0;
}


int Withdraw(){
	int to_back;
	do {
		MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;
		if((conn=connect_mysql())==NULL) exit(1);

		char c_code[8];
	    char q_name[100];
		printf("Enter the course code to withdraw:\n");
		scanf("%s",c_code);

		sprintf(q_name,"call withdraw(%s,\"%s\");",sid,c_code);
		if(mysql_query(conn,q_name)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		//printf("Your Full Transcript: \n Course Code, Grade\n");
		res=mysql_use_result(conn);
		row=mysql_fetch_row(res);
		if (!strcmp(row[0],"0")) printf("No such course in your transcript! \n");
		else if (!strcmp(row[0],"1")) printf("You can not withdraw this course: already graded!\n");
		else {
			printf("Withdraw successfully!\n");
			if (!strcmp(row[1],"1")) printf("Warning: Enrollment of this course goes below 50%% of the MaxEnrollment!\n");	
	               
		}
	  

	    mysql_free_result(res);
	    mysql_close(conn);

	    printf("Enter 1 to contiue, or 2 to back\n");
	    scanf("%d",&to_back);
	    if (to_back==2) break;
	}while(1);
	return 0;
}


int Personal_Details(){
	int option;
	int dsp_preference();
	int config_preference();
	do{
		option=dsp_preference();
		if(option==1) config_preference();
		else break;
	}while(1);
	return 0;
}

int dsp_preference(){
	int option;

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	if((conn=connect_mysql())==NULL) exit(1);

	printf("\n-------------------- Your Preference --------------------\n");

	char q_name[100];
	sprintf(q_name,"select MaximumPreferredEnrollment,NonPreferredClassroomType from student where Id=%s;",sid);

	if(mysql_query(conn,q_name)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	//printf("Your Full Transcript: \n Course Code, Grade\n");
	res=mysql_use_result(conn);
    row=mysql_fetch_row(res);
    if (row[0]) printf("MaxPreferredEnrollment: %s\n", row[0]);
    else printf("MaxPreferredEnrollment: NULL\n");

    if (row[1]) printf("NonPreferredClassroomType: %s\n", row[1]);
    else printf("NonPreferredClassroomType: NULL\n");

    mysql_free_result(res);
    mysql_close(conn);

    printf("Enter 1 to configure, or 2 to go back: \n");
    scanf("%d",&option);
    return option;

}

int config_preference(){
	int option;
	char value[20];
	char q_name[100];

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	if((conn=connect_mysql())==NULL) exit(1);

	printf("\n-------------------- Your Preference --------------------\n");

	printf("Enter option, value to configure your Preference:\n");
	printf(">>option: 1 for MaxPreferredEnrollment, or 2 for NonPreferredClassroomType\n");
	printf(">>value: a valid value of the corresponding attribute, or none to detelet the current value\n");

	scanf("%d,%s",&option,value);
    if(option==1) {
    	if(strcmp(value,"none")) sprintf(q_name,"update student set MaximumPreferredEnrollment = %s where Id=%s;",value,sid);
    	else sprintf(q_name,"update student set MaximumPreferredEnrollment = NULL where Id=%s;",sid);
    }
    else {
    	if (strcmp(value,"none")) sprintf(q_name,"update student set NonPreferredClassroomType = \"%s\" where Id=%s;",value,sid);
    	else sprintf(q_name,"update student set NonPreferredClassroomType = NULL where Id=%s;",sid);
    }

 	if(mysql_query(conn,q_name)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	else printf("Configured successfully!\n");
    mysql_free_result(res);
    mysql_close(conn);
  
	return 0;
}






