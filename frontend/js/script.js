/**use window on load to wait for DOM elements to load*/
$(window).on("load", () =>
    getAppointments()
);

function getAppointments() {
    $.ajax({
        type: "GET",
        url: "../backend/controller/AppointmentController.php",
        cache: "false",
        dataType: "json",
        success: function (response) {
            response.forEach((item) => addItemToList(item));
        }
    })
}

function getAppointment($appointmentID) {
    console.log("Get AppointmentID")
    $.ajax({
        type: "GET",
        url: "../backend/controller/AppointmentController.php?id=" + $appointmentID,
        cache: "false",
        dataType: "json",
        success: function (response) {
            //show detail div, and load appointment details into fields;
        }
    })
}

function addItemToList(item) {
    $("#list").append("<p>" + item.Title + "</p>").addEventListener("click", () => getAppointment(item.AppointmentID))
}


/*
function submit() {
    $data = {
        "AppointmentID": $appointmentID,
        "Username": $("#user").val(),
    }


}


function addDate() {
    let newDateObject = "<div></div>"
}
*/

/* TODO:
    hide details
    toggle details onclick

* */
// datepicker fügt datum in datelist hinzu
/// <div id="dateList">
//      <p>2022-05-01 14:00:00</p>
//      <p>2022-05-01 16:00:00</p>
// </div>

/// [{"AppointmentID":"1","Title":"TestAppointment","Location":"TestLocation","DueDate":"2022-05-01 17:08:26"},
// {"AppointmentID":"2","Title":"TestAppointment2","Location":"TestLocation2","DueDate":"2022-05-01 17:08:44"}]