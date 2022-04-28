/**use window on load to wait for DOM elements to load*/
$(window).on("load", () => {
    /**hide detail view, div for newAppointment and div for new dates on load*/
    $("#details").hide();
    $("#dates").hide();
    $("#newAppointment").hide();

    /**load appointment list*/
    getAppointments();
    /** add click event for new Appointment button, back button */
    $("#createAppointment").click(()=>createNewAppointment());
    $("#back").click(()=>back());

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
        url: "../backend/ServiceHandler.php",
        cache: "false",
        dataType: "json",
        success: function (response) {
            response.forEach((item) => addItemToList(item));
        }
    })
}

/**select specific appointment from api, hide the list and show details*/
function getAppointment(appointmentID) {
    $("#list").hide(); //geändert /////////////
    console.log("Get AppointmentID")
    $.ajax({
        type: "GET",
        url: "../backend/ServiceHandler.php?id=" + appointmentID,
        cache: "false",
        dataType: "json",
        success: function (response) {
            createDetailView(response, appointmentID);
        }
    });
}

/**send vote request to api*/
function submitVote($appointmentID){
    let voteInput = {
        "appointmentID": $appointmentID,
        "username": $("#user").val(),
        "dateID": $("input[name='vote']:checked").val()
    };

    $.ajax({
        url:"../backend/ServiceHandler.php",
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
    $("#list").hide();

    console.log("testsetessetsetset");
    loadNewAppointmentForm();
    loadDateForm();
    $("#details").hide();
    $("#createAppointment").hide();
    $("#dates").show();
    $("#newAppointment").show();

    $("#submitNewAppointment").click(()=>{
        $.ajax({
            url:"../backend/controller/AppointmentController.php",
            type: "POST",
            dataType: "json",
            data: createAppointmentObject(),
            success: function(response) {
                console.log(response);
            },
            error: function(e){
                console.log(this.data);
            }
        });
    });
}

function createAppointmentObject(){
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
    return newAppointment;
}

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

function loadDateForm(){
    $("#dates").append(
        "<br><label htmlFor='dateOption'>Date Option</label><br>"+
        "<input type='date' id='dateOption'><br>"+
        "<label htmlFor='startTime'>Start Time</label><br>"+
        "<input type='time' id='startTime'><br>"+
        "<label htmlFor='endTime'>End Time</label><br>"+
        "<input type='time' id='endTime'>"+
        "<button id='addDate' className='btn btn-outline-success'>Add Date</button><br>"+ // BUTTON add date
        "<button id='submitNewAppointment' className='btn btn-outline-success'>Submit</button><br>"+ //BUTTON submit
        "<button id='cancel' className='btn btn-outline-danger'>Cancel</button><br>" + //BUTTON cancel
        "<div className='col-sm' id='dateList'><br></div>");

    $("#cancel").click(()=>{
        $("#dateList").empty();
        $("#dates").hide();
        $("#newAppointment").hide();
    });

    /** add click event for "addDate to list" button*/
    $("#addDate").click(()=> addDateOption());
}

function loadNewAppointmentForm(){
    $("#newAppointment").append(
        "<h3>New Appointment</h3><br>"+
        "<label for='new_title'>Title</label><br>"+
        "<input type='text' id='new_title'><br>"+
        "<label for='new_location'>Location</label><br>"+
        "<input type='text' id='new_location'><br>"+
        "<label for='new_dueDate'>Votable until</label><br>"+
        "<input type='date' id='new_dueDate'>");
}