INSERT INTO Restaurant (restaurant_id, name) VALUES 
(1, 'McRonalds'),
(2, 'PizzaHub'),
(3, 'DeliItaly'),
(4, 'UltraSandwich'),
(5, 'Starducks');

INSERT INTO Branch (restaurant_id, branch_no, location, seats) VALUES 
(1, 1,'Admiralty', 10),
(1, 2,'Central', 20),
(2, 1,'Causeway Bay', 5),
(3, 1,'Admiralty', 25),
(3, 2,'Wan Chai', 45),
(3, 3,'Causeway Bay', 35),
(4, 1,'Central', 170),
(4, 2,'Admiralty', 100),
(4, 3,'North Point', 120),
(5, 1,'Central', 80),
(5, 2,'Wan Chai', 40);

INSERT INTO RestaurantCategory (restaurant_id, category) VALUES 
(1, 'Take Away'),
(1, 'Fast Food'),
(2, 'Italian'),
(3, 'Cafe'),
(3, 'Italian'),
(4, 'Light meal'),
(5, 'Take Away'),
(5, 'Light meal'),
(5, 'Cafe');

INSERT INTO Member (member_id, name, birthday, joined, points) VALUES 
(1, 'Cleo', '1983-09-25','2017', 690),
(2, 'Evan', '1988-03-28','1989', 130),
(3, 'Todd', '1966-06-22','1967', 190),
(4, 'Lonny', '1973-04-05','2016', 10),
(5, 'Keith', '1991-06-19','1992', 380),
(6, 'Royce', '1965-06-14','2006', 300),
(7, 'Mavis', '1977-08-21','1981', 840),
(8, 'Alvin', '1998-04-17','2008', 900),
(9, 'Ira', '1993-07-17','2015', 100),
(10, 'Dino', '1968-12-26','2012', 150),
(11, 'A. Bella', '1989-11-26','2016', 20);

INSERT INTO visits (visit_id, member_id, restaurant_id, branch_no, `date`, score) VALUES 
(1, 1, 4, 3,'2015-12-30', 5),
(2, 8, 4, 3,'2016-01-15', 4),
(3, 1, 2, 1,'2018-06-16', 5),
(4, 2, 3, 2,'2018-06-14', 5),
(5, 1, 2, 1,'2018-12-09', 3),
(6, 3, 3, 2,'2018-01-16', 6),
(7, 2, 4, 1,'2018-03-26', 3),
(8, 4, 3, 2,'2018-08-02', 7),
(9, 5, 3, 1,'2018-04-22', 8),
(10, 4, 5, 2,'2018-05-10', 0),
(11, 5, 5, 2,'2018-05-08', 6),
(12, 3, 5, 1,'2018-06-21', 1),
(13, 6, 5, 1,'2018-03-06', 2),
(14, 4, 5, 1,'2018-02-12', 3),
(15, 7, 4, 2,'2018-06-07', 1),
(16, 5, 4, 2,'2018-12-04', 2),
(17, 3, 4, 2,'2018-01-19', 3),
(18, 5, 3, 3,'2018-01-27', 5),
(19, 7, 3, 3,'2018-06-11', 5),
(20, 2, 3, 3,'2018-11-25', 5),
(21, 6, 3, 3,'2018-07-27', 4);
