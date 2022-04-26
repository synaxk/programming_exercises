DROP DATABASE appointmentfinder;
CREATE DATABASE appointmentfinder;
USE appointmentfinder;

CREATE TABLE appointment (
                             appointmentID INT NOT NULL AUTO_INCREMENT,
                             title VARCHAR(255) NOT NULL,
                             location VARCHAR(255) NOT NULL,
                             dueDate DATETIME NOT NULL,
                             PRIMARY KEY (AppointmentID)
);

CREATE TABLE date (
                      dateID INT NOT NULL AUTO_INCREMENT,
                      startDate DATETIME NOT NULL,
                      endDate DATETIME NOT NULL,
                      appointmentID INT NOT NULL REFERENCES appointment,
                      PRIMARY KEY (dateID)
);

CREATE TABLE comment (
                         commentID INT NOT NULL AUTO_INCREMENT,
                         appointmentID INT NOT NULL REFERENCES appointment,
                         username VARCHAR(255) NOT NULL,
                         text VARCHAR(255) NOT NULL,
                         PRIMARY KEY (CommentID)
);

CREATE TABLE vote (
                      voteID INT NOT NULL AUTO_INCREMENT,
                      appointmentID INT NOT NULL REFERENCES appointment,
                      dateID INT NOT NULL REFERENCES date,
                      username VARCHAR(255) NOT NULL,
                      PRIMARY KEY (voteID)
);

GRANT ALL PRIVILEGES ON appointmentfinder.* TO 'svc_appointmentfinder'@'localhost' WITH GRANT OPTION;


use appointmentfinder;
INSERT INTO appointment (Title, Location, DueDate)
VALUES  ('TestAppointment1', 'TestLocation1', NOW()+ INTERVAL 7 day),
        ('TestAppointment2', 'TestLocation2', NOW()+ INTERVAL 7 day);

INSERT INTO date (startDate, endDate, appointmentID)
VALUES (NOW() + INTERVAL 14 day, NOW() + INTERVAL 14 day + INTERVAL 2 hour, 1),
       (NOW() + INTERVAL 16 day, NOW() + INTERVAL 16 day + INTERVAL 2 hour, 1);

INSERT INTO vote (appointmentID, dateID, username)
VALUES  (1, 1, 'RudiDerRude'),
        (1, 1, 'ThorstenMitBorsten');

INSERT INTO comment (appointmentID, username, text)
VALUES (1, 'ThorstenMitBorsten', 'Hi ich bin der Thorsten');
