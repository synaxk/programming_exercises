CREATE DATABASE AppointmentFinder;
##DROP DATABASE AppointmentFinder;
USE AppointmentFinder;

CREATE TABLE Appointment (
    AppointmentID INT NOT NULL AUTO_INCREMENT,
    Title VARCHAR(255) NOT NULL,
    Location VARCHAR(255) NOT NULL,
    DueDate DATETIME NOT NULL,
    AppointmentDate DATETIME,
    Duration INT NOT NULL,
    PRIMARY KEY (AppointmentID)
);

CREATE TABLE Date (
    DateID INT NOT NULL AUTO_INCREMENT,
    Date DATETIME NOT NULL,
    AppointmentID INT NOT NULL REFERENCES Appointment,
    PRIMARY KEY (DateID)
);

CREATE TABLE Vote (
    VoteID INT NOT NULL AUTO_INCREMENT,
    DateID INT NOT NULL REFERENCES Date,
    Username VARCHAR(255) NOT NULL,
    Comment VARCHAR(255),
    PRIMARY KEY (VoteID)
);
