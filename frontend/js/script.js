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

    $("#back").click(()=>back());

    $("#cancel").click(()=>{
        $("#dateList").empty();
        $("#dates").hide();
        $("#newAppointment").hide();

    });
});

function back() {
    $("#details").empty().hide();

    $("#newAppointment").hide();
    $("#createAppointment").show();
    $("#list").empty().show();
    getAppointments();

}

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
    $("#details").append("<label for='comment'>Comment</label><br><textarea id='new_comment'>"+
        "</textarea><br><button id='submitComment'>Submit Comment</button>");

    /**add date options*/
    item.dates.forEach((date) => $("#vote_options").append("<input type='radio' name='vote' id=date'"
        + date.dateID +"' value='"+ date.dateID+ "'><label for='date"+ date.dateID
        +"'>" +date.startDate+ " - "+ date.endDate+"</label><br>"));

    /**add comments /////////////////////TODO comment adden*/
    //$("#submitComment").click()
   // item.comments.forEach((comment) => $("#comments").append("<div>"+ comment.username +": "+ comment.text +"</div>"));
}

function createNewAppointment(){
    $("#details").hide();
    $("#list").hide();
    $("#createAppointment").hide();
    $("#dates").show();
    $("#dateList").show();
    $("#newAppointment").show();


    /** add click event for "addDate to list" button*/
    $("#addDate").click(()=> addDateOption());
    $("#submitNewAppointment").click(()=>{

        let dates = [];

        $("#dateList").children(".dateValue").each((index, item) => {
                console.log(item.innerText);
                if(item.innerText != "") { //only dates are stored in array, not <b> and <p>
                    let opts = item.innerText.split(',');
                    dates[index] = {"startDate": opts[0],
                        "endDate": opts[1]
                    }
                }
            }
        );

        let newAppointment = {
            "title": $("#new_title").val(),
            "location": $("#new_location").val(),
            "dueDate": $("#new_dueDate").val(),
            "dates": dates
        };

        $.ajax({
            url:"../backend/controller/AppointmentController.php",
            type: "POST",
            dataType: "json",
            data: newAppointment,
            success: function(response) {
                console.log(response);

            },
            error: function(e){
                console.log(this.data);
            }
        });
    });

    back();
}
//////////////////////////////////////////////
//when creating a new appointment -> add as many date options as you like
//appends input from date input fields + button with onclick to remove dateOption from list
function addDateOption() {
    if($("#dateOption").val() != "" && $("#startTime").val()) {
        $("#dateList").append("<p><button class='btn btn-outline-danger btn-sm' onclick='$(this).parent().remove()'>X</button>Date: "
            + $("#dateOption").val() + " Start Time: " + $("#startTime").val() + " End Time: "+$("#endTime").val()+"</p>" +
            "<p class='dateValue' style='display: none'>" + $("#dateOption").val() +" "+$("#startTime").val()+":00,"
            + $("#dateOption").val() +" "+$("#endTime").val() + ":00</p>");
    }
}



/*
function submit() {
    $data = {
        "AppointmentID": $appointmentID,
        "Username": $("#user").val(),
    }


}

/// [{"AppointmentID":"1","Title":"TestAppointment","Location":"TestLocation","DueDate":"2022-05-01 17:08:26"},
// {"AppointmentID":"2","Title":"TestAppointment2","Location":"TestLocation2","DueDate":"2022-05-01 17:08:44"}]

 */