#DROP DATABASE AppointmentFinder;
CREATE DATABASE AppointmentFinder;
USE AppointmentFinder;

CREATE TABLE Appointment (
                             AppointmentID INT NOT NULL AUTO_INCREMENT,
                             Title VARCHAR(255) NOT NULL,
                             Location VARCHAR(255) NOT NULL,
                             DueDate DATETIME NOT NULL,
                             PRIMARY KEY (AppointmentID)
);

CREATE TABLE Date (
                      DateID INT NOT NULL AUTO_INCREMENT,
                      StartDate DATETIME NOT NULL,
                      EndDate DATETIME NOT NULL,
                      AppointmentID INT NOT NULL REFERENCES Appointment,
                      PRIMARY KEY (DateID)
);

CREATE TABLE Comment (
                         CommentID INT NOT NULL AUTO_INCREMENT,
                         AppointmentID INT NOT NULL REFERENCES Appointment,
                         Username VARCHAR(255) NOT NULL,
                         Text VARCHAR(255) NOT NULL,
                         PRIMARY KEY (CommentID)
);

CREATE TABLE Vote (
                      VoteID INT NOT NULL AUTO_INCREMENT,
                      AppointmentID INT NOT NULL REFERENCES Appointment,
                      DateID INT NOT NULL REFERENCES Date,
                      Username VARCHAR(255) NOT NULL,
                      PRIMARY KEY (VoteID)
);
