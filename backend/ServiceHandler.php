<?php
require_once "db/DataHandler.php";
require_once "businessLogic/AppointmentHandler.php";
require_once "models/Appointment.php";

/**AppointmentHandler controls read and store methods*/
$ah = new AppointmentHandler();
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST["appointmentID"])) {
        if (isset($_POST["dateID"])) {
            $ah->vote($_POST);
            echo json_encode("Vote submitted successfully.");
        } elseif (isset($_POST["text"])) {
            $ah->comment($_POST);
            echo json_encode("Comment added successfully.");
        } elseif (isset($_POST["delete"])) {
            $result = $ah->delete($_POST["appointmentID"]);
            if ($result) {
                echo json_encode("Appointment deleted successfully");
            }
        }
    } else {
        $ah->createAppointment($_POST);
        echo json_encode("Appointment created successfully.");
    }

    /**Get Appointment-Detail or List of Appointments*/
} elseif ($_SERVER["REQUEST_METHOD"] == "GET") {
    if (isset($_GET["id"])) {
        $appointment_ID = $_GET["id"];
        $appointment = $ah->getAppointmentDetails($appointment_ID);
        echo json_encode($appointment->getData());
    } else {
        $data = $ah->getAppointments();
        echo json_encode($data);
    }
}