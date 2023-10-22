let flipped = [];
let counter = 0;
let time = 0;
let interval;


function init() {
    document.getElementById("user").innerHTML = window.prompt("Username:", "");
    interval = setInterval( () => {
        document.getElementById("time").innerHTML = (++time).toString();
    }, 1000);

    let nodes = generateCards();
    nodes = shuffle(nodes);
    appendNodes(nodes);
}

function count() {
    document.getElementById("counter").innerHTML = (++counter).toString();
}

function addNode(item, index) {
    let div = document.getElementById("spielbereich");
    div.appendChild(item);
    if (((index + 1) % 4) === 0) {
        div.appendChild(document.createElement("br"));
    }
}

function appendNodes(nodes) {
    nodes.forEach(addNode);
}

function shuffle(nodes) {
    nodes = nodes.sort(() => Math.random() - 0.5);
    return nodes;
}

function game() {
    let size;
    flipped.push(this);
    size = flipped.length;

    if (size === 2) {
        compare(flipped).then(r => {
            if (document.getElementsByClassName("matched").length === 16) {
                clearInterval(interval);
            }
        });
        flipped = [];
    }
}

async function compare(flipped) {
    let num1, num2 = 0;
    count();
    num1 = parseInt(flipped[0].id.match(/[0-9]+/));
    num2 = parseInt(flipped[1].id.match(/[0-9]+/));
    if (num1 + num2 === 17) {
        await new Promise(r => setTimeout(r, 1000));
        flipped.forEach((item) => {
            item.classList.toggle("matched")
            item.removeEventListener("click", flip)
            item.removeEventListener("click", game)
        });
    } else {
        await new Promise(r => setTimeout(r, 1000));
        flipped.forEach((item) => {
            item.classList.toggle("hidden")
        });
    }
}

function flip() {
    this.classList.toggle("hidden");
}

function generateCards() {
    let nodes = [];
    let img;
    for (let i = 0; i < 16; i++) {
        nodes[i] = document.createElement("div");
        nodes[i].classList = "karte hidden";
        nodes[i].id = `card${i + 1}`;
        nodes[i].addEventListener("click", flip, false);
        nodes[i].addEventListener("click", game, false);
        img = document.createElement("img")
        img.src = `pics/card${i + 1}.png`;
        img.alt = `card${i + 1}`;
        nodes[i].appendChild(img);
    }
    return nodes;
}