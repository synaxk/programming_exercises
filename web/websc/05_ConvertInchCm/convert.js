var cmValue = document.getElementById("cm");
var inchValue = document.getElementById("inch");
console.log(cmValue);
cmValue.addEventListener("keydown", function () {
    return setTimeout(function () { return convert("cm"); }, 500);
});
inchValue.addEventListener("keydown", function () {
    return setTimeout(function () { return convert("inch"); }, 500);
});
function convert(from) {
    var result = 0;
    if (from === "cm") {
        console.log(cmValue.value);
        result = parseFloat(cmValue.value) / 2.54;
        result = Math.round((result) * 100) / 100;
        inchValue.value = result.toString();
    }
    else if (from === "inch") {
        result = parseFloat(inchValue.value) * 2.54;
        result = Math.round((result) * 100) / 100;
        cmValue.value = result.toString();
    }
    return result;
}
