<?php

/*
 * example data
 { "appointmentID": 1,
   "title": "Lerntreffen",
   "location": "FH",
   "dueDate": "01052022",
   "dates": [
     {"dateID": 1, "startDate": "12.04.2022 14:30", "endDate": "12.04.2022 17:30"},
     {"dateID": 2, "startDate": "13.04.2022 15:30", "endDate": "12.04.2022 18:30"},
     {"dateID": 3, "startDate": "12.04.2022 16:00", "endDate": "12.04.2022 19:00"}
   ],
    "votes": [
      {"username": "RudiDerRüde", "dateID": 1},
      {"username": "RudiDerRüde", "dateID": 2}
      {"username": "ThorstenMitBorsten", "dateID": 2}
    ],
    "comments": [
      {"username": "RudiDerRüde", "text": "Mit wäre der 13 lieber. LG Rudi"},
      {"username": "ThorstenMitBorsten", "text": "Mir auch. LG Thorsten"}
    ]
 }
*/

class Appointment {
    private $appointmentID;
    private $title;
    private $location;
    private $dueDate;

    private $dates;
    private $votes;
    private $comments;

    public function __construct($appointmentID, $title, $location, $dueDate) {
        $this->appointmentID = $appointmentID;
        $this->title = $title;
        $this->location = $location;
        $this->dueDate = $dueDate;
    }

    /**
     * @return mixed
     */
    public function getID() {
        return $this->appointmentID;
    }

    public function setDates($dates) {
        $this->dates = $dates;
    }

    public function setComments($comments) {
        $this->comments = $comments;
    }

    public function setVotes($votes) {
        $this->votes = $votes;
    }




}