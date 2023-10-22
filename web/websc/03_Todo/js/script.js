function addItem() {
    let item = $("#item").val();
    if (item === "") {
        return;
    }
    $("ol").append("<li>"+ item + "</li>").children().hover(showRemove);
    console.log("element added");
    $("#item").val('');
}

function showRemove() {
    $(this).css('cursor', 'url(img/trash-can-icon-32x32.png), auto').on("click", () => $(this).remove()
    );
}

function sortList(order) {
    let list = [];
    list = $('ol').children();
    if (order === "asc") {
        list.sort(sortAsc);
    } else {
        list.sort(sortDesc);
    }
    $('ol').empty().append(list).children().hover(showRemove);
}

function sortAsc(a, b){
    console.log("testAsc" + $(a).text() + b);
    return ($(b).text()) < ($(a).text()) ? 1 : -1;
}

function sortDesc(a, b) {
    console.log("testDesc");
    return ($(b).text()) > ($(a).text()) ? 1 : -1;
}

/**use window on load to wait for DOM elements to load*/
$(window).on("load", () =>
    $("#add").on("click", addItem)
);