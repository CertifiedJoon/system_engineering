CREATE DATABASE `sqlchallenge`;
USE `sqlchallenge`;

CREATE TABLE Publishers (
  publisher_id INT NOT NULL,
  name VARCHAR(200) NOT NULL,
  PRIMARY KEY (publisher_id)
);

CREATE TABLE Authors (
  author_id INT NOT NULL,
  name VARCHAR(200) NOT NULL,
  PRIMARY KEY (author_id)
);

CREATE TABLE Members (
  member_id INT NOT NULL,
  name VARCHAR(200) NOT NULL,
  PRIMARY KEY (member_id)
);



