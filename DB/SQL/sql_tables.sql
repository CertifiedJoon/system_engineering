# Please create a new database for this exercise if you use your own mysql
    # DROP DATABASE IF EXISTS `sql_exercise`;
    # CREATE DATABASE `sql_exercise`;
    # USE `sql_exercise`;

# Else if you use mysql of hku, change the following `sql_exercise` to your own database

USE `sql_exercise`;

CREATE TABLE Restaurant (
  restaurant_id INT NOT NULL,
  name VARCHAR(200) NOT NULL,
  PRIMARY KEY (restaurant_id)
);

CREATE TABLE Branch (
  restaurant_id INT NOT NULL,
  branch_no INT NOT NULL,
  location VARCHAR(200) NOT NULL,
  seats INT NOT NULL,
  PRIMARY KEY (restaurant_id,branch_no),
  FOREIGN KEY (restaurant_id) REFERENCES Restaurant (restaurant_id)
);

CREATE TABLE RestaurantCategory (
  restaurant_id INT NOT NULL,
  category VARCHAR(200) NOT NULL,
  PRIMARY KEY (restaurant_id,category),
  FOREIGN KEY (restaurant_id) REFERENCES Restaurant (restaurant_id)
);

CREATE TABLE Member (
  member_id INT NOT NULL,
  name VARCHAR(200) NOT NULL,
  birthday date NOT NULL,
  joined INT NOT NULL,
  points INT NOT NULL,
  PRIMARY KEY (member_id)
);

CREATE TABLE visits (
  visit_id INT NOT NULL,
  member_id INT NOT NULL,
  restaurant_id INT NOT NULL,
  branch_no INT NOT NULL,
  `date` date NOT NULL,
  score INT NOT NULL,
  PRIMARY KEY (visit_id),
  FOREIGN KEY (member_id) REFERENCES Member (member_id),
  FOREIGN KEY (restaurant_id,branch_no) REFERENCES Branch (restaurant_id, branch_no)
);
