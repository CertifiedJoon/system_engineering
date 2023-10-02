CREATE TABLE AppUser (
  user_id INT NOT NULL PRIMARY KEY,
  name VARCHAR(80)
);

CREATE TABLE Customer (
  user_id INT NOT NULL PRIMARY KEY,
  birthday DATE,
  FOREIGN KEY (user_id) REFERENCES AppUser (user_id)
);

CREATE TABLE CustomerPhone (
  user_id INT NOT NULL,
  phone VARCHAR(80) NOT NULL,
  PRIMARY KEY (user_id, phone),
  FOREIGN KEY (user_id) REFERENCES Customer (user_id)
);

CREATE TABLE Admin (
  user_id INT NOT NULL PRIMARY KEY, 
  supervisor_id INT,
  FOREIGN KEY (user_id) REFERENCES AppUser (user_id),
  FOREIGN KEY (supervisor_id) REFERENCES Admin (user_id)
);

CREATE TABLE Album (
  album_id INT NOT NULL PRIMARY KEY,
  name VARCHAR(80),
  creation_date DATE,
  user_id INT NOT NULL,
  FOREIGN KEY (user_id) REFERENCES AppUser (user_id)
);

CREATE TABLE Image (
  album_id INT NOT NULL,
  image_id INT NOT NULL,
  filename VARCHAR(80),
  PRIMARY KEY (album_id, image_id),
  FOREIGN KEY (album_id) REFERENCES Album (album_id)
);

CREATE TABLE Product (
  product_id INT NOT NULL PRIMARY KEY,
  description TEXT, 
  price decimal(10,2)
);

CREATE TABLE PrintOrder (
  order_id INT NOT NULL PRIMARY KEY, 
  user_id INT NOT NULL,
  status VARCHAR(80),
  pic_id INT,
  FOREIGN KEY (user_id) REFERENCES Customer (user_id),
  FOREIGN KEY (pic_id) REFERENCES Admin (user_id)
);

CREATE TABLE Item (
  order_id INT NOT NULL,
  item_id INT NOT NULL,
  quantity INT,
  product_id INT NOT NULL, 
  album_id INT NOT NULL,
  image_id INT NOT NULL,
  PRIMARY KEY (order_id, item_id),
  FOREIGN KEY (order_id) REFERENCES PrintOrder (order_id),
  FOREIGN KEY (product_id) REFERENCES Product (product_id),
  FOREIGN KEY (album_id, image_id) REFERENCES Image (album_id, image_id)
);
