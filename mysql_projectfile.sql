create database hc;
use hc;
create table user_details
(
first_name varchar(255) not null, 
last_name varchar (255) not null, 
gender enum('male', 'female', 'others') not null,  
account_no integer auto_increment,  
address varchar(255) not null,  
username varchar (255) not null,  
password varchar (255) not null,  
primary key(account_no) )
);

Alter table user_details auto_increment = 12001;
select * from user_details;