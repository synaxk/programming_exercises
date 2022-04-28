/**use window on load to wait for DOM elements to load*/
$(window).on("load", () => {
    /**hide detail view, div for newAppointment and div for new dates on load*/
    $("#details").hide();
    $("#dates").hide();
    $("#newAppointment").hide();

    /**load appointment list*/
    getAppointments();
    /** add click event for new Appointment button, back button */
    $("#newButton").click(()=>createNewAppointment());
    $("#backButton").click(()=>back());

});

function back() {
    $("#details").empty().hide();
    $("#newAppointment").empty().hide();
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
            console.log(response);
            $("#list").empty().show();
            response.forEach((item) => addItemToList(item));
            $("#newButton").show();
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
            back();
            console.log(response);
        },
        error: function(e){
            console.log(this.data);
        }
    });
}

/**add appointments to the list*/
function addItemToList(item) {
    let valid = "<p>Vote until: "+item.dueDate+"</p>";

    if (Date.now() > Date.parse(item.dueDate)) {
        valid = "<p style='color:red'>Expired since: "+item.dueDate+"</p>";
    }

    $("#list").append("<div class='item row'><div class='col-sm'><h5 onclick='getAppointment("+item.appointmentID+")'>" + item.title + "</h5> "
         +item.location + valid +"</div>"+
        "<div class='col-sm' style='padding-top: 20px'><button class='btn btn-outline-danger btn-sm' onclick='deleteAppointment("+item.appointmentID+");" +
        "$(this).parent().parent().remove()'>X</button></div></div><br>");

}

function deleteAppointment(appointmentID) {
    $.ajax({
        url:"../backend/ServiceHandler.php",
        type: "POST",
        dataType: "json",
        data: {"appointmentID": appointmentID,
                "delete": true},
        success: function(response) {
            console.log(response);
        }
    });
}

/**create appointment-details view with date options and comments*/
function createDetailView(item, appointmentID) {
    $("#list").hide(); //funktioniert nicht wenn vorher detail
    console.log(item);
    /**create detail view of appointment*/
    $("#details").prepend("<h4>"+ item.title+" </h4><p>"+item.location+"</p>" +
        "<p>Vote until: "+item.dueDate+"</p><div id='vote_options' ><h6>Please choose a date</h6></div>" +
        "<label for='user'>Username:</label><br><input id='user' type='text'/>" +
        "<button class='btn btn-outline-success' onclick='submitVote("+appointmentID+")'>Vote</button>" +
        "<div id='comments'>Comments:</div>").show();
    $("#details").append("<label for='comment'>Comment</label><br><textarea id='new_comment'>"+
        "</textarea><br><button class='btn btn-outline-success' id='submitComment'>Submit Comment</button>");

    /**add date options*/
    if (item.dates.length > 0) {
        let vote = "";
        if (item.votes.length > 0) {
            let voteCount = 0;
            item.votes.forEach((vote) => {
                console.log(item.dates);

            });

            vote = "Votes: "+ voteCount;

        }
        item.dates.forEach((date) => $("#vote_options").append("<input type='radio' name='vote' id=date'"
            + date.dateID +"' value='"+ date.dateID+ "'><label for='date"+ date.dateID
            +"'>" +date.startDate+ " - "+ date.endDate+"</label><br>"));
    }

    item.comments.forEach((comment) => $("#comments").append("<div>"+ comment.username +": "+ comment.text +"</div>"));

    /*add comments*/
    if($("#comment").val() != "") {
        $("#submitComment").click(() => addComment(appointmentID, $("#user").val(), $("#new_comment").val()));
    }


    if (Date.now() > Date.parse(item.dueDate)) {
        $("#details").children().attr('disabled', true);
    }

}

function addComment(appointmentID, user, comment){
    let newComment = {
        "username": user,
        "appointmentID": appointmentID,
        "text": comment
    };
    $.ajax({
        url:"../backend/ServiceHandler.php",
        type: "POST",
        dataType: "json",
        data: newComment,
        success: function(response) {
            back();
            console.log(response);
        },
        error: function(e){
            console.log("tsetetstsetset");
        }
    });

}

function createNewAppointment(){
    /**hide list and detail view*/
    $("#list").hide();
    $("#details").hide();

    /**create appointment-form in newAppointment div*/
    loadNewAppointmentForm();

    /**button*/
    $("#newButton").hide();

    /**show newAppointment div*/
    $("#newAppointment").show();

    $("#submitNewAppointment").click(()=>{
        $.ajax({
            url:"../backend/ServiceHandler.php",
            type: "POST",
            dataType: "json",
            data: createAppointmentObject(),
            success: function(response) {
                console.log(response);
                $("#newAppointment").empty().hide();
                getAppointments();
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
        "<br><label for='dateOption'>Date Option</label><br>"+
        "<input type='date' id='dateOption'><br>"+
        "<label for='startTime'>Start Time</label><br>"+
        "<input type='time' id='startTime'><br>"+
        "<label for='endTime'>End Time</label><br>"+
        "<input type='time' id='endTime'>"+
        "<button id='addDate' className='btn btn-outline-success'>Add Date</button><br>"+ // BUTTON add date
        "<button id='submitNewAppointment' className='btn btn-outline-success'>Submit</button><br>"+ //BUTTON submit
        "<button id='cancel' className='btn btn-outline-danger'>Cancel</button><br>" + //BUTTON cancel
        "<div class='col-sm' id='dateList'><br></div>");

    $("#cancel").click(()=>{
        back();
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
        "<input type='date' id='new_dueDate'>" +
        "<div id='dates'></div>"
    );
    loadDateForm();
}