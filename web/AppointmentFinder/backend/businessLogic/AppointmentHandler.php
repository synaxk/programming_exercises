<?php

class AppointmentHandler {
    private $appointments;

    public function __construct() {

    }


    /**get a list of appointments*/
    public function getAppointments() {
        /**init datahandler with tablename to select the data from*/
        $dh = new DataHandler('appointment');
        return $dh->selectAll();
    }

    /**get one specific appointment with relational data
        + dates
        + votes
        + comments*/
    public function getAppointmentDetails($appointmentID) {

        /**Select Appointment and create new Appointment Object*/
        $dh = new DataHandler('appointment');
        $data = $dh->selectAll("appointmentID=$appointmentID")[0]; //[0] because the function returns an array
        $appointment = new Appointment($data['appointmentID'], $data['title'], $data['location'], $data['dueDate']);

        /**change table to select dates*/
        $dh->changeTable('date');
        $dates = $dh->selectAll("appointmentID=$appointmentID");
        /**loop through dates and add voteCount*/
        foreach($dates as $key => $date) {
            $dh->changeTable('vote');
            $votes = $dh->selectCount("dateID={$date['dateID']} AND appointmentID=$appointmentID")[0];
            $dates[$key]['votes'] = $votes['count'];
        }
        $appointment->setDates($dates);

        /***/
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
        /**delete dependencies*/
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