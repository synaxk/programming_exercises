<?php

class AppointmentHandler
{
    private $appointments;

    public function __construct()
    {

    }


    public function getAppointments() {
        $dh = new DataHandler('Appointment');
        return $dh->selectAll();
    }

    public function getAppointmentDetails($appointmentID) {
        $dh = new DataHandler('Appointment');
        $data = $dh->selectAll("AppointmentID='" . $appointmentID . "'");
        $appointment = new Appointment($data['AppointmentID'], $data['Title'], $data['Location'], $data['DueDate']);

        $dh->changeTable('Date');
        $dates = $dh->selectAll("AppointmentID='" . $appointmentID . "'");
        $appointment->setDates($dates);

        $dh->changeTable('Vote');
        $votes = $dh->selectAll("AppointmentID='" . $appointmentID . "'");
        $appointment->setVotes($votes);

        $dh->changeTable('Comment');
        $comments = $dh->selectAll("AppointmentID='" . $appointmentID . "'");
        $appointment->setComments($comments);

        return $appointment;
    }
}