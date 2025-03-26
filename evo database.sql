create table users(
    user_id number generated always as identity primary key,
    username varchar2(255) unique not null,
    password varchar2(255) not null
);
select * from users;
drop table users;

create table teachers(
    teacher_id number generated always as identity primary key,
    user_id number,
    constraint user_fk foreign key (user_id) references users(user_id)
);
select teacher_id from teachers where user_id = (select user_id from users where username = '3bdo');
select student_id from students where user_id = (select user_id from users where username = 'abdullah');

drop table teachers;
select * from teachers;

create table classes(
    class_id number generated always as identity primary key,
    name varchar2(255) unique not null
);
drop table classes;
select * from classes;

create table students(
    student_id number generated always as identity primary key,
    class_id number,
    user_id number,
    constraint class_fk foreign key (class_id) references classes(class_id),
    constraint user_id foreign key (user_id) references users(user_id)
);
drop table students;
select * from students;

create table subjects(
    sub_id number generated always as identity primary key,
    sub_name varchar2(255) unique not null,
    teacher_id number,
    constraint teacher_fk foreign key (teacher_id) references teachers(teacher_id)
);
select * from subjects;

create table gredes_with_subject(
    sub_id number ,
    grade_date date primary key,
    grade number not null,
    student_id number ,
    constraint student_fk foreign key (student_id) references students(student_id),
    constraint sub_id foreign key (sub_id) references subjects(sub_id)
);
select * from grades_with_subject;

create table class_with_teacher(
    teacher_id references teachers(teacher_id),
    class_id references classes(class_id)
);
select * from class_with_teacher;

create table teacher_with_student(
    teacher_id references teachers (teacher_id),
    student_id references students (student_id)
);
select * from teacher_with_student;

create table student_with_subject(
    student_id references students (student_id),
    subject_id references subjects (sub_id)
);
select * from student_with_subject;


insert into classes (name) values ('science');

insert into users (username, password) values ('abdullah', '123');
insert into students (user_id, class_id) select user_id, class_id from users,classes where username = 'abdullah' and name = 'science' FETCH FIRST 1 ROW ONLY;


insert into users (username, password) values ('3bdo', '123');
insert into teachers (user_id) select user_id from users where username = '3bdo' FETCH FIRST 1 ROW ONLY;


select user_id from users where username = '3bdo' and password = '123';

select * from teachers where user_id = 2;
select * from students where user_id = 2;
