-- Drop all tables in reverse order of dependency
DROP TABLE teacher_with_grade CASCADE CONSTRAINTS;
DROP TABLE student_with_subject CASCADE CONSTRAINTS;
DROP TABLE teacher_with_student CASCADE CONSTRAINTS;
DROP TABLE class_with_teacher CASCADE CONSTRAINTS;
DROP TABLE degree_with_subject CASCADE CONSTRAINTS;
DROP TABLE subjects CASCADE CONSTRAINTS;
DROP TABLE students CASCADE CONSTRAINTS;
DROP TABLE teachers CASCADE CONSTRAINTS;
DROP TABLE admins CASCADE CONSTRAINTS;
DROP TABLE classes CASCADE CONSTRAINTS;
DROP TABLE grade CASCADE CONSTRAINTS;
DROP TABLE users CASCADE CONSTRAINTS;

SELECT username FROM users 
WHERE user_id = (SELECT user_id FROM students WHERE student_id = 1);
select * from degree_with_subject;
select * from users where user_id = (select user_id from students where student_id = 1);
select * from students;



-- Recreate tables with proper constraints
CREATE TABLE users(
    user_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    username VARCHAR2(255) UNIQUE NOT NULL,
    password VARCHAR2(255) NOT NULL
);

CREATE TABLE grade(
    grade_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    grade_name VARCHAR2(255) UNIQUE
);

CREATE TABLE classes(
    class_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR2(255) UNIQUE NOT NULL
);

CREATE TABLE teachers(
    teacher_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id NUMBER,
    CONSTRAINT teacher_user_fk FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

CREATE TABLE students(
    student_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    class_id NUMBER,
    user_id NUMBER,
    grade_id NUMBER,
    CONSTRAINT student_class_fk FOREIGN KEY (class_id) REFERENCES classes(class_id) ON DELETE SET NULL,
    CONSTRAINT student_user_fk FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    CONSTRAINT student_grade_fk FOREIGN KEY (grade_id) REFERENCES grade(grade_id) ON DELETE SET NULL
);

CREATE TABLE subjects(
    sub_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    sub_name VARCHAR2(255) UNIQUE NOT NULL,
    teacher_id NUMBER,
    CONSTRAINT subject_teacher_fk FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id) ON DELETE CASCADE
);

CREATE TABLE degree_with_subject(
    sub_id NUMBER,
    degree_date DATE,
    degree NUMBER NOT NULL,
    assignment VARCHAR2(255) DEFAULT 'Quiz' NOT NULL,
    student_id NUMBER,
    teacher_comment VARCHAR2(255) ,
    PRIMARY KEY (sub_id, degree_date),
    CONSTRAINT degree_subject_fk FOREIGN KEY (sub_id) REFERENCES subjects(sub_id) ON DELETE CASCADE,
    CONSTRAINT degree_student_fk FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
);

CREATE TABLE class_with_teacher(
    teacher_id NUMBER,
    class_id NUMBER,
    PRIMARY KEY (teacher_id, class_id),
    CONSTRAINT cwt_teacher_fk FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id) ON DELETE CASCADE,
    CONSTRAINT cwt_class_fk FOREIGN KEY (class_id) REFERENCES classes(class_id) ON DELETE CASCADE
);

CREATE TABLE teacher_with_student(
    teacher_id NUMBER,
    student_id NUMBER,
    PRIMARY KEY (teacher_id, student_id),
    CONSTRAINT tws_teacher_fk FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id) ON DELETE CASCADE,
    CONSTRAINT tws_student_fk FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
);

CREATE TABLE student_with_subject(
    student_id NUMBER,
    subject_id NUMBER,
    PRIMARY KEY (student_id, subject_id),
    CONSTRAINT sws_student_fk FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE,
    CONSTRAINT sws_subject_fk FOREIGN KEY (subject_id) REFERENCES subjects(sub_id) ON DELETE CASCADE
);

CREATE TABLE teacher_with_grade(
    grade_id NUMBER,
    teacher_id NUMBER,
    PRIMARY KEY (grade_id, teacher_id),
    CONSTRAINT twg_grade_fk FOREIGN KEY (grade_id) REFERENCES grade(grade_id) ON DELETE CASCADE,
    CONSTRAINT twg_teacher_fk FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id) ON DELETE CASCADE
);

CREATE TABLE admins(
    admin_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id NUMBER REFERENCES users (user_id) ON DELETE CASCADE
);

-- Insert Grades
INSERT INTO grade (grade_name) VALUES ('Grade 9');
INSERT INTO grade (grade_name) VALUES ('Grade 10');
INSERT INTO grade (grade_name) VALUES ('Grade 11');
INSERT INTO grade (grade_name) VALUES ('Grade 12');

-- Insert Classes
INSERT INTO classes (name) VALUES ('Science A');
INSERT INTO classes (name) VALUES ('Science B');
INSERT INTO classes (name) VALUES ('Arts A');
INSERT INTO classes (name) VALUES ('Commerce A');
INSERT INTO classes (name) VALUES ('Technology B');

