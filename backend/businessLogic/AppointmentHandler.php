<?php

class AppointmentHandler {
    private $appointments;

    public function __construct() {

    }


    public function getAppointments() {
        $dh = new DataHandler('appointment');
        return $dh->selectAll();
    }

    public function getAppointmentDetails($appointmentID) {
        $dh = new DataHandler('appointment');
        $data = $dh->selectAll("appointmentID='" . $appointmentID . "'");
        $appointment = new Appointment($data['appointmentID'], $data['title'], $data['location'], $data['dueDate']);

        $dh->changeTable('date');
        $dates = $dh->selectAll("appointmentID='" . $appointmentID . "'");
        $appointment->setDates($dates);

        $dh->changeTable('vote');
        $votes = $dh->selectAll("appointmentID='" . $appointmentID . "'");
        $appointment->setVotes($votes);

        $dh->changeTable('comment');
        $comments = $dh->selectAll("appointmentID='" . $appointmentID . "'");
        $appointment->setComments($comments);
        return $appointment;
    }

    public function createAppointment($data) {
        $appointment = new Appointment($data['title'], $data['location'], $data['dueDate']);

        $dates = $data['dates'];
        $votes = $data['votes'];
        $comments = $data['comments'];

        //todo: store appointment -> get new appointment id
        // store relations
        $dh = new DataHandler('appointment');

        $dh->insert();
    }

    public function vote($data) {
        $dh = new DataHandler('vote');
        $dh->insert($data);
    }

    public function comment($data) {
        $dh = new DataHandler('comment');
        $dh->insert($data);
    }
}