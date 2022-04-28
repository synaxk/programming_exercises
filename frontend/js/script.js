/**use window on load to wait for DOM elements to load*/
$(window).on("load", () => {
    /**hide detail view, div for newAppointment and div for new dates on load*/
    $("#details").hide();
    $("#dates").hide();
    $("#dateList").hide();
    $("#newAppointment").hide();

    /**load appointment list*/
    getAppointments();

    /** add click event for new Appointment button */
    $("#createAppointment").click(()=>createNewAppointment());



    /**click .outside class to leave and clear the detail view //////////////////KANN WEG
    $(".outside").on("click", () => {
        $("#details").hide().children().remove();
        $("#list").show();
    })*/
});

/**load appointment list from the api*/
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

/**select specific appointment from api, hide the list and show details*/
function getAppointment(appointmentID) {
    $("#list").toggle(); //geändert /////////////
    console.log("Get AppointmentID")
    $.ajax({
        type: "GET",
        url: "../backend/controller/AppointmentController.php?id=" + appointmentID,
        cache: "false",
        dataType: "json",
        success: function (response) {
            createDetailView(response, appointmentID);
        }
    })
}

/**send vote request to api*/
function submitVote($appointmentID){
    let voteInput = {
        "appointmentID": $appointmentID,
        "username": $("#user").val(),
        "dateID": $("input[name='vote']:checked").val()
    };

    $.ajax({
        url:"../backend/controller/AppointmentController.php",
        type: "POST",
        dataType: "json",
        data: voteInput,
        success: function(response) {
            console.log(response);

        },
        error: function(e){
            console.log(this.data);
        }
    });
}

/**add appointments to the list*/
function addItemToList(item) {
    console.log(item);
    $("#list").append("<p><p><h5 onclick='getAppointment("+item.appointmentID+")'>" + item.title +  "</h5>" +
        "</p><p>"+item.location+"</p><p>Vote until: "+item.dueDate+"</p></p>");

}

/**create appointment-details view with date options and comments*/
function createDetailView(item, appointmentID) {
    $("#list").hide(); //funktioniert nicht wenn vorher detail
    console.log(item);
    /**create detail view of appointment*/
    $("#details").prepend("<h4>"+ item.title+" </h4><p>"+item.location+"</p>" +
        "<p>Vote until: "+item.dueDate+"</p><div id='vote_options' ><h6>Please choose a date</h6></div>" +
        "<label for='user'>Username:</label><br><input id='user' type='text'/>" +
        "<button onclick='submitVote("+appointmentID+")'>Vote</button>" +
        "<div id='comments'>Comments:</div>").show();

    /**add date options*/
    item.dates.forEach((date) => $("#vote_options").append("<input type='radio' name='vote' id=date'"
        + date.dateID +"' value='"+ date.dateID+ "'><label for='date"+ date.dateID
        +"'>" +date.startDate+ " - "+ date.endDate+"</label><br>"));

    /**add comments /////////////////////TODO comment adden*/
   // item.comments.forEach((comment) => $("#comments").append("<div>"+ comment.username +": "+ comment.text +"</div>"));
}

function createNewAppointment(){
    $("#details").toggle();
    $("#list").toggle();
    $("#createAppointment").hide();
    $("#dates").show();
    $("#dateList").show();
    $("#newAppointment").show();


    /** add click event for "addDate to list" button*/
    $("#addDate").click(()=> addDateOption());
    $("#submitNewAppointment").click(()=>{
        var childrenArray = $("#dateList").children().text(); //zugewiesen wird ein string in dem alle als text stehen
        console.log(childrenArray);
       var listItem = $("#dateList").first();
        //(for(child in listItems){
        //    console.log(listItems[child]);
       // }

    });


}
//////////////////////////////////////////////
//when creating a new appointment -> add as many date options as you like
//appends input from date input fields + button with onclick to remove dateOption from list
function addDateOption() {
    if($("#dateOption").val() != "" && $("#startTime").val()) {
        $("#dateList").append("<p><button class='btn btn-outline-danger btn-sm' onclick='$(this).parent().remove()'>X</button>Date: "
            + $("#dateOption").val() + " Start Time: " + $("#startTime").val() + "</p>");
    }
}



///////////////////
//date options array :


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