-- Insert Users (Admins)
INSERT INTO users (username, password) VALUES ('admin1', 'admin123');
INSERT INTO users (username, password) VALUES ('admin2', 'admin456');
INSERT INTO users (username, password) VALUES ('abdullah', '123');




INSERT INTO admins (user_id) VALUES (1);
INSERT INTO admins (user_id) VALUES (2);
INSERT INTO admins (user_id) select user_id from users where username = 'abdullah';

-- Insert Users (Teachers)
INSERT INTO users (username, password) VALUES ('john.smith', 'teacher1');
INSERT INTO users (username, password) VALUES ('sarah.jones', 'teacher2');
INSERT INTO users (username, password) VALUES ('michael.wang', 'teacher3');
INSERT INTO users (username, password) VALUES ('emily.davis', 'teacher4');

-- Insert Teachers
INSERT INTO teachers (user_id) VALUES (3); 
INSERT INTO teachers (user_id) VALUES (4); 
INSERT INTO teachers (user_id) VALUES (5); 
INSERT INTO teachers (user_id) VALUES (6); 

-- Insert Users (Students)
INSERT INTO users (username, password) VALUES ('student1', 'pass123');
INSERT INTO users (username, password) VALUES ('student2', 'pass123');
INSERT INTO users (username, password) VALUES ('student3', 'pass123');
INSERT INTO users (username, password) VALUES ('student4', 'pass123');
INSERT INTO users (username, password) VALUES ('student5', 'pass123');
INSERT INTO users (username, password) VALUES ('student6', 'pass123');
INSERT INTO users (username, password) VALUES ('student7', 'pass123');
INSERT INTO users (username, password) VALUES ('student8', 'pass123');

-- Insert Students
INSERT INTO students (user_id, class_id, grade_id) VALUES (7, 1, 1);       
INSERT INTO students (user_id, class_id, grade_id) VALUES (8, 1, 1);       
INSERT INTO students (user_id, class_id, grade_id) VALUES (9, 2, 2);        
INSERT INTO students (user_id, class_id, grade_id) VALUES (10, 2, 2);       
INSERT INTO students (user_id, class_id, grade_id) VALUES (11, 3, 3);    
INSERT INTO students (user_id, class_id, grade_id) VALUES (12, 4, 4);        
INSERT INTO students (user_id, class_id, grade_id) VALUES (13, 5, 3);         
INSERT INTO students (user_id, class_id, grade_id) VALUES (14, 5, 4);         

-- Insert Subjects
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Mathematics', 1); 
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Physics', 1);      
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Chemistry', 2);   
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Biology', 2);     
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Literature', 3);   
INSERT INTO subjects (sub_name, teacher_id) VALUES ('History', 3);      
INSERT INTO subjects (sub_name, teacher_id) VALUES ('Computer Science', 4); 

-- Assign Teachers to Classes
INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (1, 1); 
INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (2, 1); 
INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (1, 2); 
INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (3, 3); 
INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (4, 5);
-- Assign Teachers to Grades
INSERT INTO teacher_with_grade (grade_id, teacher_id) VALUES (1, 1); 
INSERT INTO teacher_with_grade (grade_id, teacher_id) VALUES (2, 1);
INSERT INTO teacher_with_grade (grade_id, teacher_id) VALUES (3, 3); 
INSERT INTO teacher_with_grade (grade_id, teacher_id) VALUES (4, 4); 

-- Assign Students to Subjects
-- Science A students take Math and Physics
INSERT INTO student_with_subject (student_id, subject_id) VALUES (1, 1); 
INSERT INTO student_with_subject (student_id, subject_id) VALUES (1, 2);
INSERT INTO student_with_subject (student_id, subject_id) VALUES (2, 1); 
INSERT INTO student_with_subject (student_id, subject_id) VALUES (2, 2);
-- Technology B students take Computer Science
INSERT INTO student_with_subject (student_id, subject_id) VALUES (7, 7); 
INSERT INTO student_with_subject (student_id, subject_id) VALUES (8, 7);

