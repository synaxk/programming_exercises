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
        $data = $dh->selectAll("appointmentID=$appointmentID")[0];
        $appointment = new Appointment($data['appointmentID'], $data['title'], $data['location'], $data['dueDate']);

        $dh->changeTable('date');
        $dates = $dh->selectAll("appointmentID=$appointmentID");
        foreach($dates as $key => $date) {
            $dh->changeTable('vote');
            $votes = $dh->selectCount("dateID={$date['dateID']} AND appointmentID=$appointmentID")[0];
            $dates[$key]['votes'] = $votes['count'];
        }
        $appointment->setDates($dates);

        $dh->changeTable('vote');
        $votes = $dh->selectAll("appointmentID=$appointmentID");
        $appointment->setVotes($votes);

        $dh->changeTable('comment');
        $comments = $dh->selectAll("appointmentID=$appointmentID");
        $appointment->setComments($comments);
        return $appointment;
    }

    public function createAppointment($data) {
        $dates = $data["dates"];
        unset($data["dates"]);

        //todo: store appointment -> get new appointment id
        // store relations
    //    $data["dueDate"] = "2022-05-12 12:12:12";
        $dh = new DataHandler('appointment');
        $appointmentID = $dh->insert($data);

        $dh->changeTable("date");
        foreach ($dates as $date) {
            $date["appointmentID"] = $appointmentID;
            $dh->insert($date);
        }
    }

    public function delete($appointmentID){
        /**delete relations*/
        /**start with votes to remove "used" (FK -Dates in vote table) dates*/
        $dh = new DataHandler('vote');
        $dh->delete("appointmentID=$appointmentID");

        $dh->changeTable('date');
        $dh->delete("appointmentID=$appointmentID");

        $dh->changeTable('comment');
        $dh->delete("appointmentID=$appointmentID");

        /**delete Appointment*/
        $dh->changeTable('appointment');
        $dh->delete("appointmentID=$appointmentID");
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