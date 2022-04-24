<?php
require_once "../db/DataHandler.php";
require_once "../businessLogic/AppointmentHandler.php";
require_once "../models/Appointment.php";

if ($_SERVER["REQUEST_METHOD"] == "POST") {

} elseif ($_SERVER["REQUEST_METHOD"] == "GET") {
    $ah = new AppointmentHandler();

    if (isset($_GET["id"])) {
        $appointment_ID = $_GET["id"];
        $appointment = $ah->getAppointmentDetails($appointment_ID);
        print_r($appointment);
    } else {
        $data = $ah->getAppointments();
        echo json_encode($data);
    }
}