-- Insert Grades for Students
-- Grades for Mathematics (subject_id = 1)
-- Mathematics grades (subject_id = 1)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (1, TO_DATE('15-01-2025', 'DD-MM-YYYY'), 78, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (2, TO_DATE('16-01-2025', 'DD-MM-YYYY'), 88, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (3, TO_DATE('17-01-2025', 'DD-MM-YYYY'), 90, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (4, TO_DATE('18-01-2025', 'DD-MM-YYYY'), 55, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (5, TO_DATE('18-01-2025', 'DD-MM-YYYY'), 89, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (6, TO_DATE('18-01-2025', 'DD-MM-YYYY'), 64, 1);

INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (7, TO_DATE('18-01-2025', 'DD-MM-YYYY'), 93, 1);



INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (1, TO_DATE('20-02-2025', 'DD-MM-YYYY'), 82, 1);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (1, TO_DATE('10-03-2025', 'DD-MM-YYYY'), 85, 2);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (1, TO_DATE('25-04-2025', 'DD-MM-YYYY'), 90, 2);

-- Physics grades (subject_id = 2)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (2, TO_DATE('18-01-2025', 'DD-MM-YYYY'), 88, 1);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (2, TO_DATE('22-02-2025', 'DD-MM-YYYY'), 92, 1);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (2, TO_DATE('12-03-2025', 'DD-MM-YYYY'), 85, 2);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (2, TO_DATE('28-04-2025', 'DD-MM-YYYY'), 94, 2);

-- Chemistry grades (subject_id = 3)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (3, TO_DATE('16-01-2025', 'DD-MM-YYYY'), 75, 3);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (3, TO_DATE('21-02-2025', 'DD-MM-YYYY'), 80, 3);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (3, TO_DATE('11-03-2025', 'DD-MM-YYYY'), 82, 4);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (3, TO_DATE('26-04-2025', 'DD-MM-YYYY'), 87, 4);

-- Biology grades (subject_id = 4)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (4, TO_DATE('17-01-2025', 'DD-MM-YYYY'), 92, 3);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (4, TO_DATE('23-02-2025', 'DD-MM-YYYY'), 88, 3);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (4, TO_DATE('13-03-2025', 'DD-MM-YYYY'), 95, 4);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (4, TO_DATE('29-04-2025', 'DD-MM-YYYY'), 90, 4);

-- Computer Science grades (subject_id = 7)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (7, TO_DATE('19-01-2025', 'DD-MM-YYYY'), 85, 7);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (7, TO_DATE('24-02-2025', 'DD-MM-YYYY'), 92, 7);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (7, TO_DATE('14-03-2025', 'DD-MM-YYYY'), 88, 8);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (7, TO_DATE('30-04-2025', 'DD-MM-YYYY'), 95, 8);

-- Literature grades (subject_id = 5)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (5, TO_DATE('20-01-2025', 'DD-MM-YYYY'), 78, 5);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (5, TO_DATE('25-02-2025', 'DD-MM-YYYY'), 82, 5);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (5, TO_DATE('15-03-2025', 'DD-MM-YYYY'), 85, 6);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (5, TO_DATE('01-05-2025', 'DD-MM-YYYY'), 90, 6);

-- History grades (subject_id = 6)
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (6, TO_DATE('21-01-2025', 'DD-MM-YYYY'), 88, 5);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (6, TO_DATE('26-02-2025', 'DD-MM-YYYY'), 92, 5);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (6, TO_DATE('16-03-2025', 'DD-MM-YYYY'), 85, 6);
INSERT INTO degree_with_subject (sub_id, degree_date, degree, student_id) 
VALUES (6, TO_DATE('02-05-2025', 'DD-MM-YYYY'), 94, 6);
-- Assign Teachers to Students (advisors)
INSERT INTO teacher_with_student (teacher_id, student_id) VALUES (1, 1); 
INSERT INTO teacher_with_student (teacher_id, student_id) VALUES (1, 2); 
INSERT INTO teacher_with_student (teacher_id, student_id) VALUES (4, 7);
INSERT INTO teacher_with_student (teacher_id, student_id) VALUES (4, 8); 




-- Drop all tables in reverse order of dependency
DROP TABLE teacher_with_grade CASCADE CONSTRAINTS;
DROP TABLE student_with_subject CASCADE CONSTRAINTS;
DROP TABLE teacher_with_student CASCADE CONSTRAINTS;
DROP TABLE class_with_teacher CASCADE CONSTRAINTS;
DROP TABLE degree_with_subject CASCADE CONSTRAINTS;
DROP TABLE subjects CASCADE CONSTRAINTS;
DROP TABLE students CASCADE CONSTRAINTS;
DROP TABLE teachers CASCADE CONSTRAINTS;
DROP TABLE classes CASCADE CONSTRAINTS;
DROP TABLE grade CASCADE CONSTRAINTS;
DROP TABLE users CASCADE CONSTRAINTS;

-- Then run your original table creation script again


				SELECT s.sub_name, d.degree 
				FROM (
				   SELECT d.sub_id, d.degree, 
				          ROW_NUMBER() OVER (PARTITION BY d.sub_id ORDER BY d.degree_date DESC) as rn 
				   FROM degree_with_subject d 
				   WHERE d.student_id = 1
				) d 
				JOIN subjects s ON d.sub_id = s.sub_id 
				WHERE d.rn = 1 
				ORDER BY s.sub_name;
				
SELECT s.sub_name, AVG(d.degree) as degree
FROM (
    SELECT d.sub_id, d.degree, 
           ROW_NUMBER() OVER (PARTITION BY d.sub_id ORDER BY d.degree_date DESC) as rn 
    FROM degree_with_subject d 
    WHERE d.student_id = 1
) d 
JOIN subjects s ON d.sub_id = s.sub_id 
WHERE d.rn = 1 
GROUP BY s.sub_name
ORDER BY s.sub_name
