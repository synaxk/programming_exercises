$(window).on("load", () =>
    loadProducts()
);

//Ajax call to products.json
function loadProducts() {
    $.ajax({
        type: "GET",
        url: "products.json",
        cache: "false",
        data: {},
        dataType: "json",
        success: function (response) {
            response.forEach((item, index) => {
                addItem(item, index);
            })
            //slideDown list with border
            $("#products").hide().css('border', '1px solid #A5ACB2').slideDown(250);
        },
        error: () => {
            //display styled error message
            $("#products").append("<p style='color:red; font-weight: bold; text-align: center'>an error occurred</p>")
        }
    });
}

//add item(JSON object) with alternating background color
function addItem(data, index) {
    let line;
    if (index % 2 === 0) {
        line = "<p onclick='selectItem(this);'><b>" + data.name + "</b> for " + data.price +"</p>";
    } else {
        line = "<p onclick='selectItem(this);' class='greyline'><b>" + data.name + "</b> for " + data.price +"</p>";
    }
    $("#products").append(line);
}

// function selectItem, hide product list and fade-in selected item afterwards
function selectItem(item) {
    $("#products").slideUp(250).promise().done(
        $("#selected").hide().append(item).fadeIn(150)
        //Wenn fadein auf 150ms konfiguriert ist sieht die Anzeige in meinem
        //Browser fehlerhaft aus, bei 1000 ms sieht es aber gut aus...
    )
}