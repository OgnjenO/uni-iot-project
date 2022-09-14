var styleElemBefore = document.head.appendChild(document.createElement("style"));
var styleElemAfter = document.head.appendChild(document.createElement("style"));

var ws = new WebSocket("ws://alpha.ognjeno.info:5499");

ws.onmessage = function(ev) {
    const obj = JSON.parse(ev.data);
    switch (obj.event) {
        case 'rotencoder':
            handleRotencoder(obj);
        case 'keypad':
            handleKeypad(obj);
        case 'joystick':
            handleJoystick(obj);
    }
}

let colorMapping = {
    '1': 'AliceBlue',
    '2': 'AntiqueWhite',
    '3': 'Aqua',
    '4': 'Aquamarine',
    '5': 'Azure',
    '6': 'Beige',
    '7': 'Black',
    '8': 'Blue',
    '9': 'BlueViolet',
    '0': 'Brown',
    'A': 'CadetBlue',
    'B': 'Chocolate',
    'C': 'Coral',
    'D': 'CornflowerBlue',
    '#': 'Crimson',
    '*': 'Cyan',
};

function handleRotencoder(obj) {
    document.querySelector(".hOne").style.transform = `rotate(${obj.value}deg)`;
}

function handleKeypad(obj) {
    document.querySelector(".hOne").style.backgroundColor = colorMapping[obj.key];
    styleElemBefore.innerHTML = `.hOne:before {background: ${colorMapping[obj.key]};}`;
    styleElemAfter.innerHTML = `.hOne:after {background: ${colorMapping[obj.key]};}`;
}

function handleJoystick(obj) {
    $(".hOne").finish().animate({
        left: obj.x
    });
    $(".hOne").finish().animate({
        top: obj.y
    });
}