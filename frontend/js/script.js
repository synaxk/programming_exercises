/**use window on load to wait for DOM elements to load*/
$(window).on("load", () => {
    getAppointments();
    $("#details").hide();
    $("#addDate").click(addDateOption);
    $("#dates").hide()
});


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
           console.log(response);
            $("#details").append("<p><strong>"+ response.title+"  </strong></p><p>"+response.location+"</p><p>Vote until: "+response.dueDate+"</p>" );
            response.date.forEach((item, index) => $("#vote_options").append("<input> type='radio' name='vote' id=date'"+ item.dateID +"' value='" + item.dateID+ "'><label for=date'"+ item.dateID +"'>" +item.startDate+ " - "+ item.endDate+"</label><br>");
            $("#details").show();

        }
    })
}

function addItemToList(item) {
    console.log(item);
    $("#list").append("<p><p><h5 onclick='getAppointment("+item.appointmentID+")'>" + item.title +  "</h5>" +
        "</p><p>"+item.location+"</p><p>Vote until: "+item.dueDate+"</p></p>");

}

//////////////////////////////////////////////
//when creating a new appointment -> add as many date options as you like
//appends input from date input fields + button with onclick to remove dateOption from list
function addDateOption() {
    console.log("something");
    $("#dateList").append("<p><button class='btn btn-outline-danger btn-sm' onclick='$(this).parent().remove()'>X</button>Date: "
        + $("#dateOption").val() + " Start Time: "+$("#startTime").val()+"</p>");
};

function removeDate(){
    console.log(this);
    $(this).remove();
}
//TO DO Remove

$("#submitDates").on("click", function(){
    var listItems = $("#dateList").childNodes;
    for(child in listItems){
        console.log(listItems[child]);
    }
});

////////////////////////////////////////////

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