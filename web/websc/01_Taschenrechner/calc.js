let history = [];

function calculate(op) {
    let num1, num2, result = 0;
    num1 = parseFloat(document.getElementById('number1').value);
    num2 = parseFloat(document.getElementById('number2').value);

    switch (op) {
        case 'add':
            result = add(num1,num2);
            history.push(num1 + " + " + num2 + " = " + result + "\n");
            console.log(result);
            break;
        case 'sub':
            result = sub(num1,num2);
            history.push(num1 + " - " + num2 + " = " + result + "\n");
            break;
        case 'mul':
            result = mul(num1,num2);
            history.push(num1 + " * " + num2 + " = " + result + "\n");
            break;
        case 'div':
            result = div(num1,num2);
            history.push(num1 + " / " + num2 + " = " + result + "\n");
            break;
    }
    console.log("test");
    document.getElementById("result").value = result.toString();
    document.getElementById("history").value = history;

}

function add(x,y) {
    return x+y;
}

function sub(x,y) {
    return x-y;
}

function mul(x,y) {
    return x*y;
}

function div(x,y) {
    return x/y;
}