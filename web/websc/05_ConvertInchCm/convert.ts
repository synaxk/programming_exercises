const cmValue = <HTMLInputElement> document.getElementById("cm");
const inchValue = <HTMLInputElement> document.getElementById("inch");
console.log(cmValue);

cmValue.addEventListener("keydown", () =>
    setTimeout(() => convert("cm"), 100)
);

inchValue.addEventListener("keydown", () =>
    setTimeout(() => convert("inch"), 100)
);

function convert(from: string) {
    let result = 0;
    if (from === "cm") {
        console.log(cmValue.value);
        result = parseFloat(cmValue.value) /  2.54;
        result = Math.round((result) * 100) / 100;
        inchValue.value = result.toString();
    } else if (from === "inch") {
        result = parseFloat(inchValue.value) * 2.54;
        result = Math.round((result) * 100) / 100;
        cmValue.value = result.toString();
    }
    return result;
}
