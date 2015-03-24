var addon = require('./build/Release/addon');

setInterval(function () {
    addon(function (msg) {
        console.log(msg);
    })
}, 1000);